// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***************************************************************************

NES/Famicom cartridge emulation for Bensheng PCBs

This file emulates the Bensheng BMC-BS-5 board used by
several configurable multigame cartridges.

***************************************************************************/

#include "emu.h"
#include "benshieng.h"

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"

//-------------------------------------------------
//  input ports
//-------------------------------------------------

static INPUT_PORTS_START(benshieng)
	PORT_START("CARTDIPS")

	PORT_DIPNAME(0x03, 0x00, "Multicart Mode")
	PORT_DIPSETTING(0x00, "Mode 0")
	PORT_DIPSETTING(0x01, "Mode 1")
	PORT_DIPSETTING(0x02, "Mode 2")
	PORT_DIPSETTING(0x03, "Mode 3")
INPUT_PORTS_END

//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_BENSHIENG, nes_benshieng_device, "nes_benshieng", "NES Cart Bensheng BMC-BS-5 PCB")

nes_benshieng_device::nes_benshieng_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_BENSHIENG, tag, owner, clock)
	, m_cartdips(*this, "CARTDIPS")
{
}

ioport_constructor nes_benshieng_device::device_input_ports() const
{
	return INPUT_PORTS_NAME(benshieng);
}

void nes_benshieng_device::pcb_reset()
{
	// All four CPU windows initially contain the final
	// 8 KiB PRG-ROM bank.
	for (int bank = 0; bank < 4; bank++)
		prg8_x(bank, 0x0f);

	// All four PPU windows initially contain CHR-ROM bank zero.
	for (int bank = 0; bank < 4; bank++)
		chr2_x(bank * 2, 0x00, CHRROM);
}

/*-------------------------------------------------

Bensheng BMC-BS-5

Used by several configurable 4-in-1 multicarts.

CPU $8000-$9FFF writes select one of four 2 KiB
CHR-ROM banks. Address bits A11-A10 select the PPU
window, while address bits A4-A0 select the bank.

CPU $A000-$BFFF writes select one of four 8 KiB
PRG-ROM banks. Address bits A11-A10 select the CPU
window, while address bits A3-A0 select the bank.

The cartridge's four-position mode switch selects
address line A4, A5, A6, or A7 as the PRG-register
write-enable condition.

The written data byte is not used for banking.

NES 2.0: mapper 286

In MAME: Supported.

-------------------------------------------------*/

void nes_benshieng_device::write_h(offs_t offset, u8 data)
{
	LOG("benshieng write_h, offset: %04x, data: %02x\n", offset, data);

	const u8 window = BIT(offset, 10, 2);

	switch (offset & 0x7000)
	{
		case 0x0000:
		case 0x1000:
			chr2_x(window * 2, offset & 0x1f, CHRROM);
			break;

		case 0x2000:
		case 0x3000:
		{
			const u8 dip_setting = m_cartdips->read() & 0x03;

			if (BIT(offset, dip_setting + 4))
				prg8_x(window, offset & 0x0f);

			break;
		}
	}
}