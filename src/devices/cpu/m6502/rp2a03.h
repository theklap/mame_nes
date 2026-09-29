// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
/***************************************************************************

    rp2a03.h

    6502, NES variant

***************************************************************************/

#ifndef MAME_CPU_M6502_RP2A03_H
#define MAME_CPU_M6502_RP2A03_H

#pragma once

#include "m6502.h"
#include "sound/nes_apu.h"

class rp2a03_core_device : public m6502_device {
public:
    typedef device_delegate<void ()> mmc5_reset_scanline_irq_delegate;
    typedef device_delegate<void (uint8_t data)> mmc5_register_write_delegate;

    rp2a03_core_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

    virtual std::unique_ptr<util::disasm_interface> create_disassembler() override;
    virtual void do_exec_full() override;
    virtual void do_exec_partial() override;

    bool get_apu_clk1_is_high();
    bool get_cpu_is_reading();
    bool get_rmw_1();
    uint8_t read_4016_4017(uint16_t adr);
    uint16_t get_adr_bus();
    uint8_t get_open_bus();
    uint8_t get_data_bus();
    void set_data_bus(uint8_t data);
    void set_open_bus(uint8_t data);
    void dmc_halt_next_read(bool reload, bool explicit_stop);
    void oam_halt_next_read();
    void dmc_clear_halt();
    void oam_clear_halt();
    uint16_t get_prevReadAddress();
    void run_suspended_cpu_dma_cycle();
    void queue_delayed_nmi(int cycles);
    void cancel_delayed_nmi();
    void queue_delayed_mapper_irq(int cycles);
    void cancel_delayed_mapper_irq();
    bool get_is_pal() const { return is_pal; }
    void set_is_pal(bool state) { is_pal = state; }
    void set_famicom_controller_timing(bool state) { m_famicom_controller_timing = state; }
    int64_t get_previous_cpu_write_cycle() const { return m_previous_cpu_write_cycle; }
    int64_t get_last_cpu_write_cycle() const { return m_last_cpu_write_cycle; }
    uint8_t get_last_cpu_write_latch() const { return last_cpu_write_latch; }
    void set_m_exram_control(int value);
    void set_open_bus_ranges(const uint32_t *ranges, int count);
    bool is_open_bus_address(uint16_t adr) const;

    template <typename... T>
    void set_mmc5_reset_scanline_irq(T &&... args) {
        m_mmc5_reset_scanline_irq.set(std::forward<T>(args)...);
        m_mmc5_reset_scanline_irq.resolve();
    }

    template <typename... T>
    void set_mmc5_ppuctrl_write(T &&... args) {
        m_mmc5_ppuctrl_write.set(std::forward<T>(args)...);
        m_mmc5_ppuctrl_write.resolve();
    }

    template <typename... T>
    void set_mmc5_ppumask_write(T &&... args) {
        m_mmc5_ppumask_write.set(std::forward<T>(args)...);
        m_mmc5_ppumask_write.resolve();
    }

protected:
    rp2a03_core_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock);

    virtual void init() override;
    virtual void device_start() override ATTR_COLD;
    virtual void device_reset() override ATTR_COLD;
    virtual void execute_run() override;
    virtual void execute_set_input(int inputnum, int state) override;
    virtual space_config_vector memory_space_config() const override;
    virtual void state_import(const device_state_entry &entry) override;
    virtual void state_export(const device_state_entry &entry) override;
    virtual void state_string_export(const device_state_entry &entry, std::string &str) const override;
    virtual void prefetch_end() override;

    uint8_t read(uint16_t adr);
    uint8_t read_9(uint16_t adr);
    void write(uint16_t adr, uint8_t val);
    void write_1(uint16_t adr, uint8_t val);
    void write_9(uint16_t adr, uint8_t val);
    uint8_t read_arg(uint16_t adr);
    uint8_t read_pc();
    uint8_t read_pc_noirq() { return read_arg(m_PC); }
    uint8_t read_arg_noirq(uint16_t adr) { return read_arg(adr); }
    uint8_t read_sync(uint16_t adr);
    void set_var_read();
    void prefetch_start();
    void prefetch_end_noirq();
    void do_halt();
    void handle_dma_rdy_stall();
    void mark_oam_dma_halt_cycle();
    bool is_branch_opcode(u8 op);
    bool is_irq_oam_dma_window();
    bool branch_delay_match_for_new_irq(int state);
    bool branch_need_irq_match();
    bool dma_window_interrupt_eligible();

#define O(o) void o ## _full(); void o ## _partial()

	// NMOS 6502 opcodes
	// documented opcodes
	O(adc_aba); O(adc_abx); O(adc_aby); O(adc_idx); O(adc_idy); O(adc_imm); O(adc_zpg); O(adc_zpx);
	O(and_aba); O(and_abx); O(and_aby); O(and_imm); O(and_idx); O(and_idy); O(and_zpg); O(and_zpx);
	O(asl_aba); O(asl_abx); O(asl_acc); O(asl_zpg); O(asl_zpx);
	O(bcc_rel);
	O(bcs_rel);
	O(beq_rel);
	O(bit_aba); O(bit_zpg);
	O(bmi_rel);
	O(bne_rel);
	O(bpl_rel);
	O(brk_imp);
	O(bvc_rel);
	O(bvs_rel);
	O(clc_imp);
	O(cld_imp);
	O(cli_imp);
	O(clv_imp);
	O(cmp_aba); O(cmp_abx); O(cmp_aby); O(cmp_idx); O(cmp_idy); O(cmp_imm); O(cmp_zpg); O(cmp_zpx);
	O(cpx_aba); O(cpx_imm); O(cpx_zpg);
	O(cpy_aba); O(cpy_imm); O(cpy_zpg);
	O(dec_aba); O(dec_abx); O(dec_zpg); O(dec_zpx);
	O(dex_imp);
	O(dey_imp);
	O(eor_aba); O(eor_abx); O(eor_aby); O(eor_idx); O(eor_idy); O(eor_imm); O(eor_zpg); O(eor_zpx);
	O(inc_aba); O(inc_abx); O(inc_zpg); O(inc_zpx);
	O(inx_imp);
	O(iny_imp);
	O(jmp_adr); O(jmp_ind);
	O(jsr_adr);
	O(lda_aba); O(lda_abx); O(lda_aby); O(lda_idx); O(lda_idy); O(lda_imm); O(lda_zpg); O(lda_zpx);
	O(ldx_aba); O(ldx_aby); O(ldx_imm); O(ldx_zpg); O(ldx_zpy);
	O(ldy_aba); O(ldy_abx); O(ldy_imm); O(ldy_zpg); O(ldy_zpx);
	O(lsr_aba); O(lsr_abx); O(lsr_acc); O(lsr_zpg); O(lsr_zpx);
	O(nop_imp);
	O(ora_aba); O(ora_abx); O(ora_aby); O(ora_imm); O(ora_idx); O(ora_idy); O(ora_zpg); O(ora_zpx);
	O(pha_imp);
	O(php_imp);
	O(pla_imp);
	O(plp_imp);
	O(rol_aba); O(rol_abx); O(rol_acc); O(rol_zpg); O(rol_zpx);
	O(ror_aba); O(ror_abx); O(ror_acc); O(ror_zpg); O(ror_zpx);
	O(rti_imp);
	O(rts_imp);
	O(sbc_aba); O(sbc_abx); O(sbc_aby); O(sbc_idx); O(sbc_idy); O(sbc_imm); O(sbc_zpg); O(sbc_zpx);
	O(sec_imp);
	O(sed_imp);
	O(sei_imp);
	O(sta_aba); O(sta_abx); O(sta_aby); O(sta_idx); O(sta_idy); O(sta_zpg); O(sta_zpx);
	O(stx_aba); O(stx_zpg); O(stx_zpy);
	O(sty_aba); O(sty_zpg); O(sty_zpx);
	O(tax_imp);
	O(tay_imp);
	O(tsx_imp);
	O(txa_imp);
	O(txs_imp);
	O(tya_imp);

	// exceptions
	O(reset);

	// undocumented reliable instructions
	O(dcp_aba); O(dcp_abx); O(dcp_aby); O(dcp_idx); O(dcp_idy); O(dcp_zpg); O(dcp_zpx);
	O(isb_aba); O(isb_abx); O(isb_aby); O(isb_idx); O(isb_idy); O(isb_zpg); O(isb_zpx);
	O(lax_aba); O(lax_aby); O(lax_idx); O(lax_idy); O(lax_zpg); O(lax_zpy);
	O(rla_aba); O(rla_abx); O(rla_aby); O(rla_idx); O(rla_idy); O(rla_zpg); O(rla_zpx);
	O(rra_aba); O(rra_abx); O(rra_aby); O(rra_idx); O(rra_idy); O(rra_zpg); O(rra_zpx);
	O(sax_aba); O(sax_idx); O(sax_zpg); O(sax_zpy);
	O(sbx_imm);
	O(sha_aby); O(sha_idy);
	O(shs_aby);
	O(shx_aby);
	O(shy_abx);
	O(slo_aba); O(slo_abx); O(slo_aby); O(slo_idx); O(slo_idy); O(slo_zpg); O(slo_zpx);
	O(sre_aba); O(sre_abx); O(sre_aby); O(sre_idx); O(sre_idy); O(sre_zpg); O(sre_zpx);

	// undocumented unreliable instructions
	//   behaviour differs between visual6502 and online docs, which
	//   is a clear sign reliability is not to be expected
	//   implemented version follows visual6502
	O(anc_imm);
	O(ane_imm);
	O(arr_imm);
	O(asr_imm);
	O(las_aby);
	O(lxa_imm);

	// nop variants
	O(nop_imm); O(nop_aba); O(nop_abx); O(nop_zpg); O(nop_zpx);

	// system killers
	O(kil_non);

    O(adc_nd_aba); O(adc_nd_abx); O(adc_nd_aby); O(adc_nd_idx); O(adc_nd_idy); O(adc_nd_imm); O(adc_nd_zpg); O(adc_nd_zpx);
    O(arr_nd_imm);
    O(isb_nd_aba); O(isb_nd_abx); O(isb_nd_aby); O(isb_nd_idx); O(isb_nd_idy); O(isb_nd_zpg); O(isb_nd_zpx);
    O(rra_nd_aba); O(rra_nd_abx); O(rra_nd_aby); O(rra_nd_idx); O(rra_nd_idy); O(rra_nd_zpg); O(rra_nd_zpx);
    O(sbc_nd_aba); O(sbc_nd_abx); O(sbc_nd_aby); O(sbc_nd_idx); O(sbc_nd_idy); O(sbc_nd_imm); O(sbc_nd_zpg); O(sbc_nd_zpx);

#undef O

    mmc5_reset_scanline_irq_delegate m_mmc5_reset_scanline_irq;
    mmc5_register_write_delegate m_mmc5_ppuctrl_write;
    mmc5_register_write_delegate m_mmc5_ppumask_write;

    uint8_t prev_IR;
    uint8_t next_IR;
    bool cpu_is_reading;
    uint8_t cpu_data_bus;
    uint8_t cpu_external_bus;
    uint16_t adr_bus;
    int delay;
    bool nmi_pending_1;
    bool irq_delay;
    bool apu_irq_delay;
    bool nmi_delay;
    bool branched;
    bool paged;
    int64_t oam_dma_halt_cycle;
    bool rmw_1;
    bool apu_irq_branch_delay;
    bool irq_branch_delay;
    bool nmi_branch_delay;
    bool apu_clk1_is_high;
    bool dmc_halt;
    bool oam_halt;
    bool dmc_dma_explicit_stop;
    int write_cycles_since_dma_halt_request;
    bool dmc_dma_reload;
    bool m_famicom_controller_timing;
    int64_t prev_4016_write;
    int64_t prev_4017_write;
    int64_t prev_4016_read;
    int64_t prev_4017_read;
    uint8_t last_4016_val;
    uint8_t last_4017_val;
    bool inst_halted;
    bool next_read;
    bool prev_next_read;
    bool need_irq;
    uint16_t prevReadAddress;
    int m_exram_control;
    int64_t nmi_cpu_cycle;
    bool nmi_overlap_brk_irq;
    bool m_real_brk;
    uint8_t last_cpu_write_latch;
    int64_t m_previous_cpu_write_cycle;
    int64_t m_last_cpu_write_cycle;
    bool mapper_irq;
    int mapper_irq_delay;
    int64_t mapper_irq_cpu_cycle;
    static constexpr int MAX_OPEN_BUS_RANGES = 16;
    uint32_t m_open_bus_ranges[MAX_OPEN_BUS_RANGES];
    int m_ob_count;
    bool is_pal;
};

class rp2a03_device : public rp2a03_core_device, public device_mixer_interface {
public:
    rp2a03_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
    void rp2a03_map(address_map &map);

protected:
    rp2a03_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock);
    required_device<nesapu_device> m_apu;
    virtual void device_add_mconfig(machine_config &config) override;
    void apu_irq(int state);
    uint8_t apu_read_mem(offs_t offset);
};

class rp2a03g_device : public rp2a03_device {
public:
    rp2a03g_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

protected:
    virtual void device_add_mconfig(machine_config &config) override;
};

#define RP2A03_NTSC_XTAL XTAL(21'477'272)
#define RP2A03_PAL_XTAL XTAL(26'601'712)
#define NTSC_APU_CLOCK (RP2A03_NTSC_XTAL/12)
#define PAL_APU_CLOCK (RP2A03_PAL_XTAL/16)
#define PALC_APU_CLOCK (RP2A03_PAL_XTAL/15)

enum {
    RP2A03_IRQ_LINE = m6502_device::IRQ_LINE,
    RP2A03_APU_IRQ_LINE = m6502_device::APU_IRQ_LINE,
    RP2A03_NMI_LINE = m6502_device::NMI_LINE,
    RP2A03_SET_OVERFLOW = m6502_device::V_LINE
};

DECLARE_DEVICE_TYPE(RP2A03_CORE, rp2a03_core_device)
DECLARE_DEVICE_TYPE(RP2A03, rp2a03_device)
DECLARE_DEVICE_TYPE(RP2A03G, rp2a03g_device)

#endif // MAME_CPU_M6502_RP2A03_H
