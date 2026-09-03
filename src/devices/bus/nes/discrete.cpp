// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for PCBs mostly based on discrete components

 Here we emulate the following PCBs

 * PCB with IC 74x161x161x32 [mapper 70 & 152]
 * PCB with IC 74x139x74 [mapper 87]
 * PCB with IC 74x377 [mapper 11]
 * PCB with IC 74x161x138 [mapper 38]

 ***********************************************************************************************************/


#include "emu.h"
#include "discrete.h"


#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_74X161X161X32, nes_74x161x161x32_device, "nes_74x161",      "NES Cart Discrete Logic (74*161/161/32) PCB")
DEFINE_DEVICE_TYPE(NES_74X139X74,     nes_74x139x74_device,     "nes_74x139",      "NES Cart Discrete Logic (74*139/74) PCB")
DEFINE_DEVICE_TYPE(NES_74X377,        nes_74x377_device,        "nes_74x377",      "NES Cart Discrete Logic (74*377) PCB")
DEFINE_DEVICE_TYPE(NES_74X161X138,    nes_74x161x138_device,    "nes_bitcorp_dis", "NES Cart Discrete Logic (74*161/138) PCB")


nes_74x161x161x32_device::nes_74x161x161x32_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_74X161X161X32, tag, owner, clock)
{
}

nes_74x139x74_device::nes_74x139x74_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_74X139X74, tag, owner, clock)
{
}

nes_74x377_device::nes_74x377_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_74X377, tag, owner, clock)
{
}

nes_74x161x138_device::nes_74x161x138_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_74X161X138, tag, owner, clock)
{
}

void nes_74x161x161x32_device::pcb_reset()
{
	logerror("BANDAI-74*161/161/32 mapper: %u\n", m_pcb_ctrl_mirror ? 152 : 70);
	
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);
	if (m_pcb_ctrl_mirror)
		set_nt_mirroring(PPU_MIRROR_LOW);
}


/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 Discrete logic board using 74x161 counters and
 a 74x32 OR gate

 The board uses bus conflicts when writing its
 PRG-ROM and CHR-ROM bank register.

 Mapper 70 has hardwired nametable mirroring.
 Data bits 4-7 select the 16 KiB PRG-ROM bank and
 data bits 0-3 select the 8 KiB CHR-ROM bank.

 Mapper 152 replaces PRG bank bit 3 with one-screen
 mirroring control. Data bits 4-6 select the PRG-ROM
 bank and data bit 7 selects the CIRAM page.

 iNES: mappers 70 and 152

 In MAME: Supported.

 -------------------------------------------------*/

void nes_74x161x161x32_device::write_h(offs_t offset, u8 data)
{
	LOG("74x161x161x32 write_h, offset: %04x, data: %02x\n", offset, data);

	data = account_bus_conflict(offset, data);

	if (m_pcb_ctrl_mirror) {
		set_nt_mirroring(BIT(data, 7) ? PPU_MIRROR_HIGH : PPU_MIRROR_LOW);
		prg16_89ab(BIT(data, 4, 3));
	}
	else {
		prg16_89ab(BIT(data, 4, 4));
	}

	chr8(data & 0x0f, CHRROM);
}

/*-------------------------------------------------

 Discrete logic board using a 74x139 decoder and
 a 74x74 flip-flop

 Used by several Konami and Jaleco games.

 The board has fixed 32 KiB PRG-ROM and one
 switchable 8 KiB CHR-ROM bank. Writes throughout
 $6000-$7FFF select the CHR-ROM bank.

 Data bits 0 and 1 are connected in reverse order,
 producing the bank number (D0 << 1) | D1. The
 remaining data bits are ignored.

 Nametable mirroring is hardwired by the cartridge.

 iNES: mapper 87

 In MAME: Supported.

 -------------------------------------------------*/

void nes_74x139x74_device::write_m(offs_t offset, u8 data)
{
	LOG("74x139x74 write_m, offset: %04x, data: %02x\n", offset, data);

	chr8(bitswap<2>(data, 0, 1), CHRROM);
}

/*-------------------------------------------------

 Color Dreams discrete logic board using a 74x377
 octal latch

 Used by Color Dreams, Bunch Games and Wisdom Tree,
 with compatible NINA-007 and homebrew variants.

 Writes throughout $8000-$FFFF select one 32 KiB
 PRG-ROM bank and one 8 KiB CHR bank:

     D7-D4: 8 KiB CHR bank
     D3-D0: 32 KiB PRG bank

 Physical Color Dreams boards have a maximum of
 128 KiB PRG-ROM. They use only D0-D1 for PRG
 banking, while D2-D3 operate the CIC-defeat
 circuit. The generalized mapper definition allows
 all four bits to support compatible extended boards.

 Most boards have AND-type bus conflicts. Images
 for prototypes without bus conflicts must disable
 them through their cartridge metadata.

 Nametable mirroring is hardwired.

 iNES: mapper 11

 In MAME: Supported.

 -------------------------------------------------*/

void nes_74x377_device::write_h(offs_t offset, u8 data)
{
	LOG("74x377 write_h, offset: %04x, data: %02x\n", offset, data);

	data = account_bus_conflict(offset, data);

	chr8(BIT(data, 4, 4), m_chr_source);
	prg32(data & 0x0f);
}

/*-------------------------------------------------

 Bit Corporation discrete logic board using a
 74x161 counter and a 74x138 decoder

 Games: Crime Busters

 The write-only bank register is decoded throughout
 $7000-$7FFF:

     D3-D2: 8 KiB CHR-ROM bank
     D1-D0: 32 KiB PRG-ROM bank
     D7-D4: ignored

 Because of incomplete address decoding, accidental
 copies of the register are also present throughout
 $E000-$FFFF.

 Nametable mirroring is hardwired.

 iNES: mapper 38

 In MAME: Supported.

 -------------------------------------------------*/

void nes_74x161x138_device::write_m(offs_t offset, u8 data)
{
	LOG("74x161x138 write_m, offset: %04x, data: %02x\n", offset, data);

	if (offset < 0x1000) {
		return;
	}

	chr8(BIT(data, 2, 2), CHRROM);
	prg32(data & 0x03);
}

void nes_74x161x138_device::write_h(offs_t offset, u8 data)
{
	LOG("74x161x138 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset < 0x6000) {
		return;
	}

	chr8(BIT(data, 2, 2), CHRROM);
	prg32(data & 0x03);
}
