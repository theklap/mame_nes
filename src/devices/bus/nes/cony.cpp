// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Cony/Yoko PCBs


 Here we emulate the following PCBs

 * UNL-CONY [mapper 83]
 * UNL-YOKO [mapper 264]

 ***********************************************************************************************************/


#include "emu.h"
#include "cony.h"


#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_CONY,   nes_cony_device,   "nes_cony",   "NES Cart Cony PCB")
DEFINE_DEVICE_TYPE(NES_CONY1K, nes_cony1k_device, "nes_cony1k", "NES Cart Cony 1K PCB")
DEFINE_DEVICE_TYPE(NES_YOKO,   nes_yoko_device,   "nes_yoko",   "NES Cart Yoko PCB")


nes_cony_device::nes_cony_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock, u16 extra_addr, u8 mask)
	: nes_nrom_device(mconfig, type, tag, owner, clock), m_irq_count(0), m_irq_enable(0), irq_timer(nullptr), m_extra_addr(extra_addr), m_mask(mask), m_mode_reg(0), m_outer_reg(0)
{
}

nes_cony_device::nes_cony_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_cony_device(mconfig, NES_CONY, tag, owner, clock, 0x1100, 0x1f)
{
}

nes_cony1k_device::nes_cony1k_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_cony_device(mconfig, NES_CONY1K, tag, owner, clock, 0x1100, 0x1f)
{
}

nes_yoko_device::nes_yoko_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_cony_device(mconfig, NES_YOKO, tag, owner, clock, 0x1400, 0x0f)
{
}

void nes_cony_device::device_start()
{
	common_start();

	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_delay = 0;
	m_irq_source = 0x00;
	m_irq_last_a12 = false;

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	irq_timer = timer_alloc(FUNC(nes_cony_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_delay));
	save_item(NAME(m_irq_source));
	save_item(NAME(m_irq_last_a12));

	save_item(NAME(m_mmc_prg_bank));
	save_item(NAME(m_mmc_vrom_bank));
	save_item(NAME(m_extra_ram));
	save_item(NAME(m_mode_reg));
	save_item(NAME(m_outer_reg));
}

void nes_cony_device::pcb_reset()
{
	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_delay = 0;
	m_irq_source = 0x00;
	m_irq_last_a12 = false;

	m_mode_reg = 0;
	m_outer_reg = 0;

	std::fill(std::begin(m_extra_ram), std::end(m_extra_ram), 0x00);
	std::fill(std::begin(m_mmc_prg_bank), std::end(m_mmc_prg_bank), 0x00);
	std::fill(std::begin(m_mmc_vrom_bank), std::end(m_mmc_vrom_bank), 0x00);

	set_prg();
	set_chr();

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}

/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 Cony ASIC board

 Used by several Cony and Yoko fighting games and
 by Dragon Ball Party.

 Mapper 83 has three known variants:

 Submapper 0:
   256 KiB CHR-ROM with 1 KiB banking and no WRAM.

 Submapper 1:
   512 KiB CHR-ROM with 2 KiB banking and no WRAM.

 Submapper 2:
   1 MiB CHR-ROM with 1 KiB inner banking, a
   256 KiB PRG/CHR outer bank, and four switchable
   8 KiB banks of PRG-NVRAM.

 The ASIC supports 16 KiB, 32 KiB and individual
 8 KiB PRG-ROM banking modes, four mirroring modes,
 four bytes of mirrored scratch RAM, and a two-bit
 solder-pad setting.

 The 16-bit IRQ counter can increment or decrement.
 Register $8318 selects CPU M2 clocks with value $00
 or unfiltered PPU A12 rising edges with value $FF.
 The decisive data bit in $8318 is not known.

 iNES: mapper 83
 NES 2.0: mapper 83, submappers 0-2

 In MAME: Supported.

 -------------------------------------------------*/

void nes_cony_device::irq_clock()
{
	if (!m_irq_enable || !m_irq_count) {
		return;
	}

	if (BIT(m_mode_reg, 6)) {
		m_irq_count--;
	}
	else {
		m_irq_count++;
	}

	if (!m_irq_count) {
		m_irq_enable = 0;
		m_irq_delay = 2;
	}
}

TIMER_CALLBACK_MEMBER(nes_cony_device::irq_timer_tick)
{
	// Only $FF is known to select PPU A12. Treat all other
	// values as CPU M2 until the decisive bit is identified.
	if (m_irq_source != 0xff) {
		irq_clock();
	}
}

void nes_cony_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address)
{
	if (!m_irq_delay) {
		return;
	}

	m_irq_delay--;

	if (!m_irq_delay) {
		m_maincpu6502->queue_delayed_mapper_irq(2);
	}
}

void nes_cony_device::ppu_bus_address(uint16_t ppu_address, uint64_t ppu_cycle, int ppu_tick, bool odd_frame)
{
	const bool current_a12 = BIT(ppu_address, 12);

	if (m_irq_source == 0xff && current_a12 && !m_irq_last_a12) {
		irq_clock();
	}

	m_irq_last_a12 = current_a12;
}

void nes_cony_device::write_l(offs_t offset, u8 data)
{
	LOG("cony write_l, offset: %04x, data: %02x\n", offset, data);

	const offs_t address = offset + 0x4100;

	// Scratch RAM:
	// A~[0101 ..01 .... 00AA]
	if ((address & 0xf30c) == 0x5100) {
		m_extra_ram[address & 0x03] = data;
	}
}

u8 nes_cony_device::read_l(offs_t offset)
{
	LOG("cony read_l, offset: %04x\n", offset);

	const offs_t address = offset + 0x4100;

	// Scratch RAM:
	// A~[0101 ..01 .... 00AA]
	if ((address & 0xf30c) == 0x5100) {
		return m_extra_ram[address & 0x03];
	}

	// Solder-pad input:
	// (address & $F100) == $5000
	if ((address & 0xf100) == 0x5000) {
		const u8 solder_pad = (m_extra_addr >> 10) & 0x03;

		return (get_open_bus() & 0xfc) | solder_pad;
	}

	return get_open_bus();
}

void nes_cony_device::write_m(offs_t offset, u8 data)
{
	LOG("cony write_m, offset: %04x, data: %02x\n", offset, data);

	if (!m_battery.empty()) {
		const offs_t ram_address = ((m_outer_reg >> 6) * 0x2000) + offset;

		m_battery[ram_address & (m_battery.size() - 1)] = data;
	}
}

u8 nes_cony_device::read_m(offs_t offset)
{
	LOG("cony read_m, offset: %04x\n", offset);

	if (!m_battery.empty()) {
		const offs_t ram_address = ((m_outer_reg >> 6) * 0x2000) + offset;

		return m_battery[ram_address & (m_battery.size() - 1)];
	}

	if (BIT(m_mode_reg, 5)) {
		const offs_t prg_address = (m_mmc_prg_bank[3] * 0x2000) + offset;

		return m_prg[prg_address & (m_prg_size - 1)];
	}

	return get_open_bus();
}

void nes_cony_device::set_prg()
{
	switch (m_mode_reg & 0x18) {
		case 0x00:
			prg16_89ab(m_outer_reg);
			prg16_cdef(m_outer_reg | (m_mask >> 1));
			break;

		case 0x08:
			prg32(m_outer_reg >> 1);
			break;

		case 0x10:
		case 0x18: {
			const int base = (m_outer_reg << 1) & ~m_mask;

			prg8_89(base | (m_mmc_prg_bank[0] & m_mask));
			prg8_ab(base | (m_mmc_prg_bank[1] & m_mask));
			prg8_cd(base | (m_mmc_prg_bank[2] & m_mask));
			prg8_ef(base | m_mask);
			break;
		}
	}
}

void nes_cony_device::set_chr()
{
	chr2_0(m_mmc_vrom_bank[0], CHRROM);
	chr2_2(m_mmc_vrom_bank[1], CHRROM);
	chr2_4(m_mmc_vrom_bank[6], CHRROM);
	chr2_6(m_mmc_vrom_bank[7], CHRROM);
}

void nes_cony_device::write_h(offs_t offset, u8 data)
{
	LOG("cony write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x0300) {
		case 0x0000:
			m_outer_reg = data;
			set_prg();
			set_chr();
			break;

		case 0x0100:
			m_mode_reg = data;
			set_prg();

			switch (data & 0x03) {
				case 0:
					set_nt_mirroring(PPU_MIRROR_VERT);
					break;

				case 1:
					set_nt_mirroring(PPU_MIRROR_HORZ);
					break;

				case 2:
					set_nt_mirroring(PPU_MIRROR_LOW);
					break;

				case 3:
					set_nt_mirroring(PPU_MIRROR_HIGH);
					break;
			}
			break;

		case 0x0200:
			if (BIT(offset, 0)) {
				// $8201: set the counter high byte and copy
				// mode-register bit 7 into the IRQ enable latch.
				m_irq_count = (m_irq_count & 0x00ff) | (data << 8);
				m_irq_enable = BIT(m_mode_reg, 7);
			}
			else {
				// $8200: acknowledge the IRQ and set the
				// counter low byte.
				m_irq_count = (m_irq_count & 0xff00) | data;
				m_irq_delay = 0;

				set_irq_line(CLEAR_LINE);
				m_maincpu6502->cancel_delayed_mapper_irq();
			}
			break;

		case 0x0300:
			switch (offset & 0x18) {
				case 0x00:
				case 0x08:
					m_mmc_prg_bank[offset & 0x03] = data;
					set_prg();
					break;

				case 0x10:
					m_mmc_vrom_bank[offset & 0x07] = data;
					set_chr();
					break;

				case 0x18:
					// $8318-$831F: IRQ counter source.
					// $00 selects CPU M2 and $FF selects
					// unfiltered PPU A12 rising edges.
					m_irq_source = data;
					break;
			}
			break;
	}
}

/*-------------------------------------------------

 Cony ASIC variant with 1 KiB CHR-ROM banking

 Used by Mapper 83 submappers 0 and 2. Submapper 2
 uses bits 4-5 of the outer register to select one
 of four 256 KiB CHR-ROM outer banks. Those bits
 wrap away for submapper 0's 256 KiB CHR-ROM.

 -------------------------------------------------*/

void nes_cony1k_device::set_chr()
{
	const int outer_bank = BIT(m_outer_reg, 4, 2) << 8;

	for (int i = 0; i < 8; i++) {
		chr1_x(i, outer_bank | m_mmc_vrom_bank[i], CHRROM);
	}
}

/*-------------------------------------------------

 Yoko ASIC board

 Games: Mortal Kombat II/V Pro, Master Fighter VI'

 This board is closely related to the 2 KiB CHR-ROM
 banking version of the Cony Mapper 83 ASIC.

 CPU address lines A10-A11 select the register groups
 that use A8-A9 on Mapper 83. This moves the Cony
 $8100, $8200-$8201, $8300-$8317 and $8318
 registers to $8400, $8800-$8801, $8C00-$8C17
 and $8C18 respectively.

 The board inherits the Cony PRG banking, mirroring
 and 16-bit incrementing or decrementing IRQ counter.
 The solder-pad input is decoded around $5000, while
 four bytes of mirrored scratch RAM begin at $5400.

 NES 2.0: mapper 264
 UNIF: UNL-YOKO

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_yoko_device::read_l(offs_t offset)
{
	LOG("yoko read_l, offset: %04x\n", offset);

	offset += 0x0100;

	if (offset >= m_extra_addr) {
		return m_extra_ram[offset & 0x03];
	}

	if (offset >= 0x1000) {
		const u8 solder_pad = (m_extra_addr >> 10) & 0x03;

		return (get_open_bus() & 0xfc) | solder_pad;
	}

	return get_open_bus();
}

void nes_yoko_device::write_l(offs_t offset, u8 data)
{
	LOG("yoko write_l, offset: %04x, data: %02x\n", offset, data);

	offset += 0x0100;

	if (offset >= m_extra_addr) {
		m_extra_ram[offset & 0x03] = data;
	}
}

void nes_yoko_device::write_h(offs_t offset, u8 data)
{
	LOG("yoko write_h, offset: %04x, data: %02x\n", offset, data);

	const offs_t translated_offset = (offset & 0xf0ff) | ((offset & 0x0c00) >> 2);

	nes_cony_device::write_h(translated_offset, data);
}