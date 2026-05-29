// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Konami PCBs


 Here we emulate the following PCBs (multiple mappers needed due to different wirings in the pcbs
 causing different addresses to be used for bankswitch & irq)

 * Konami VRC-1 [mapper 75]
 * Konami VRC-2 [mapper 22,23,25]
 * Konami VRC-3 [mapper 73]
 * Konami VRC-4 [mapper 21,23,25]
 * Konami VRC-6 [mapper 24,26]
 * Konami VRC-7 [mapper 85]

 ***********************************************************************************************************/


#include "emu.h"
#include "konami.h"

#include "speaker.h"
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

DEFINE_DEVICE_TYPE(NES_VRC1, nes_konami_vrc1_device, "nes_vrc1", "NES Cart Konami VRC-1 PCB")
DEFINE_DEVICE_TYPE(NES_VRC2, nes_konami_vrc2_device, "nes_vrc2", "NES Cart Konami VRC-2 PCB")
DEFINE_DEVICE_TYPE(NES_VRC3, nes_konami_vrc3_device, "nes_vrc3", "NES Cart Konami VRC-3 PCB")
DEFINE_DEVICE_TYPE(NES_VRC4, nes_konami_vrc4_device, "nes_vrc4", "NES Cart Konami VRC-4 PCB")
DEFINE_DEVICE_TYPE(NES_VRC6, nes_konami_vrc6_device, "nes_vrc6", "NES Cart Konami VRC-6 PCB")
DEFINE_DEVICE_TYPE(NES_VRC7, nes_konami_vrc7_device, "nes_vrc7", "NES Cart Konami VRC-7 PCB")


nes_konami_vrc1_device::nes_konami_vrc1_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_VRC1, tag, owner, clock)
{
}

nes_konami_vrc2_device::nes_konami_vrc2_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, type, tag, owner, clock), m_latch(0)
{
}

nes_konami_vrc2_device::nes_konami_vrc2_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_konami_vrc2_device(mconfig, NES_VRC2, tag, owner, clock)
{
}

nes_konami_vrc3_device::nes_konami_vrc3_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_VRC3, tag, owner, clock), m_irq_count(0), m_irq_count_latch(0), m_irq_enable(0), m_irq_enable_latch(0), m_irq_mode(0), irq_timer(nullptr)
{
}

nes_konami_vrc4_device::nes_konami_vrc4_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, type, tag, owner, clock)
	, m_prg_flip(0)
	, m_wram_enable(0)
	, m_irq_count(0)
	, m_irq_count_latch(0)
	, m_irq_enable(0)
	, m_irq_enable_latch(0)
	, m_irq_mode(0)
	, m_irq_prescale(0)
	, m_irq_delay(0)
	, irq_timer(nullptr)
{
}

nes_konami_vrc4_device::nes_konami_vrc4_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_konami_vrc4_device(mconfig, NES_VRC4, tag, owner, clock)
{
}

nes_konami_vrc6_device::nes_konami_vrc6_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_konami_vrc4_device(mconfig, NES_VRC6, tag, owner, clock), m_vrc6snd(*this, "vrc6snd")
{
}

nes_konami_vrc7_device::nes_konami_vrc7_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_konami_vrc4_device(mconfig, NES_VRC7, tag, owner, clock), m_vrc7snd(*this, "vrc7snd")
{
}



void nes_konami_vrc1_device::device_start()
{
	common_start();
	save_item(NAME(m_mmc_vrom_bank));
}

void nes_konami_vrc1_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	m_mmc_vrom_bank[0] = m_mmc_vrom_bank[1] = 0;
}

void nes_konami_vrc2_device::device_start()
{
	common_start();
	save_item(NAME(m_mmc_vrom_bank));
	save_item(NAME(m_latch));
}

void nes_konami_vrc2_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	m_latch = 0;
	std::fill(std::begin(m_mmc_vrom_bank), std::end(m_mmc_vrom_bank), 0x00);
}

void nes_konami_vrc3_device::device_start()
{
	common_start();
	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");
	irq_timer = timer_alloc(FUNC(nes_konami_vrc3_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	save_item(NAME(m_irq_mode));
	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_enable_latch));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_count_latch));
}

void nes_konami_vrc3_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	m_irq_mode = 0;
	m_irq_enable = 0;
	m_irq_enable_latch = 0;
	m_irq_count = 0;
	m_irq_count_latch = 0;
	m_irq_delay = 0;
}

void nes_konami_vrc4_device::device_start()
{
	common_start();
	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");
	irq_timer = timer_alloc(FUNC(nes_konami_vrc4_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	save_item(NAME(m_irq_mode));
	save_item(NAME(m_irq_prescale));
	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_enable_latch));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_count_latch));
	save_item(NAME(m_prg_flip));
	save_item(NAME(m_wram_enable));
	save_item(NAME(m_mmc_prg_bank));
	save_item(NAME(m_mmc_vrom_bank));
	save_item(NAME(m_irq_delay));
}

void nes_konami_vrc4_device::pcb_reset()
{
	m_irq_mode = 0;
	m_irq_prescale = 0;
	m_irq_enable = 0;
	m_irq_enable_latch = 0;
	m_irq_count = 0;
	m_irq_count_latch = 0;
	m_irq_delay = 0;

	m_prg_flip = 0;
	m_wram_enable = 0;
	m_mmc_prg_bank[0] = 0;
	m_mmc_prg_bank[1] = 0;
	set_prg();

	for (int i = 0; i < 8; i++)
		m_mmc_vrom_bank[i] = i;
	set_chr();
}

void nes_konami_vrc7_device::pcb_reset()
{
	nes_konami_vrc4_device::pcb_reset();
	prg32((m_prg_chunks >> 1) - 1);
}

void nes_konami_vrc6_device::device_start()
{
	nes_konami_vrc4_device::device_start();

	save_item(NAME(m_vrc6_b003));
	save_item(NAME(m_vrc6_chr));
}

void nes_konami_vrc6_device::pcb_reset()
{
	nes_konami_vrc4_device::pcb_reset();

	// IMPORTANT:
	// device_nes_cart_interface::nt_r/nt_w still uses the current mirroring mode.
	// Force vertical so offset 0x000 = NTA and offset 0x400 = NTB.
	// Actual VRC6 nametable logic is handled by nt_r/nt_w below.
	set_nt_mirroring(PPU_MIRROR_VERT);

	m_vrc6_b003 = 0;
	m_wram_enable = 0;

	for (int i = 0; i < 8; i++)
		m_vrc6_chr[i] = i;

	prg16_89ab(0);
	prg8_cd((m_prg_chunks * 2) - 2);
	prg8_ef((m_prg_chunks * 2) - 1);

	set_chr();
}


/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 Konami VRC1 and Jaleco JF20, JF22

 Games: Exciting Boxing, Ganbare Goemon!, Tetsuwan Atom

 iNES: mapper 75

 In MAME: Supported.

 -------------------------------------------------*/

void nes_konami_vrc1_device::write_h(offs_t offset, u8 data)
{
	LOG("VRC-1 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x7000)
	{
		case 0x0000:
		case 0x2000:
		case 0x4000:
			prg8_x(BIT(offset, 13, 2), data & 0x0f);
			break;
		case 0x1000:
			set_nt_mirroring(data & 1 ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			m_mmc_vrom_bank[0] = (m_mmc_vrom_bank[0] & 0x0f) | BIT(data, 1) << 4;
			m_mmc_vrom_bank[1] = (m_mmc_vrom_bank[1] & 0x0f) | BIT(data, 2) << 4;
			chr4_0(m_mmc_vrom_bank[0], CHRROM);
			chr4_4(m_mmc_vrom_bank[1], CHRROM);
			break;
		case 0x6000:
			m_mmc_vrom_bank[0] = (m_mmc_vrom_bank[0] & 0x10) | (data & 0x0f);
			chr4_0(m_mmc_vrom_bank[0], CHRROM);
			break;
		case 0x7000:
			m_mmc_vrom_bank[1] = (m_mmc_vrom_bank[1] & 0x10) | (data & 0x0f);
			chr4_4(m_mmc_vrom_bank[1], CHRROM);
			break;
	}
}

/*-------------------------------------------------

 Konami VRC-2

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_konami_vrc2_device::read_m(offs_t offset)
{
	LOG("VRC-2 read_m, offset: %04x\n", offset);

	if (!m_battery.empty() || !m_prgram.empty())
		return device_nes_cart_interface::read_m(offset);
	else    // VRC2 was planned with EEPROM support; the non-working feature behaves as a vestigial 1-bit latch in $6000-$6fff on certain boards (contraj, ggoemon2 depend on this)
		return (offset < 0x1000) ? (get_open_bus() & 0xfe) | (m_latch & 1) : get_open_bus();
}

void nes_konami_vrc2_device::write_m(offs_t offset, u8 data)
{
	LOG("VRC-2 write_m, offset: %04x, data: %02x\n", offset, data);

	if (!m_battery.empty() || !m_prgram.empty())
		device_nes_cart_interface::write_m(offset, data);
	else if (offset < 0x1000)
		m_latch = data;
}

void nes_konami_vrc2_device::write_h(offs_t offset, u8 data)
{
	LOG("VRC-2 write_h, offset: %04x, data: %02x\n", offset, data);

	u8 addr_lines = bitswap<2>(offset, m_vrc_ls_prg_a, m_vrc_ls_prg_b);

	switch (offset & 0x7000)
	{
		case 0x0000:
		case 0x2000:
			prg8_x(BIT(offset, 13), data);
			break;

		case 0x1000:
			set_nt_mirroring(data & 1 ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			break;

		case 0x3000:
		case 0x4000:
		case 0x5000:
		case 0x6000:
		{
			int bank  = 2 * (BIT(offset, 12, 3) - 3) + BIT(addr_lines, 1);
			int shift = BIT(addr_lines, 0) * 4;

			// VRC2 uses 4-bit writes for each half
			u8 nib  = data & 0x0f;
			u8 mask = 0x0f << shift;

			// 1) Merge nibble WITHOUT any VRC2a shift
			m_mmc_vrom_bank[bank] = (m_mmc_vrom_bank[bank] & ~mask) | ((nib << shift) & mask);

			// 2) Apply VRC2a quirk only when *using* the value
			u8 chr_bank = m_mmc_vrom_bank[bank];
			if (m_vrc_ls_chr)          // for mapper 22 set this to 1
				chr_bank >>= 1;        // VRC2a ignores low bit

			chr1_x(bank, chr_bank, m_chr_source);
			break;
		}

		default:
			LOGMASKED(LOG_UNHANDLED, "VRC-2 write_h uncaught write, addr: %04x value: %02x\n", offset + 0x8000, data);
			break;
	}
}


/*-------------------------------------------------

 Konami VRC3

 Games: Salamander

 iNES: mapper 73

 In MAME: Supported.

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_konami_vrc3_device::irq_timer_tick)
{
	if (m_irq_enable)
	{
		u16 mask = m_irq_mode ? 0x00ff : 0xffff; // 8 or 16 bit mode?

		// upper byte only incremented and reloaded in 16-bit mode
		m_irq_count = (m_irq_count & ~mask) | ((m_irq_count + 1) & mask);
		if (!(m_irq_count & mask))
		{
			m_irq_delay = 2;  //set_irq_line(ASSERT_LINE);
			m_irq_count = (m_irq_count & ~mask) | (m_irq_count_latch & mask);
		}
	}
}

void nes_konami_vrc3_device::ppu_to_mapper(int scanline, unsigned dot) {
	if (m_irq_delay > 0)
	{
		--m_irq_delay;
		if (m_irq_delay == 0)
			m_maincpu6502->queue_delayed_mapper_irq(2);
	}
}

void nes_konami_vrc3_device::write_h(offs_t offset, u8 data)
{
	LOG("VRC-3 write_h, offset: %04x, data: %02x\n", offset, data);

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
			m_irq_mode = BIT(data, 2);
			m_irq_enable = BIT(data, 1);
			m_irq_enable_latch = BIT(data, 0);
			
			if (m_irq_enable)
				m_irq_count = m_irq_count_latch;
			
			if (m_irq_delay > 0)
				m_maincpu6502->cancel_delayed_mapper_irq();
			
			m_irq_delay = 0;
			set_irq_line(CLEAR_LINE);
			break;
		case 0x5000:
			m_irq_enable = m_irq_enable_latch;
			
			if (m_irq_delay > 0)
				m_maincpu6502->cancel_delayed_mapper_irq();

			m_irq_delay = 0;
			set_irq_line(CLEAR_LINE);
			break;
		case 0x7000:
			prg16_89ab(data & 0x07);
			break;
		default:
			LOGMASKED(LOG_UNHANDLED, "VRC-3 write_h uncaught write, offset %04x, data: %02x\n", offset, data);
			break;
	}
}

/*-------------------------------------------------

 Konami VRC-4

 In MAME: Supported

 -------------------------------------------------*/
void nes_konami_vrc4_device::ppu_to_mapper(int scanline, unsigned dot) {
	if (m_irq_delay > 0)
	{
		--m_irq_delay;
		if (m_irq_delay == 0)
			m_maincpu6502->queue_delayed_mapper_irq(2);
	}
}


void nes_konami_vrc4_device::irq_tick()
{
	if (m_irq_count == 0xff)
	{
		m_irq_count = m_irq_count_latch;
		m_irq_delay = 2; //set_irq_line(ASSERT_LINE);
	}
	else
	{
		m_irq_count++;
	}
}

TIMER_CALLBACK_MEMBER(nes_konami_vrc4_device::irq_timer_tick)
{
	if (m_irq_enable)
	{
		if (m_irq_mode) // cycle mode
			irq_tick();
		else    // scanline mode
		{
			// A prescaler divides the passing CPU cycles by 114, 114, then 113 (and repeats that order).
			// This approximates 113+2/3 CPU cycles, which is one NTSC scanline.
			// Since this is a CPU-based IRQ, though, it is triggered also during non visible scanlines...
			m_irq_prescale -= 3;

			if (m_irq_prescale <= 0)
			{
				m_irq_prescale += 341;
				irq_tick();
			}
		}
	}
}

void nes_konami_vrc4_device::irq_ack_w()
{
	if(m_irq_delay > 0)
		m_maincpu6502->cancel_delayed_mapper_irq();
	
	m_irq_enable = m_irq_enable_latch;
	m_irq_delay = 0;
	set_irq_line(CLEAR_LINE);
}

void nes_konami_vrc4_device::irq_ctrl_w(u8 data)
{
	if(m_irq_delay > 0)
		m_maincpu6502->cancel_delayed_mapper_irq();

	m_irq_mode = BIT(data, 2);
	m_irq_enable = BIT(data, 1);
	m_irq_enable_latch = BIT(data, 0);

	// Any write to IRQ control acknowledges pending IRQ and resets prescaler.
	m_irq_delay = 0;
	set_irq_line(CLEAR_LINE);
	m_irq_prescale = 341;

	// Reload IRQ counter from latch only if E is set.
	if (m_irq_enable)
		m_irq_count = m_irq_count_latch;
}

void nes_konami_vrc4_device::set_mirror(u8 data)
{
	switch (data & 0x03)
	{
		case 0x00: set_nt_mirroring(PPU_MIRROR_VERT); break;
		case 0x01: set_nt_mirroring(PPU_MIRROR_HORZ); break;
		case 0x02: set_nt_mirroring(PPU_MIRROR_LOW); break;
		case 0x03: set_nt_mirroring(PPU_MIRROR_HIGH); break;
	}
}

void nes_konami_vrc4_device::set_prg(int prg_base, int prg_mask)
{
	const u8 bank0  = prg_base | (m_mmc_prg_bank[0] & prg_mask);
	const u8 bank1  = prg_base | (m_mmc_prg_bank[1] & prg_mask);
	const u8 fixed2 = prg_base | (prg_mask & ~1);
	const u8 fixed3 = prg_base | prg_mask;

	if (!m_prg_flip)
	{
		// normal: $8000 switchable, $A000 switchable, $C000 fixed, $E000 fixed
		prg8_x(0, bank0);
		prg8_x(1, bank1);
		prg8_x(2, fixed2);
		prg8_x(3, fixed3);
	}
	else
	{
		// flipped: $8000 fixed, $A000 switchable, $C000 switchable, $E000 fixed
		prg8_x(0, fixed2);
		prg8_x(1, bank1);
		prg8_x(2, bank0);
		prg8_x(3, fixed3);
	}
}

void nes_konami_vrc4_device::set_chr(int chr_base, int chr_mask)
{
	for (int i = 0; i < 8; i++)
		chr1_x(i, chr_base | (m_mmc_vrom_bank[i] & chr_mask), m_chr_source);
}

u8 nes_konami_vrc4_device::read_m(offs_t offset)
{
	
	LOG("VRC-4 read_m, offset: %04x\n", offset);

	if (m_wram_enable)
		return device_nes_cart_interface::read_m(offset);
	else
		return get_open_bus();
}

void nes_konami_vrc4_device::write_m(offs_t offset, u8 data)
{
	LOG("VRC-4 write_m, offset: %04x, data: %02x\n", offset, data);

	if (m_wram_enable)
		device_nes_cart_interface::write_m(offset, data);
}

void nes_konami_vrc4_device::write_h(offs_t offset, u8 data)
{
	LOG("VRC-4 write_h, offset: %04x, data: %02x\n", offset, data);

	u8 addr_lines = bitswap<2>(offset, m_vrc_ls_prg_a, m_vrc_ls_prg_b);
	
	switch (offset & 0x7000)
	{
		case 0x0000:
		case 0x2000:
			m_mmc_prg_bank[BIT(offset, 13)] = data;
			set_prg();
			break;

		case 0x1000:
			switch (addr_lines)
			{
				case 0:
					// $9000 - mirroring
					set_mirror(data);
					break;

				case 2:
					m_wram_enable = data & 0x01;
					m_prg_flip = (data >> 1) & 0x01;
					set_prg();
					break;

				case 3:
					// $9003 - external select strobe on VRC4
					// Not currently used here.
					break;

				default:
					// $9001 or any otherwise-unused decode
					break;
			}
			break;

		case 0x3000:
		case 0x4000:
		case 0x5000:
		case 0x6000:
		{
			int bank = 2 * (BIT(offset, 12, 3) - 3) + BIT(addr_lines, 1);
			int shift = BIT(addr_lines, 0) * 4;
			int mask = shift ? 0x1f0 : 0x0f;
			m_mmc_vrom_bank[bank] = (m_mmc_vrom_bank[bank] & ~mask) | ((data << shift) & mask);
			set_chr();
			break;
		}

		case 0x7000:
			switch (addr_lines)
			{
				case 0:
					m_irq_count_latch = (m_irq_count_latch & 0xf0) | (data & 0x0f);
					break;

				case 1:
					m_irq_count_latch = (m_irq_count_latch & 0x0f) | ((data & 0x0f) << 4);
					break;

				case 2:
					irq_ctrl_w(data);
					break;

				case 3:
					irq_ack_w();
					break;
			}
			break;
	}
}

/*-------------------------------------------------

 Konami VRC-6

 In MAME: Supported. It also uses konami_irq (there are IRQ
 issues though: see Akumajou Densetsu intro).

 TODO: Implement other CHR modes not used by any of
 the three released VRC6 games?

 -------------------------------------------------*/
u8 nes_konami_vrc6_device::vrc6_nt_bank(int nt) const
{
	nt &= 3;

	int reg = 6;

	switch (m_vrc6_b003 & 0x07)
	{
		case 0:
		case 6:
		case 7: // h-mirror-ish: 6677
			reg = 6 | (nt >> 1);
			break;

		case 2:
		case 3:
		case 4: // v-mirror-ish: 6767
			reg = 6 | (nt & 1);
			break;

		case 1:
		case 5: // 4-screen-ish: 4567
			reg = 4 | nt;
			break;
	}

	u8 bank = m_vrc6_chr[reg];

	// Bit 5 set = replace/force A10 behavior.
	if (BIT(m_vrc6_b003, 5))
	{
		switch (m_vrc6_b003 & 0x0f)
		{
			case 0:
			case 7: // vertical
				bank &= 0xfe;
				bank |= nt & 1;
				break;

			case 3:
			case 4: // horizontal
				bank &= 0xfe;
				bank |= nt >> 1;
				break;

			case 8:
			case 15: // 1-screen A
				bank &= 0xfe;
				break;

			case 11:
			case 12: // 1-screen B
				bank |= 1;
				break;
		}
	}

	return bank;
}

u8 nes_konami_vrc6_device::nt_r(offs_t offset)
{
	offset &= 0x0fff;

	const int nt = offset >> 10;
	const offs_t inner = offset & 0x03ff;

	u8 bank = vrc6_nt_bank(nt);

	if (!BIT(m_vrc6_b003, 4))
	{
		// CIRAM / NTRAM mode
		bank &= 1;
		return device_nes_cart_interface::nt_r((bank * 0x400) + inner);
	}

	// CHR-ROM nametable mode:
	// raw 1 KB CHR ROM bank selected by VRC6 $B003 logic
	return get_vrom_base()[
		((bank * 0x400) + inner) & (get_vrom_size() - 1)
	];
}

void nes_konami_vrc6_device::nt_w(offs_t offset, u8 data)
{
	offset &= 0x0fff;

	const int nt = offset >> 10;
	const offs_t inner = offset & 0x03ff;

	u8 bank = vrc6_nt_bank(nt);

	if (!BIT(m_vrc6_b003, 4))
	{
		bank &= 1;
		device_nes_cart_interface::nt_w((bank * 0x400) + inner, data);
	}
}

void nes_konami_vrc6_device::write_h(offs_t offset, u8 data)
{
	LOG("VRC-6 write_h, offset: %04x, data: %02x\n", offset, data);

	const u8 addr_lines = bitswap<2>(offset, m_vrc_ls_prg_a, m_vrc_ls_prg_b);

	switch (offset & 0x7000)
	{
		case 0x0000:
			// $8000-$8003 : 16 KB PRG at $8000
			prg16_89ab(data & 0x0f);
			break;

		case 0x1000:
			// $9000-$9002 : pulse 1
			// $9003       : not used as audio register
			if (addr_lines <= 2)
				m_vrc6snd->write(addr_lines, data);
			break;

		case 0x2000:
			// $A000-$A002 : pulse 2
			if (addr_lines <= 2)
				m_vrc6snd->write(addr_lines | 0x100, data);
			break;

		case 0x3000:
		if (addr_lines == 3)
		{
			// $B003 : PPU banking style / mirroring / WRAM enable
			m_vrc6_b003 = data;

			// For normal commercial mode 0 with bit 5 set:
			// data bits 3-2 select V/H/1screen A/1screen B.
			//set_mirror((data >> 2) & 0x03);

			m_wram_enable = BIT(data, 7);

			// Re-apply CHR because bits 0-1 and bit 5 change how CHR regs map.
			set_chr();
		}
		else
		{
			m_vrc6snd->write(addr_lines | 0x200, data);
		}
		break;

		case 0x4000:
			// $C000-$C003 : 8 KB PRG at $C000
			prg8_cd(data & 0x1f);
			break;

		case 0x5000:
			// $D000-$D003 : CHR banks 0-3
			m_vrc6_chr[addr_lines] = data;
			set_chr();
			break;

		case 0x6000:
			// $E000-$E003 : CHR banks 4-7
			m_vrc6_chr[4 + addr_lines] = data;
			set_chr();
			break;

		case 0x7000:
			switch (addr_lines)
			{
				case 0:
					// $F000 : IRQ latch
					m_irq_count_latch = data;
					break;

				case 1:
					// $F001 : IRQ control
					irq_ctrl_w(data);
					break;

				case 2:
					// $F002 : IRQ acknowledge
					irq_ack_w();
					break;

				default:
					// $F003 unused
					break;
			}
			break;
	}
}

void nes_konami_vrc6_device::set_chr()
{
	static const u8 ptables[3][8] =
	{
		{ 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07 },
		{ 0x80,0xc0,0x81,0xc1,0x82,0xc2,0x83,0xc3 },
		{ 0x00,0x01,0x02,0x03,0x84,0xc4,0x85,0xc5 }
	};

	int mode = m_vrc6_b003 & 0x03;
	if (mode == 3)
		mode = 2;

	for (int i = 0; i < 8; i++)
	{
		const u8 pt = ptables[mode][i];
		u8 bank = m_vrc6_chr[pt & 7];

		if (BIT(m_vrc6_b003, 5) && BIT(pt, 7))
		{
			bank &= 0xfe;
			if (BIT(pt, 6))
				bank |= 1;
		}

		chr1_x(i, bank, m_chr_source);
	}
}


//-------------------------------------------------
//  device_add_mconfig - add device configuration
//-------------------------------------------------

void nes_konami_vrc6_device::device_add_mconfig(machine_config &config)
{
	// additional sound hardware
	SPEAKER(config, "addon").front_center();

	// TODO: this is not how VRC6 clock signaling works!
	// The board uses the CLK pin in reality, not hardcoded NTSC values!
	VRC6(config, m_vrc6snd, XTAL(21'477'272)/12).add_route(ALL_OUTPUTS, "addon", 1.0);
}

/*-------------------------------------------------

 Konami VRC7

 Games: Lagrange Point, Tiny Toon Adventures 2

 iNES: mapper 85

 In MAME: Supported. It also uses konami_irq.

 -------------------------------------------------*/

void nes_konami_vrc7_device::write_h(offs_t offset, u8 data)
{
	LOG("VRC-7 write_h, offset: %04x, data: %02x\n", offset, data);

	u8 reg = bitswap<4>(offset, 14, 13, 12, m_vrc_ls_prg_a);

	switch (reg)
	{
		case 0x0:
		case 0x1:
		case 0x2:
			// 8 KB PRG banks at $8000, $A000, $C000
			prg8_x(reg, data & 0x3f);
			break;

		case 0x3:
			// FM synth register select / data
			// One address selects register, the other writes data.
			if (BIT(offset, 5))
				m_vrc7snd->data_w(data);
			else
				m_vrc7snd->address_w(data);
			break;

		case 0x4:
		case 0x5:
		case 0x6:
		case 0x7:
		case 0x8:
		case 0x9:
		case 0xa:
		case 0xb:
			// 1 KB CHR banks 0-7
			chr1_x(reg - 4, data, m_chr_source);
			break;

		case 0xc:
			// Mirroring + WRAM enable
			set_mirror(data & 0x03);
			m_wram_enable = BIT(data, 7);
			break;

		case 0xd:
			// IRQ latch
			m_irq_count_latch = data;
			break;

		case 0xe:
			// IRQ control
			irq_ctrl_w(data);
			break;

		case 0xf:
			// IRQ acknowledge
			irq_ack_w();
			break;
	}
}


//-------------------------------------------------
//  MACHINE_DRIVER( vrc7 )
//-------------------------------------------------

// From NESdev wiki: The VRC7, in addition to being a mapper chip, also produces 6 channels of
// 2-operator FM Synthesis Audio. It is a derivative of the Yamaha YM2413 OPLL, implementing a
// subset of its features and containing a custom fixed patch set.
// The synthesis core is related to the Yamaha YM2413 OPLL:
// - Register layout is the same.
// - VRC7 has 6 channels, OPLL has 9.
// - VRC7 has no rhythm channels, OPLL does (the last 3 channels are either FM or Rhythm on OPLL).
// - VRC7 built-in instruments are not the same as OPLL instruments.
// - VRC7 has no readily-accessible status register, under normal circumstances it is write-only;
//   OPLL has an undocumented, 2-bit 'internal state' register.
// - VRC7 has an internal state output pin (may be serial version of the 2 bit internal state register)
//   and has one output pin for audio, multiplexed for all 6 channels; OPLL has two output pins, one for
//   FM and one for Rhythm, and has no special status pin.

void nes_konami_vrc7_device::device_add_mconfig(machine_config &config)
{
	// additional sound hardware
	SPEAKER(config, "addon").front_center();

	// TODO: this is not how VRC7 clock signaling works!
	// The board uses the CLK pin in reality, not hardcoded NTSC values!
	//DS1001(config, m_vrc7snd, XTAL(21'477'272)/6).add_route(0, "addon", 1.0).add_route(1, "addon", 0.0);
	DS1001(config, m_vrc7snd, XTAL(21'477'272)/6).add_route(ALL_OUTPUTS, "addon", 1.0);
}
