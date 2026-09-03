// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************

 NES/Famicom cartridge emulation for J.Y. Company ASIC boards

 Supported iNES mappers:
   35  - Type C with 8KB WRAM
   90  - Type A
   209 - Type C
   211 - Type B

 The programmable IRQ counter supports the following clock sources:
   - CPU M2 cycles
   - unfiltered PPU A12 rising edges
   - PPU memory reads
   - CPU writes (not implemented; no known software requires this source)

To-Do:
-CPU-write IRQ source mode 3 remains unimplemented.
-$C007 behavior remains unknown.
-The multiplier currently returns the result immediately; real hardware takes eight M2 cycles.
-Your jumper reading covers $5000/$5400, but not the alternate $5C00 wiring.

 ***********************************************************************************************************/


#include "emu.h"
#include "jy.h"

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------
static INPUT_PORTS_START(nes_jy)
	PORT_START("DIP")
	PORT_DIPNAME(0x40, 0x00, "J.Y. DIP Switch 1")
	PORT_DIPSETTING(0x00, DEF_STR(Off))
	PORT_DIPSETTING(0x40, DEF_STR(On))
	PORT_DIPNAME(0x80, 0x00, "J.Y. DIP Switch 2")
	PORT_DIPSETTING(0x00, DEF_STR(Off))
	PORT_DIPSETTING(0x80, DEF_STR(On))
INPUT_PORTS_END

ioport_constructor nes_jy_typea_device::device_input_ports() const
{
	return INPUT_PORTS_NAME(nes_jy);
}

DEFINE_DEVICE_TYPE(NES_JY_TYPEA, nes_jy_typea_device, "nes_jya", "NES Cart JY Company Type A PCB")
DEFINE_DEVICE_TYPE(NES_JY_TYPEB, nes_jy_typeb_device, "nes_jyb", "NES Cart JY Company Type B PCB")
DEFINE_DEVICE_TYPE(NES_JY_TYPEC, nes_jy_typec_device, "nes_jyc", "NES Cart JY Company Type C PCB")


nes_jy_typea_device::nes_jy_typea_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock) :
	nes_nrom_device(mconfig, type, tag, owner, clock),
	m_accumulator(0),
	m_test(0),
	m_bank_6000(0),
	m_irq_mode(0),
	m_irq_count(0),
	m_irq_prescale(0),
	m_irq_prescale_mask(0xff),
	m_irq_flip(0),
	m_irq_enable(0),
	m_irq_up(0),
	m_irq_down(0),
	m_irq_last_a12(false),
	m_irq_delay(0),
	irq_timer(nullptr),
	m_dips(*this, "DIP")
{
}

nes_jy_typea_device::nes_jy_typea_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	nes_jy_typea_device(mconfig, NES_JY_TYPEA, tag, owner, clock)
{
}

nes_jy_typeb_device::nes_jy_typeb_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock)
	: nes_jy_typea_device(mconfig, type, tag, owner, clock)
{
}

nes_jy_typeb_device::nes_jy_typeb_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_jy_typeb_device(mconfig, NES_JY_TYPEB, tag, owner, clock)
{
}

nes_jy_typec_device::nes_jy_typec_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_jy_typeb_device(mconfig, NES_JY_TYPEC, tag, owner, clock)
{
}

void nes_jy_typea_device::device_start()
{
	common_start();
	irq_timer = timer_alloc(FUNC(nes_jy_typea_device::irq_timer_tick), this);
	irq_timer->reset();
	timer_freq = clocks_to_attotime(1);

	save_item(NAME(m_mul));
	save_item(NAME(m_accumulator));
	save_item(NAME(m_test));
	save_item(NAME(m_mmc_prg_bank));
	save_item(NAME(m_mmc_nt_bank));
	save_item(NAME(m_mmc_vrom_bank));
	save_item(NAME(m_reg));
	save_item(NAME(m_chr_latch));
	save_item(NAME(m_bank_6000));

	save_item(NAME(m_irq_prescale));
	save_item(NAME(m_irq_prescale_mask));
	save_item(NAME(m_irq_mode));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_flip));
	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_up));
	save_item(NAME(m_irq_down));
	save_item(NAME(m_irq_last_a12));
	save_item(NAME(m_irq_delay));
}

void nes_jy_typea_device::pcb_reset()
{
	prg32(0);
	chr8(0, m_chr_source);

	m_mul[0] = 0;
	m_mul[1] = 0;
	m_accumulator = 0;
	m_test = 0;

	memset(m_mmc_prg_bank, 0xff, sizeof(m_mmc_prg_bank));
	memset(m_mmc_nt_bank, 0, sizeof(m_mmc_nt_bank));
	memset(m_mmc_vrom_bank, 0xffff, sizeof(m_mmc_vrom_bank));
	memset(m_reg, 0, sizeof(m_reg));

	m_chr_latch[0] = 0;
	m_chr_latch[1] = 4;
	m_bank_6000 = 0;

	update_prg();
	update_chr();
	update_mirror();

	m_irq_mode = 0;
	m_irq_count = 0;
	m_irq_prescale = 0;
	m_irq_prescale_mask = 0xff;
	m_irq_flip = 0;
	m_irq_enable = 0;
	m_irq_up = 0;
	m_irq_down = 0;
	m_irq_last_a12 = false;
	m_irq_delay = 0;

	irq_timer->adjust(attotime::never);
	set_irq_line(CLEAR_LINE);
}


/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 JY Company Type A board emulation

 iNES: mapper 90

 This board uses standard CIRAM nametable mirroring
 selected by the J.Y. ASIC mirroring register.

 -------------------------------------------------*/


uint8_t nes_jy_typea_device::nt_r(offs_t offset)
{
	int page = ((offset & 0xc00) >> 10);
	irq_clock(false, 2);
	return m_nt_access[page][offset & 0x3ff];
}

uint8_t nes_jy_typea_device::chr_r(offs_t offset)
{
	int bank = offset >> 10;
	irq_clock(false, 2);
	return m_chr_access[bank][offset & 0x3ff];
}

void nes_jy_typea_device::irq_clock(bool blanked, int mode)
{
	if (m_irq_mode != mode)
		return;

	if ((!m_irq_down && !m_irq_up) || (m_irq_down && m_irq_up))
		return;

	bool clock = false;
	bool fire = false;

	if (m_irq_down) {
		if ((m_irq_prescale & m_irq_prescale_mask) == 0) {
			clock = true;

			if (m_irq_prescale_mask == 0x07)
				m_irq_prescale = (m_irq_prescale & 0xf8) | 0x07;
			else
				m_irq_prescale = 0xff;
		} else if (m_irq_prescale_mask == 0x07) {
			m_irq_prescale = (m_irq_prescale & 0xf8) | ((m_irq_prescale - 1) & 0x07);
		} else {
			m_irq_prescale--;
		}
	}

	if (m_irq_up) {
		if ((m_irq_prescale & m_irq_prescale_mask) == m_irq_prescale_mask) {
			clock = true;

			if (m_irq_prescale_mask == 0x07)
				m_irq_prescale &= 0xf8;
			else
				m_irq_prescale = 0;
		} else if (m_irq_prescale_mask == 0x07) {
			m_irq_prescale = (m_irq_prescale & 0xf8) | ((m_irq_prescale + 1) & 0x07);
		} else {
			m_irq_prescale++;
		}
	}

	if (!clock)
		return;

	if (m_irq_down) {
		if (m_irq_count == 0) {
			fire = true;
			m_irq_count = 0xff;
		} else {
			m_irq_count--;
		}
	}

	if (m_irq_up) {
		if (m_irq_count == 0xff) {
			fire = true;
			m_irq_count = 0;
		} else {
			m_irq_count++;
		}
	}

	if (fire && m_irq_enable && !blanked)
		m_irq_delay = 2;
}

TIMER_CALLBACK_MEMBER(nes_jy_typea_device::irq_timer_tick)
{
	irq_clock(false, 0);
}

void nes_jy_typea_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address)
{
	if (m_irq_delay) {
		m_irq_delay--;

		if (!m_irq_delay)
			set_irq_line(ASSERT_LINE);
	}
}

void nes_jy_typea_device::ppu_bus_address(uint16_t address, uint64_t, int, bool)
{
	const bool a12 = BIT(address, 12);

	if (a12 && !m_irq_last_a12)
		irq_clock(false, 1);

	m_irq_last_a12 = a12;
}

// 0x5000-0x5fff : sort of protection?
uint8_t nes_jy_typea_device::read_l(offs_t offset)
{
	LOG("J.Y. read_l, offset: %04x\n", offset);

	offset += 0x100;

	if (offset >= 0x1000 && offset < 0x1800)
		return (get_open_bus() & 0x3f) | m_dips->read();

	if (offset >= 0x1800) {
		switch (offset & 0x07) {
			case 0:
				return (m_mul[0] * m_mul[1]) & 0xff;

			case 1:
				return (m_mul[0] * m_mul[1]) >> 8;

			case 2:
				return m_accumulator;

			case 3:
				return m_test;
		}
	}

	return get_open_bus();
}

void nes_jy_typea_device::write_l(offs_t offset, uint8_t data)
{
	LOG("J.Y. write_l, offset: %04x, data: %02x\n", offset, data);

	offset += 0x100;

	if (offset < 0x1800)
		return;

	switch (offset & 0x07) {
		case 0:
			m_mul[0] = data;
			break;

		case 1:
			m_mul[1] = data;
			break;

		case 2:
			m_accumulator += data;
			break;

		case 3:
			m_accumulator = 0;
			m_test = data;
			break;
	}
}

// $6000-$7FFF: WRAM, or PRG ROM when $D000 bit 7 is set
uint8_t nes_jy_typea_device::read_m(offs_t offset)
{
	LOG("JY Company read_m, offset: %04x\n", offset);

	if (BIT(m_reg[0], 7))
		return m_prg[(m_bank_6000 & m_prg_mask) * 0x2000 + (offset & 0x1fff)];

	return device_nes_cart_interface::read_m(offset);
}

void nes_jy_typea_device::write_m(offs_t offset, uint8_t data)
{
	LOG("JY Company write_m, offset: %04x, data: %02x\n", offset, data);

	if (!BIT(m_reg[0], 7))
		device_nes_cart_interface::write_m(offset, data);
}

inline uint8_t nes_jy_typea_device::unscramble(uint8_t bank)
{
	return bitswap<8>(bank & 0x7f,7,0,1,2,3,4,5,6);
}

void nes_jy_typea_device::update_prg()
{
	uint8_t exPrg = (m_reg[3] & 0x06) << 5;
	uint8_t last = (m_reg[0] & 0x04) ? m_mmc_prg_bank[3] : 0x3f;

	switch (m_reg[0] & 0x03)
	{
		case 0: // 32KB
			prg32((last & 0x0f) | (exPrg >> 2));
			m_bank_6000 = (((m_mmc_prg_bank[3] * 4) + 3) & 0x3f) | (exPrg >> 2);
			break;

		case 1: // 16KB
			prg16_89ab((m_mmc_prg_bank[1] & 0x1f) | (exPrg >> 1));
			prg16_cdef((last & 0x1f) | (exPrg >> 1));
			m_bank_6000 = (((m_mmc_prg_bank[3] * 2) + 1) & 0x1f) | (exPrg >> 1);
			break;

		case 2: // 8KB
			prg8_89(m_mmc_prg_bank[0] | exPrg);
			prg8_ab(m_mmc_prg_bank[1] | exPrg);
			prg8_cd(m_mmc_prg_bank[2] | exPrg);
			prg8_ef(last | exPrg);
			m_bank_6000 = m_mmc_prg_bank[3] | exPrg;
			break;

		case 3: // 8KB Alt
			prg8_89((unscramble(m_mmc_prg_bank[0]) & 0x3f) | exPrg);
			prg8_ab((unscramble(m_mmc_prg_bank[1]) & 0x3f) | exPrg);
			prg8_cd((unscramble(m_mmc_prg_bank[2]) & 0x3f) | exPrg);
			if (m_reg[0] & 0x04)
				prg8_ef((unscramble(m_mmc_prg_bank[3]) & 0x3f) | exPrg);
			else
				prg8_ef((unscramble(last) & 0x3f) | exPrg);
			m_bank_6000 = (unscramble(m_mmc_prg_bank[3]) & 0x3f) | exPrg;
			break;
	}
}

void nes_jy_typea_device::update_chr()
{
	// in 1KB & 2KB mode, PPU 0x800-0xfff always mirrors 0x000-0x7ff (0x1800-0x1fff not affected)
	int chr_mirror_mode = BIT(m_reg[3], 7) << 1;

	// Case (m_reg[3] & 0x20 == 0)
	// Block mode enabled: in this case lower bits select a 256KB page inside CHRROM
	// and the low bytes of m_mmc_vrom_bank select the banks inside such a page

	// docs suggest m_reg[3] & 0x1f for chr_page below,
	// but 45 in 1 (JY-120A) menu requires to use this (from NEStopia)
	uint8_t chr_page = (m_reg[3] & 1) | ((m_reg[3] & 0x18) >> 2);
	uint32_t extra_chr_base = BIT(m_reg[3], 5) ? 0 : (chr_page * 0x100);
	uint32_t extra_chr_mask = BIT(m_reg[3], 5) ? 0xffffff : 0xff;

	switch (m_reg[0] & 0x18)
	{
		case 0x00:  // 8KB
			extra_chr_base >>= 3;
			extra_chr_mask >>= 3;
			chr8(extra_chr_base | (m_mmc_vrom_bank[0] & extra_chr_mask), m_chr_source);
			break;

		case 0x08:  // 4KB
			extra_chr_base >>= 2;
			extra_chr_mask >>= 2;
			// Type A & B games have fixed m_chr_latch[0] = 0 and m_chr_latch[1] = 4
			// Type C games can change them at each CHR access!
			chr4_0(extra_chr_base | (m_mmc_vrom_bank[m_chr_latch[0]] & extra_chr_mask), m_chr_source);
			chr4_4(extra_chr_base | (m_mmc_vrom_bank[m_chr_latch[1]] & extra_chr_mask), m_chr_source);
			break;

		case 0x10:  // 2KB
			extra_chr_base >>= 1;
			extra_chr_mask >>= 1;
			chr2_0(extra_chr_base | (m_mmc_vrom_bank[0] & extra_chr_mask), m_chr_source);
			chr2_2(extra_chr_base | (m_mmc_vrom_bank[2 ^ chr_mirror_mode] & extra_chr_mask), m_chr_source);
			chr2_4(extra_chr_base | (m_mmc_vrom_bank[4] & extra_chr_mask), m_chr_source);
			chr2_6(extra_chr_base | (m_mmc_vrom_bank[6] & extra_chr_mask), m_chr_source);
			break;

		case 0x18:  // 1KB
			chr1_0(extra_chr_base | (m_mmc_vrom_bank[0] & extra_chr_mask), m_chr_source);
			chr1_1(extra_chr_base | (m_mmc_vrom_bank[1] & extra_chr_mask), m_chr_source);
			chr1_2(extra_chr_base | (m_mmc_vrom_bank[2 ^ chr_mirror_mode] & extra_chr_mask), m_chr_source);
			chr1_3(extra_chr_base | (m_mmc_vrom_bank[3 ^ chr_mirror_mode] & extra_chr_mask), m_chr_source);
			chr1_4(extra_chr_base | (m_mmc_vrom_bank[4] & extra_chr_mask), m_chr_source);
			chr1_5(extra_chr_base | (m_mmc_vrom_bank[5] & extra_chr_mask), m_chr_source);
			chr1_6(extra_chr_base | (m_mmc_vrom_bank[6] & extra_chr_mask), m_chr_source);
			chr1_7(extra_chr_base | (m_mmc_vrom_bank[7] & extra_chr_mask), m_chr_source);
			break;
	}
}

void nes_jy_typea_device::update_mirror_typea()
{
	switch (m_reg[1] & 3)
	{
		case 0: set_nt_mirroring(PPU_MIRROR_VERT); break;
		case 1: set_nt_mirroring(PPU_MIRROR_HORZ); break;
		case 2: set_nt_mirroring(PPU_MIRROR_LOW); break;
		case 3: set_nt_mirroring(PPU_MIRROR_HIGH); break;
	}
}

void nes_jy_typea_device::update_banks(int reg)
{
	switch (reg & 3)
	{
		case 0:
			update_prg();
			update_chr();
			update_mirror();
			break;
		case 1:
			update_mirror();
			break;
		case 2:
			update_mirror();
			break;
		case 3:
			update_prg();
			update_chr();
			break;
	}
}


void nes_jy_typea_device::write_h(offs_t offset, uint8_t data)
{
	LOG("J.Y. write_h, offset: %04x, data: %02x\n", offset, data);

	if (BIT(offset, 11) && (offset & 0x7000) != 0x4000)
		return;

	switch (offset & 0x7000) {
		case 0x0000:
			offset &= 3;
			data &= 0x3f;
			if (m_mmc_prg_bank[offset] != data)
			{
				m_mmc_prg_bank[offset] = data;
				update_prg();
			}
			break;
		case 0x1000:
			offset &= 7;
			if ((m_mmc_vrom_bank[offset] & 0xff) != data)
			{
				m_mmc_vrom_bank[offset] = (m_mmc_vrom_bank[offset] & 0xff00) | data;
				update_chr();
			}
			break;
		case 0x2000:
			offset &= 7;
			if ((m_mmc_vrom_bank[offset] & 0xff00) != (data << 8))
			{
				m_mmc_vrom_bank[offset] = (m_mmc_vrom_bank[offset] & 0x00ff) | (data << 8);
				update_chr();
			}
			break;
		case 0x3000:
			if (!(offset & 4))
			{
				offset &= 3;
				m_mmc_nt_bank[offset] = (m_mmc_nt_bank[offset] & 0xff00) | data;
			}
			else
			{
				offset &= 3;
				m_mmc_nt_bank[offset] = (m_mmc_nt_bank[offset] & 0x00ff) | data << 8;
			}
			update_mirror();
			break;
		case 0x4000:
			switch (offset & 7) {
				case 0:
					if (BIT(data, 0)) {
						m_irq_enable = 1;
					} else {
						m_irq_delay = 0;
						m_irq_prescale = 0;
						set_irq_line(CLEAR_LINE);
						m_irq_enable = 0;
					}
					break;

				case 1:
					m_irq_mode = data & 0x03;
					m_irq_prescale_mask = BIT(data, 2) ? 0x07 : 0xff;
					m_irq_down = BIT(data, 7);
					m_irq_up = BIT(data, 6);

					if (m_irq_mode == 0 && m_irq_down != m_irq_up)
						irq_timer->adjust(attotime::zero, 0, timer_freq);
					else
						irq_timer->adjust(attotime::never);
					break;

				case 2:
					m_irq_delay = 0;
					m_irq_prescale = 0;
					set_irq_line(CLEAR_LINE);
					m_irq_enable = 0;
					break;

				case 3:
					m_irq_enable = 1;
					break;

				case 4:
					m_irq_prescale = data ^ m_irq_flip;
					break;

				case 5:
					m_irq_count = data ^ m_irq_flip;
					break;

				case 6:
					m_irq_flip = data;
					break;

				case 7:
					// The function of $c007 is unknown, and no known software uses it.
					break;
			}
			break;
		case 0x5000:
			if (m_reg[offset & 3] != data)
			{
				m_reg[offset & 3] = data;
				update_banks(offset & 3);
			}
			break;
	}
}

/*-------------------------------------------------

 JY Company Type B board emulation

 iNES: mapper 211

 This board supports extended nametable control,
 allowing each nametable page to select CIRAM or
 CHR ROM independently.

 -------------------------------------------------*/

void nes_jy_typeb_device::update_mirror_typeb()
{
	for (int i = 0; i < 4; i++)
	{
		if (BIT(m_reg[0], 6))    // CHRROM
			set_nt_page(i, VROM, m_mmc_nt_bank[i], 0);
		else    // might be either CHRROM or CIRAM
		{
			// CHRROM is only used if bit 7 of the NT Reg does not match bit7 of reg[2].
			if ((m_mmc_nt_bank[i] ^ m_reg[2]) & 0x80)
				set_nt_page(i, VROM, m_mmc_nt_bank[i], 0);
			else
				set_nt_page(i, CIRAM, m_mmc_nt_bank[i] & 1, 1);
		}
	}
}

/*-------------------------------------------------

 JY Company Type C board emulation

 iNES: mappers 35 and 209

 Mapper 35 is equivalent to mapper 209 but explicitly
 specifies 8KB of WRAM.

 These boards can switch between the Type A and
 Type B mirroring modes.

 -------------------------------------------------*/

void nes_jy_typec_device::update_mirror_typec()
{
	if (BIT(m_reg[0], 5))
		update_mirror_typeb();
	else
		update_mirror_typea();
}

uint8_t nes_jy_typec_device::chr_r(offs_t offset)
{
	int bank = offset >> 10;

	irq_clock(false, 2);

	switch (offset & 0xff0) {
		case 0xfd0:
			m_chr_latch[BIT(offset, 12)] = bank & 0x4;
			update_chr();
			break;

		case 0xfe0:
			m_chr_latch[BIT(offset, 12)] = (bank & 0x4) | 0x2;
			update_chr();
			break;
	}

	return m_chr_access[bank][offset & 0x3ff];
}
