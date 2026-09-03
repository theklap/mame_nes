// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for C&E PCBs


 Here we emulate the following PCBs

 * C&E Decathlon [mapper 244]
 * C&E Feng Shen Bang [mapper 246]
 * C&E Sheng Huo Lie Zhuan [mapper 240]

 ***********************************************************************************************************/


#include "emu.h"
#include "cne.h"


#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_CNE_DECATHL, nes_cne_decathl_device, "nes_cne_deca", "NES Cart C&E Decathlon PCB")
DEFINE_DEVICE_TYPE(NES_CNE_FSB,     nes_cne_fsb_device,     "nes_cne_fsb",  "NES Cart C&E Feng Shen Bang PCB")
DEFINE_DEVICE_TYPE(NES_CNE_SHLZ,    nes_cne_shlz_device,    "nes_cne_shlz", "NES Cart C&E Sheng Huo Lie Zhuan PCB")


nes_cne_decathl_device::nes_cne_decathl_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_CNE_DECATHL, tag, owner, clock)
{
}

nes_cne_fsb_device::nes_cne_fsb_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_CNE_FSB, tag, owner, clock)
{
}

nes_cne_shlz_device::nes_cne_shlz_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_CNE_SHLZ, tag, owner, clock)
{
}

void nes_cne_fsb_device::device_start()
{
	common_start();

	save_item(NAME(m_prg_reg));
	save_item(NAME(m_chr_reg));

	for (int i = 0; i < 4; i++) {
		m_prg_reg[i] = 0xff;
		m_chr_reg[i] = 0x00;
	}
}

void nes_cne_fsb_device::pcb_reset()
{
	for (int i = 0; i < 4; i++) {
		prg8_x(i, m_prg_reg[i]);
		chr2_x(i << 1, m_chr_reg[i], CHRROM);
	}
}

/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 C&E Decathlon board emulation

 Games: Decathlon

 Writes throughout $8000-$FFFF select either a
 32 KiB PRG-ROM bank or an 8 KiB CHR-ROM bank.

 Data bit 3 selects PRG or CHR banking. Data bits
 6-4 rearrange the selected bank number as a simple
 copy-protection mechanism.

 The board has bus conflicts, so the mapper receives
 the bitwise AND of the CPU data and currently
 mapped PRG-ROM data.

 Cat Ninden Teyandee translations incorrectly marked
 as mapper 244 are MMC3 games and should not use
 this device.

 iNES: mapper 244

 In MAME: Supported.

 -------------------------------------------------*/

void nes_cne_decathl_device::write_h(offs_t offset, u8 data)
{
	static constexpr u8 prg_permutation[4][4] = {
		{ 0, 1, 2, 3 },
		{ 3, 2, 1, 0 },
		{ 0, 2, 1, 3 },
		{ 3, 1, 2, 0 }
	};

	static constexpr u8 chr_permutation[8][8] = {
		{ 0, 1, 2, 3, 4, 5, 6, 7 },
		{ 0, 2, 1, 3, 4, 6, 5, 7 },
		{ 0, 1, 4, 5, 2, 3, 6, 7 },
		{ 0, 4, 1, 5, 2, 6, 3, 7 },
		{ 0, 4, 2, 6, 1, 5, 3, 7 },
		{ 0, 2, 4, 6, 1, 3, 5, 7 },
		{ 7, 6, 5, 4, 3, 2, 1, 0 },
		{ 7, 6, 5, 4, 3, 2, 1, 0 }
	};

	LOG("cne_decathl write_h, offset: %04x, data: %02x\n", offset, data);

	data = account_bus_conflict(offset, data);

	if (BIT(data, 3)) {
		const u8 permutation = BIT(data, 4, 3);
		const u8 bank = BIT(data, 0, 3);

		chr8(chr_permutation[permutation][bank], CHRROM);
	}
	else {
		const u8 permutation = BIT(data, 4, 2);
		const u8 bank = BIT(data, 0, 2);

		prg32(prg_permutation[permutation][bank]);
	}
}

/*-------------------------------------------------

 C & E G0151-1 board

 Games: Fēngshénbǎng: Fúmó Sān Tàizǐ

 Four independently switchable 8 KiB PRG-ROM banks
 are mapped at $8000-$FFFF. Four independently
 switchable 2 KiB CHR-ROM banks are mapped at
 PPU $0000-$1FFF.

 Registers $6000-$6003 select the PRG-ROM banks,
 while $6004-$6007 select the CHR-ROM banks.
 A3 and A4 are not decoded, mirroring the registers
 through $601F.

 The board has 2 KiB of battery-backed PRG-RAM at
 $6800-$6FFF.

 Reads from $FFE4-$FFE7, $FFEC-$FFEF,
 $FFF4-$FFF7 and $FFFC-$FFFF force PRG address
 line A17 high.

 Nametable mirroring is fixed by the cartridge
 header.

 iNES: mapper 246

 In MAME: Supported.

 -------------------------------------------------*/

void nes_cne_fsb_device::write_m(offs_t offset, u8 data)
{
	LOG("cne_fsb write_m, offset: %04x, data: %02x\n", offset, data);

	if (offset < 0x0020) {
		const int reg = offset & 0x07;

		if (reg < 4) {
			m_prg_reg[reg] = data;
			prg8_x(reg, data);
		}
		else {
			m_chr_reg[reg & 0x03] = data;
			chr2_x((reg & 0x03) << 1, data, CHRROM);
		}

		return;
	}

	if (offset >= 0x0800 && offset < 0x1000) {
		device_nes_cart_interface::write_m(offset - 0x0800, data);
	}
}

u8 nes_cne_fsb_device::read_m(offs_t offset)
{
	LOG("cne_fsb read_m, offset: %04x\n", offset);

	if (offset >= 0x0800 && offset < 0x1000) {
		return device_nes_cart_interface::read_m(offset - 0x0800);
	}

	return get_open_bus();
}

u8 nes_cne_fsb_device::read_h(offs_t offset)
{
	LOG("cne_fsb read_h, offset: %04x\n", offset);

	if ((offset & 0x7fe4) == 0x7fe4) {
		const u8 bank = m_prg_reg[3] | 0x10;

		return m_prg[((bank * 0x2000) + (offset & 0x1fff)) & (m_prg_size - 1)];
	}

	return hi_access_rom(offset);
}

/*-------------------------------------------------

 C & E boards for Jing Ke Xin Zhuan and
 Sheng Huo Lie Zhuan

 These GNROM-like boards select a 32 KiB PRG-ROM
 bank with data bits 4-5 and an 8 KiB CHR-ROM bank
 with data bits 0-3.

 The two known boards decode the register differently:

 Jing Ke Xin Zhuan:
     (address & $E800) == $4800

 Sheng Huo Lie Zhuan:
     (address & $E100) == $4100

 Mapper 240 does not distinguish between these board
 variants, so both known address-decode patterns are
 accepted.

 Both boards map PRG-RAM at $6000-$7FFF. Nametable
 mirroring is fixed by the cartridge header.

 iNES: mapper 240

 In MAME: Supported.

 -------------------------------------------------*/

void nes_cne_shlz_device::write_l(offs_t offset, u8 data)
{
	LOG("cne_shlz write_l, offset: %04x, data: %02x\n", offset, data);

	const offs_t address = offset + 0x4100;
	const bool jing_ke_decode = (address & 0xe800) == 0x4800;
	const bool sheng_huo_decode = (address & 0xe100) == 0x4100;

	if (jing_ke_decode || sheng_huo_decode) {
		prg32(BIT(data, 4, 2));
		chr8(data & 0x0f, CHRROM);
	}
}
