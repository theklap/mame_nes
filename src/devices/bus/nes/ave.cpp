// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for AVE PCBs


 Here we emulate the following PCBs

	* AVE NINA-001/NINA-002 [mapper 34, submapper 1]
	* AVE NINA-03/NINA-06 [mapper 79]
	* AVE Maxi 15 [mapper 234]


 ***********************************************************************************************************/


#include "emu.h"
#include "ave.h"


#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_NINA001, nes_nina001_device, "nes_nina001", "NES Cart AVE Nina-001 PCB")
DEFINE_DEVICE_TYPE(NES_NINA006, nes_nina006_device, "nes_nina006", "NES Cart AVE Nina-006 PCB")
DEFINE_DEVICE_TYPE(NES_MAXI15,  nes_maxi15_device,  "nes_maxi15",  "NES Cart AVE Maxi 15 PCB")


nes_nina001_device::nes_nina001_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_NINA001, tag, owner, clock)
{
}

nes_nina006_device::nes_nina006_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_NINA006, tag, owner, clock)
{
}

nes_maxi15_device::nes_maxi15_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_MAXI15, tag, owner, clock)
{
}




void nes_maxi15_device::device_start()
{
	common_start();
	save_item(NAME(m_reg));
}

void nes_maxi15_device::pcb_reset() {
	prg32(0);
	chr8(0, CHRROM);

	// Both mapper registers are cleared on reset.
	m_reg[0] = 0;
	m_reg[1] = 0;
}

void nes_nina001_device::pcb_reset()
{
	// NINA-001:
	//   CPU $8000-$FFFF = 32KB PRG ROM bank
	//   PPU $0000-$0FFF = 4KB CHR ROM bank
	//   PPU $1000-$1FFF = 4KB CHR ROM bank
	//
	// Do not inherit the generic NROM chr8(..., m_chr_source) reset here.
	// This board has CHR ROM, and mapper 34 submapper 1 diagnostics will
	// incorrectly detect CHR RAM if PPU writes can affect $0000-$1FFF.
	prg32(0);
	chr4_0(0, CHRROM);
	chr4_4(0, CHRROM);
}



/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 AVE NINA-001 board emulation

 iNES: mapper 34

 Notice that in this board the bankswitch regs
 overlaps WRAM, so that writes to the regs are
 then readable back in WRAM (WRAM is tested by
 Impossible Mission II at start)

 In MAME: Supported.

 -------------------------------------------------*/

void nes_nina001_device::write_m(offs_t offset, u8 data)
{
	LOG("nina-001 write_m, offset: %04x, data: %02x\n", offset, data);

	device_nes_cart_interface::write_m(offset, data); // write WRAM

	switch (offset)
	{
		case 0x1ffd:
			prg32(data);
			break;
		case 0x1ffe:
			chr4_0(data, CHRROM);
			break;
		case 0x1fff:
			chr4_4(data, CHRROM);
			break;
	}
}

void nes_nina001_device::chr_w(offs_t offset, u8 data)
{
	// NINA-001 uses CHR ROM. PPU writes to pattern space must not modify
	// backing CHR data, or CHR-RAM detection will falsely pass.
}

/*-------------------------------------------------

 AVE NINA-003, NINA-006 and MB-91 boards emulation

 Games: Krazy Kreatures, Poke Block, Puzzle, Pyramid,
 Solitaire, Ultimate League Soccer

 iNES: mapper 79

 In MAME: Supported.

 -------------------------------------------------*/

void nes_nina006_device::write_l(offs_t offset, u8 data)
{
	LOG("nina-006 write_l, offset: %04x, data: %02x\n", offset, data);

	offset += 0x100;
	if (BIT(offset, 8)) // $41xx, $43xx, ... $5fxx
	{
		prg32(data >> 3);
		chr8(data & 7, CHRROM);
	}
}

/*-------------------------------------------------

 AVE Maxi 15 board emulation

 Game: Maxi 15

 iNES: mapper 234

 In MAME: Supported.

 The mapper registers occupy CPU $FF80-$FF9F and
 $FFE8-$FFF7. Reading a register returns the PRG ROM
 byte at that address and clocks the same byte into the
 mapper register.

 Writes also clock the registers, but PRG ROM remains
 enabled during the write, producing a bus conflict.
 The mapper therefore receives CPU data AND PRG ROM data.

 Both registers are cleared on reset. The outer register
 becomes locked when any of its lower six bits are set,
 while the inner register remains writable.

 -------------------------------------------------*/

void nes_maxi15_device::update_register(offs_t offset, u8 data) {
	if ((offset >= 0x7f80 && offset < 0x7fa0) || (offset >= 0x7fe8 && offset < 0x7ff8)) {
		const int reg = BIT(offset, 6);

		// The inner register remains writable. The outer register
		// becomes locked when any of its lower six bits are set.
		if (reg || !(m_reg[0] & 0x3f)) {
			m_reg[reg] = data;

			const u8 mode = !BIT(m_reg[0], 6);
			const u8 outer = m_reg[0] & (0x0e | mode);

			prg32(outer | (m_reg[1] & !mode));
			chr8((outer << 2) | ((m_reg[1] >> 4) & (7 >> mode)), CHRROM);
			set_nt_mirroring(BIT(m_reg[0], 7) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
		}
	}
}

u8 nes_maxi15_device::read_h(offs_t offset) {
	LOG("Maxi 15 read_h, offset: %04x\n", offset);

	// Reads return the current PRG ROM byte and also clock that
	// byte into the applicable mapper register.
	const u8 data = hi_access_rom(offset);

	update_register(offset, data);

	return data;
}

void nes_maxi15_device::write_h(offs_t offset, u8 data) {
	LOG("Maxi 15 write_h, offset: %04x, data: %02x\n", offset, data);

	// PRG ROM remains enabled during writes, producing a CPU/ROM
	// bus conflict. The mapper sees the AND of both values.
	data = account_bus_conflict(offset, data);

	update_register(offset, data);
}
