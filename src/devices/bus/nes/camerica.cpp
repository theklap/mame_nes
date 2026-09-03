// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Camerica/Codemasters PCBs


 Here we emulate the following PCBs

 * Camerica BF9093, BF9097, BF909X [mapper 71, two variants]
 * Camerica BF9096 Boards [mapper 232]
 * Camerica Golden Five [mapper 104]

 Aladdin Deck Enhancer pass-thru cart and the corresponding minicarts
 (ALGNV11 & ALGQV11 PCBs) are emulated in a separate source file.

 ***********************************************************************************************************/


#include "emu.h"
#include "camerica.h"


#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_BF9093,  nes_bf9093_device,  "nes_bf9093",  "NES Cart Camerica BF9093 PCB")
DEFINE_DEVICE_TYPE(NES_BF9096,  nes_bf9096_device,  "nes_bf9096",  "NES Cart Camerica BF9096 PCB")
DEFINE_DEVICE_TYPE(NES_BF9096A, nes_bf9096a_device, "nes_bf9096a", "NES Cart Camerica BF9096 Alt PCB")
DEFINE_DEVICE_TYPE(NES_GOLDEN5, nes_golden5_device, "nes_golden5", "NES Cart Camerica Golden 5 PCB")


nes_bf9093_device::nes_bf9093_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_BF9093, tag, owner, clock)
{
}

nes_bf9096_device::nes_bf9096_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock, bool page_swap)
	: nes_nrom_device(mconfig, type, tag, owner, clock), m_reg(0), m_page_swap(page_swap)
{
}

nes_bf9096_device::nes_bf9096_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_bf9096_device(mconfig, NES_BF9096, tag, owner, clock, false)
{
}

nes_bf9096a_device::nes_bf9096a_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_bf9096_device(mconfig, NES_BF9096A, tag, owner, clock, true)
{
}

nes_golden5_device::nes_golden5_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_GOLDEN5, tag, owner, clock), m_lock(0), m_reg(0)
{
}

void nes_bf9093_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	if (m_pcb_ctrl_mirror)
		set_nt_mirroring(PPU_MIRROR_LOW);
}

void nes_bf9096_device::device_start()
{
	common_start();
	save_item(NAME(m_reg));
}

void nes_bf9096_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(3);

	m_reg = 0;
	
}

void nes_golden5_device::device_start()
{
	common_start();
	save_item(NAME(m_lock));
	save_item(NAME(m_reg));

	// The hardware clears these only when cartridge power is applied.
	m_lock = 0;
	m_reg = 0;
}

void nes_golden5_device::pcb_reset()
{
	// A console reset does not clear either mapper register.
	prg16_89ab(m_reg);
	prg16_cdef((m_reg & 0x70) | 0x0f);
}

/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 Camerica BF9093/BF9097 board emulation

 Games include Bee 52, Big Nose the Caveman,
 Fire Hawk, Micro Machines, Mig-29 Soviet Fighter
 and Ultimate Stuntman.

 These boards map a switchable 16 KiB PRG-ROM bank
 at $8000-$BFFF and fix the final bank at
 $C000-$FFFF.

 BF9097, used by Fire Hawk, adds mapper-controlled
 single-screen mirroring. The other mapper 71
 boards use fixed cartridge mirroring.

 Writes throughout $C000-$FFFF select the PRG bank.
 Hardware also uses A0 during writes to $E000-$FFFF
 to control the cartridge CIC stun circuit. The CIC
 circuit has no effect on CPU or PPU mapping and is
 not emulated.

 iNES: mapper 71
 NES 2.0: mapper 71, submapper 1 for BF9097

 In MAME: Supported.

 -------------------------------------------------*/

void nes_bf9093_device::write_h(offs_t offset, u8 data)
{
	LOG("bf9093 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x6000) {
		case 0x0000:
			// $8000-$9FFF: BF9097/Fire Hawk mirroring control.
			if (m_pcb_ctrl_mirror) {
				set_nt_mirroring(BIT(data, 4) ? PPU_MIRROR_HIGH : PPU_MIRROR_LOW);
			}
			break;

		case 0x4000:
		case 0x6000:
			// $C000-$FFFF: 16 KiB PRG-ROM bank at $8000-$BFFF.
			prg16_89ab(data & 0x0f);
			break;
	}
}


/*-------------------------------------------------

 Camerica BF9096 and ALGQV11 board emulation

 The BF9096 board is used by Quattro Adventure,
 Quattro Arcade and Quattro Sports.

 Writes throughout $8000-$BFFF select one of four
 64 KiB outer PRG-ROM blocks using data bits 4-3.

 Writes throughout $C000-$FFFF select one of four
 16 KiB pages within that block using data bits 1-0.
 The selected page is mapped at $8000-$BFFF, while
 page 3 of the selected block is fixed at
 $C000-$FFFF.

 The ALGQV11 Aladdin Deck Enhancer variant swaps the
 two outer-block selection bits.

 Hardware also uses A0 during writes to $E000-$FFFF
 to control the CIC stun circuit. The CIC circuit
 does not affect CPU or PPU mapping and is not
 emulated.

 iNES: mapper 232
 NES 2.0: mapper 232
           submapper 0: BF9096
           submapper 1: ALGQV11

 In MAME: Supported.

 -------------------------------------------------*/

void nes_bf9096_device::write_h(offs_t offset, u8 data)
{
	LOG("bf9096 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset < 0x4000) {
		// $8000-$BFFF: select the 64 KiB outer PRG-ROM block.
		u8 block = BIT(data, 3, 2);

		if (m_page_swap) {
			block = bitswap<2>(block, 0, 1);
		}

		m_reg = (m_reg & 0x03) | (block << 2);
	}
	else {
		// $C000-$FFFF: select the 16 KiB page at $8000-$BFFF.
		m_reg = (m_reg & 0x0c) | BIT(data, 0, 2);
	}

	prg16_89ab(m_reg);
	prg16_cdef((m_reg & 0x0c) | 0x03);
}

/*-------------------------------------------------

 Camerica Golden Five board emulation

 Games: Pegasus 5 in 1, also known as Zlota Piatka
        or Golden Five

 The cartridge contains Big Nose Freaks Out,
 Micro Machines, The Fantastic Adventures of Dizzy,
 Ultimate Stuntman and Big Nose the Caveman.

 Writes throughout $8000-$BFFF select one of five
 256 KiB outer PRG-ROM banks using data bits 2-0.
 Bit 3 permanently locks the outer register until
 the cartridge is powered off.

 Writes throughout $C000-$FFFF select the 16 KiB
 inner PRG-ROM bank at $8000-$BFFF using data
 bits 3-0. The final 16 KiB bank of the selected
 outer bank is fixed at $C000-$FFFF.

 Both registers are cleared at power-on, but they
 are not cleared by a console reset.

 iNES: mapper 104

 In MAME: Supported.

 -------------------------------------------------*/

void nes_golden5_device::write_h(offs_t offset, u8 data)
{
	LOG("golden5 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset >= 0x4000) {
		// $C000-$FFFF: select the switchable 16 KiB inner bank.
		m_reg = (m_reg & 0x70) | (data & 0x0f);
		prg16_89ab(m_reg);
		return;
	}

	if (m_lock) {
		return;
	}

	// $8000-$BFFF: select and optionally lock the outer bank.
	m_lock = BIT(data, 3);
	m_reg = (m_reg & 0x0f) | ((data & 0x07) << 4);

	prg16_89ab(m_reg);
	prg16_cdef((m_reg & 0x70) | 0x0f);
}