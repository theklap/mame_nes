// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
/***************************************************************************

    rp2a03.cpp

    6502, NES variant

***************************************************************************/

#include "emu.h"
#include "rp2a03.h"
#include "rp2a03d.h"

DEFINE_DEVICE_TYPE(RP2A03_CORE, rp2a03_core_device, "rp2a03_core", "Ricoh RP2A03 core") // needed for some VT systems with XOP instead of standard APU
DEFINE_DEVICE_TYPE(RP2A03,      rp2a03_device,      "rp2a03",      "Ricoh RP2A03")      // earliest version, found in punchout, spnchout, dkong3, VS. systems, and some early Famicoms
DEFINE_DEVICE_TYPE(RP2A03G,     rp2a03g_device,     "rp2a03g",     "Ricoh RP2A03G")     // later revision, found in front-loader NES

void rp2a03_device::rp2a03_map(address_map &map)
{

	map(0x4000, 0x4013).w(m_apu, FUNC(nesapu_device::write));
	map(0x4000, 0x4014).r(m_apu, FUNC(nesapu_device::read));
	map(0x4014, 0x4014).w(m_apu, FUNC(nesapu_device::do_oam_dma));
	map(0x4015, 0x4015).r(m_apu, FUNC(nesapu_device::status_r));
	map(0x4015, 0x4015).lw8(NAME([this](u8 data) { m_apu->write(0x15, data);})); //logerror("Write to APU 4015 from rp2a03: %d\n", data);
	map(0x4017, 0x4017).lw8(NAME([this](u8 data) { m_apu->write(0x17, data);})); //logerror("Write to APU 4017 from rp2a03: %d\n", data);
	map(0x4018, 0x40ff).r(m_apu, FUNC(nesapu_device::read));
	//map(0x4018, 0x401f).r(m_apu, FUNC(nesapu_device::read));
	
	// 0x4014 w -> NES sprite DMA (is this internal?)
	// 0x4016 w -> d0-d2: RP2A03 OUT0,OUT1,OUT2
	// 0x4016 r -> d0-d4: RP2A03 IN0
	// 0x4017 r -> d0-d4: RP2A03 IN1
}

rp2a03_core_device::rp2a03_core_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock)
	: m6502_device(mconfig, type, tag, owner, clock)
	, m_mmc5_reset_scanline_irq(*this)
	, m_mmc5_ppuctrl_write(*this)
	, m_mmc5_ppumask_write(*this)
{
}

rp2a03_core_device::rp2a03_core_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: rp2a03_core_device(mconfig, RP2A03_CORE, tag, owner, clock)
{
}

rp2a03_device::rp2a03_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock)
	: rp2a03_core_device(mconfig, type, tag, owner, clock)
	, device_mixer_interface(mconfig, *this)
	, m_apu(*this, "nesapu")
{
	m_program_config.m_internal_map = address_map_constructor(FUNC(rp2a03_device::rp2a03_map), this);
	
}

rp2a03_device::rp2a03_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: rp2a03_device(mconfig, RP2A03, tag, owner, clock)
{
}

rp2a03g_device::rp2a03g_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: rp2a03_device(mconfig, RP2A03G, tag, owner, clock)
{
}

std::unique_ptr<util::disasm_interface> rp2a03_core_device::create_disassembler()
{
	return std::make_unique<rp2a03_disassembler>();
}

void rp2a03_device::apu_irq(int state)
{
	// games relying on the APU_IRQ don't seem to work anyway? (nes software list : timelord, mig29sf, firehawk)
	//set_input_line(RP2A03_APU_IRQ_LINE, state ? ASSERT_LINE : CLEAR_LINE);
	execute_set_input(RP2A03_APU_IRQ_LINE, state ? ASSERT_LINE : CLEAR_LINE);
}

uint8_t rp2a03_device::apu_read_mem(offs_t offset)
{
	return space(AS_PROGRAM).read_byte(offset);
}

void rp2a03_device::device_add_mconfig(machine_config &config)
{
	APU_2A03(config, m_apu, DERIVED_CLOCK(1,1));
	m_apu->irq().set(FUNC(rp2a03_device::apu_irq));
	m_apu->mem_read().set(FUNC(rp2a03_device::apu_read_mem));
	m_apu->add_route(ALL_OUTPUTS, *this, 1.0, 0);
	
}

void rp2a03g_device::device_add_mconfig(machine_config &config)
{
	NES_APU(config, m_apu, DERIVED_CLOCK(1,1));
	m_apu->irq().set(FUNC(rp2a03g_device::apu_irq));
	m_apu->mem_read().set(FUNC(rp2a03g_device::apu_read_mem));
	m_apu->add_route(ALL_OUTPUTS, *this, 1.0, 0);
}

void rp2a03_core_device::device_start()
{
    m6502_device::device_start();
}

void rp2a03_core_device::init()
{
    m6502_device::init();

    save_item(NAME(prev_IR));
    save_item(NAME(next_IR));
    save_item(NAME(cpu_is_reading));
    save_item(NAME(cpu_data_bus));
    save_item(NAME(cpu_external_bus));
    save_item(NAME(adr_bus));
    save_item(NAME(delay));
    save_item(NAME(nmi_pending_1));
    save_item(NAME(irq_delay));
    save_item(NAME(apu_irq_delay));
    save_item(NAME(nmi_delay));
    save_item(NAME(branched));
    save_item(NAME(paged));
    save_item(NAME(oam_dma_halt_cycle));
    save_item(NAME(rmw_1));
    save_item(NAME(apu_irq_branch_delay));
    save_item(NAME(irq_branch_delay));
    save_item(NAME(nmi_branch_delay));
    save_item(NAME(apu_clk1_is_high));
    save_item(NAME(dmc_halt));
    save_item(NAME(oam_halt));
    save_item(NAME(dmc_dma_explicit_stop));
    save_item(NAME(write_cycles_since_dma_halt_request));
    save_item(NAME(dmc_dma_reload));
    save_item(NAME(m_famicom_controller_timing));
    save_item(NAME(prev_4016_write));
    save_item(NAME(prev_4017_write));
    save_item(NAME(prev_4016_read));
    save_item(NAME(prev_4017_read));
    save_item(NAME(last_4016_val));
    save_item(NAME(last_4017_val));
    save_item(NAME(inst_halted));
    save_item(NAME(next_read));
    save_item(NAME(prev_next_read));
    save_item(NAME(need_irq));
    save_item(NAME(prevReadAddress));
    save_item(NAME(m_exram_control));
    save_item(NAME(m_open_bus_ranges));
    save_item(NAME(m_ob_count));
    save_item(NAME(nmi_cpu_cycle));
    save_item(NAME(nmi_overlap_brk_irq));
    save_item(NAME(m_real_brk));
    save_item(NAME(mapper_irq));
    save_item(NAME(mapper_irq_delay));
    save_item(NAME(mapper_irq_cpu_cycle));
    save_item(NAME(last_cpu_write_latch));
    save_item(NAME(m_last_cpu_write_cycle));
    save_item(NAME(m_previous_cpu_write_cycle));
    save_item(NAME(is_pal));

    m_X = 0x00;
    m_P = 0x00;
    prev_IR = 0x00;
    next_IR = 0x00;
    cpu_is_reading = true;
    cpu_data_bus = 0x00;
    cpu_external_bus = 0x00;
    adr_bus = 0x0000;
    delay = 0;
    nmi_pending_1 = false;
    irq_delay = false;
    apu_irq_delay = false;
    nmi_delay = false;
    branched = false;
    paged = false;
    oam_dma_halt_cycle = 0;
    rmw_1 = false;
    apu_irq_branch_delay = false;
    irq_branch_delay = false;
    nmi_branch_delay = false;
    apu_clk1_is_high = false;
    dmc_halt = false;
    oam_halt = false;
    dmc_dma_explicit_stop = false;
    write_cycles_since_dma_halt_request = 0;
    dmc_dma_reload = false;
    m_famicom_controller_timing = false;
    prev_4016_write = 0;
    prev_4017_write = 0;
    prev_4016_read = 0;
    prev_4017_read = 0;
    last_4016_val = 0;
    last_4017_val = 0;
    inst_halted = false;
    next_read = false;
    prev_next_read = false;
    need_irq = false;
    prevReadAddress = 0x0000;
    m_exram_control = 0;
    std::fill(std::begin(m_open_bus_ranges), std::end(m_open_bus_ranges), 0);
    m_ob_count = 0;
    nmi_cpu_cycle = 0;
    nmi_overlap_brk_irq = false;
    m_real_brk = false;
    mapper_irq = false;
    mapper_irq_delay = 0;
    mapper_irq_cpu_cycle = 0;
    last_cpu_write_latch = 0x00;
    m_last_cpu_write_cycle = 0;
    m_previous_cpu_write_cycle = 0;
    is_pal = false;
}

void rp2a03_core_device::device_reset()
{
    m6502_device::device_reset();

    prev_IR = 0x00;
    next_IR = 0x00;
    cpu_is_reading = true;
    cpu_data_bus = 0x00;
    cpu_external_bus = 0x00;
    adr_bus = 0x0000;
    delay = 0;
    nmi_pending_1 = false;
    irq_delay = false;
    apu_irq_delay = false;
    nmi_delay = false;
    branched = false;
    paged = false;
    oam_dma_halt_cycle = 0;
    rmw_1 = false;
    apu_irq_branch_delay = false;
    irq_branch_delay = false;
    nmi_branch_delay = false;
    apu_clk1_is_high = false;
    dmc_halt = false;
    oam_halt = false;
    dmc_dma_explicit_stop = false;
    write_cycles_since_dma_halt_request = 0;
    dmc_dma_reload = false;
    prev_4016_write = 0;
    prev_4017_write = 0;
    prev_4016_read = 0;
    prev_4017_read = 0;
    last_4016_val = 0;
    last_4017_val = 0;
    inst_halted = false;
    next_read = false;
    prev_next_read = false;
    need_irq = false;
    prevReadAddress = 0x0000;
    nmi_cpu_cycle = 0;
    nmi_overlap_brk_irq = false;
    m_real_brk = false;
    mapper_irq = false;
    mapper_irq_delay = 0;
    mapper_irq_cpu_cycle = 0;
    last_cpu_write_latch = 0x00;
    m_last_cpu_write_cycle = 0;
    m_previous_cpu_write_cycle = 0;
}

void rp2a03_core_device::handle_dma_rdy_stall()
{
    // NES DMA drives RDY low after a haltable CPU bus cycle has already executed
    // in this generated MAME 6502 core.  We model the stolen cycle by suspending
    // the CPU for one cycle, then rolling back the micro-op state so the same
    // bus cycle is retried when execution resumes.
    if (suspended() && (oam_halt || dmc_halt))
    {
        inst_halted = true;

        if (oam_halt)
            mark_oam_dma_halt_cycle();

        if (m_inst_substate > 0)
        {
            m_inst_substate--;
        }
        else
        {
            logerror("DMA/RDY stall with inst_substate == 0 pc=%04x IR=%02x cycle=%llu\n",
                m_PC, m_IR, (unsigned long long)total_cycles());
        }

        dmc_halt = false;
        oam_halt = false;

        // If the stolen cycle overlapped opcode fetch/sync, restore the previous
        // opcode identity so the retried cycle doesn't advance the instruction stream.
        if (m_sync)
        {
            next_IR = m_IR;
            m_IR = prev_IR;
        }
    }
}


void rp2a03_core_device::execute_run()
{	
	apu_clk1_is_high = (((total_cycles()) & 0x01) == 0);
	prev_next_read = next_read;
	
	/* 
		Pass NMI_BRK and NMI_IRQ tests from Blarrg
		-- NMI Timing to interupt BRK/IRQ -- 
		Fire NMI deep into the BRK/IRQ opcode here
		but only before set/clear B flag and not after
	*/
	if (nmi_pending_1 && m_IR == 0x00 && !nmi_overlap_brk_irq)
	{
		nmi_overlap_brk_irq = true;

		if (m_real_brk)
		{
			if (m_inst_substate < 9)
			{
				nmi_pending_1 = false;
				delay = 0;
				pulse_input_line(rp2a03_core_device::NMI_LINE, attotime::zero);
				osd_printf_info("Detected: NMI interrupt BRK = Now, delay: %d\n", delay);
			}
			else
			{
				delay = 2;
				osd_printf_info("Detected: NMI interrupt BRK = Delay 1 OpCode, delay: %d\n", delay);
			}
		}
		else if (m_irq_taken)
		{
			if (m_inst_substate < 9)
			{
				nmi_pending_1 = false;
				delay = 0;
				pulse_input_line(rp2a03_core_device::NMI_LINE, attotime::zero);
				osd_printf_info("Detected: NMI interrupt IRQ = Now, delay: %d\n", delay);
			}
			else
			{
				delay = 2;
				osd_printf_info("Detected: NMI interrupt IRQ = Delay 1 OpCode, delay: %d\n", delay);
			}
		}
	}
	
	/*	Run the CPU */
	if (m_inst_substate) {
		do_exec_partial();
	}

	while (m_icount > 0)
	{
		if (m_inst_state < 0xff00) {
			m_PPC = m_NPC;
			m_inst_state = m_IR | m_inst_state_base;
			
			if (machine().debug_flags & DEBUG_FLAG_ENABLED)
				debugger_instruction_hook(pc_to_external(m_NPC));
		}
		do_exec_full();
	}

	handle_dma_rdy_stall();
}


bool rp2a03_core_device::is_branch_opcode(u8 op)
{
	switch (op)
	{
		case 0x10: case 0x30: case 0x50: case 0x70:
		case 0x90: case 0xB0: case 0xD0: case 0xF0:
			return true;
		default:
			return false;
	}
}


bool rp2a03_core_device::is_irq_oam_dma_window() 
{
	const int delta = total_cycles() - oam_dma_halt_cycle;
	return ((((delta <= 3) && (delta >= 1)) || (delta == 514)) && total_cycles() > 600);
}


bool rp2a03_core_device::branch_delay_match_for_new_irq(int state) 
{
	// In this generated MAME 6502 core, a late interrupt can arrive after
	// IR has already advanced/prefetched.  prev_IR is the branch opcode that
	// created the non-page-crossing branch delay window.
	return is_branch_opcode(prev_IR) &&
		(state == ASSERT_LINE) &&
		branched &&
		!paged &&
		(m_inst_substate > 6);
}


bool rp2a03_core_device::branch_need_irq_match() 
{
	// Used while the branch opcode is still active. The late-arrival delay
	// check uses prev_IR because IR may have advanced by that point.
	return is_branch_opcode(m_IR);
}


bool rp2a03_core_device::dma_window_interrupt_eligible() 
{
	return (m_nmi_pending || ((m_irq_state || m_apu_irq_state) && !(m_P & F_I))) && !m_inhibit_interrupts;
}



void rp2a03_core_device::execute_set_input(int inputnum, int state)
{
	switch (inputnum)
	{
	case IRQ_LINE:
	{
		// Taken non-page-crossing branch ignores IRQ/NMI on its last cycle
		if (branch_delay_match_for_new_irq(state) && !m_irq_state)
			irq_branch_delay = true;

		// Keep IRQ "wanted" across the branch window
		if (branch_need_irq_match() && (m_irq_state || state == ASSERT_LINE))
			need_irq = true;

		// If IRQ fell before we consumed the delayed branch case, force it to remain visible
		if (branch_need_irq_match() && need_irq && !state)
			state = ASSERT_LINE;

		m_irq_state = (state == ASSERT_LINE);

		// IRQ during DMA window
		if (is_irq_oam_dma_window() && dma_window_interrupt_eligible())
			irq_branch_delay = true;
		break;
	}

	case APU_IRQ_LINE:
	{
		// Same branch-delay behavior for APU IRQ
		if (branch_delay_match_for_new_irq(state) && !m_apu_irq_state)
			apu_irq_branch_delay = true;

		// Keep IRQ "wanted" across the branch window
		if (branch_need_irq_match() && (m_apu_irq_state || state == ASSERT_LINE))
			need_irq = true;

		// If IRQ fell before we consumed the delayed branch case, force it to remain visible
		if (branch_need_irq_match() && need_irq && !state)
			state = ASSERT_LINE;

		m_apu_irq_state = (state == ASSERT_LINE);

		// APU IRQ during DMA window
		if (is_irq_oam_dma_window() && dma_window_interrupt_eligible())
			apu_irq_branch_delay = true;

		break;
	}

	case NMI_LINE:
	{
		// Taken non-page-crossing branch ignores NMI on its last cycle too
		if (branch_delay_match_for_new_irq(state) && !m_nmi_state)
		{
			nmi_branch_delay = true;
			nmi_pending_1 = false;
		}

		// NMI is edge-triggered
		if (!m_nmi_state && state == ASSERT_LINE)
			m_nmi_pending = true;

		m_nmi_state = (state == ASSERT_LINE);

		// NMI during DMA window
		if (is_irq_oam_dma_window() && dma_window_interrupt_eligible())
		{
			nmi_branch_delay = true;
			nmi_pending_1 = false;
		}

		break;
	}

	case V_LINE:
		if (!m_v_state && state == ASSERT_LINE)
			m_P |= F_V;
		m_v_state = (state == ASSERT_LINE);
		break;
	}
}


device_memory_interface::space_config_vector rp2a03_core_device::memory_space_config() const
{
	if(has_configured_map(AS_OPCODES))
		return space_config_vector {
			std::make_pair(AS_PROGRAM, &m_program_config),
			std::make_pair(AS_OPCODES, &m_sprogram_config)
		};
	else
		return space_config_vector {
			std::make_pair(AS_PROGRAM, &m_program_config)
		};
}


void rp2a03_core_device::state_import(const device_state_entry &entry)
{
	switch(entry.index()) {
	case STATE_GENFLAGS:
	case M6502_P:
		m_P = m_P | (F_B|F_E);
		break;
	case M6502_PC:
		m_PC = m_NPC;
		m_irq_taken = false;
		prefetch_start();
		m_IR = m_mintf->read_sync(m_PC);
		prefetch_end();
		m_PPC = m_NPC;
		m_inst_state = m_IR | m_inst_state_base;
		break;
	}
}


void rp2a03_core_device::state_export(const device_state_entry &entry)
{
	switch(entry.index()) {
	case STATE_GENPC:     m_XPC = pc_to_external(m_PPC); break;
	case STATE_GENPCBASE: m_XPC = pc_to_external(m_NPC); break;
	}
}


void rp2a03_core_device::state_string_export(const device_state_entry &entry, std::string &str) const
{
	switch(entry.index()) {
	case STATE_GENFLAGS:
	case M6502_P:
		str = string_format("%c%c%c%c%c%c",
						m_P & F_N ? 'N' : '.',
						m_P & F_V ? 'V' : '.',
						m_P & F_D ? 'D' : '.',
						m_P & F_I ? 'I' : '.',
						m_P & F_Z ? 'Z' : '.',
						m_P & F_C ? 'C' : '.');
		break;
	}
}


void rp2a03_core_device::prefetch_start()
{		
	m_sync = true;
	m_sync_w(ASSERT_LINE);
	m_NPC = m_PC;
	
	prev_IR = m_IR;	
	
	if(!nmi_pending_1)
		nmi_overlap_brk_irq = false;
}


void rp2a03_core_device::queue_delayed_mapper_irq(int cycles)
{
	// Don't stack duplicate delayed IRQs
	if (!mapper_irq && !m_irq_state)
	{
		mapper_irq = true;
		mapper_irq_delay = cycles;
		mapper_irq_cpu_cycle = total_cycles() - 1;
	}
}


void rp2a03_core_device::cancel_delayed_mapper_irq()
{
	// MMC3 $E000 disables/acknowledges the mapper IRQ line.
	// Cancel the delayed mapper source and any one-opcode branch deferral
	// created from that source.
	mapper_irq = false;
	mapper_irq_delay = 0;
	mapper_irq_cpu_cycle = 0;

	m_irq_state = false;
	irq_delay = false;
	irq_branch_delay = false;
	need_irq = false;
}


void rp2a03_core_device::queue_delayed_nmi(int cycles)
{
	// Don't stack duplicate delayed NMIs
	if (!nmi_pending_1 && !m_nmi_pending)
	{
		nmi_pending_1 = true;
		delay = cycles;
		nmi_cpu_cycle = total_cycles() - 1;
	}
}


void rp2a03_core_device::cancel_delayed_nmi()
{
	// Only cancel before it has arrived in the normal NMI pending path
	if (nmi_pending_1 && !m_nmi_pending)
	{
		nmi_pending_1 = false;
		delay = 0;
		nmi_cpu_cycle = 0;
	}
}


void rp2a03_core_device::prefetch_end()
{
	m_real_brk = (m_IR == 0x00) && !m_irq_taken && !m_nmi_pending;
	m_sync = false;
	m_sync_w(CLEAR_LINE);
	
	//logic for branch command IRQ and NMI to fire on next command
	if (irq_delay) {
		execute_set_input(IRQ_LINE, ASSERT_LINE);
		irq_delay = false;
	}
	
	if (apu_irq_delay) {
		execute_set_input(APU_IRQ_LINE, ASSERT_LINE);
		apu_irq_delay = false;
	}
	
	if (nmi_delay) {
		pulse_input_line(rp2a03_core_device::NMI_LINE, attotime::zero);
		nmi_delay = false;
	}
	
	//logic for branch command IRQ and NMI to skip this time around but get it next time
	if (irq_branch_delay) {
		m_irq_state = false;
		irq_delay = true;
		irq_branch_delay = false;
	}
	
	if (apu_irq_branch_delay) {
		m_apu_irq_state = false;
		apu_irq_delay = true;
		apu_irq_branch_delay = false;
	}	
	
	if(nmi_branch_delay) {
		m_nmi_pending = false;
		nmi_delay = true;
		nmi_branch_delay = false;
		nmi_pending_1 = false;
	}
	
	// NMI interrupts BRK/IRQ Delay - Fire NMI
	// NMI delayed arrival into the normal CPU interrupt pending state
	if (nmi_pending_1)
	{
		if (delay > 0)
			--delay;

		if (delay == 0)
		{
			nmi_pending_1 = false;
			if(total_cycles() - nmi_cpu_cycle < 3) {
				pulse_input_line(rp2a03_core_device::NMI_LINE, attotime::zero);
			} else {
				m_nmi_pending = true;
			}
		}
	}
	
	if (mapper_irq)
	{
		if (mapper_irq_delay > 0)
			--mapper_irq_delay;

		if (mapper_irq_delay == 0)
		{
			mapper_irq = false;
			m_irq_state = true;
		}
	}
	
	if((m_nmi_pending || ((m_irq_state || m_apu_irq_state) && !(m_P & F_I))) && !m_inhibit_interrupts) {
		m_irq_taken = true;
		m_IR = 0x00;
	} else {
		m_PC++;
	}
	
	branched = false;
	paged = false;
	inst_halted = false;
	need_irq = false;
}


void rp2a03_core_device::prefetch_end_noirq()
{
	m_sync = false;
	m_sync_w(CLEAR_LINE);
	m_PC++;
}


uint8_t rp2a03_core_device::read(uint16_t adr)
{
	if ((adr == 0xfffa || adr == 0xfffb) && !m_mmc5_reset_scanline_irq.isnull()) {
		m_mmc5_reset_scanline_irq();
	}
	
	adr_bus = adr;
	prevReadAddress = adr;
	set_var_read();

	// --- 4016/4017 special timing ---
	if (!m_famicom_controller_timing && (adr == 0x4016 || adr == 0x4017))
	{
		int64_t tc = suspended() ? (total_cycles() - 1) : total_cycles();

		if (adr == 0x4016)
		{
			if (tc - prev_4016_read == 1)
			{
				prev_4016_read = tc;
				return last_4016_val;
			}
			prev_4016_read = tc;
		}
		else
		{
			if (tc - prev_4017_read == 1)
			{
				prev_4017_read = tc;
				return last_4017_val;
			}
			prev_4017_read = tc;
		}
	}

	// --- OPEN BUS CHECK ---
	if (is_open_bus_address(adr))
	{
		logerror("OPENBUS DATA: PC=%04X IR=%02X address=%04X value=%02X\n", m_PC, m_IR, adr, cpu_external_bus);
		cpu_data_bus = cpu_external_bus;
		return cpu_data_bus;
	}

	// --- 4015 reads use APU side ---
	if (adr == 0x4015) {
		cpu_data_bus = m_mintf->read(adr);
		return cpu_data_bus;
	}

	// --- normal read ---
	cpu_data_bus = m_mintf->read(adr);
	cpu_external_bus = cpu_data_bus;
		
	// Cache the real returned values so the double-read returns the SAME byte later
	if (adr == 0x4016)
		last_4016_val = cpu_data_bus;
	else if (adr == 0x4017)
		last_4017_val = cpu_data_bus;
		
	return cpu_data_bus;
}



uint8_t rp2a03_core_device::read_9(uint16_t adr)
{
	if ((adr == 0xfffa || adr == 0xfffb) && !m_mmc5_reset_scanline_irq.isnull()) {
		m_mmc5_reset_scanline_irq();
	}
	
	adr_bus = adr;
	prevReadAddress = adr;
	set_var_read();

	if (!m_famicom_controller_timing && (adr == 0x4016 || adr == 0x4017))
	{
		int64_t tc = suspended() ? (total_cycles() - 1) : total_cycles();

		if (adr == 0x4016)
		{
			if (tc - prev_4016_read == 1)
			{
				prev_4016_read = tc;
				return last_4016_val;
			}
			prev_4016_read = tc;
		}
		else
		{
			if (tc - prev_4017_read == 1)
			{
				prev_4017_read = tc;
				return last_4017_val;
			}
			prev_4017_read = tc;
		}
	}

	if (is_open_bus_address(adr))
	{
		logerror("OPENBUS DATA: PC=%04X IR=%02X address=%04X value=%02X\n", m_PC, m_IR, adr, cpu_external_bus);
		cpu_data_bus = cpu_external_bus;
		return cpu_data_bus;
	}

	if (adr == 0x4015) {
		cpu_data_bus = m_mintf->read_9(adr);
		return cpu_data_bus;
	}

	cpu_data_bus = m_mintf->read_9(adr);
	cpu_external_bus = cpu_data_bus;
		
	// Cache the real returned values so the double-read returns the SAME byte later
	if (adr == 0x4016)
		last_4016_val = cpu_data_bus;
	else if (adr == 0x4017)
		last_4017_val = cpu_data_bus;
		
	return cpu_data_bus;
}


void rp2a03_core_device::write(uint16_t adr, uint8_t val) {
	if (adr == 0x2000 && !m_mmc5_ppuctrl_write.isnull()) {
		m_mmc5_ppuctrl_write(val);
	}
	else if (adr == 0x2001 && !m_mmc5_ppumask_write.isnull()) {
		m_mmc5_ppumask_write(val);
	}
	else if (adr == 0x4014 && !m_mmc5_reset_scanline_irq.isnull()) {
		m_mmc5_reset_scanline_irq();
	}
	
	write_cycles_since_dma_halt_request++; 
	adr_bus = adr;
	cpu_is_reading = false;
	
	// Some cartridge protection logic, including BMW8544 / mapper 292,
	// observes the last data byte driven by a CPU write, not just writes to
	// the mapper's own address range.  Keep this CPU-side so cart code can
	// query the true last CPU write without coupling the CPU to a mapper.
	last_cpu_write_latch = val;
	m_previous_cpu_write_cycle = m_last_cpu_write_cycle;
	m_last_cpu_write_cycle = total_cycles();
	if (adr == 0x4016 || adr == 0x4017) {
		bool const suppress = rmw_1 && get_apu_clk1_is_high() && !(val & 1);

		if (adr == 0x4016)
			prev_4016_write = total_cycles();
		else
			prev_4017_write = total_cycles();

		if (suppress) {
			rmw_1 = false;
			cpu_data_bus = val;
			cpu_external_bus = val;
			return;
		}
	}

	rmw_1 = false; 
	m_mintf->write(adr, val); 
	cpu_data_bus = val;
	cpu_external_bus = val;
}

void rp2a03_core_device::write_1(uint16_t adr, uint8_t val) { 
	if (adr == 0x2000 && !m_mmc5_ppuctrl_write.isnull()) {
		m_mmc5_ppuctrl_write(val);
	}
	else if (adr == 0x2001 && !m_mmc5_ppumask_write.isnull()) {
		m_mmc5_ppumask_write(val);
	}
	else if (adr == 0x4014 && !m_mmc5_reset_scanline_irq.isnull()) {
		m_mmc5_reset_scanline_irq();
	}
	
	write_cycles_since_dma_halt_request++; 
	adr_bus = adr;
	rmw_1 = true; 
	cpu_is_reading = false; 
	
	// Some cartridge protection logic, including BMW8544 / mapper 292,
	// observes the last data byte driven by a CPU write, not just writes to
	// the mapper's own address range.  Keep this CPU-side so cart code can
	// query the true last CPU write without coupling the CPU to a mapper.
	last_cpu_write_latch = val;
	m_previous_cpu_write_cycle = m_last_cpu_write_cycle;
	m_last_cpu_write_cycle = total_cycles();
	if (adr == 0x4016 || adr == 0x4017) {
		if (adr == 0x4016)
			prev_4016_write = total_cycles();
		else
			prev_4017_write = total_cycles();
	}

	m_mintf->write(adr, val); 
	cpu_data_bus = val; 
	cpu_external_bus = val;
}

void rp2a03_core_device::write_9(uint16_t adr, uint8_t val) { 
	if (adr == 0x2000 && !m_mmc5_ppuctrl_write.isnull()) {
		m_mmc5_ppuctrl_write(val);
	}
	else if (adr == 0x2001 && !m_mmc5_ppumask_write.isnull()) {
		m_mmc5_ppumask_write(val);
	}
	else if (adr == 0x4014 && !m_mmc5_reset_scanline_irq.isnull()) {
		m_mmc5_reset_scanline_irq();
	}

	write_cycles_since_dma_halt_request++; 
	rmw_1 = false; 
	adr_bus = adr;
	cpu_is_reading = false; 
	
	// Some cartridge protection logic, including BMW8544 / mapper 292,
	// observes the last data byte driven by a CPU write, not just writes to
	// the mapper's own address range.  Keep this CPU-side so cart code can
	// query the true last CPU write without coupling the CPU to a mapper.
	last_cpu_write_latch = val;
	m_previous_cpu_write_cycle = m_last_cpu_write_cycle;
	m_last_cpu_write_cycle = total_cycles();
	if (adr == 0x4016) {
		if ((total_cycles() - prev_4016_write == 1) && get_apu_clk1_is_high() && !(val & 1)) {
			//osd_printf_info("0x4016 Detected: Controllers should not be strobed when the CPU transitions from a \"put\" cycle to a \"get\" cycle.\n");
			cpu_data_bus = val;
			cpu_external_bus = val;
			return;
		}
		prev_4016_write = total_cycles();
	}
	else if (adr == 0x4017) {
		if ((total_cycles() - prev_4017_write == 1) && get_apu_clk1_is_high() && !(val & 1)) {
			//osd_printf_info("0x4017 Detected: Controllers should not be strobed when the CPU transitions from a \"put\" cycle to a \"get\" cycle.\n");
			cpu_data_bus = val;
			cpu_external_bus = val;
			return;
		}
		prev_4017_write = total_cycles();
	}

	m_mintf->write_9(adr, val); 
	cpu_data_bus = val; 
	cpu_external_bus = val;
}


uint8_t rp2a03_core_device::read_arg(uint16_t adr)
{
	if ((adr == 0xfffa || adr == 0xfffb) && !m_mmc5_reset_scanline_irq.isnull()) {
		m_mmc5_reset_scanline_irq();
	}
	
	adr_bus = adr;
	prevReadAddress = adr;
	set_var_read();

	if (!m_famicom_controller_timing && (adr == 0x4016 || adr == 0x4017))
	{
		int64_t tc = suspended() ? (total_cycles() - 1) : total_cycles();

		if (adr == 0x4016)
		{
			if (tc - prev_4016_read == 1)
			{
				prev_4016_read = tc;
				return last_4016_val;
			}
			prev_4016_read = tc;
		}
		else
		{
			if (tc - prev_4017_read == 1)
			{
				prev_4017_read = tc;
				return last_4017_val;
			}
			prev_4017_read = tc;
		}
	}

	if (is_open_bus_address(adr))
	{
		logerror("OPENBUS DATA: PC=%04X IR=%02X address=%04X value=%02X\n", m_PC, m_IR, adr, cpu_external_bus);
		cpu_data_bus = cpu_external_bus;
		return cpu_data_bus;
	}

	if (adr == 0x4015) {
		cpu_data_bus =  m_mintf->read_arg(adr);
		return cpu_data_bus;
	}

	cpu_data_bus = m_mintf->read_arg(adr);
	cpu_external_bus = cpu_data_bus;
		
	// Cache the real returned values so the double-read returns the SAME byte later
	if (adr == 0x4016)
		last_4016_val = cpu_data_bus;
	else if (adr == 0x4017)
		last_4017_val = cpu_data_bus;
	
	return cpu_data_bus;
}


uint8_t rp2a03_core_device::read_pc()
{
	set_var_read();

	uint16_t adr = m_PC;
	adr_bus = adr;
	prevReadAddress = adr;
	
	if ((adr == 0xfffa || adr == 0xfffb) && !m_mmc5_reset_scanline_irq.isnull()) {
		m_mmc5_reset_scanline_irq();
	}
		
	if (!m_famicom_controller_timing && (adr == 0x4016 || adr == 0x4017))
	{
		int64_t tc = suspended() ? (total_cycles() - 1) : total_cycles();

		if (adr == 0x4016)
		{
			if (tc - prev_4016_read == 1)
			{
				prev_4016_read = tc;
				return last_4016_val;
			}
			prev_4016_read = tc;
		}
		else
		{
			if (tc - prev_4017_read == 1)
			{
				prev_4017_read = tc;
				return last_4017_val;
			}
			prev_4017_read = tc;
		}
	}

	if (is_open_bus_address(adr))
	{
		logerror("OPENBUS OPCODE: PC=%04X IR=%02X address=%04X value=%02X\n", m_PC, m_IR, adr, cpu_external_bus);
		cpu_data_bus = cpu_external_bus;
		return cpu_data_bus;
	}

	if (adr == 0x4015) {
		cpu_data_bus = m_mintf->read_arg(adr);
		return cpu_data_bus;
	}

	cpu_data_bus = m_mintf->read_arg(adr);
	cpu_external_bus = cpu_data_bus;
		
	// Cache the real returned values so the double-read returns the SAME byte later
	if (adr == 0x4016)
		last_4016_val = cpu_data_bus;
	else if (adr == 0x4017)
		last_4017_val = cpu_data_bus;
		
	return cpu_data_bus;
}


uint8_t rp2a03_core_device::read_sync(uint16_t adr)
{
	if ((adr == 0xfffa || adr == 0xfffb) && !m_mmc5_reset_scanline_irq.isnull()) {
		m_mmc5_reset_scanline_irq();
	}
		
	adr_bus = adr;
	prevReadAddress = adr;
	set_var_read();

	if (!m_famicom_controller_timing && (adr == 0x4016 || adr == 0x4017))
	{
		int64_t tc = suspended() ? (total_cycles() - 1) : total_cycles();

		if (adr == 0x4016)
		{
			if (tc - prev_4016_read == 1)
			{
				prev_4016_read = tc;
				return last_4016_val;
			}
			prev_4016_read = tc;
		}
		else
		{
			if (tc - prev_4017_read == 1)
			{
				prev_4017_read = tc;
				return last_4017_val;
			}
			prev_4017_read = tc;
		}
	}

	if (is_open_bus_address(adr))
	{
		logerror("OPENBUS OPCODE: PC=%04X IR=%02X address=%04X value=%02X\n", m_PC, m_IR, adr, cpu_external_bus);
		cpu_data_bus = cpu_external_bus;
		return cpu_data_bus;
	}

	if (adr == 0x4015) {
		cpu_data_bus = m_mintf->read_sync(adr);
		return cpu_data_bus;
	}

	cpu_data_bus = m_mintf->read_sync(adr);
	cpu_external_bus = cpu_data_bus;
		
	// Cache the real returned values so the double-read returns the SAME byte later
	if (adr == 0x4016)
		last_4016_val = cpu_data_bus;
	else if (adr == 0x4017)
		last_4017_val = cpu_data_bus;
		
	return cpu_data_bus;
}

void rp2a03_core_device::set_var_read() { 
	rmw_1 = false; 
	cpu_is_reading = true; 
}


void rp2a03_core_device::run_suspended_cpu_dma_cycle() { 	
	if(m_inst_substate) {
		do_exec_partial();
		//if(suspended())
			m_inst_substate--;
	}
	if(m_sync) {
		next_IR=m_IR;
		m_IR=prev_IR;
	}
}

void rp2a03_core_device::do_halt() {
	// Fast path: no DMA halt pending.
	if (!dmc_halt && !oam_halt)
		return;
	if(dmc_halt) {
		if(!get_is_pal()) {
			if(dmc_dma_reload) {
				if(!apu_clk1_is_high && write_cycles_since_dma_halt_request == 0 ) {
					suspend(SUSPEND_REASON_HALT,1);
					return;
				}
				if (write_cycles_since_dma_halt_request > 0) {
					suspend(SUSPEND_REASON_HALT,1);
					return;
				}
			} else {
				if (apu_clk1_is_high && write_cycles_since_dma_halt_request == 0) {
					suspend(SUSPEND_REASON_HALT,1);
					return;
				}
				if (write_cycles_since_dma_halt_request > 0) {
					suspend(SUSPEND_REASON_HALT,1);
					return;
				}
			}
		} else {
			if (m_sync) {
				suspend(SUSPEND_REASON_HALT,1);
				return;
			}			
		} 
	}
	if(oam_halt) {
		suspend(SUSPEND_REASON_HALT,1);
		return;
	}
}


void rp2a03_core_device::dmc_halt_next_read(bool a, bool b) {
	dmc_halt = true; 
	dmc_dma_reload = a; 
	write_cycles_since_dma_halt_request = 0; 
	dmc_dma_explicit_stop = b; 
}


uint16_t rp2a03_core_device::get_prevReadAddress () { 
	return prevReadAddress; 
}


void rp2a03_core_device::mark_oam_dma_halt_cycle() { 
	oam_dma_halt_cycle = total_cycles(); 
}

void rp2a03_core_device::oam_halt_next_read() { 
	oam_halt = true; 
}


void rp2a03_core_device::dmc_clear_halt() { 
	dmc_halt = false; 
	dmc_dma_explicit_stop = false; 
}


void rp2a03_core_device::oam_clear_halt() { 
	oam_halt = false; 
}


uint8_t rp2a03_core_device::read_4016_4017(uint16_t adr) {
	return read(adr); 
}


uint16_t rp2a03_core_device::get_adr_bus() { 
	return adr_bus; 
}


uint8_t rp2a03_core_device::get_open_bus() {
	return cpu_external_bus;
}


uint8_t rp2a03_core_device::get_data_bus()
{
	return cpu_data_bus;
}


void rp2a03_core_device::set_open_bus(uint8_t x) {
	cpu_external_bus = x;
}


void rp2a03_core_device::set_data_bus(uint8_t x)
{
	cpu_data_bus = x;
}


void rp2a03_core_device::set_m_exram_control(int x) { 
	m_exram_control = x; 
}


//void rp2a03_core_device::set_is_mmc5(bool x)
//{
//	is_mmc5 = x;
//}


bool rp2a03_core_device::get_apu_clk1_is_high() { 
	return apu_clk1_is_high; 
}


bool rp2a03_core_device::get_cpu_is_reading() { 
	return cpu_is_reading; 
}


bool rp2a03_core_device::get_rmw_1() { 
	return rmw_1; 
}


void rp2a03_core_device::set_open_bus_ranges(const uint32_t *ranges, int count)	{
	m_ob_count = (count > MAX_OPEN_BUS_RANGES) ? MAX_OPEN_BUS_RANGES : count;
	for (int i = 0; i < m_ob_count; i++)
		m_open_bus_ranges[i] = ranges[i];
}


bool rp2a03_core_device::is_open_bus_address(uint16_t adr) const
{
	if (m_mmc5_reset_scanline_irq.isnull() && m_ob_count == 0)
		return false;
	
	// MMC5: open bus only in ExRAM modes 0 and 1
	if(!m_mmc5_reset_scanline_irq.isnull()) {
		if (adr >= 0x5C00 && adr <= 0x5FFF) {
			if (m_exram_control == 0 || m_exram_control == 1) {
				logerror("open-bus MMC5 read @ %04x\n", adr);
				return true;
			}
		}
	}
		
	// ignore real APU registers
	if (adr == 0x4015 || adr == 0x4016 || adr == 0x4017)
		return false;

	for (int i = 0; i < m_ob_count; i++) {
		uint32_t r = m_open_bus_ranges[i];
		uint16_t start = r >> 16;
		uint16_t end   = r & 0xFFFF;

		if (adr >= start && adr <= end) {
			//logerror("open-bus read @ %04x\n", adr);
			return true;
		}
	}
	return false;
}


#include "cpu/m6502/rp2a03.hxx"
