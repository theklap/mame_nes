// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Henggedianzi PCBs


 Here we emulate the following PCBs

 * Henggedianzi Super Rich [mapper 177]
 * Henggedianzi Xing He Zhan Shi [mapper 179]


 TODO:
 - investigate relation with some TXC & Waixing boards

 ***********************************************************************************************************/


#include "emu.h"
#include "henggedianzi.h"


#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_HENGG_SRICH, nes_hengg_srich_device, "nes_hengg_srich", "NES Cart Henggedianzi Super Rich PCB")
DEFINE_DEVICE_TYPE(NES_HENGG_XHZS,  nes_hengg_xhzs_device,  "nes_hengg_xhzs",  "NES Cart Henggedianzi Xing He Zhan Shi PCB")


nes_hengg_srich_device::nes_hengg_srich_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_HENGG_SRICH, tag, owner, clock)
{
}

nes_hengg_xhzs_device::nes_hengg_xhzs_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_HENGG_XHZS, tag, owner, clock)
{
}

void nes_hengg_xhzs_device::pcb_reset() {
	prg32(0);
	chr8(0, CHRRAM);
	set_nt_mirroring(PPU_MIRROR_VERT);
}

/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

Henggedianzi board

Games: Mei Guo Fu Hao, Shang Gu Shen Jian,
Wang Zi Fu Chou Ji

The board has a switchable 32 KiB PRG-ROM bank at
$8000-$FFFF, fixed 8 KiB CHR-RAM and 8 KiB of
battery-backed WRAM at $6000-$7FFF.

Writes throughout $8000-$FFFF select the PRG-ROM
bank with data bits 0-4. Data bit 5 controls
nametable mirroring.

The board does not have bus conflicts.

iNES: mapper 177

In MAME: Supported.

-------------------------------------------------*/

uint8_t nes_hengg_srich_device::read_m(offs_t offset) {
	if (!m_battery.empty()) {
		return m_battery[offset & (m_battery.size() - 1)];
	}

	if (!m_prgram.empty()) {
		return m_prgram[offset & (m_prgram.size() - 1)];
	}

	return get_open_bus();
}

void nes_hengg_srich_device::write_m(offs_t offset, uint8_t data) {
	if (!m_battery.empty()) {
		m_battery[offset & (m_battery.size() - 1)] = data;
		return;
	}

	if (!m_prgram.empty()) {
		m_prgram[offset & (m_prgram.size() - 1)] = data;
	}
}

void nes_hengg_srich_device::write_h(offs_t offset, uint8_t data) {
	LOG("hengg_srich write_h, offset: %04x, data: %02x\n", offset, data);

	prg32(data & 0x1F);
	set_nt_mirroring(BIT(data, 5) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
}

/*-------------------------------------------------

Legacy Henggedianzi board assignment

Game: Xing He Zhan Shi

The board has a switchable 32 KiB PRG-ROM bank,
fixed 8 KiB CHR-RAM and 8 KiB of battery-backed
WRAM at $6000-$7FFF.

The board powers up with vertical nametable
mirroring.

Writes throughout $5000-$5FFF select a 32 KiB
PRG-ROM bank using data bits 1-7. The game accesses
this register at $5011.

Writes throughout $8000-$FFFF subsequently control
nametable mirroring using data bit 0.

This legacy mapper 179 assignment duplicates mapper
176. NESdev specifies that mapper 176 should be used.

iNES: mapper 179, obsolete; use mapper 176

In MAME: Supported through legacy assignment.

-------------------------------------------------*/

uint8_t nes_hengg_xhzs_device::read_m(offs_t offset) {
	if (!m_battery.empty()) {
		return m_battery[offset & (m_battery.size() - 1)];
	}

	if (!m_prgram.empty()) {
		return m_prgram[offset & (m_prgram.size() - 1)];
	}

	return get_open_bus();
}

void nes_hengg_xhzs_device::write_m(offs_t offset, uint8_t data) {
	if (!m_battery.empty()) {
		m_battery[offset & (m_battery.size() - 1)] = data;
		return;
	}

	if (!m_prgram.empty()) {
		m_prgram[offset & (m_prgram.size() - 1)] = data;
	}
}

void nes_hengg_xhzs_device::write_l(offs_t offset, uint8_t data) {
	LOG("hengg_xhzs write_l, offset: %04x, data: %02x\n", offset, data);

	offset += 0x4100;

	if (offset >= 0x5000) {
		prg32(data >> 1);
	}
}

void nes_hengg_xhzs_device::write_h(offs_t offset, uint8_t data) {
	LOG("hengg_xhzs write_h, offset: %04x, data: %02x\n", offset, data);

	set_nt_mirroring(BIT(data, 0) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
}