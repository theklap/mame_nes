// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Namcot PCBs


 Here we emulate the following PCBs

 * Namcot 3433 & 3443 (aka DxROM) [mapper 88, 206, 154]
 * Namcot 3446 [mapper 76]
 * Namcot 3425 [mapper 95]
 * Namcot 163 [mapper 19]
 * Namcot 175 [mapper 210]
 * Namcot 340 [mapper 210]

 TODO:
 - Quinty is not working (same issue of Mendel Palace on TxROM boards, of course)

 ***********************************************************************************************************/


#include "emu.h"
#include "namcot.h"
#include "cpu/m6502/m6502.h"
#include "ui/uimain.h"

#include "speaker.h"

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_NAMCOT3433, nes_namcot3433_device, "nes_namcot3433", "NES Cart Namcot 3433 & 3443 / DxROM PCB")
DEFINE_DEVICE_TYPE(NES_NAMCOT3446, nes_namcot3446_device, "nes_namcot3446", "NES Cart Namcot 3446 PCB")
DEFINE_DEVICE_TYPE(NES_NAMCOT3425, nes_namcot3425_device, "nes_namcot3425", "NES Cart Namcot 3425 PCB")
DEFINE_DEVICE_TYPE(NES_NAMCOT340,  nes_namcot340_device,  "nes_namcot340",  "NES Cart Namcot 340 PCB")
DEFINE_DEVICE_TYPE(NES_NAMCOT175,  nes_namcot175_device,  "nes_namcot175",  "NES Cart Namcot 175 PCB")
DEFINE_DEVICE_TYPE(NES_NAMCOT163,  nes_namcot163_device,  "nes_namcot163",  "NES Cart Namcot 163 PCB")


nes_namcot3433_device::nes_namcot3433_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, type, tag, owner, clock), m_latch(0)
{
}

nes_namcot3433_device::nes_namcot3433_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_NAMCOT3433, tag, owner, clock), m_latch(0)
{
}

nes_namcot3446_device::nes_namcot3446_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_NAMCOT3446, tag, owner, clock), m_latch(0)
{
}

nes_namcot3425_device::nes_namcot3425_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_NAMCOT3425, tag, owner, clock), m_latch(0)
{
}

nes_namcot340_device::nes_namcot340_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, type, tag, owner, clock) {
}

nes_namcot340_device::nes_namcot340_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_namcot340_device(mconfig, NES_NAMCOT340, tag, owner, clock) {
}

nes_namcot175_device::nes_namcot175_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_namcot340_device(mconfig, NES_NAMCOT175, tag, owner, clock)
	, m_wram_protect(0) {
}

nes_namcot163_device::nes_namcot163_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_namcot340_device(mconfig, NES_NAMCOT163, tag, owner, clock)
	, m_irq_count(0)
	, m_irq_enable(0)
	, delay_irq(0)
	, irq_timer(nullptr)
	, m_maincpu6502(nullptr)
	, m_wram_protect(0)
	, m_latch(0)
	, m_chr_bank{}
	, m_namco163snd(*this, "n163") {
}



void nes_namcot3433_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_namcot3433_device::pcb_reset()
{
	prg16_89ab(m_prg_chunks - 2);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	m_latch = 0;
}

void nes_namcot3446_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_namcot3446_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	m_latch = 0;
}

void nes_namcot3425_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
	save_item(NAME(m_reg));
}

void nes_namcot3425_device::pcb_reset() {
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	m_latch = 0;
	m_reg[0] = 0;
	m_reg[1] = 0;

	update_mirroring();
}

void nes_namcot340_device::device_start() {
	common_start();
}

void nes_namcot340_device::pcb_reset() {
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);
	set_nt_mirroring(PPU_MIRROR_VERT);
}

void nes_namcot175_device::device_start() {
	nes_namcot340_device::device_start();
	save_item(NAME(m_wram_protect));
}

void nes_namcot175_device::pcb_reset() {
	nes_namcot340_device::pcb_reset();
	m_wram_protect = 0;
}

void nes_namcot163_device::device_start() {
	nes_namcot340_device::device_start();

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	irq_timer = timer_alloc(FUNC(nes_namcot163_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_enable));
	save_item(NAME(delay_irq));
	save_item(NAME(m_wram_protect));
	save_item(NAME(m_latch));
	save_item(NAME(m_chr_bank));

	if (m_n163_vol == 2) {
		m_namco163snd->set_output_gain(ALL_OUTPUTS, 0.0f);
	} else if (m_n163_vol == 3) {
		m_namco163snd->set_output_gain(ALL_OUTPUTS, 1.125f);
	} else if (m_n163_vol == 4) {
		m_namco163snd->set_output_gain(ALL_OUTPUTS, 1.17f);
	} else if (m_n163_vol == 5) {
		m_namco163snd->set_output_gain(ALL_OUTPUTS, 1.19f);
	}
}

void nes_namcot163_device::pcb_reset() {
	nes_namcot340_device::pcb_reset();

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();

	m_irq_count = 0;
	m_irq_enable = 0;
	delay_irq = 0;
	m_wram_protect = 0;
	m_latch = 0;

	memset(m_chr_bank, 0, sizeof(m_chr_bank));
}

/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 DxROM & Namcot 3433 - 3443 - 3453 board emulation

 Games: Dragon Spirit - Aratanaru Densetsu, Namcot Mahjong, Quinty,
 Devilman

 These are the same board, but DRROM (and Tengen 800004) have
 4-screen mirroring

 iNES: mappers 88, 206 (same as 88 but possibly 4-screen mirroring),
 and 154 (same as 88 but with additional mirroring control)

 -------------------------------------------------*/

void nes_namcot3433_device::dxrom_write(offs_t offset, uint8_t data) {
	LOG("dxrom_write, offset: %04x, data: %02x\n", offset, data);

	// NAMCOT-3453 connects D6 to its single-screen mirroring latch.
	// The latch responds to even writes throughout $8000-$FFFF.
	if (!(offset & 1) && m_pcb_ctrl_mirror) {
		set_nt_mirroring(BIT(data, 6) ? PPU_MIRROR_HIGH : PPU_MIRROR_LOW);
	}

	// PRG-ROM and CHR-ROM banking registers only occupy $8000-$9FFF.
	if (offset >= 0x2000) {
		return;
	}

	if (offset & 1) {
		switch (m_latch & 0x07) {
			case 0:
				chr2_0(data >> 1, CHRROM);
				break;

			case 1:
				chr2_2(data >> 1, CHRROM);
				break;

			case 2:
				chr1_4(data | 0x40, CHRROM);
				break;

			case 3:
				chr1_5(data | 0x40, CHRROM);
				break;

			case 4:
				chr1_6(data | 0x40, CHRROM);
				break;

			case 5:
				chr1_7(data | 0x40, CHRROM);
				break;

			case 6:
				prg8_89(data);
				break;

			case 7:
				prg8_ab(data);
				break;
		}
	}
	else {
		m_latch = data;
	}
}

/*-------------------------------------------------

 Namcot 3446 board emulation

 Game: Digital Devil Monogatari - Megami Tensei

 This board uses a Namcot 108 mapper with different
 CHR-ROM connections. Commands 2-5 select four
 independent 2 KiB CHR-ROM banks, while commands
 6-7 select the two switchable 8 KiB PRG-ROM banks.

 Commands 0-1 are not connected. Nametable mirroring
 is fixed by the cartridge board.

 iNES: mapper 76

 In MAME: Supported.

 -------------------------------------------------*/

void nes_namcot3446_device::write_h(offs_t offset, uint8_t data) {
	LOG("namcot3446 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset >= 0x2000) {
		return;
	}

	if (offset & 1) {
		switch (m_latch) {
			case 2:
				chr2_0(data, CHRROM);
				break;

			case 3:
				chr2_2(data, CHRROM);
				break;

			case 4:
				chr2_4(data, CHRROM);
				break;

			case 5:
				chr2_6(data, CHRROM);
				break;

			case 6:
				prg8_89(data);
				break;

			case 7:
				prg8_ab(data);
				break;
		}
	}
	else {
		m_latch = data & 0x07;
	}
}

/*-------------------------------------------------

 Namcot 3425 board emulation

 Game: Dragon Buster

 This board uses a Namcot 108 mapper with CIRAM A10
 controlled by bit 5 of CHR-ROM registers R0 and R1.

 R0 bit 5 selects the CIRAM page used by nametable
 pages 0-1. R1 bit 5 selects the CIRAM page used by
 nametable pages 2-3. This permits horizontal and
 single-screen mirroring.

 The $8000 and $8001 register interface is mirrored
 throughout $8000-$FFFF.

 iNES: mapper 95

 In MAME: Supported.

 -------------------------------------------------*/

void nes_namcot3425_device::write_h(offs_t offset, uint8_t data) {
	LOG("namcot3425 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset & 1) {
		switch (m_latch) {
			case 0:
				chr2_0(data >> 1, CHRROM);
				m_reg[0] = BIT(data, 5);
				update_mirroring();
				break;

			case 1:
				chr2_2(data >> 1, CHRROM);
				m_reg[1] = BIT(data, 5);
				update_mirroring();
				break;

			case 2:
			case 3:
			case 4:
			case 5:
				chr1_x(2 + m_latch, data, CHRROM);
				break;

			case 6:
				prg8_89(data);
				break;

			case 7:
				prg8_ab(data);
				break;
		}
	}
	else {
		m_latch = data & 0x07;
	}
}

void nes_namcot3425_device::update_mirroring() {
	set_nt_page(0, CIRAM, m_reg[0], 1);
	set_nt_page(1, CIRAM, m_reg[0], 1);
	set_nt_page(2, CIRAM, m_reg[1], 1);
	set_nt_page(3, CIRAM, m_reg[1], 1);
}

/*-------------------------------------------------

 Namcot-340 board emulation

 Games: Famista '92, '93 & '94, Top Striker,
 Wagyan Land 2 & 3

 Namcot-340 is a reduced version of the Namcot-163
 ASIC without expansion audio, IRQ functionality or
 CIRAM/CHR-ROM nametable mapping.

 Eight registers at $8000-$BFFF select 1 KiB CHR-ROM
 banks. Registers at $E000, $E800 and $F000 select
 the three switchable 8 KiB PRG-ROM banks.

 Bits 7-6 written to $E000 select single-screen,
 vertical or horizontal nametable mirroring.

 NES 2.0: mapper 210, submapper 2

 In MAME: Supported.

 -------------------------------------------------*/

void nes_namcot340_device::n340_hiwrite(offs_t offset, uint8_t data) {
	LOG("n340_hiwrite, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x7800) {
		case 0x0000:
		case 0x0800:
		case 0x1000:
		case 0x1800:
		case 0x2000:
		case 0x2800:
		case 0x3000:
		case 0x3800:
			chr1_x(offset >> 11, data, CHRROM);
			break;

		case 0x6000:
			switch (data & 0xc0) {
				case 0x00:
					set_nt_mirroring(PPU_MIRROR_LOW);
					break;

				case 0x40:
					set_nt_mirroring(PPU_MIRROR_VERT);
					break;

				case 0x80:
					set_nt_mirroring(PPU_MIRROR_HIGH);
					break;

				case 0xc0:
					set_nt_mirroring(PPU_MIRROR_HORZ);
					break;
			}

			prg8_89(data & 0x3f);
			break;

		case 0x6800:
			prg8_ab(data & 0x3f);
			break;

		case 0x7000:
			prg8_cd(data & 0x3f);
			break;
	}
}

/*-------------------------------------------------

 Namcot-175 board emulation

 Games: Chibi Maruko-chan - Uki Uki Shopping,
 Family Circuit '91, Famista '91,
 Heisei Tensai Bakabon

 Namcot-175 is a reduced version of the Namcot-163
 ASIC without expansion audio, IRQ functionality or
 mapper-controlled nametable mirroring.

 Eight registers at $8000-$BFFF select 1 KiB CHR-ROM
 banks. Registers at $E000, $E800 and $F000 select
 the three switchable 8 KiB PRG-ROM banks.

 Writes to $C000-$C7FF control external PRG-RAM.
 Bit 0 disables PRG-RAM reads and writes when set.
 Family Circuit '91 contains 2 KiB of battery-backed
 PRG-RAM mirrored throughout $6000-$7FFF.

 Nametable mirroring is hardwired by the cartridge.

 NES 2.0: mapper 210, submapper 1

 In MAME: Supported.

 -------------------------------------------------*/

uint8_t nes_namcot175_device::read_m(offs_t offset) {
	if (!m_wram_protect) {
		if (!m_battery.empty()) {
			return m_battery[offset & (m_battery.size() - 1)];
		}

		if (!m_prgram.empty()) {
			return m_prgram[offset & (m_prgram.size() - 1)];
		}
	}

	return get_open_bus();
}

void nes_namcot175_device::write_m(offs_t offset, uint8_t data) {
	if (!m_wram_protect) {
		if (!m_battery.empty()) {
			m_battery[offset & (m_battery.size() - 1)] = data;
		}

		if (!m_prgram.empty()) {
			m_prgram[offset & (m_prgram.size() - 1)] = data;
		}
	}
}

void nes_namcot175_device::write_h(offs_t offset, uint8_t data) {
	LOG("namcot175 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x7800) {
		case 0x4000:
			m_wram_protect = data & 0x01;
			break;

		case 0x6000:
			prg8_89(data & 0x3f);
			break;

		default:
			n340_hiwrite(offset, data);
			break;
	}
}

/*-------------------------------------------------

 Namcot-163 board emulation

 Games: Battle Fleet, Famista '90, Megami Tensei II,
 Juvei Quest, etc.

 Compared to Namcot-175 here we have mapper controlled
 mirroring, NTRAM mapping to VRAM and additional
 sound hw inside the chip and some internal RAM.

 iNES: mapper 19

 In MAME: Supported

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_namcot163_device::irq_timer_tick) {
	if (m_irq_enable && m_irq_count < 0x7fff) {
		m_irq_count++;

		if (m_irq_count == 0x7fff)
			delay_irq = 2;
	}
}

void nes_namcot163_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) {
	if (delay_irq > 0) {
		delay_irq--;

		if (delay_irq == 0)
			m_maincpu6502->queue_delayed_mapper_irq(2);
	}
}

void nes_namcot163_device::chr_w(offs_t offset, uint8_t data) {
	const int bank = offset >> 10;
	const uint8_t chr_bank = m_chr_bank[bank];
	const bool disable_ciram = bank < 4 ? BIT(m_latch, 6) : BIT(m_latch, 7);

	if (!disable_ciram && chr_bank >= 0xe0) {
		const offs_t ciram_offset = (BIT(chr_bank, 0) << 10) | (offset & 0x03ff);
		m_ciram[ciram_offset] = data;
	}
}

uint8_t nes_namcot163_device::chr_r(offs_t offset) {
	const int bank = offset >> 10;
	const uint8_t chr_bank = m_chr_bank[bank];
	const bool disable_ciram = bank < 4 ? BIT(m_latch, 6) : BIT(m_latch, 7);

	if (!disable_ciram && chr_bank >= 0xe0) {
		const offs_t ciram_offset = (BIT(chr_bank, 0) << 10) | (offset & 0x03ff);
		return m_ciram[ciram_offset];
	}

	return m_chr_access[bank][offset & 0x03ff];
}

uint8_t nes_namcot163_device::read_m(offs_t offset) {
	if (!m_battery.empty())
		return m_battery[offset & (m_battery.size() - 1)];

	if (!m_prgram.empty())
		return m_prgram[offset & (m_prgram.size() - 1)];

	return get_open_bus();
}

void nes_namcot163_device::write_m(offs_t offset, uint8_t data) {
	const int bank = BIT(offset, 11, 2);

	if ((m_wram_protect & 0xf0) != 0x40 || BIT(m_wram_protect, bank))
		return;

	if (!m_battery.empty())
		m_battery[offset & (m_battery.size() - 1)] = data;

	if (!m_prgram.empty())
		m_prgram[offset & (m_prgram.size() - 1)] = data;
}

void nes_namcot163_device::write_l(offs_t offset, uint8_t data) {
	LOG("namcot163 write_l, offset: %04x, data: %02x\n", offset, data);

	offset += 0x100;

	switch (offset & 0x1800) {
		case 0x0800:
			m_namco163snd->data_w(data);
			break;

		case 0x1000:
			m_irq_count = (m_irq_count & 0x7f00) | data;
			delay_irq = 0;

			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			break;

		case 0x1800:
			m_irq_count = (m_irq_count & 0x00ff) | ((data & 0x7f) << 8);
			m_irq_enable = BIT(data, 7);
			delay_irq = 0;

			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			break;

		default:
			break;
	}
}

uint8_t nes_namcot163_device::read_l(offs_t offset) {
	LOG("namcot163 read_l, offset: %04x\n", offset);

	offset += 0x100;

	switch (offset & 0x1800) {
		case 0x0800:
			return m_namco163snd->data_r();

		case 0x1000:
			return m_irq_count & 0xff;

		case 0x1800:
			return ((m_irq_count >> 8) & 0x7f) | (m_irq_enable ? 0x80 : 0x00);

		default:
			return get_open_bus();
	}
}

void nes_namcot163_device::set_mirror(uint8_t page, uint8_t data)
{
	if (data < 0xe0)
		set_nt_page(page, VROM, data, 0);
	else
		set_nt_page(page, CIRAM, data & 0x01, 1);
}

void nes_namcot163_device::write_h(offs_t offset, uint8_t data)
{
	LOG("namcot163 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x7800)
	{
		case 0x0000:
		case 0x0800:
		case 0x1000:
		case 0x1800:
		case 0x2000:
		case 0x2800:
		case 0x3000:
		case 0x3800:
		{
			int bank = offset >> 11;

			m_chr_bank[bank] = data;
			chr1_x(bank, data, CHRROM);
			break;
		}

		case 0x4000:
		case 0x4800:
		case 0x5000:
		case 0x5800:
			set_mirror(BIT(offset, 11, 2), data);
			break;

		case 0x6000:
			m_namco163snd->disable_w(BIT(data, 6) ? ASSERT_LINE : CLEAR_LINE);
			prg8_89(data & 0x3f);
			break;

		case 0x6800:
			m_latch = data & 0xc0;
			prg8_ab(data & 0x3f);
			break;

		case 0x7000:
			prg8_cd(data & 0x3f);
			break;

		case 0x7800:
			// Bits 0-3 control WRAM protection and bits 0-6 select
			// the N163 sound RAM address. Bit 7 enables auto-increment.
			m_wram_protect = data;
			m_namco163snd->addr_w(data);
			break;
	}
}

//-------------------------------------------------
//  device_add_mconfig - add device configuration
//-------------------------------------------------

void nes_namcot163_device::device_add_mconfig(machine_config &config)
{
	// additional sound hardware
	SPEAKER(config, "addon").front_center();

	// TODO: Correct clock input / divider?
	NAMCO_163(config, m_namco163snd, XTAL(21'477'272)/12).add_route(ALL_OUTPUTS, "addon", 0.5);
}
