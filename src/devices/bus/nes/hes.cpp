// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for HES PCBs


 Here we emulate the HES PCBs (both the one with hardwired mirroring and the one with mapper-controlled
 mirroring used by HES 6 in 1) [mapper 113]


 ***********************************************************************************************************/


#include "emu.h"
#include "hes.h"


#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_HES, nes_hes_device, "nes_hes", "NES Cart HES PCB")


nes_hes_device::nes_hes_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_HES, tag, owner, clock)
{
}


/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

HES NTD-8 multicart board

Games: HES 6-in-1, Mind Blower Pak, Total Funpak

This board has a switchable 32 KiB PRG-ROM bank at
$8000-$FFFF and a switchable 8 KiB CHR-ROM bank at
PPU $0000-$1FFF.

The control register is decoded throughout $4100-$41FF,
$4300-$43FF and every subsequent odd-numbered page
through $5F00-$5FFF.

Data bits 3-5 select the PRG-ROM bank. Data bits 0-2
and 6 select the CHR-ROM bank. Data bit 7 controls
nametable mirroring.

The board has no PRG-RAM and no bus conflicts.

iNES: mapper 113

In MAME: Supported.

-------------------------------------------------*/

void nes_hes_device::write_l(offs_t offset, u8 data) {
	LOG("hes write_l, offset: %04x, data: %02x\n", offset, data);

	const offs_t address = offset + 0x4100;

	if (BIT(address, 8)) {
		prg32(BIT(data, 3, 3));
		chr8(bitswap<4>(data, 6, 2, 1, 0), CHRROM);
		set_nt_mirroring(BIT(data, 7) ? PPU_MIRROR_VERT : PPU_MIRROR_HORZ);
	}
}