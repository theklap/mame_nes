// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Tengen PCBs


 Here we emulate the following PCBs

 * Tengen 800032 [mapper 64]
 * Tengen 800037 [mapper 158]

 Note, Tetris' Tengen 800008 [mapper 148] is implemented in sachen.cpp.

 ***********************************************************************************************************/


#include "emu.h"
#include "tengen.h"
#include "cpu/m6502/m6502.h"

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_TENGEN_800032, nes_tengen032_device, "nes_tengen032", "NES Cart Tengen 800032 PCB")
DEFINE_DEVICE_TYPE(NES_TENGEN_800037, nes_tengen037_device, "nes_tengen037", "NES Cart Tengen 800037 PCB")


nes_tengen032_device::nes_tengen032_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, type, tag, owner, clock)
	, m_latch(0)
	, m_irq_count(0)
	, m_irq_count_latch(0)
	, m_irq_mode(0)
	, m_irq_reset(0)
	, m_irq_enable(0)
	, delay_irq(0)
	, m_irq_delay_cpu_cycle(0)
	, m_irq_cpu_delay(0)
	, irq_timer(nullptr)
	, m_prev_ppu_addr(0)
	, m_maincpu6502(nullptr)
{
}

nes_tengen032_device::nes_tengen032_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_tengen032_device(mconfig, NES_TENGEN_800032, tag, owner, clock)
{
}

nes_tengen037_device::nes_tengen037_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_tengen032_device(mconfig, NES_TENGEN_800037, tag, owner, clock)
{
}

void nes_tengen032_device::device_start()
{
	common_start();

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	irq_timer = timer_alloc(FUNC(nes_tengen032_device::irq_timer_tick), this);
	timer_freq = clocks_to_attotime(4);
	irq_timer->adjust(attotime::never);
	
	a12_timer = timer_alloc(FUNC(nes_tengen032_device::a12_timer_tick), this);
	a12_timer_freq = clocks_to_attotime(1);
	a12_timer->adjust(attotime::never);

	save_item(NAME(m_mmc_prg_bank));
	save_item(NAME(m_mmc_vrom_bank));
	save_item(NAME(m_latch));

	save_item(NAME(m_irq_mode));
	save_item(NAME(m_irq_reset));
	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_count_latch));
	save_item(NAME(delay_irq));
	save_item(NAME(m_irq_delay_cpu_cycle));
	save_item(NAME(m_irq_cpu_delay));

	save_item(NAME(m_prev_ppu_addr));
	save_item(NAME(m_a12_m2_counter));
	save_item(NAME(m_irq_reload_extra));
	save_item(NAME(m_irq_direct_after_mode_switch));
}

void nes_tengen032_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	std::fill(std::begin(m_mmc_prg_bank), std::end(m_mmc_prg_bank), 0x00);
	std::fill(std::begin(m_mmc_vrom_bank), std::end(m_mmc_vrom_bank), 0x00);

	m_latch = 0;

	m_irq_mode = 0;
	m_irq_reset = 0;
	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_count_latch = 0xff;
	delay_irq = 0;
	m_irq_delay_cpu_cycle = 0;
	m_irq_cpu_delay = 0;

	m_prev_ppu_addr = 0;

	m_a12_m2_counter = 0;
	m_irq_reload_extra = 0;
	m_irq_direct_after_mode_switch = false;
	
	const attotime m2_falling_edge = attotime::from_ticks(1, clock() * 2);
	a12_timer->adjust(m2_falling_edge, 0, a12_timer_freq);
	irq_timer->adjust(attotime::never);

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}

void nes_tengen037_device::pcb_reset() {
	nes_tengen032_device::pcb_reset();
	set_chr();
}

/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 Tengen 800032 Board

 Games: Klax, Road Runner, Rolling Thunder, Shinobi, Skulls
 & Crossbones, Xybots

 This is very similar to MMC-3 (or more probably to the
 Namcot predecessor of MMC-3), but with more registers
 and with an alternative IRQ mode

 iNES: mapper 64

 In MAME: Supported.

 -------------------------------------------------*/
void nes_tengen032_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) {
	if (m_irq_cpu_delay > 0) {
		const u64 current_cpu_cycle = m_maincpu6502->total_cycles();
		const u64 elapsed_cpu_cycles = current_cpu_cycle - m_irq_delay_cpu_cycle;

		if (elapsed_cpu_cycles > 0) {
			if (elapsed_cpu_cycles >= u64(m_irq_cpu_delay)) {
				m_irq_cpu_delay = 0;
				m_irq_delay_cpu_cycle = 0;

				if (m_irq_direct_after_mode_switch) {
					m_irq_direct_after_mode_switch = false;
					set_irq_line(ASSERT_LINE);
				} else {
					delay_irq = 2;
				}
			} else {
				m_irq_cpu_delay -= int(elapsed_cpu_cycles);
				m_irq_delay_cpu_cycle = current_cpu_cycle;
			}
		}
	} else if (delay_irq > 0) {
		--delay_irq;

		if (delay_irq == 0) {
			m_maincpu6502->queue_delayed_mapper_irq(2);
			delay_irq = -1;
		}
	}
}

inline void nes_tengen032_device::irq_clock() {
	const bool reloading = m_irq_reset;
	bool trigger_irq = false;

	if (m_irq_count == 0) {
		m_irq_count = m_irq_count_latch | (reloading ? m_irq_reload_extra : 0);

		if (reloading && m_irq_count == 0 && m_irq_enable)
			trigger_irq = true;
	} else {
		--m_irq_count;

		if (m_irq_count == 0 && m_irq_enable)
			trigger_irq = true;
	}

	m_irq_reset = 0;

	if (trigger_irq && m_irq_cpu_delay == 0 && delay_irq == 0) {
		m_irq_cpu_delay = m_irq_mode ? 4 : 2;
		m_irq_delay_cpu_cycle = m_maincpu6502->total_cycles();
	}
}

void nes_tengen032_device::ppu_bus_address(uint16_t ppu_addr, uint64_t ppu_cycles, int ppu_tick, bool odd_frame) {
	m_prev_ppu_addr = ppu_addr;
}

TIMER_CALLBACK_MEMBER(nes_tengen032_device::a12_timer_tick) {
	if (BIT(m_prev_ppu_addr, 12)) {
		if (!m_irq_mode && m_a12_m2_counter == 0) {
			irq_clock();
		}

		m_a12_m2_counter = 16;
	} else if (m_a12_m2_counter > 0) {
		--m_a12_m2_counter;
	}
}

TIMER_CALLBACK_MEMBER(nes_tengen032_device::irq_timer_tick) {
	if (m_irq_mode)
		irq_clock();
}

void nes_tengen032_device::set_prg()
{
	u8 prg_flip = (m_latch & 0x40) >> 5;

	prg8_89(m_mmc_prg_bank[0 ^ prg_flip]);
	prg8_ab(m_mmc_prg_bank[1]);
	prg8_cd(m_mmc_prg_bank[2 ^ prg_flip]);
}

void nes_tengen032_device::set_chr()
{
	u8 chr_flip = (m_latch & 0x80) >> 5;

	if (m_latch & 0x20)
	{
		chr1_x(0 ^ chr_flip, m_mmc_vrom_bank[0], CHRROM);
		chr1_x(1 ^ chr_flip, m_mmc_vrom_bank[6], CHRROM);
		chr1_x(2 ^ chr_flip, m_mmc_vrom_bank[1], CHRROM);
		chr1_x(3 ^ chr_flip, m_mmc_vrom_bank[7], CHRROM);
	}
	else
	{
		chr2_x(0 ^ chr_flip, m_mmc_vrom_bank[0] >> 1, CHRROM);
		chr2_x(2 ^ chr_flip, m_mmc_vrom_bank[1] >> 1, CHRROM);
	}

	chr1_x(4 ^ chr_flip, m_mmc_vrom_bank[2], CHRROM);
	chr1_x(5 ^ chr_flip, m_mmc_vrom_bank[3], CHRROM);
	chr1_x(6 ^ chr_flip, m_mmc_vrom_bank[4], CHRROM);
	chr1_x(7 ^ chr_flip, m_mmc_vrom_bank[5], CHRROM);
}

void nes_tengen032_device::write_h(offs_t offset, u8 data)
{
	u8 helper, cmd;
	LOG("tengen032 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x6001)
	{
		case 0x0000:
			helper = m_latch ^ data;
			m_latch = data;

			// Has PRG Mode changed?
			if (helper & 0x40)
				set_prg();

			// Has CHR Mode changed?
			if (helper & 0xa0)
				set_chr();
			break;

		case 0x0001:
			cmd = m_latch & 0x0f;
			switch (cmd)
			{
				case 0: case 1:
				case 2: case 3:
				case 4: case 5:
					m_mmc_vrom_bank[cmd] = data;
					set_chr();
					break;
				case 6: case 7:
					m_mmc_prg_bank[cmd - 6] = data;
					set_prg();
					break;
				case 8: case 9:
					m_mmc_vrom_bank[cmd - 2] = data;
					set_chr();
					break;
				case 0x0f:
					m_mmc_prg_bank[2] = data;
					set_prg();
					break;
			}
			break;

		case 0x2000:
			set_nt_mirroring(BIT(data, 0) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			break;

		case 0x4000: // $C000 - IRQ reload value
			m_irq_count_latch = data;
			break;

		case 0x4001: {
			const bool old_irq_mode = m_irq_mode;

			m_irq_mode = BIT(data, 0);
			m_irq_count = 0;
			m_irq_reset = 1;
			m_irq_reload_extra = m_a12_m2_counter ? 0 : 1;
			m_irq_direct_after_mode_switch = old_irq_mode && !m_irq_mode;

			if (m_irq_mode)
				irq_timer->adjust(timer_freq, 0, timer_freq);
			else
				irq_timer->adjust(attotime::never);

			break;
		}

		case 0x6000: // $E000 - acknowledge and disable
			m_irq_enable = 0;
			m_irq_cpu_delay = 0;
			m_irq_delay_cpu_cycle = 0;
			delay_irq = 0;
			m_irq_direct_after_mode_switch = false;
			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			break;

		case 0x6001: // $E001 - enable
			m_irq_enable = 1;
			break;

		default:
			LOG("Tengen 800032 write. addr: %04x value: %02x\n", offset + 0x8000, data);
			break;
	}
}

/*-------------------------------------------------

 Tengen 800037 Board

 Games: Alien Syndrome

 Same as above (mapper chip RAMBO-1) but CHR A17 output
 is connected to CIRAM A10, so this differs from 800032
 exactly in the same way as TLSROM differs from plain
 MMC-3

 iNES: mapper 158

 In MAME: Supported.

 -------------------------------------------------*/

void nes_tengen037_device::set_chr() {
	nes_tengen032_device::set_chr();

	// Mapper 158 connects RAMBO-1 CHR A17 to CIRAM A10.
	//
	// When K=0, commands 0 and 1 select paired 2 KiB banks.
	// When K=1, commands 8 and 9 provide the second 1 KiB bank
	// for each pair.
	static constexpr u8 bank_2k[8] = { 0, 0, 1, 1, 2, 3, 4, 5 };
	static constexpr u8 bank_1k[8] = { 0, 6, 1, 7, 2, 3, 4, 5 };

	const u8 *const bank = BIT(m_latch, 5) ? bank_1k : bank_2k;
	const int start = BIT(m_latch, 7) ? 4 : 0;

	for (int i = 0; i < 4; i++)
		set_nt_page(i, CIRAM, BIT(m_mmc_vrom_bank[bank[start + i]], 7), 1);
}

void nes_tengen037_device::write_h(offs_t offset, u8 data) {
	LOG("tengen037 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x6001) {
		case 0x2000:
			// Mapper 158 controls CIRAM A10 through CHR A17,
			// so the normal RAMBO-1 mirroring register is disconnected.
			break;

		default:
			nes_tengen032_device::write_h(offset, data);
			break;
	}
}
