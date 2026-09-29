#!/usr/bin/python
# license:BSD-3-Clause
# copyright-holders:Olivier Galibert

from __future__ import print_function

import io
import logging
import sys

USAGE = """
Usage:
%s prefix {opc.lst|-} disp.lst device.inc deviced.inc
"""
MAX_STATES = 0

def load_opcodes(fname):
    """Load opcodes from .lst file"""
    opcodes = []
    logging.info("load_opcodes: %s", fname)
    try:
        f = io.open(fname, "r")
    except Exception:
        err = sys.exc_info()[1]
        logging.error("cannot read opcodes file %s [%s]", fname, err)
        sys.exit(1)

    for line in f:
        if line.startswith("#"): continue
        line = line.rstrip()
        if not line: continue
        if line.startswith(" ") or line.startswith("\t"):
            # append instruction to last opcode
            if line == '\tprefetch();':
                opcodes[-1][1].append("\tprefetch_start();")
                opcodes[-1][1].append("\tm_IR = read_sync(m_PC);")
                opcodes[-1][1].append("\tprefetch_end();")
            elif line == '\tprefetch_noirq();':
                opcodes[-1][1].append("\tprefetch_start();")
                opcodes[-1][1].append("\tm_IR = read_sync(m_PC);")
                opcodes[-1][1].append("\tprefetch_end_noirq();")
            else:
                opcodes[-1][1].append(line)
        else:
            # add new opcode
            opcodes.append((line, []))
    return opcodes


def load_disp(fname):
    logging.info("load_disp: %s", fname)
    states = []
    try:
        f = io.open(fname, "r")
    except Exception:
        err = sys.exc_info()[1]
        logging.error("cannot read display file %s [%s]", fname, err)
        sys.exit(1)
    for line in f:
        if line.startswith("#"): continue
        line = line.strip()
        if not line: continue
        tokens = line.split()
        states += tokens
    return states

def emit(f, text):
    """write string to file"""
    print(text, file=f)


def identify_line_type(ins):
    if "eat-all-cycles" in ins: return "EAT"
    if "read" in ins: return "MEMORY_READ"
    if "write" in ins: return "MEMORY_WRITE"
    return "NONE"


def samples_interrupt(ins, single_cycle):
    return (single_cycle or "read_sync" not in ins) and "_noirq" not in ins


RDY_GATED_DEVICES = {"m6502", "m6510"}
CMOS_DEVICES = {"w65c02", "r65c02", "r65c19", "w65c02s", "m65ce02", "m4510", "w65816"}


def prepare_rp2a03_instructions(name, instructions):
    instructions = list(instructions)

    replacements = {
        "\twrite(m_SP, m_irq_taken ? m_P & ~F_B : m_P);": "\twrite(m_SP, m_irq_taken ? ((m_P & ~F_B) | F_T) : (m_P | F_B | F_T));",
        "\twrite(m_SP, m_P);": "\twrite(m_SP, (m_P | F_B | F_T));",
        "\tm_TMP = read(m_SP) | (F_B|F_E);": "\tm_TMP = read(m_SP) & 0xCF;// | (F_B|F_E);",
        "\tm_P = read(m_SP) | (F_B|F_E);": "\tm_P = read(m_SP) & 0xCF;// | (F_B|F_E);",
        "\tm_A = m_TMP2 | 0x51;": "\tm_A = m_TMP2 & m_SP;",
        "\tm_X = 0xff;": "\tm_X = m_A;\n\tm_SP = set_l(m_SP, m_A);",
        "\tset_nz(m_TMP2);": "\tset_nz(m_A);" if name == "las_aby" else "\tset_nz(m_TMP2);"
    }
    instructions = [replacements.get(ins, ins) for ins in instructions]

    halted_values = {
        "sha_aby": ("m_A & m_X", "m_A & m_X & ((m_TMP >> 8)+1)"),
        "sha_idy": ("m_A & m_X", "m_A & m_X & ((m_TMP >> 8)+1)"),
        "shs_aby": ("m_A & m_X", "m_A & m_X & ((m_TMP >> 8)+1)"),
        "shx_aby": ("m_X", "m_X & ((m_TMP >> 8)+1)"),
        "shy_abx": ("m_Y", "m_Y & ((m_TMP >> 8)+1)")
    }
    if name in halted_values:
        halted, running = halted_values[name]
        original = "\tm_TMP2 = %s;" % running
        replacement = "\tif(inst_halted) {\n\t\tm_TMP2 = %s;\n\t} else {\n\t\tm_TMP2 = %s;\n\t}" % (halted, running)
        instructions = [replacement if ins == original else ins for ins in instructions]

    return instructions


def save_opcodes(f, device, opcodes):
    rdy_gated = device in RDY_GATED_DEVICES
    interrupt_sampled = device not in CMOS_DEVICES
    rp2a03_cycle_hook = device == "rp2a03_core"

    if rp2a03_cycle_hook:
        interrupt_sampled = False

    for name, instructions in opcodes:
        if rp2a03_cycle_hook:
            instructions = prepare_rp2a03_instructions(name, instructions)
        single_cycle = sum(identify_line_type(ins) in ("MEMORY_READ", "MEMORY_WRITE") for ins in instructions) == 1
        emit(f, "void %s_device::%s_full()" % (device, name))
        emit(f, "{")
        substate = 1
        for ins_index, ins in enumerate(instructions):
            line_type = identify_line_type(ins)
            next_line_type = next((identify_line_type(next_ins) for next_ins in instructions[ins_index + 1:] if identify_line_type(next_ins) in ("MEMORY_READ", "MEMORY_WRITE")), None)
            if line_type == "EAT":
                emit(f, "\tdebugger_wait_hook();")
                emit(f, "\tm_icount = 0;")
                emit(f, "\tm_inst_substate = %d;" % substate)
                emit(f, "\treturn;")
                substate += 1
            elif line_type in ("MEMORY_READ", "MEMORY_WRITE"):
                if rdy_gated and line_type == "MEMORY_READ":
                    emit(f, "\twhile(!m_rdy_state) {")
                    emit(f, "\t\tm_icount--;")
                    emit(f, "\t\tif(m_icount <= 0) {")
                    emit(f, "\t\t\tm_inst_substate = %d;" % substate)
                    emit(f, "\t\t\treturn;")
                    emit(f, "\t\t}")
                    emit(f, "\t}")
                if interrupt_sampled and samples_interrupt(ins, single_cycle):
                    emit(f, "\tsample_interrupt();")
                emit(f, ins)
                if rp2a03_cycle_hook and name == "brk_imp" and line_type == "MEMORY_READ" and "read_pc()" in ins:
                    emit(f, "\tnext_read = false;")
                elif rp2a03_cycle_hook and not name.startswith(("rra_nd_", "isb_nd_")) and next_line_type and next_line_type != line_type:
                    emit(f, "\tnext_read = %s;" % ("true" if next_line_type == "MEMORY_READ" else "false"))
                emit(f, "\tm_icount--;")
                if rp2a03_cycle_hook and line_type == "MEMORY_READ" and not (name == "rol_zpx" and ins.strip() == "m_TMP = read_pc();"):
                    emit(f, "\tdo_halt();")
                emit(f, "\tif(m_icount <= 0) {")
                emit(f, "\t\tif(access_to_be_redone()) {")
                emit(f, "\t\t\tm_icount++;")
                emit(f, "\t\t\tm_inst_substate = %d;" % substate)
                emit(f, "\t\t} else")
                emit(f, "\t\t\tm_inst_substate = %d;" % (substate+1))
                emit(f, "\t\treturn;")
                emit(f, "\t}")
                substate += 2
            else:
                emit(f, ins)
        emit(f, "}")
        emit(f, "")

        emit(f, "void %s_device::%s_partial()" % (device, name))
        emit(f, "{")
        emit(f, "\tswitch(m_inst_substate) {")
        emit(f, "case 0:")
        substate = 1
        for ins_index, ins in enumerate(instructions):
            line_type = identify_line_type(ins)
            next_line_type = next((identify_line_type(next_ins) for next_ins in instructions[ins_index + 1:] if identify_line_type(next_ins) in ("MEMORY_READ", "MEMORY_WRITE")), None)
            if line_type == "EAT":
                emit(f, "\tdebugger_wait_hook();")
                emit(f, "\tm_icount = 0;")
                emit(f, "\tm_inst_substate = %d;" % substate)
                emit(f, "\treturn;")
                emit(f, "\tcase %d:;" % substate)
                substate += 1
            elif line_type in ("MEMORY_READ", "MEMORY_WRITE"):
                emit(f, "\t[[fallthrough]];")
                emit(f, "case %d:" % substate)
                if rdy_gated and line_type == "MEMORY_READ":
                    emit(f, "\twhile(!m_rdy_state) {")
                    emit(f, "\t\tm_icount--;")
                    emit(f, "\t\tif(m_icount <= 0) {")
                    emit(f, "\t\t\tm_inst_substate = %d;" % substate)
                    emit(f, "\t\t\treturn;")
                    emit(f, "\t\t}")
                    emit(f, "\t}")
                if interrupt_sampled and samples_interrupt(ins, single_cycle):
                    emit(f, "\tsample_interrupt();")
                emit(f, ins)
                if rp2a03_cycle_hook and name == "brk_imp" and line_type == "MEMORY_READ" and "read_pc()" in ins:
                    emit(f, "\tnext_read = false;")
                elif rp2a03_cycle_hook and not name.startswith(("rra_nd_", "isb_nd_")) and next_line_type and next_line_type != line_type:
                    emit(f, "\tnext_read = %s;" % ("true" if next_line_type == "MEMORY_READ" else "false"))
                emit(f, "\tm_icount--;")
                if rp2a03_cycle_hook and line_type == "MEMORY_READ":
                    emit(f, "\tdo_halt();")
                emit(f, "\tif(m_icount <= 0) {")
                emit(f, "\t\tif(access_to_be_redone()) {")
                emit(f, "\t\t\tm_icount++;")
                emit(f, "\t\t\tm_inst_substate = %d;" % substate)
                emit(f, "\t\t} else")
                emit(f, "\t\t\tm_inst_substate = %d;" % (substate+1))
                emit(f, "\t\treturn;")
                emit(f, "\t}")
                emit(f, "\t[[fallthrough]];")
                emit(f, "case %d:;" % (substate+1))
                substate += 2
            else:
                emit(f, ins)
        emit(f, "\tbreak;")
        emit(f, "}")
        emit(f, "\tm_inst_substate = 0;")
        emit(f, "}")
        emit(f, "")


DO_EXEC_FULL_PROLOG="""\
void %(device)s_device::do_exec_full()
{
\tswitch(m_inst_state) {
"""

DO_EXEC_FULL_EPILOG="""\
\t}
}
"""

DO_EXEC_PARTIAL_PROLOG="""\
void %(device)s_device::do_exec_partial()
{
\tswitch(m_inst_state) {
"""

DO_EXEC_PARTIAL_EPILOG="""\
\t}
}
"""

DISASM_PROLOG="""\
const %(device)s_disassembler::disasm_entry %(device)s_disassembler::disasm_entries[0x%(disasm_count)x] = {
"""

DISASM_EPILOG="""\
};
"""

def save_tables(f, device, states):
    total_states = len(states)

    d = { "device": device,
          "disasm_count": total_states-1
          }

    emit(f, DO_EXEC_FULL_PROLOG % d)
    for n, state in enumerate(states):
        if state == ".": continue
        if n < total_states - 1:
            emit(f, "\tcase 0x%02x: %s_full(); break;" % (n, state))
        else:
            emit(f, "\tcase %s: %s_full(); break;" % ("STATE_RESET", state))
    emit(f, DO_EXEC_FULL_EPILOG % d)

    emit(f, DO_EXEC_PARTIAL_PROLOG % d)
    for n, state in enumerate(states):
        if state == ".": continue
        if n < total_states - 1:
            emit(f, "\tcase 0x%02x: %s_partial(); break;" % (n, state))
        else:
            emit(f, "\tcase %s: %s_partial(); break;" % ("STATE_RESET", state))
    emit(f, DO_EXEC_PARTIAL_EPILOG % d)

def save_dasm(f, device, states):
    total_states = len(states)

    d = { "device": device,
          "disasm_count": total_states-1
          }

    emit(f, DISASM_PROLOG % d )
    for n, state in enumerate(states):
        if state == ".": continue
        if n == total_states - 1: break
        tokens = state.split("_")
        opc = tokens[0]
        mode = tokens[-1]
        extra = "0"
        if opc in ["jsr", "bsr", "callf", "jpi", "jsb", "jsl"]:
            extra = "STEP_OVER"
        elif opc in ["rts", "rti", "rtn", "retf", "tpi", "rtl"]:
            extra = "STEP_OUT"
        elif opc in ["bcc", "bcs", "beq", "bmi", "bne", "bpl", "bvc", "bvs", "bbr", "bbs", "bbc", "bar", "bas"]:
            extra = "STEP_COND"
        emit(f, '\t{ "%s", DASM_%s, %s },' % (opc, mode, extra))
    emit(f, DISASM_EPILOG % d)

def saves(fname, device, opcodes, states):
    logging.info("saving: %s", fname)
    try:
        f = open(fname, "w")
    except Exception:
        err = sys.exc_info()[1]
        logging.error("cannot write file %s [%s]", fname, err)
        sys.exit(1)
    save_opcodes(f, device, opcodes)
    emit(f, "\n")
    save_tables(f, device, states)
    f.close()


def saved(fname, device, opcodes, states):
    logging.info("saving: %s", fname)
    try:
        f = open(fname, "w")
    except Exception:
        err = sys.exc_info()[1]
        logging.error("cannot write file %s [%s]", fname, err)
        sys.exit(1)
    save_dasm(f, device, states)
    f.close()


def main(argv):
    debug = False
    logformat=("%(levelname)s:"
               "%(module)s:"
               "%(lineno)d:"
               "%(threadName)s:"
               "%(message)s")
    if debug:
        logging.basicConfig(level=logging.INFO, format=logformat)
    else:
        logging.basicConfig(level=logging.WARNING, format=logformat)


    if len(argv) != 6:
        print(USAGE % argv[0])
        return 1

    mode = argv[1]
    device_name = argv[2]

    opcodes = []
    if argv[3] !=  "-":
        opcodes = load_opcodes(argv[3])
        logging.info("found %d opcodes", len(opcodes))
    else:
        logging.info("skipping opcode reading")


    states = load_disp(argv[4])
    logging.info("loaded %s states", len(states))

    assert (len(states) & 0xff) == 1
    if mode == 's':
        saves(argv[5], device_name, opcodes, states)
    else:
        saved(argv[5], device_name, opcodes, states)


# ======================================================================
if __name__ == "__main__":
    sys.exit(main(sys.argv))