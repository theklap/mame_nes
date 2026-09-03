// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************

 NES/Famicom cartridge emulation for the Galoob Game Genie

 The Game Genie is a passthrough cartridge containing a 4 KiB
 menu ROM and hardware capable of intercepting three CPU reads
 in the $8000-$FFFF region.

 Each replacement can optionally compare the inserted cartridge's
 value before replacing it. Once the menu disables its ROM, CPU and
 PPU accesses pass through to the inserted cartridge.

 The Game Genie is passthrough hardware and does not have a normal
 iNES or NES 2.0 mapper number.

 TODO: Emulate the documented electrical quirks and bus conflicts.

 ***********************************************************************************************************/

#include "emu.h"
#include "ggenie.h"

#include "bus/nes/nes_carts.h"

#define LOG_HIFREQ (1U << 1)
#define LOG_INFO   (1U << 2)

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_INFO | LOG_GENERAL)
#else
#define VERBOSE (LOG_INFO)
#endif

#include "logmacro.h"

//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_GGENIE, nes_ggenie_device, "nes_ggenie", "NES Cart Game Genie PCB")

nes_ggenie_device::nes_ggenie_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_GGENIE, tag, owner, clock)
	, m_ggslot(*this, "gg_slot")
	, m_gg_bypass(false)
{
}

void nes_ggenie_device::device_start()
{
	common_start();

	save_item(NAME(m_gg_bypass));
	save_item(NAME(m_gg_addr));
	save_item(NAME(m_gg_repl));
	save_item(NAME(m_gg_comp));
	save_item(NAME(m_gg_enable));
	save_item(NAME(m_gg_is_comp));
}

void nes_ggenie_device::pcb_start(running_machine &machine, u8 *ciram_ptr, bool cart_mounted)
{
	device_nes_cart_interface::pcb_start(machine, ciram_ptr, cart_mounted);

	m_ggslot->pcb_start(m_ciram);

	prg32(0);

	m_gg_bypass = false;

	for (int i = 0; i < 3; i++) {
		m_gg_addr[i] = 0;
		m_gg_repl[i] = 0;
		m_gg_comp[i] = 0;
		m_gg_enable[i] = false;
		m_gg_is_comp[i] = false;
	}
}

void nes_ggenie_device::pcb_reset()
{
	// Reset reaches the inserted cartridge but does not return
	// the Game Genie to its code-entry interface.
	m_ggslot->pcb_reset();
}

/*-------------------------------------------------
 mapper-specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 Galoob Game Genie

 The 4 KiB menu ROM is mirrored through $8000-$FFFF.
 While the menu is active, $4020-$7FFF is undriven.

 Codes are written through registers at $8000-$800C.
 After the menu enables bypass mode, CPU and PPU
 accesses are forwarded to the inserted cartridge.

 The Game Genie can replace three CPU read values in
 $8000-$FFFF. Each replacement can optionally require
 the inserted cartridge to return a comparison value.

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_ggenie_device::read_ex(offs_t offset) {
	if (m_gg_bypass && m_ggslot->m_cart) {
		return m_ggslot->read_ex(offset);
	}

	return get_open_bus();
}

u8 nes_ggenie_device::read_l(offs_t offset) {
	if (m_gg_bypass && m_ggslot->m_cart) {
		return m_ggslot->read_l(offset);
	}

	return get_open_bus();
}

u8 nes_ggenie_device::read_m(offs_t offset) {
	if (m_gg_bypass && m_ggslot->m_cart) {
		return m_ggslot->read_m(offset);
	}

	return get_open_bus();
}

u8 nes_ggenie_device::read_h(offs_t offset)
{
	if (!m_gg_bypass) {
		return m_prg[offset & 0x0fff];
	}

	if (!m_ggslot->m_cart) {
		return get_open_bus();
	}

	const u8 cart_value = m_ggslot->m_cart->hi_access_rom(offset);

	for (int i = 0; i < 3; i++) {
		if (!m_gg_enable[i] || offset != m_gg_addr[i]) {
			continue;
		}

		if (!m_gg_is_comp[i] || m_gg_comp[i] == cart_value) {
			return m_gg_repl[i];
		}
	}

	return cart_value;
}

void nes_ggenie_device::write_ex(offs_t offset, u8 data)
{
	if (m_gg_bypass) {
		m_ggslot->write_ex(offset, data);
	}
}

void nes_ggenie_device::write_l(offs_t offset, u8 data)
{
	if (m_gg_bypass) {
		m_ggslot->write_l(offset, data);
	}
}

void nes_ggenie_device::write_m(offs_t offset, u8 data)
{
	if (m_gg_bypass) {
		m_ggslot->write_m(offset, data);
	}
}

void nes_ggenie_device::write_h(offs_t offset, u8 data)
{
	LOGMASKED(LOG_HIFREQ, "ggenie write_h, offset: %04x, data: %02x\n", offset, data);

	if (m_gg_bypass) {
		m_ggslot->write_h(offset, data);
		return;
	}

	// The menu writes all twelve code bytes to $8001-$800C,
	// starting at $800C and proceeding downward.
	if (offset) {
		const offs_t register_offset = offset - 1;
		const int code = BIT(register_offset, 2, 2);

		if (code >= 3) {
			return;
		}

		switch (register_offset & 0x03) {
			case 0:
				m_gg_addr[code] &= 0x00ff;
				m_gg_addr[code] |= (data & 0x7f) << 8;
				break;

			case 1:
				m_gg_addr[code] &= 0x7f00;
				m_gg_addr[code] |= data;
				break;

			case 2:
				m_gg_comp[code] = data;
				break;

			case 3:
				m_gg_repl[code] = data;
				break;
		}

		return;
	}

	// $8000 controls bypass, comparison and code enable state.
	m_gg_bypass = BIT(data, 0);

	for (int i = 0; i < 3; i++) {
		m_gg_is_comp[i] = BIT(data, i + 1);
		m_gg_enable[i] = !BIT(data, i + 4);
	}

	// Bit 7 is unused and is normally clear.
	LOGMASKED(LOG_INFO, "Game Genie Summary:\n");

	for (int i = 0; i < 3; i++) {
		LOGMASKED(LOG_INFO, "Code %d: %s\n", i, m_gg_enable[i] ? "Yes" : "No");

		if (m_gg_enable[i]) {
			LOGMASKED(LOG_INFO, "\tAddress: $%04X\n", 0x8000 | m_gg_addr[i]);
			LOGMASKED(LOG_INFO, "\tReplacement: $%02X\n", m_gg_repl[i]);

			if (m_gg_is_comp[i]) {
				LOGMASKED(LOG_INFO, "\tCompare: $%02X\n", m_gg_comp[i]);
			}
		}
	}
}

u8 nes_ggenie_device::chr_r(offs_t offset)
{
	if (m_gg_bypass && m_ggslot->m_cart) {
		return m_ggslot->m_cart->chr_r(offset);
	}

	// The Game Genie has no CHR-ROM. Discrete logic generates
	// four possible pattern bytes from PPU address lines.
	static constexpr u8 chr_lut[4] = {
		0x00,
		0xf0,
		0x0f,
		0xff
	};

	return chr_lut[BIT(offset, BIT(offset, 2) ? 4 : 6, 2)];
}

void nes_ggenie_device::chr_w(offs_t offset, u8 data)
{
	if (m_gg_bypass && m_ggslot->m_cart) {
		m_ggslot->m_cart->chr_w(offset, data);
	}
}

u8 nes_ggenie_device::nt_r(offs_t offset)
{
	if (m_gg_bypass && m_ggslot->m_cart) {
		return m_ggslot->m_cart->nt_r(offset);
	}

	return device_nes_cart_interface::nt_r(offset);
}

void nes_ggenie_device::nt_w(offs_t offset, u8 data)
{
	if (m_gg_bypass && m_ggslot->m_cart) {
		m_ggslot->m_cart->nt_w(offset, data);
	}
	else {
		device_nes_cart_interface::nt_w(offset, data);
	}
}

void nes_ggenie_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, u16 ppu_address)
{
	if (m_gg_bypass && m_ggslot->m_cart) {
		m_ggslot->m_cart->ppu_to_mapper(scanline, dot, ppu_tick, ppu_address);
	}
}

void nes_ggenie_device::ppu_bus_address(u16 address, u64 ppu_cycle, int ppu_tick, bool odd_frame)
{
	if (m_gg_bypass && m_ggslot->m_cart) {
		m_ggslot->m_cart->ppu_bus_address(address, ppu_cycle, ppu_tick, odd_frame);
	}
}

void nes_ggenie_device::ppu_odd_frame_skip()
{
	if (m_gg_bypass && m_ggslot->m_cart) {
		m_ggslot->m_cart->ppu_odd_frame_skip();
	}
}

void nes_ggenie_device::mmc1_ppu_phase(bool upper_chr, uint16_t ppu_address)
{
	if (m_gg_bypass && m_ggslot->m_cart) {
		m_ggslot->m_cart->mmc1_ppu_phase(upper_chr, ppu_address);
	}
}

void nes_ggenie_device::mmc5_clock_ppu_read(uint16_t ppu_addr_bus) {
	if (m_gg_bypass && m_ggslot->m_cart) {
		m_ggslot->m_cart->mmc5_clock_ppu_read(ppu_addr_bus);
	}
}

void nes_ggenie_device::mmc5_reset_scanline_irq_state() {
	if (m_gg_bypass && m_ggslot->m_cart) {
		m_ggslot->m_cart->mmc5_reset_scanline_irq_state();
	}
}

void nes_ggenie_device::mmc5_real_ppuctrl_write(uint8_t data) {
	if (m_gg_bypass && m_ggslot->m_cart) {
		m_ggslot->m_cart->mmc5_real_ppuctrl_write(data);
	}
}

void nes_ggenie_device::mmc5_real_ppumask_write(uint8_t data) {
	if (m_gg_bypass && m_ggslot->m_cart) {
		m_ggslot->m_cart->mmc5_real_ppumask_write(data);
	}
}

//-------------------------------------------------
//  device_add_mconfig
//-------------------------------------------------

void nes_ggenie_device::device_add_mconfig(machine_config &config)
{
	NES_CART_SLOT(config, "gg_slot", DERIVED_CLOCK(1, 1), nes_cart, nullptr).set_must_be_loaded(false);
}