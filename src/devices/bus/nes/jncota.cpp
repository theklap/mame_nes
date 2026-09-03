// license:BSD-3-Clause
// copyright-holders:kmg
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Shenzhen Jncota PCBs


 Here we emulate the following PCBs

 * Jncota KT-1001 [mapper 551]

 ***********************************************************************************************************/


#include "emu.h"
#include "jncota.h"


#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_JNCOTA_KT1001, nes_jncota_kt1001_device, "nes_jncota_kt1001", "NES Cart Shenzhen Jncota KT-1001 PCB")


nes_jncota_kt1001_device::nes_jncota_kt1001_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_JNCOTA_KT1001, tag, owner, clock)
{
}

void nes_jncota_kt1001_device::device_start()
{
	common_start();
	save_item(NAME(m_reg));
}

void nes_jncota_kt1001_device::pcb_reset()
{
	prg32(0);
	chr8(0, CHRROM);

	std::fill(std::begin(m_reg), std::end(m_reg), 0);
}


/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 Shenzhen Jncota board KT-1001 (this is the serial
 on the cart, PCB is likely KT-00X for some X)

 Games: Jing Ke Xin Zhuan, Sheng Huo Lie Zhuan,
 Zhan Guo Feng Yun, Xia Ke Chuan Qi

 This board is very similar to mapper 178 but with
 bankable CHRROM instead of PRGRAM and without
 selectable mirroring.

 NES 2.0: mapper 551

 In MAME: Supported.

 -------------------------------------------------*/

void nes_jncota_kt1001_device::write_l(offs_t offset, u8 data)
{
	LOG("jncota_kt1001 write_l, offset: %04x, data: %02x\n", offset, data);

	offset += 0x100;

	switch (offset & 0x1803) {
		case 0x0800:
		case 0x0801:
		case 0x0802:
		{
			m_reg[offset & 0x03] = data;

			const u16 bank = (u16(m_reg[2]) << 3) | BIT(m_reg[1], 0, 3);
			const u16 mode = !BIT(m_reg[0], 2);

			if (BIT(m_reg[0], 1)) {
				// UNROM mode.
				prg16_89ab(bank);
				prg16_cdef(bank | mode | 0x06);
			}
			else {
				// NROM mode.
				prg16_89ab(bank & ~mode);
				prg16_cdef(bank | mode);
			}
			break;
		}

		case 0x0803:
			chr8(data, CHRROM);
			break;
	}
}