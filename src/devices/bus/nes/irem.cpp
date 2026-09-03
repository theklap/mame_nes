// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for IREM PCBs


 Here we emulate the following PCBs

 * IREM LROG017 [mapper 77]
 * IREM Holy Diver [mapper 78]
 * IREM TAM-S1 [mapper 97]
 * IREM G101 [mapper 32]
 * IREM H3001 [mapper 65]

 ***********************************************************************************************************/


#include "emu.h"
#include "irem.h"

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

DEFINE_DEVICE_TYPE(NES_LROG017,  nes_lrog017_device,  "nes_lrog017",  "NES Cart Irem Discrete 74*161/161/21/138 PCB")
DEFINE_DEVICE_TYPE(NES_HOLYDIVR, nes_holydivr_device, "nes_holydivr", "NES Cart Irem Holy Diver PCB")
DEFINE_DEVICE_TYPE(NES_TAM_S1,   nes_tam_s1_device,   "nes_tam_s1",   "NES Cart Irem TAM-S1 PCB")
DEFINE_DEVICE_TYPE(NES_G101,     nes_g101_device,     "nes_g101",     "NES Cart Irem G-101 PCB")
DEFINE_DEVICE_TYPE(NES_H3001,    nes_h3001_device,    "ns_h3001",     "NES Cart Irem H-3001 PCB")


nes_lrog017_device::nes_lrog017_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_LROG017, tag, owner, clock)
{
}

nes_holydivr_device::nes_holydivr_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_HOLYDIVR, tag, owner, clock)
{
}

nes_tam_s1_device::nes_tam_s1_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_TAM_S1, tag, owner, clock)
{
}

nes_g101_device::nes_g101_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock, u8 prg_mask)
	: nes_nrom_device(mconfig, type, tag, owner, clock), m_latch(0), m_reg(0), m_prg_mask(prg_mask)
{
}

nes_g101_device::nes_g101_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_g101_device(mconfig, NES_G101, tag, owner, clock, 0x1f)
{
}

nes_h3001_device::nes_h3001_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_g101_device(mconfig, NES_H3001, tag, owner, clock, 0x3f)
	, m_irq_count(0)
	, m_irq_count_latch(0)
	, m_irq_enable(0)
	, m_irq_delay(0)
	, irq_timer(nullptr)
	, m_maincpu6502(nullptr)
{
}




void nes_lrog017_device::pcb_reset()
{
	prg32(0);
	chr2_0(0, CHRROM);
	chr2_2(0, CHRRAM);
	chr2_4(1, CHRRAM);
	chr2_6(2, CHRRAM);
}

void nes_holydivr_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);
}

void nes_tam_s1_device::pcb_reset()
{
	prg16_89ab(m_prg_chunks - 1);
	prg16_cdef(0);
}

void nes_g101_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
	save_item(NAME(m_reg));
}

void nes_g101_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	m_latch = 0;
	m_reg = 0;
}

void nes_h3001_device::device_start()
{
	nes_g101_device::device_start();
	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");
	irq_timer = timer_alloc(FUNC(nes_h3001_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_count_latch));
	save_item(NAME(m_irq_delay));
}

void nes_h3001_device::pcb_reset()
{
	nes_g101_device::pcb_reset();
	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_count_latch = 0;
	m_irq_delay = 0;

	if (m_maincpu6502)
		m_maincpu6502->cancel_delayed_mapper_irq();

	set_irq_line(CLEAR_LINE);
}


/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

Irem LROG017 discrete board

Game: Napoleon Senki

This board maps a switchable 32 KiB PRG-ROM bank at
$8000-$FFFF and a switchable 2 KiB CHR-ROM bank at
PPU $0000-$07FF.

The remaining PPU pattern-table space at $0800-$1FFF
contains 6 KiB of fixed CHR-RAM. The cartridge uses
four-screen nametable mapping.

Writes throughout $8000-$FFFF select both banks and
are subject to bus conflicts.

iNES: mapper 77

In MAME: Supported.

-------------------------------------------------*/

void nes_lrog017_device::write_h(offs_t offset, u8 data) {
	LOG("lrog017 write_h, offset: %04x, data: %02x\n", offset, data);

	data = account_bus_conflict(offset, data);

	prg32(data & 0x0f);
	chr2_0(data >> 4, CHRROM);
}

/*-------------------------------------------------

Irem Holy Diver board

Game: Holy Diver

Mapper 78 is shared by two boards with incompatible
nametable mirroring:

Submapper 1:
Jaleco JF-16, used by Uchuusen - Cosmo Carrier.
Bit 3 selects one-screen mirroring.

Submapper 3:
Irem Holy Diver.
Bit 3 selects horizontal or vertical mirroring.

This device implements the Holy Diver variant. The
JF-16 variant is handled by nes_jf16_device.

iNES: mapper 78
NES 2.0: mapper 78, submapper 3

In MAME: Supported.

-------------------------------------------------*/

void nes_holydivr_device::write_h(offs_t offset, u8 data) {
	LOG("holydivr write_h, offset: %04x, data: %02x\n", offset, data);

	data = account_bus_conflict(offset, data);

	prg16_89ab(data & 0x07);
	set_nt_mirroring(BIT(data, 3) ? PPU_MIRROR_VERT : PPU_MIRROR_HORZ);
	chr8(data >> 4, CHRROM);
}

/*-------------------------------------------------

Irem TAM-S1 board

Game: Kaiketsu Yanchamaru

The first 16 KiB PRG-ROM bank is fixed at
$8000-$BFFF. Writes throughout $8000-$BFFF select
a 16 KiB PRG-ROM bank at $C000-$FFFF.

Data bits 0-4 select the PRG-ROM bank. Bit 7 selects
horizontal or vertical nametable mirroring.

The cartridge contains 8 KiB of fixed CHR-RAM and
has no PRG-RAM. This board does not have bus
conflicts.

iNES: mapper 97

In MAME: Supported.

-------------------------------------------------*/

void nes_tam_s1_device::write_h(offs_t offset, u8 data) {
	LOG("tam_s1 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset < 0x4000) {
		prg16_cdef(data & 0x1f);
		set_nt_mirroring(BIT(data, 7) ? PPU_MIRROR_VERT : PPU_MIRROR_HORZ);
	}
}

/*-------------------------------------------------

Irem G-101 board

Games: Image Fight, Major League,
Kaiketsu Yanchamaru 2, Paaman

The last 8 KiB PRG-ROM bank is fixed at $E000-$FFFF.
$A000-$BFFF is independently switchable. Bit 1 of
$9000 exchanges the switchable $8000-$9FFF bank with
the fixed second-last bank at $C000-$DFFF. Bit 0
controls horizontal or vertical mirroring.

Major League uses NES 2.0 submapper 1. Its CIRAM A10
is tied high for fixed upper one-screen mirroring,
and the $9000 register is disabled. This fixes the
PRG layout to 8 KiB + 8 KiB + 16 KiB fixed.

iNES: mapper 32
NES 2.0: mapper 32, submapper 1 for Major League

In MAME: Supported.

-------------------------------------------------*/

void nes_g101_device::set_prg() {
	prg8_x(0 ^ m_latch, m_reg);
	prg8_x(2 ^ m_latch, m_prg_mask - 1);
}

void nes_g101_device::write_h(offs_t offset, u8 data) {
	LOG("g101 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x7000) {
		case 0x0000:
			m_reg = data & m_prg_mask;
			set_prg();
			break;

		case 0x1000:
			// This register is disabled on Major League.
			if (m_pcb_ctrl_mirror) {
				m_latch = data & 0x02;
				set_prg();
				set_nt_mirroring(BIT(data, 0) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			}
			break;

		case 0x2000:
			prg8_ab(data & m_prg_mask);
			break;

		case 0x3000:
			chr1_x(offset & 0x07, data, CHRROM);
			break;
	}
}

/*-------------------------------------------------

Irem H3001 board

Games: Daiku no Gen San 2 - Akage no Dan no Gyakushuu,
Kaiketsu Yanchamaru 3, Spartan X 2

The H3001 provides switchable 8 KiB PRG-ROM and
1 KiB CHR-ROM banks, three mirroring modes and a
16-bit CPU M2-cycle IRQ counter.

$9003 controls the IRQ enable and acknowledges a
pending IRQ. $9004 reloads the counter from the
16-bit latch and acknowledges a pending IRQ. When
the enabled counter reaches zero, it stops and
requests an IRQ. It does not wrap or automatically
reload.

iNES: mapper 65

In MAME: Supported.

-------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_h3001_device::irq_timer_tick) {
	if (m_irq_delay) {
		m_irq_delay--;

		if (!m_irq_delay) {
			m_maincpu6502->queue_delayed_mapper_irq(2);
		}
	}

	if (m_irq_enable && m_irq_count) {
		m_irq_count--;

		if (!m_irq_count) {
			m_irq_enable = 0;
			m_irq_delay = 2;
		}
	}
}

void nes_h3001_device::write_h(offs_t offset, u8 data) {
	LOG("h3001 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x7007) {
		default:
			nes_g101_device::write_h(offset, data);
			break;

		case 0x1000:
			m_latch = BIT(data, 7) << 1;
			set_prg();
			break;

		case 0x1001:
			if (BIT(data, 6)) {
				set_nt_mirroring(PPU_MIRROR_LOW);
			}
			else {
				set_nt_mirroring(BIT(data, 7) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			}
			break;

		case 0x1002:
			break;

		case 0x1003:
			m_irq_enable = BIT(data, 7);
			m_irq_delay = 0;
			m_maincpu6502->cancel_delayed_mapper_irq();
			set_irq_line(CLEAR_LINE);
			break;

		case 0x1004:
			m_irq_count = m_irq_count_latch;
			m_irq_delay = 0;
			m_maincpu6502->cancel_delayed_mapper_irq();
			set_irq_line(CLEAR_LINE);
			break;

		case 0x1005:
			m_irq_count_latch = (m_irq_count_latch & 0x00ff) | (data << 8);
			break;

		case 0x1006:
			m_irq_count_latch = (m_irq_count_latch & 0xff00) | data;
			break;

		case 0x1007:
			break;
	}
}