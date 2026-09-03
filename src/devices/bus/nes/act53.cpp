// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************

 NES/Famicom cartridge emulation for the Action 53 multicart mapper

 This emulates the INL-ROM Action 53 Multi-Discrete PCB designed
 by Damian Yerrick (Tepples). It combines the banking capabilities
 of several common discrete cartridge boards in a single multicart
 mapper using iNES mapper 28.

 ***********************************************************************************************************/


#include "emu.h"
#include "act53.h"


#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_ACTION53, nes_action53_device, "nes_action53", "NES Cart Action 53 PCB")


nes_action53_device::nes_action53_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_ACTION53, tag, owner, clock)
	, m_sel(0)
{
}



void nes_action53_device::device_start()
{
	common_start();
	save_item(NAME(m_sel));
	save_item(NAME(m_reg));
}

void nes_action53_device::pcb_start(running_machine &machine, u8 *ciram_ptr, bool cart_mounted)
{
	device_nes_cart_interface::pcb_start(machine, ciram_ptr, cart_mounted);

	// At power-on, the last 16 KiB of PRG ROM must appear at
	// $C000-$FFFF. The remaining hardware state is unspecified,
	// so initialize it deterministically and apply that state.
	m_sel = 0;
	m_reg[0] = 0;
	m_reg[1] = 0;
	m_reg[2] = 0;
	m_reg[3] = (m_prg_chunks - 1) >> 1;

	chr8(m_reg[0] & 0x03, m_chr_source);
	update_prg();
	update_mirr();
}

void nes_action53_device::pcb_reset()
{
	// A soft reset does not change the mapper register contents.
}


/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 Action 53 Multi-Discrete Board

 iNES: mapper 28

 In MAME: Supported.

 The mapper contains four logical registers: $00, $01,
 $80 and $81.

 Writes to $5000-$5FFF select a register using data
 bits 7 and 0:

   D7 D0   Register
   -----   --------
    0  0      $00
    0  1      $01
    1  0      $80
    1  1      $81

 Writes to $8000-$FFFF update the selected register.
 There are no bus conflicts.

 The $5000-$5FFF register-select range is write-only.
 The cartridge does not drive reads from $4100-$5FFF,
 so reads in that range return CPU open bus.

 $6000-$7FFF contains WRAM when fitted to the cartridge;
 otherwise, reads in that range return CPU open bus.

 R:$00:  [...M ..CC]

 C = Inner CHR bank
 M = Mirroring override

 The mirroring bit overwrites bit 0 of R:$80 when bit 1
 of R:$80 is clear.

 R:$01:  [...M PPPP]

 P = Inner PRG bank
 M = Mirroring override

 The mirroring bit overwrites bit 0 of R:$80 when bit 1
 of R:$80 is clear.

 R:$80:  [..GG PSMM]

 G = Game size:
     0 = 32 KiB
     1 = 64 KiB
     2 = 128 KiB
     3 = 256 KiB

 P = PRG banking mode:
     0 = 32 KiB
     1 = 16 KiB

 S = PRG slot selection:
     0 = $8000 fixed to the bottom of the selected
         32 KiB outer bank; $C000 is switchable
     1 = $8000 is switchable; $C000 fixed to the top
         of the selected 32 KiB outer bank

 S is ignored in 32 KiB mode.

 M = Nametable mirroring:
     0 = One-screen, lower CIRAM bank
     1 = One-screen, upper CIRAM bank
     2 = Vertical
     3 = Horizontal

 R:$81:  [BBBB BBBB]

 B = Outer PRG bank

 At power-on, the last 16 KiB of PRG ROM is mapped at
 $C000-$FFFF. All other power-on register state is
 unspecified. A soft reset does not change the mapper
 registers.

 -------------------------------------------------*/

void nes_action53_device::update_prg()
{
	const u8 size = BIT(m_reg[2], 4, 2);
	const u16 mask = u16(0xffffU << (size + 1));
	const bool mode_32k = !BIT(m_reg[2], 3);
	u16 outer = u16(m_reg[3]) << 1;
	const u16 inner = (u16(m_reg[1]) << mode_32k) & ~mask;

	u16 prg_lo = (outer & mask) | inner;
	u16 prg_hi = prg_lo;

	if (mode_32k)
	{
		// 32 KiB mode: the inner bank selects an even 16 KiB
		// bank and CPU A14 selects the following bank.
		++prg_hi;
	}
	else if (BIT(m_reg[2], 2))
	{
		// 16 KiB mode with $8000 switchable and $C000 fixed
		// to the top of the selected 32 KiB outer bank.
		prg_hi = ++outer;
	}
	else
	{
		// 16 KiB mode with $8000 fixed to the bottom of the
		// selected 32 KiB outer bank and $C000 switchable.
		prg_lo = outer;
	}

	prg16_89ab(prg_lo);
	prg16_cdef(prg_hi);
}

void nes_action53_device::update_mirr()
{
	switch (m_reg[2] & 0x03)
	{
		case 0:
			set_nt_mirroring(PPU_MIRROR_LOW);
			break;
		case 1:
			set_nt_mirroring(PPU_MIRROR_HIGH);
			break;
		case 2:
			set_nt_mirroring(PPU_MIRROR_VERT);
			break;
		case 3:
			set_nt_mirroring(PPU_MIRROR_HORZ);
			break;
	}
}

void nes_action53_device::write_l(offs_t offset, u8 data)
{
	LOG("action 53 write_l, offset: %04x, data: %02x\n", offset, data);
	
	// Low cartridge offsets begin at CPU address $4100.
	// Only writes at $5000-$5FFF select a register.
	offset += 0x100;
	if (offset >= 0x1000)
		m_sel = bitswap<2>(data, 7, 0);
}


void nes_action53_device::write_h(offs_t offset, u8 data)
{
	LOG("action 53 write_h, offset: %04x, data: %02x\n", offset, data);

	// Every write must be processed, even if the selected register
	// already contains the same value. Registers $00 and $01 can
	// overwrite mirroring mode bit 0.
	m_reg[m_sel] = data;

	switch (m_sel)
	{
		case 0:
			if (!BIT(m_reg[2], 1))
			{
				m_reg[2] &= 0xfe;
				m_reg[2] |= BIT(data, 4);
				update_mirr();
			}

			chr8(m_reg[0] & 0x03, m_chr_source);
			break;

		case 1:
			if (!BIT(m_reg[2], 1))
			{
				m_reg[2] &= 0xfe;
				m_reg[2] |= BIT(data, 4);
				update_mirr();
			}

			update_prg();
			break;

		case 2:
			update_prg();
			update_mirr();
			break;

		case 3:
			update_prg();
			break;
	}
}
