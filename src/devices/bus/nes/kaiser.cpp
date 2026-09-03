// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Kaiser PCBs


 Here we emulate the following Kaiser bootleg PCBs

 * Kaiser KS106C
 * Kaiser KS202
 * Kaiser KS7010
 * Kaiser KS7012
 * Kaiser KS7013B
 * Kaiser KS7016
 * Kaiser KS7016B
 * Kaiser KS7017
 * Kaiser KS7021A
 * Kaiser KS7022
 * Kaiser KS7030
 * Kaiser KS7031
 * Kaiser KS7032
 * Kaiser KS7037
 * Kaiser KS7057
 * Kaiser KS7058

 ***********************************************************************************************************/


#include "emu.h"
#include "kaiser.h"
#include "cpu/m6502/m6502.h"

#define LOG_HIFREQ (1U << 1)

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_KS106C,  nes_ks106c_device,  "nes_ks106c",  "NES Cart Kaiser KS-106C PCB")
DEFINE_DEVICE_TYPE(NES_KS202,   nes_ks202_device,   "nes_ks202",   "NES Cart Kaiser KS-202 PCB")
DEFINE_DEVICE_TYPE(NES_KS7010,  nes_ks7010_device,  "nes_ks7010",  "NES Cart Kaiser KS-7010 PCB")
DEFINE_DEVICE_TYPE(NES_KS7012,  nes_ks7012_device,  "nes_ks7012",  "NES Cart Kaiser KS-7012 PCB")
DEFINE_DEVICE_TYPE(NES_KS7013B, nes_ks7013b_device, "nes_ks7013b", "NES Cart Kaiser KS-7013B PCB")
DEFINE_DEVICE_TYPE(NES_KS7016,  nes_ks7016_device,  "nes_ks7016",  "NES Cart Kaiser KS-7016 PCB")
DEFINE_DEVICE_TYPE(NES_KS7016B, nes_ks7016b_device, "nes_ks7016b", "NES Cart Kaiser KS-7016B PCB")
DEFINE_DEVICE_TYPE(NES_KS7017,  nes_ks7017_device,  "nes_ks7017",  "NES Cart Kaiser KS-7017 PCB")
DEFINE_DEVICE_TYPE(NES_KS7021A, nes_ks7021a_device, "nes_ks7021a", "NES Cart Kaiser KS-7021A PCB")
DEFINE_DEVICE_TYPE(NES_KS7022,  nes_ks7022_device,  "nes_ks7022",  "NES Cart Kaiser KS-7022 PCB")
DEFINE_DEVICE_TYPE(NES_KS7030,  nes_ks7030_device,  "nes_ks7030",  "NES Cart Kaiser KS-7030 PCB")
DEFINE_DEVICE_TYPE(NES_KS7031,  nes_ks7031_device,  "nes_ks7031",  "NES Cart Kaiser KS-7031 PCB")
DEFINE_DEVICE_TYPE(NES_KS7032,  nes_ks7032_device,  "nes_ks7032",  "NES Cart Kaiser KS-7032 PCB")
DEFINE_DEVICE_TYPE(NES_KS7037,  nes_ks7037_device,  "nes_ks7037",  "NES Cart Kaiser KS-7037 PCB")
DEFINE_DEVICE_TYPE(NES_KS7057,  nes_ks7057_device,  "nes_ks7057",  "NES Cart Kaiser KS-7057 PCB")
DEFINE_DEVICE_TYPE(NES_KS7058,  nes_ks7058_device,  "nes_ks7058",  "NES Cart Kaiser KS-7058 PCB")


nes_ks106c_device::nes_ks106c_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_KS106C, tag, owner, clock), m_latch(0)
{
}

nes_ks7058_device::nes_ks7058_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_sxrom_device(mconfig, NES_KS7058, tag, owner, clock)
{
}

nes_ks7022_device::nes_ks7022_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_KS7022, tag, owner, clock), m_latch(0)
{
}

nes_ks7032_device::nes_ks7032_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, type, tag, owner, clock)
	, m_latch(0)
	, m_irq_count(0)
	, m_irq_count_latch(0)
	, m_irq_enable(0)
	, m_irq_delay(0)
	, irq_timer(nullptr)
	, m_maincpu6502(nullptr)
{
}

nes_ks7032_device::nes_ks7032_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_ks7032_device(mconfig, NES_KS7032, tag, owner, clock)
{
}

nes_ks202_device::nes_ks202_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_ks7032_device(mconfig, NES_KS202, tag, owner, clock)
{
}

nes_ks7016_device::nes_ks7016_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock, u8 a15_flip)
	: nes_nrom_device(mconfig, type, tag, owner, clock), m_latch(0), m_a15_flip(a15_flip)
{
}

nes_ks7016_device::nes_ks7016_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_ks7016_device(mconfig, NES_KS7016, tag, owner, clock, 0x00)
{
}

nes_ks7016b_device::nes_ks7016b_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_ks7016_device(mconfig, NES_KS7016B, tag, owner, clock, 0x04)
{
}

nes_ks7017_device::nes_ks7017_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_KS7017, tag, owner, clock), m_latch(0), m_irq_count(0), m_irq_status(0), m_irq_enable(0), irq_timer(nullptr)
{
}

nes_ks7021a_device::nes_ks7021a_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_KS7021A, tag, owner, clock)
{
}

nes_ks7010_device::nes_ks7010_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_KS7010, tag, owner, clock), m_latch(0)
{
}

nes_ks7012_device::nes_ks7012_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_KS7012, tag, owner, clock)
{
}

nes_ks7013b_device::nes_ks7013b_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_KS7013B, tag, owner, clock)
{
}

nes_ks7030_device::nes_ks7030_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_KS7030, tag, owner, clock)
{
}

nes_ks7031_device::nes_ks7031_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_KS7031, tag, owner, clock)
{
}

nes_ks7037_device::nes_ks7037_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_KS7037, tag, owner, clock), m_reg(0)
{
}

nes_ks7057_device::nes_ks7057_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_KS7057, tag, owner, clock)
{
}




void nes_ks106c_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_ks106c_device::pcb_reset()
{
	prg32(m_latch);
	chr8(m_latch, CHRROM);
	set_nt_mirroring(BIT(m_latch, 0) ? PPU_MIRROR_VERT : PPU_MIRROR_HORZ);
	m_latch = (m_latch + 1) & 0x03;
}

void nes_ks7022_device::device_start()
{
	common_start();

	save_item(NAME(m_latch));
}

void nes_ks7022_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	m_latch = 0;
}

void nes_ks7032_device::device_start()
{
	common_start();
	irq_timer = timer_alloc(FUNC(nes_ks7032_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));
	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_latch));
	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_count_latch));
	save_item(NAME(m_reg));
	save_item(NAME(m_irq_delay));
}

void nes_ks7032_device::pcb_reset()
{
	m_latch = 0;
	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_count_latch = 0;
	m_irq_delay = 0;

	m_maincpu6502->cancel_delayed_mapper_irq();
	set_irq_line(CLEAR_LINE);
	std::fill(std::begin(m_reg), std::end(m_reg), 0x00);

	prg_update();
	prg8_ef((m_prg_chunks << 1) - 1);
	chr8(0, m_chr_source);
}

void nes_ks202_device::pcb_reset()
{
	nes_ks7032_device::pcb_reset();

	m_reg[1] = 0x10;
	m_reg[2] = 0x10;
	m_reg[3] = 0x10;
	m_reg[4] = 0x10;

	prg_update();
	prg8_ef(0x1f);
}

void nes_ks7016_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_ks7016_device::pcb_reset()
{
	prg8_89(0x0c ^ m_a15_flip);
	prg8_ab(0x0d ^ m_a15_flip);
	prg8_cd(0x0e ^ m_a15_flip);
	prg8_ef(0x0f ^ m_a15_flip);
	chr8(0, CHRRAM);

	m_latch = 0;
}

void nes_ks7017_device::device_start()
{
	common_start();

	irq_timer = timer_alloc(FUNC(nes_ks7017_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_latch));
	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_status));
	save_item(NAME(m_irq_delay));
}

void nes_ks7017_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(2);
	chr8(0, m_chr_source);

	m_latch = 0;
	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_status = 0;
	m_irq_delay = 0;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}

void nes_ks7021a_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, CHRROM);
}

void nes_ks7010_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_ks7010_device::pcb_reset()
{
	prg16_89ab(0x05);    // all upper banks are fixed
	prg16_cdef(0x03);
	chr8(0, CHRROM);

	m_latch = 0;
}

void nes_ks7012_device::pcb_reset()
{
	prg32(1);
	chr8(0, CHRRAM);
}

void nes_ks7013b_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(7);
	chr8(0, CHRRAM);
}

void nes_ks7030_device::device_start()
{
	common_start();
	save_item(NAME(m_reg));
}

void nes_ks7030_device::pcb_reset()
{
	prg32((m_prg_chunks >> 1) - 1);
	chr8(0, CHRRAM);

	m_reg[0] = 0;
	m_reg[1] = 0;
}

void nes_ks7031_device::device_start()
{
	common_start();
	save_item(NAME(m_reg));
}

void nes_ks7031_device::pcb_reset()
{
	chr8(0, CHRRAM);

	std::fill(std::begin(m_reg), std::end(m_reg), 0x00);
}

void nes_ks7037_device::device_start()
{
	common_start();
	save_item(NAME(m_reg));
}

void nes_ks7037_device::pcb_reset()
{
	prg8_89(0);
	prg8_ab(0x0e);
	prg8_cd(0);
	prg8_ef(0x0f);
	chr8(0, CHRRAM);

	m_reg = 0;
}

void nes_ks7057_device::device_start()
{
	common_start();
	save_item(NAME(m_reg));
}

void nes_ks7057_device::pcb_reset()
{
	prg8_ab(0x0d);
	prg16_cdef(0x07);
	chr8(0, CHRRAM);

	std::fill(std::begin(m_reg), std::end(m_reg), 0x00);
}

/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 Kaiser Board KS106C

 Games: 4 in 1

 No need to use handlers. At reset the banks change
 and so does the game.

 NES 2.0: mapper 352

 In MAME: Supported.

 -------------------------------------------------*/

/*-------------------------------------------------

Kaiser KS-7058 board

Game: Tui Do Woo Ma Jeung

The board uses a KS203 MMC1-compatible ASIC with
hardwired nametable mirroring. MMC1 mirroring writes
therefore have no effect.

NES 2.0: mapper 1, submapper 7
Legacy iNES: mapper 171

In MAME: Supported.

-------------------------------------------------*/

void nes_ks7058_device::set_prg() {
	prg32(0);
}

void nes_ks7058_device::set_mirror() {
	// Nametable mirroring is hardwired on the cartridge.
}

/*-------------------------------------------------

Kaiser KS-7022 board

Game: 15-in-1

$8000 controls nametable mirroring. Writing $A000
loads the pending PRG/CHR bank. Reading the reset
vector at $FFFC applies that bank, mapping the same
16 KiB PRG-ROM bank at $8000-$BFFF and $C000-$FFFF.

iNES: mapper 175

In MAME: Supported.

-------------------------------------------------*/

void nes_ks7022_device::write_h(offs_t offset, u8 data)
{
	LOG("ks7022 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset == 0x0000) {
		set_nt_mirroring(BIT(data, 2) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
	}
	else if (offset == 0x2000) {
		m_latch = data & 0x0f;
	}
}

u8 nes_ks7022_device::read_h(offs_t offset)
{
	LOG("ks7022 read_h, offset: %04x\n", offset);

	if (offset == 0x7ffc) {
		chr8(m_latch, CHRROM);
		prg16_89ab(m_latch);
		prg16_cdef(m_latch);
	}

	return hi_access_rom(offset);
}

/*-------------------------------------------------

Kaiser KS-7032 board

Games: FDS conversions of Bubble Bobble,
Super Mario Bros. 2 and Exciting Soccer

The board uses a KS202 ASIC providing four
switchable 8 KiB PRG-ROM banks and a cycle-based
16-bit IRQ counter derived from the VRC3.

After the counter overflows, the mapper IRQ passes
through the two-PPU-dot and delayed CPU IRQ path.

iNES: mapper 142

In MAME: Supported.

-------------------------------------------------*/

void nes_ks7032_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address)
{
	if (!m_irq_delay) {
		return;
	}

	m_irq_delay--;

	if (!m_irq_delay) {
		m_maincpu6502->queue_delayed_mapper_irq(2);
	}
}

TIMER_CALLBACK_MEMBER(nes_ks7032_device::irq_timer_tick)
{
	if (m_irq_enable && ++m_irq_count == 0) {
		m_irq_enable = 0;
		m_irq_count = m_irq_count_latch;
		m_irq_delay = 2;
	}
}

void nes_ks7032_device::prg_update()
{
	prg8_89(m_reg[1]);
	prg8_ab(m_reg[2]);
	prg8_cd(m_reg[3]);
}

void nes_ks7032_device::write_h(offs_t offset, u8 data)
{
	LOG("ks7032_write, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x7000)
	{
		case 0x0000:
		case 0x1000:
		case 0x2000:
		case 0x3000:
		{
			int shift = 4 * BIT(offset, 12, 2);
			m_irq_count_latch &= ~(0x000f << shift);
			m_irq_count_latch |= (data & 0x0f) << shift;
			break;
		}
		case 0x4000:
			m_irq_enable = BIT(data, 1);
			m_irq_delay = 0;

			if (m_irq_enable) {
				m_irq_count = m_irq_count_latch;
			}

			m_maincpu6502->cancel_delayed_mapper_irq();
			set_irq_line(CLEAR_LINE);
			break;
		case 0x5000:
			m_irq_delay = 0;
			m_maincpu6502->cancel_delayed_mapper_irq();
			set_irq_line(CLEAR_LINE);
			break;
		case 0x6000:
			m_latch = data & 0x07;
			break;
		case 0x7000:
			m_reg[m_latch] = (m_reg[m_latch] & 0xf0) | (data & 0x0f);
			prg_update();
			break;
	}
}

u8 nes_ks7032_device::read_m(offs_t offset)
{
	LOG("ks7032 read_m, offset: %04x\n", offset);
	return m_prg[(m_reg[4] * 0x2000 + offset) & (m_prg_size - 1)];
}

/*-------------------------------------------------

Kaiser SMB3 board with KS202

Game: Super Mario Bros. 3 (pirate conversion)

The KS202 ASIC provides the mapper 142 banking and
cycle-based IRQ functionality. A PAL16L8ANC overlays
additional registers at $F000-$FFFF for PRG upper
bank bits, mirroring and 1 KiB CHR-ROM banking.

Writes to $F000-$FFFF reach both the PAL and KS202.
The four PRG upper-bank bits power up set.

The device is named after the KS202 because the
board's actual PCB designation is not known.

iNES: mapper 56

In MAME: Supported.

-------------------------------------------------*/

void nes_ks202_device::write_h(offs_t offset, u8 data)
{
	LOG("ks202 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset >= 0x7000) {
		switch (offset & 0x0c00) {
			case 0x0000:
			{
				const int reg = (offset & 0x03) + 1;

				m_reg[reg] = (m_reg[reg] & 0x0f) | (data & 0x10);

				if (reg == 4) {
					prg8_ef(m_reg[4] | 0x0f);
				}
				break;
			}

			case 0x0800:
				set_nt_mirroring(BIT(data, 0) ? PPU_MIRROR_VERT : PPU_MIRROR_HORZ);
				break;

			case 0x0c00:
				chr1_x(offset & 0x07, data & 0x7f, CHRROM);
				break;
		}
	}

	// The KS202 bank-data register is physically
	// superimposed on every $F000-$FFFF write.
	nes_ks7032_device::write_h(offset, data);
}

u8 nes_ks202_device::read_m(offs_t offset)
{
	return device_nes_cart_interface::read_m(offset);
}

/*-------------------------------------------------

 Kaiser Boards KS7016, KS7016B

 Games: Exciting Basket, Meikyuu Jiin Dababa FDS Conversions

 These two variants have fixed upper 32K PRG and switchable
 8K PRG at 0x6000-0x7fff. The only difference appears to be
 a flipped bit in the bank numbers. KS7016 puts banks 0x0c
 through 0x0f in the upper PRG; KS7016B uses 0x08 to 0x0b.
 For the switchable 8K PRG the latched bank # patterns are:

   KS7016:  0 1 2 3 4 5 6 7 8 9 A B 8 9 A B
   KS7016B: 0 1 2 3 4 5 6 7 C D E F C D E F

 (NB: only KS7016B has been verified against PCB, KS7016 is a surmise.)

 NES 2.0: mapper 306, mapper 549

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_ks7016_device::read_m(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "ks7016 read_m, offset: %04x\n", offset);
	return m_prg[(m_latch * 0x2000 + offset) & (m_prg_size - 1)];
}

void nes_ks7016_device::write_h(offs_t offset, u8 data)
{
	LOG("ks7016 write_h, offset: %04x, data: %02x\n", offset, data);

	m_latch = BIT(offset, 2, 4);
	if (m_latch & 0x08)
		m_latch = (m_latch & 0x0b) | m_a15_flip;
}

/*-------------------------------------------------

Kaiser Board KS7017

Game: Almana no Kiseki FDS conversion

The board provides 8 KiB of PRG-RAM at $6000-$7FFF,
a switchable 16 KiB PRG-ROM bank at $8000-$BFFF,
and fixed PRG-ROM bank 2 at $C000-$FFFF.

Writes to $4A00-$4AFF select a bank using address
lines A2, A3 and A6. A write to $5100 commits the
selected bank.

The board reproduces the FDS-style 16-bit timer IRQ,
mirroring control and IRQ status register.

NES 2.0: mapper 303

In MAME: Supported.

-------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_ks7017_device::irq_timer_tick)
{
	if (m_irq_enable) {
		if (--m_irq_count == 0) {
			m_irq_enable = 0;
			m_irq_status |= 0x01;
			m_irq_delay = 2;
		}
	}
}

void nes_ks7017_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address)
{
	if (!m_irq_delay) {
		return;
	}

	m_irq_delay--;

	if (!m_irq_delay) {
		m_maincpu6502->queue_delayed_mapper_irq(2);
	}
}

void nes_ks7017_device::write_l(offs_t offset, u8 data)
{
	LOG("ks7017 write_l, offset: %04x, data: %02x\n", offset, data);

	offset += 0x100;

	switch (offset & 0x1f00) {
		case 0x0a00:
			m_latch = bitswap<3>(offset, 6, 3, 2);
			break;

		case 0x1100:
			prg16_89ab(m_latch);
			break;
	}
}

void nes_ks7017_device::write_ex(offs_t offset, u8 data)
{
	LOG("ks7017 write_ex, offset: %04x, data: %02x\n", offset, data);

	offset += 0x4020;

	switch (offset) {
		case 0x4020:
			m_irq_count = (m_irq_count & 0xff00) | data;
			break;

		case 0x4021:
			m_irq_count = (m_irq_count & 0x00ff) | (data << 8);
			m_irq_enable = 1;
			break;

		case 0x4025:
			set_nt_mirroring(BIT(data, 3) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			break;
	}
}

u8 nes_ks7017_device::read_ex(offs_t offset)
{
	LOG("ks7017 read_ex, offset: %04x\n", offset);

	offset += 0x4020;

	if (offset == 0x4030) {
		const u8 result = (get_open_bus() & 0xfe) | (m_irq_status & 0x01);

		m_irq_status &= ~0x01;
		m_irq_delay = 0;

		m_maincpu6502->cancel_delayed_mapper_irq();
		set_irq_line(CLEAR_LINE);

		return result;
	}

	return get_open_bus();
}

/*-------------------------------------------------

Kaiser Board KS7021A

Games: Contra, Getsu Fuma Den

This discrete-logic board was used for conversions
of games originally using the Konami VRC2.

It provides a switchable 16 KiB PRG-ROM bank at
$8000-$BFFF, a fixed 16 KiB PRG-ROM bank at
$C000-$FFFF, eight individually switchable 1 KiB
CHR-ROM banks, and selectable mirroring.

NES 2.0: mapper 525

In MAME: Supported.

-------------------------------------------------*/

void nes_ks7021a_device::write_h(offs_t offset, u8 data)
{
	LOG("ks7021a write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x7000) {
		case 0x0000:
			prg16_89ab(BIT(data, 1, 3));
			break;

		case 0x1000:
			set_nt_mirroring(BIT(data, 0) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			break;

		case 0x3000:
			chr1_x(offset & 0x07, data & 0x7f, CHRROM);
			break;
	}
}

/*-------------------------------------------------

Kaiser Board KS7010

Game: Akumajou Dracula FDS conversion

The board maps a switchable 8 KiB PRG-ROM bank at
$6000-$7FFF. The remaining four 8 KiB PRG-ROM
banks are fixed in the order 10, 11, 6 and 7.

The switchable PRG-ROM bank and the 8 KiB CHR-ROM
bank share a four-bit latch. Reading certain PRG-ROM
addresses loads the latch from CPU address lines
A2-A5.

The exact address decoding remains unknown. The
implementation recognizes the addresses known to
be used by the game.

NES 2.0: mapper 554

In MAME: Supported.

-------------------------------------------------*/

u8 nes_ks7010_device::read_m(offs_t offset)
{
	const u8 data = m_prg[(m_latch * 0x2000 + offset) & (m_prg_size - 1)];

	LOGMASKED(LOG_HIFREQ, "ks7010 read_m, offset: %04x, data: %02x\n", offset, data);

	return data;
}

u8 nes_ks7010_device::read_h(offs_t offset)
{
	if ((offset >= 0x4ab6 && offset <= 0x4ad6) || offset == 0x6be2 || offset == 0x6be3 || offset == 0x6e32 || offset == 0x7ffc) {
		m_latch = BIT(offset, 2, 4);
		chr8(m_latch, CHRROM);
	}

	const u8 data = hi_access_rom(offset);

	LOGMASKED(LOG_HIFREQ, "ks7010 read_h, offset: %04x, data: %02x\n", offset, data);

	return data;
}

/*-------------------------------------------------

Kaiser Board KS7012

Game: Zanac FDS conversion

The board contains two 32 KiB PRG-ROM banks.
Writing to $E0A0 selects bank 0, while writing to
$EE36 selects bank 1. The written data is ignored.

The exact address decoding remains unknown, so only
the two addresses known to be used by the game are
currently recognized.

The board also contains 8 KiB of unbanked PRG-RAM
at $6000-$7FFF and 8 KiB of unbanked CHR-RAM.

Mirroring is hardwired.

NES 2.0: mapper 346

In MAME: Supported.

-------------------------------------------------*/

void nes_ks7012_device::write_h(offs_t offset, u8 data)
{
	LOG("ks7012 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset == 0x60a0) {
		prg32(0);
	}
	else if (offset == 0x6e36) {
		prg32(1);
	}
}

/*-------------------------------------------------

Kaiser Board KS7013B

Game: Highway Star bootleg

The board maps a switchable 16 KiB PRG-ROM bank at
$8000-$BFFF and fixes PRG-ROM bank 7 at
$C000-$FFFF.

Writes throughout $6000-$7FFF select the switchable
PRG-ROM bank using data bits 0-2. Reads from this
area are open bus.

Writes throughout $8000-$FFFF select horizontal or
vertical mirroring using data bit 0.

The board contains 8 KiB of unbanked CHR-RAM.

NES 2.0: mapper 312

In MAME: Supported.

-------------------------------------------------*/

void nes_ks7013b_device::write_m(offs_t offset, u8 data)
{
	LOG("ks7013b write_m, offset: %04x, data: %02x\n", offset, data);

	prg16_89ab(data & 0x07);
}

void nes_ks7013b_device::write_h(offs_t offset, u8 data)
{
	LOG("ks7013b write_h, offset: %04x, data: %02x\n", offset, data);

	set_nt_mirroring(BIT(data, 0) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
}

/*-------------------------------------------------

 Kaiser Board KS7030

 Games: Doki Doki Panic (FDS conversion)

 This board has a complicated memory layout. The last
 32k of the mask ROM is fixed into 0x8000-0xffff, but
 8k of WRAM and 8k of bankable PRG (in two 4k banks)
 are adjacent and overlaid. The ranges are as follows.

   WRAM:                   PRG:
     - 3k, 0x6000-0x6bff     - 1k, 0x6c00-0x6fff (reg 1, last 1k)
     - 2k, 0xb800-0xbfff     - 4k, 0x7000-0x7fff (reg 0)
     - 3k, 0xcc00-0xd7ff     - 3k, 0xc000-0xcc00 (reg 1, initial 3k)

 The two registers latch part of the address in writes
 to 0x8000-0x8fff and 0x9000-0x9fff respectively.

 NES 2.0: mapper 347

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_ks7030_device::read_m(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "ks7030 read_m, offset: %04x\n", offset);
	if (offset < 0x0c00)         // first of 3k WRAM
		return m_prgram[offset];
	else if (offset < 0x1000)    // last 1k of 4k PRG bank
		return m_prg[m_reg[1] * 0x1000 + offset];
	else                         // 4k PRG
		return m_prg[0x10000 + m_reg[0] * 0x1000 + (offset & 0x0fff)];
}

void nes_ks7030_device::write_m(offs_t offset, u8 data)
{
	LOG("ks7030 write_m, offset: %04x\n", offset);
	if (offset < 0x0c00)         // first 3k of WRAM
		m_prgram[offset] = data;
}

u8 nes_ks7030_device::read_h(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "ks7030 read_h, offset: %04x\n", offset);
	if (offset < 0x3800 || offset >= 0x5800)    // fixed 32k PRG, split 14k and 10k windows
		return m_prg[0x18000 + (offset & 0x7fff)];
	else if (offset < 0x4000)                   // middle 2k of WRAM
		return m_prgram[offset - 0x2c00];
	else if (offset < 0x4c00)                   // first 3k of 4k PRG bank
		return m_prg[m_reg[1] * 0x1000 + (offset & 0x0fff)];
	else                                        // last 3k of WRAM
		return m_prgram[offset - 0x3800];
}

void nes_ks7030_device::write_h(offs_t offset, u8 data)
{
	LOG("ks7030 write_h, offset: %04x\n", offset);
	if (offset < 0x1000)
	{
		set_nt_mirroring(BIT(offset, 3) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
		m_reg[0] = offset & 0x07;
	}
	else if (offset < 0x2000)
		m_reg[1] = offset & 0x0f;
	else if (offset >= 0x3800 && offset < 0x4000)    // middle 2k of WRAM
		m_prgram[offset - 0x2c00] = data;
	else if (offset >= 0x4c00 && offset < 0x5800)    // last 3k of WRAM
		m_prgram[offset - 0x3800] = data;
}

/*-------------------------------------------------

 Kaiser Board KS7031

 Games: Dracula II FDS Conversion

 The board maps four independently switchable 2 KiB
 PRG-ROM banks at $6000-$7FFF.

 The fixed $8000-$FFFF region contains sixteen 2 KiB
 PRG-ROM banks stored in reverse order: bank 15 is
 mapped at $8000 and bank 0 is mapped at $F800.

 Writes throughout $8000-$FFFF select one of the four
 $6000-$7FFF windows using address bits A12-A11.
 Data bits 5-0 select the 2 KiB PRG-ROM bank.

 The board contains 8 KiB of unbanked CHR-RAM.

 NES 2.0: mapper 305

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_ks7031_device::read_m(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "ks7031 read_m, offset: %04x\n", offset);

	const u8 reg = BIT(offset, 11, 2);
	const u32 address = m_reg[reg] * 0x0800 + (offset & 0x07ff);

	return m_prg[address];
}

u8 nes_ks7031_device::read_h(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "ks7031 read_h, offset: %04x\n", offset);

	const u8 bank = 0x0f - BIT(offset, 11, 4);
	const u32 address = bank * 0x0800 + (offset & 0x07ff);

	return m_prg[address];
}

void nes_ks7031_device::write_h(offs_t offset, u8 data)
{
	LOG("ks7031 write_h, offset: %04x, data: %02x\n", offset, data);

	const u8 reg = BIT(offset, 11, 2);

	m_reg[reg] = data & 0x3f;
}

/*-------------------------------------------------

 Kaiser Board KS7037

 Games: Metroid (FDS conversion)

 The board is based on a Kaiser clone of the Tengen
 MIMIC-1 ASIC.

 PRG-RAM is split between $6000-$6FFF and
 $B000-$BFFF. A fixed 4 KiB PRG-ROM bank is mapped
 at $7000-$7FFF.

 Switchable 8 KiB PRG-ROM banks are mapped at
 $8000-$9FFF and $C000-$DFFF. The remaining PRG-ROM
 regions are fixed.

 Even writes throughout $8000-$9FFF select one of
 eight registers. Odd writes update the selected
 register. Registers 2-5 control individual CIRAM
 nametable pages, while registers 6-7 control the
 two switchable PRG-ROM windows.

 The board contains 8 KiB of unbanked CHR-RAM.

 NES 2.0: mapper 307

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_ks7037_device::read_m(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "ks7037 read_m, offset: %04x\n", offset);

	if (offset < 0x1000) {
		return m_prgram[offset];
	}

	return m_prg[0x0f * 0x1000 + (offset & 0x0fff)];
}

void nes_ks7037_device::write_m(offs_t offset, u8 data)
{
	LOG("ks7037 write_m, offset: %04x, data: %02x\n", offset, data);

	if (offset < 0x1000) {
		m_prgram[offset] = data;
	}
}

u8 nes_ks7037_device::read_h(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "ks7037 read_h, offset: %04x\n", offset);

	if (offset >= 0x3000 && offset < 0x4000) {
		return m_prgram[0x1000 + (offset & 0x0fff)];
	}

	return hi_access_rom(offset);
}

void nes_ks7037_device::write_h(offs_t offset, u8 data)
{
	LOG("ks7037 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x7000) {
		case 0x0000:
		case 0x1000:
			if (!BIT(offset, 0)) {
				m_reg = data & 0x07;
			}
			else if (m_reg >= 6) {
				prg8_x((m_reg & 0x01) << 1, data);
			}
			else if (m_reg >= 2) {
				set_nt_page(bitswap<2>(m_reg, 0, 2), CIRAM, data & 0x01, 1);
			}
			break;

		case 0x3000:
			m_prgram[0x1000 + (offset & 0x0fff)] = data;
			break;
	}
}

/*-------------------------------------------------

 Kaiser Board KS7057

 Games: Gyruss (FDS conversion)

 Eight independently switchable 2 KiB PRG-ROM banks
 are mapped from $6000-$9FFF.

 Each bank register is split into low- and high-nibble
 writes. Address lines A2-A11 are not decoded, so each
 register is mirrored throughout its 4 KiB region.

 Registers $B000-$C003 control the four banks mapped
 at $8000-$9FFF. Registers $D000-$E003 control the
 four banks mapped at $6000-$7FFF.

 The fixed PRG-ROM regions are:

   $A000-$BFFF: 8 KiB bank $0D
   $C000-$FFFF: 16 KiB bank $07

 Writes throughout $8000-$9FFF control nametable
 mirroring using data bit 0.

 The board contains 8 KiB of unbanked CHR-RAM.

 NES 2.0: mapper 302

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_ks7057_device::read_m(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "ks7057 read_m, offset: %04x\n", offset);

	const u8 reg = BIT(offset, 11, 2) + 4;
	const u32 address = m_reg[reg] * 0x0800 + (offset & 0x07ff);

	return m_prg[address];
}

u8 nes_ks7057_device::read_h(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "ks7057 read_h, offset: %04x\n", offset);

	if (offset < 0x2000) {
		const u8 reg = BIT(offset, 11, 2);
		const u32 address = m_reg[reg] * 0x0800 + (offset & 0x07ff);

		return m_prg[address];
	}

	return hi_access_rom(offset);
}

void nes_ks7057_device::write_h(offs_t offset, u8 data)
{
	LOG("ks7057 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset < 0x2000) {
		set_nt_mirroring(BIT(data, 0) ? PPU_MIRROR_VERT : PPU_MIRROR_HORZ);
		return;
	}

	switch (offset & 0x7000) {
		case 0x3000:
		case 0x4000:
		case 0x5000:
		case 0x6000:
		{
			const u8 reg = bitswap<4>(offset, 14, 13, 12, 1) - 6;

			if (BIT(offset, 0)) {
				m_reg[reg] &= 0x0f;
				m_reg[reg] |= (data & 0x03) << 4;
			}
			else {
				m_reg[reg] &= 0x30;
				m_reg[reg] |= data & 0x0f;
			}
			break;
		}
	}
}