// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Bandai PCBs


 Here we emulate the following PCBs

 * Bandai FCG [mapper 16] (older design, regs are only accessed in 0x6000-0x7fff range)
 * Bandai LZ93D50 [mapper 16] (this extends FCG by solving issues when writing to 0x8000-0xffff)
 * Bandai LZ93D50 + 24C01 EEPROM [mapper 159]
 * Bandai LZ93D50 + 24C02 EEPROM [mapper 16]
 * Bandai Famicom Jump 2 (aka LZ93D50 + SRAM) [mapper 153]
 * Bandai Oeka Kids [mapper 96]

 * Bandai Datach Joint ROM System [mapper 157] is emulated in a separate source file
   to implement also the subslot, but the PCB is basically a Bandai LZ93D50 + 24C02 EEPROM
   PCB with added barcode reader and subslot

 * Bandai Karaoke Studio [mapper 188] is emulated in a separate source file
   to implement also the subslot and the mic inputs

 ***********************************************************************************************************/


#include "emu.h"
#include "bandai.h"

#include "cpu/m6502/m6502.h"

#define LOG_UNHANDLED (1U << 1)

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_UNHANDLED | LOG_GENERAL)
#else
#define VERBOSE (LOG_UNHANDLED)
#endif
#include "logmacro.h"



//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_OEKAKIDS,      nes_oekakids_device,      "nes_oeka",        "NES Cart Bandai Oeka Kids PCB")
DEFINE_DEVICE_TYPE(NES_FCG,           nes_fcg_device,           "nes_fcg",         "NES Cart Bandai FCG PCB")
DEFINE_DEVICE_TYPE(NES_LZ93D50,       nes_lz93d50_device,       "nes_lz93d50",     "NES Cart Bandai LZ93D50 PCB")
DEFINE_DEVICE_TYPE(NES_LZ93D50_24C01, nes_lz93d50_24c01_device, "nes_lz93d50_ep1", "NES Cart Bandai LZ93D50 + 24C01 PCB")
DEFINE_DEVICE_TYPE(NES_LZ93D50_24C02, nes_lz93d50_24c02_device, "nes_lz93d50_ep2", "NES Cart Bandai LZ93D50 + 24C02 PCB")
DEFINE_DEVICE_TYPE(NES_FJUMP2,        nes_fjump2_device,        "nes_fjump2",      "NES Cart Bandai Famicom Jump II PCB")


nes_oekakids_device::nes_oekakids_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_OEKAKIDS, tag, owner, clock),
	m_reg(0),
	m_latch(0),
	m_latch_clock(false)
{
}

nes_fcg_device::nes_fcg_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, type, tag, owner, clock),
	m_irq_count(0),
	m_irq_latch(0),
	m_irq_enable(0),
	m_irq_delay(0),
	irq_timer(nullptr),
	m_maincpu6502(nullptr)
{
}

nes_fcg_device::nes_fcg_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_fcg_device(mconfig, NES_FCG, tag, owner, clock)
{
}

nes_lz93d50_device::nes_lz93d50_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock)
	: nes_fcg_device(mconfig, type, tag, owner, clock)
{
}

nes_lz93d50_device::nes_lz93d50_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_lz93d50_device(mconfig, NES_LZ93D50, tag, owner, clock)
{
}

nes_lz93d50_24c01_device::nes_lz93d50_24c01_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock)
	: nes_lz93d50_device(mconfig, type, tag, owner, clock),
	m_i2cmem(*this, "i2cmem"),
	m_i2c_dir(1)
{
}

nes_lz93d50_24c01_device::nes_lz93d50_24c01_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_lz93d50_24c01_device(mconfig, NES_LZ93D50_24C01, tag, owner, clock)
{
}

nes_lz93d50_24c02_device::nes_lz93d50_24c02_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_lz93d50_24c01_device(mconfig, NES_LZ93D50_24C02, tag, owner, clock)
{
}

nes_fjump2_device::nes_fjump2_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_lz93d50_device(mconfig, NES_FJUMP2, tag, owner, clock),
	m_prg_outer_select(0),
	m_wram_enable(false)
{
}

void nes_oekakids_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
	save_item(NAME(m_reg));
	save_item(NAME(m_latch_clock));
}

void nes_oekakids_device::pcb_reset()
{
	m_reg = 0;
	m_latch = 0;
	m_latch_clock = false;

	prg32(0);
	set_nt_mirroring(PPU_MIRROR_VERT);
	update_chr();
}

void nes_fcg_device::device_start()
{
	common_start();
	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");
	irq_timer = timer_alloc(FUNC(nes_fcg_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_latch));
	save_item(NAME(m_irq_delay));
}

void nes_fcg_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_latch = 0;
	m_irq_delay = 0;

	if (m_maincpu6502)
		m_maincpu6502->cancel_delayed_mapper_irq();

	set_irq_line(CLEAR_LINE);
}

void nes_lz93d50_24c01_device::device_start()
{
	common_start();
	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");
	irq_timer = timer_alloc(FUNC(nes_lz93d50_24c01_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_latch));
	save_item(NAME(m_irq_delay));
	save_item(NAME(m_i2c_dir));
}

void nes_lz93d50_24c01_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_latch = 0;
	m_irq_delay = 0;
	m_i2c_dir = 1;
	m_i2cmem->write_sda(1);
	m_i2cmem->write_scl(1);

	if (m_maincpu6502)
		m_maincpu6502->cancel_delayed_mapper_irq();

	set_irq_line(CLEAR_LINE);
}

void nes_fjump2_device::device_start()
{
	common_start();
	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");
	irq_timer = timer_alloc(FUNC(nes_fjump2_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_latch));
	save_item(NAME(m_irq_delay));
	save_item(NAME(m_reg));
	save_item(NAME(m_prg_outer_select));
	save_item(NAME(m_wram_enable));
}

void nes_fjump2_device::pcb_reset()
{
	chr8(0, CHRRAM);

	memset(m_reg, 0, sizeof(m_reg));
	m_prg_outer_select = 0;
	m_wram_enable = false;

	set_prg();

	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_latch = 0;
	m_irq_delay = 0;

	m_maincpu6502->cancel_delayed_mapper_irq();
	set_irq_line(CLEAR_LINE);
}

/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 Bandai Oeka Kids board emulation

 Games:
 - Oeka Kids: Anpanman no Hiragana Daisuki
 - Oeka Kids: Anpanman to Oekaki Shiyou!!

 The board uses a 74161 for 32 KiB PRG-ROM banking,
 a 7474 for the two-bit CHR-RAM latch, and a 7402 to
 generate the latch clock.

 The latch clock is:

     PPU A13 AND NOT PPU A12

 A rising edge occurs when the PPU leaves pattern-table
 space and begins a nametable fetch in $2000-$2FFF.
 PPU A8-A9 are captured on that edge.

 PPU $0000-$0FFF:
     CHR bank = written D2 plus latched PPU A9-A8

 PPU $1000-$1FFF:
     CHR bank = written D2 plus fixed low bits %11

 Written D1-D0 select the 32 KiB PRG-ROM bank.
 Writes are subject to PRG-ROM bus conflicts.

 The Oeka Kids tablet is emulated separately as a
 Famicom expansion-port controller.

 iNES: mapper 96

 In MAME: Supported

 -------------------------------------------------*/

void nes_oekakids_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) {
	const bool latch_clock = BIT(ppu_address, 13) && !BIT(ppu_address, 12);

	if (latch_clock && !m_latch_clock) {
		m_latch = BIT(ppu_address, 8, 2);
		update_chr();
	}

	m_latch_clock = latch_clock;
}

void nes_oekakids_device::update_chr() {
	chr4_0(m_reg | m_latch, CHRRAM);
	chr4_4(m_reg | 0x03, CHRRAM);
}

void nes_oekakids_device::write_h(offs_t offset, uint8_t data) {
	LOG("oeka kids write_h, offset: %04x, data: %02x\n", offset, data);

	data = account_bus_conflict(offset, data);

	prg32(data & 0x03);

	m_reg = data & 0x04;
	update_chr();
}

/*-------------------------------------------------

 Bandai FCG / LZ93D50 board emulation

 The original FCG-1/FCG-2 ASIC decodes its registers
 throughout CPU $6000-$7FFF. Its IRQ latch registers
 directly modify the active IRQ counter.

 The later LZ93D50 ASIC decodes its registers throughout
 CPU $8000-$FFFF. Registers $800B-$800C modify an IRQ
 latch that is copied into the active counter by a write
 to $800A.

 Board variants may contain SRAM, a 24C01 EEPROM, a
 24C02 EEPROM, or the Datach barcode reader and subslot.
 The EEPROM SDA input is returned on CPU D4 during reads
 from $6000-$7FFF. All other undriven bits retain CPU
 open bus.

 Famicom Jump II uses an LZ93D50 with 8 KiB of
 battery-backed WRAM and repurposes the first four CHR
 register outputs as its outer PRG-ROM bank control.

 Datach hardware is implemented in a separate source file.

 iNES: mappers 16, 153, 157 and 159

 In MAME: Supported

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_fcg_device::irq_timer_tick)
{
	if (!m_irq_enable)
		return;

	if (m_irq_count)
		m_irq_count--;

	if (!m_irq_count)
	{
		m_irq_enable = 0;
		m_irq_delay = 2;
	}
}

void nes_fcg_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address)
{
	if (m_irq_delay > 0)
	{
		m_irq_delay--;

		if (!m_irq_delay)
			m_maincpu6502->queue_delayed_mapper_irq(2);
	}
}

void nes_fcg_device::fcg_write(offs_t offset, uint8_t data)
{
	LOG("fcg_write, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x0f)
	{
		case 0: case 1: case 2: case 3:
		case 4: case 5: case 6: case 7:
			chr1_x(offset & 0x07, data, m_chr_source);
			break;
		case 8:
			prg16_89ab(data);
			break;
		case 9:
			switch (data & 0x03)
			{
				case 0: set_nt_mirroring(PPU_MIRROR_VERT); break;
				case 1: set_nt_mirroring(PPU_MIRROR_HORZ); break;
				case 2: set_nt_mirroring(PPU_MIRROR_LOW); break;
				case 3: set_nt_mirroring(PPU_MIRROR_HIGH); break;
			}
			break;
		case 0x0a:
			m_irq_enable = data & 0x01;
			m_irq_delay = 0;
			m_maincpu6502->cancel_delayed_mapper_irq();
			set_irq_line(CLEAR_LINE);

			if (m_irq_enable && !m_irq_count)
			{
				m_irq_delay = 2;
				m_irq_enable = 0;
			}
			break;
		case 0x0b:
			m_irq_count = (m_irq_count & 0xff00) | data;
			break;
		case 0x0c:
			m_irq_count = (m_irq_count & 0x00ff) | (data << 8);
			break;
		default:
			LOGMASKED(LOG_UNHANDLED, "fcg_write uncaught write, offset: %04x, data: %02x\n", offset, data);
			break;
	}
}

void nes_lz93d50_device::fcg_write(offs_t offset, uint8_t data)
{
	LOG("lz93d50_write, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x0f)
	{
		case 0: case 1: case 2: case 3:
		case 4: case 5: case 6: case 7:
			chr1_x(offset & 0x07, data, m_chr_source);
			break;
		case 8:
			prg16_89ab(data);
			break;
		case 9:
			switch (data & 0x03)
			{
				case 0: set_nt_mirroring(PPU_MIRROR_VERT); break;
				case 1: set_nt_mirroring(PPU_MIRROR_HORZ); break;
				case 2: set_nt_mirroring(PPU_MIRROR_LOW); break;
				case 3: set_nt_mirroring(PPU_MIRROR_HIGH); break;
			}
			break;
		case 0x0a:
			m_irq_enable = BIT(data, 0);
			m_irq_count = m_irq_latch;
			m_irq_delay = 0;

			m_maincpu6502->cancel_delayed_mapper_irq();
			set_irq_line(CLEAR_LINE);

			if (m_irq_enable && !m_irq_count)
			{
				m_irq_enable = 0;
				m_irq_delay = 2;
			}
			break;
		case 0x0b:
			m_irq_latch = (m_irq_latch & 0xff00) | data;
			break;
		case 0x0c:
			m_irq_latch = (m_irq_latch & 0x00ff) | (data << 8);
			break;
		default:
			LOGMASKED(LOG_UNHANDLED, "lz93d50_write uncaught write, offset: %04x, data: %02x\n", offset, data);
			break;
	}
}

void nes_fcg_device::write_m(offs_t offset, uint8_t data)
{
	LOG("fcg write_m, offset: %04x, data: %02x\n", offset, data);

	if (m_battery.empty() && m_prgram.empty())
		fcg_write(offset & 0x0f, data);
	else if (!m_battery.empty())
		m_battery[offset] = data;
	else
		m_prgram[offset] = data;
}

// FCG board does not access regs in 0x8000-0xffff space!
// only later design lz93d50 (and its variants do)!

void nes_lz93d50_24c01_device::write_h(offs_t offset, uint8_t data)
{
	LOG("lz93d50_24c01 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x0f)
	{
		case 0x0d:
			/*
			    EEPROM control:

			        bit 7 = read direction / release SDA
			        bit 6 = SDA output when writing
			        bit 5 = SCL

			    I2C SDA is open-drain.  When the mapper is reading from
			    EEPROM, it must release SDA so the EEPROM can drive it.
			*/
			m_i2c_dir = BIT(data, 7);
			m_i2cmem->write_sda(m_i2c_dir ? 1 : BIT(data, 6));
			m_i2cmem->write_scl(BIT(data, 5));
			break;

		default:
			fcg_write(offset & 0x0f, data);
			break;
	}
}

uint8_t nes_lz93d50_24c01_device::read_m(offs_t offset)
{
	LOG("lz93d50 EEPROM read, offset: %04x\n", offset);

	return (get_open_bus() & 0xef)
			| (m_i2c_dir ? ((m_i2cmem->read_sda() & 1) << 4) : 0x00);
}

//-------------------------------------------------
//  SERIAL I2C DEVICE
//-------------------------------------------------

void nes_lz93d50_24c01_device::device_add_mconfig(machine_config &config)
{
	I2C_X24C01(config, m_i2cmem);
}

void nes_lz93d50_24c02_device::device_add_mconfig(machine_config &config)
{
	I2C_24C02(config, m_i2cmem);
}


/*-------------------------------------------------

 Bandai BANDAI-JUMP2 board emulation

 This LZ93D50 variant contains 8 KiB of battery-backed
 WRAM and 8 KiB of unbanked CHR RAM.

 Registers $8000-$8003 contain four copies of the outer
 256 KiB PRG-ROM bank bit. PPU A10-A11 select which one
 of these four register outputs drives the PRG bank line.
 Software must therefore write the same value to all four
 registers to prevent PRG banking from changing as the
 PPU renders.

 Registers $8004-$8007 are disabled because the ASIC's
 PPU A12 and A13 inputs are grounded on this board.

 $8008 selects the switchable 16 KiB PRG bank within the
 selected 256 KiB outer bank. $800D bit 5 enables WRAM.

 iNES: mapper 153

 In MAME: Supported

 -------------------------------------------------*/
void nes_fjump2_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address)
{
	nes_fcg_device::ppu_to_mapper(scanline, dot, ppu_tick, ppu_address);

	const uint8_t prg_outer_select = BIT(ppu_address, 10, 2);

	if (m_prg_outer_select != prg_outer_select)
	{
		m_prg_outer_select = prg_outer_select;
		set_prg();
	}
}

void nes_fjump2_device::set_prg()
{
	const uint8_t prg_base = m_reg[m_prg_outer_select] << 4;

	prg16_89ab(prg_base | m_reg[4]);
	prg16_cdef(prg_base | 0x0f);
}

uint8_t nes_fjump2_device::read_m(offs_t offset)
{
	LOG("fjump2 read_m, offset: %04x\n", offset);

	if (!m_wram_enable)
		return get_open_bus();

	return m_battery[offset & (m_battery.size() - 1)];
}

void nes_fjump2_device::write_m(offs_t offset, uint8_t data)
{
	LOG("fjump2 write_m, offset: %04x, data: %02x\n", offset, data);

	if (m_wram_enable)
		m_battery[offset & (m_battery.size() - 1)] = data;
}

void nes_fjump2_device::write_h(offs_t offset, uint8_t data)
{
	LOG("fjump2 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x0f)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		{
			const uint8_t reg = offset & 0x03;

			m_reg[reg] = BIT(data, 0);

			if (reg == m_prg_outer_select)
				set_prg();

			break;
		}

		case 4:
		case 5:
		case 6:
		case 7:
			// PPU A12 and A13 are grounded, so these registers
			// cannot be selected on this board.
			break;

		case 8:
			m_reg[4] = data & 0x0f;
			set_prg();
			break;

		case 0x0d:
			m_wram_enable = BIT(data, 5);
			break;

		default:
			fcg_write(offset & 0x0f, data);
			break;
	}
}