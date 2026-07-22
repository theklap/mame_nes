// license:BSD-3-Clause
// copyright-holders:Brad Oliver,Fabio Priuli
/*****************************************************************************

    nes.c

    Nintendo Entertainment System (Famicom)

 ****************************************************************************/

#include "emu.h"
#include "cpu/m6502/m6502.h"
#include "nes.h"

bool g_nes_p1_a_pressed_edge = false;

/***************************************************************************
    FUNCTIONS
***************************************************************************/

// to be probably removed (it does nothing since a long time)
int nes_state::nes_ppu_vidaccess( int address, int data )
{
	return data;
}

//-------------------------------------------------
//  machine_reset
//-------------------------------------------------

void nes_state::machine_reset()
{
	// Reset the mapper variables. Will also mark the char-gen ram as dirty
	if (m_cartslot)
		m_cartslot->pcb_reset();

	last_2016_read = 0;

	m_maincpu->reset();
	m_ppu->reset();	
	
}

// ------------------------------------------------------------
// Build CPU open-bus ranges from actual cart decode/install state.
//
// This is not a full mapper truth table.  It only marks ranges
// that are known to be undriven for normal boards.
//
// $4020-$40FF:
//   Cartridge expansion-register area.  Open bus unless read_ex()
//   was actually installed.
//
// $4100-$5FFF:
//   Low cartridge area.  Most simple Nintendo boards do not decode it.
//   Even though read_l() is installed in the CPU map, the default/NROM
//   handler returns get_open_bus(), so tell the CPU this range is also
//   open bus.
//
// $6000-$7FFF:
//   PRG RAM / battery RAM if present.  Otherwise open bus.
// ------------------------------------------------------------
static std::vector<open_bus_range> compute_open_bus_ranges(
	nes_cart_slot_device *cartslot,
	bool cart_reads_ex)
{
	std::vector<open_bus_range> ranges;

	if (!cartslot || !cartslot->m_cart)
		return ranges;

	device_nes_cart_interface *cart = cartslot->m_cart;
	const int pcb_id = cartslot->get_pcb_id();

	// $4018-$401F is normally disabled/test APU/IO space on retail NES.
	// No normal device drives reads here, so it should read as CPU open bus.
	ranges.push_back({ 0x4018, 0x401f });

	// $4020-$40FF is cartridge expansion space.
	// Only boards that install read_ex() should drive reads here.
	if (!cart_reads_ex)
		ranges.push_back({ 0x4020, 0x40ff });

	// $4100-$5FFF is open bus only for simple boards that do not decode
	// low cartridge space.
	
	// NROM-368 maps PRG ROM from $4800-$FFFF.
	// Therefore only $4100-$47FF is open bus in the low cart area,
	// and $6000-$7FFF must NOT be treated as open bus just because
	// the cart has no PRG RAM.
	if (pcb_id == STD_NROM368)
	{
		ranges.push_back({ 0x4100, 0x47ff });
		return ranges;
	}
	
	switch (pcb_id)
	{
		case STD_NROM:
		case STD_UXROM:
		case STD_UN1ROM:
		case UXROM_CC:
		case STD_CNROM:
		case STD_CPROM:
		case STD_AXROM:
		case STD_AMROM:
		case STD_BXROM:
		case STD_GXROM:
		case SUNSOFT_1:
		case SUNSOFT_2:
		case SUNSOFT_3:
			ranges.push_back({ 0x4100, 0x5fff });
			break;

		default:
			break;
	}

	// $6000-$7FFF:
	//
	// If the loaded cart reports no PRG WRAM and no PRG NVWRAM, then the
	// normal PRG-RAM area is undriven and should read as CPU open bus.
	//
	// The RAM sizes here already come from the loader's best source:
	//   - softlist PCB data
	//   - NES 2.0 header
	//   - iNES/header fallback
	//   - extrainfo/overrides
	//
	// Only exclude boards that are known to decode $6000-$7FFF for something
	// other than normal PRG RAM, such as mapper registers, protection, EEPROM,
	// or special status reads.
	const bool no_prg_ram = !cart->get_prgram_size() && !cart->get_battery_size();

	if (no_prg_ram)
	{
		bool range_6000_is_special = false;

		switch (pcb_id)
		{
			case STD_EXROM:
			case STD_HKROM:
			case UNL_BMW8544:
			case KONAMI_VRC6:
				range_6000_is_special = true;
				break;
			case KONAMI_VRC2:
				range_6000_is_special = true;
				ranges.push_back({ 0x7000, 0x7fff });
				break;
			case SUNSOFT_FME7:
			case SUNSOFT_5:
				range_6000_is_special = true;
				break;

			default:
				break;
		}

		if (!range_6000_is_special)
			ranges.push_back({ 0x6000, 0x7fff });
	}

	return ranges;
}

//-------------------------------------------------
//  machine_start
//-------------------------------------------------

void nes_state::machine_start()
{
	m_maincpu6502 = downcast<m6502_device *>(&*m_maincpu);
	address_space &space = m_maincpu->space(AS_PROGRAM);
	
	// Fill main RAM with an arbitrary pattern (alternating 0x00/0xff) for software that depends on its contents at boot up (tsk tsk!)
	// The fill value is a compromise since certain games malfunction with zero-filled memory, others with one-filled memory
	// Examples: Minna no Taabou won't boot with all 0x00, Sachen's Dancing Block won't boot with all 0xff, Terminator 2 skips its copyright screen with all 0x00
	/*for (int i = 0; i < 0x800; i += 2)
	{
		m_mainram[i] = 0x00;
		m_mainram[i + 1] = 0xff;
	}*/
	// Fill main RAM with semi-random power-up contents for software that uses
	// uninitialized RAM as an RNG seed or otherwise depends on power-up state.
	for (int i = 0; i < 0x800; i++) {
		m_mainram[i] = machine().rand() & 0xff;
	}

	// CIRAM (Character Internal RAM)
	// NES has 2KB of internal RAM which can be used to fill the 4x1KB banks of PPU RAM at $2000-$2fff
	// Line A10 is exposed to the carts, so that games can change CIRAM mapping in PPU (we emulate this with the set_nt_mirroring
	// function). CIRAM can also be disabled by the game (if e.g. VROM or cart RAM has to be used in PPU...
	m_ciram = std::make_unique<uint8_t[]>(0x800);
	// CIRAM power-up contents are not guaranteed on real hardware.
	// Use a deterministic compatibility pattern rather than relying on all-zero RAM.
	for (int i = 0; i < 0x800; i += 2)
	{
		m_ciram[i + 0] = 0x00;
		m_ciram[i + 1] = 0xff;
	}
	// other pointers got set in the loading routine, because they 'belong' to the cart itself

	m_io_disksel = ioport("FLIPDISK");

	if (m_cartslot && m_cartslot->m_cart)
	{			
		// Set up memory handlers
		space.install_read_handler(0x4100, 0x5fff, read8sm_delegate(*m_cartslot, FUNC(nes_cart_slot_device::read_l)));
		space.install_write_handler(0x4100, 0x5fff, write8sm_delegate(*m_cartslot, FUNC(nes_cart_slot_device::write_l)));
		space.install_read_handler(0x6000, 0x7fff, read8sm_delegate(*m_cartslot, FUNC(nes_cart_slot_device::read_m)));
		space.install_write_handler(0x6000, 0x7fff, write8sm_delegate(*m_cartslot, FUNC(nes_cart_slot_device::write_m)));
		for(int i = 0; i < 4; i++)
			space.install_read_bank(0x8000 + 0x2000*i, 0x9fff + 0x2000*i, m_prg_bank[i]);
		space.install_write_handler(0x8000, 0xffff, write8sm_delegate(*m_cartslot, FUNC(nes_cart_slot_device::write_h)));

		m_ppu->space(AS_PROGRAM).install_readwrite_handler(0, 0x1fff, read8sm_delegate(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::chr_r)), write8sm_delegate(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::chr_w)));
		m_ppu->space(AS_PROGRAM).install_readwrite_handler(0x2000, 0x3eff, read8sm_delegate(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::nt_r)), write8sm_delegate(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::nt_w)));
		//m_ppu->set_scanline_callback(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::scanline_irq));
		//m_ppu->set_hblank_callback(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::hblank_irq));
		m_ppu->set_latch(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::ppu_latch));
		//add here for mmc3 and mmc5 mapper on nes
		m_ppu->set_ppu_to_mapper(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::ppu_to_mapper));

		// install additional handlers (read_h, read_ex, write_ex)
		static const int r_h_pcbs[] =
		{
			AVE_MAXI15,
			BANDAI_DATACH,
			BANDAI_KARAOKE,
			BATMAP_SRRX,
			BMC_70IN1,
			BMC_800IN1,
			BMC_8157,
			BMC_970630C,
			BMC_DS927,
			BMC_KC885,
			BMC_TELETUBBIES,
			BMC_VT5201,
			BTL_PALTHENA,
			CAMERICA_ALADDIN,
			GG_NROM,
			KAISER_KS7010,
			KAISER_KS7022,
			KAISER_KS7030,
			KAISER_KS7031,
			KAISER_KS7037,
			KAISER_KS7057,
			SACHEN_3013,
			SACHEN_3014,
			STD_DISKSYS,
			STD_EXROM,
			STD_NROM368,
			SUNSOFT_DCS,
			UNL_2708,
			UNL_2A03PURITANS,
			UNL_43272,
			UNL_EH8813A,
			UNL_LH10,
			UNL_LH32,
			UNL_RT01
		};

		static const int w_ex_pcbs[] =
		{
			BMC_N32_4IN1,
			BTL_SMB2JB,
			BTL_YUNG08,
			UNL_AC08,
			UNL_SMB2J
		};

		static const int rw_ex_pcbs[] =
		{
			BTL_09034A,
			KAISER_KS7017,
			STD_DISKSYS,
			UNL_603_5052
		};

		int pcb_id = m_cartslot->get_pcb_id();

		// ------------------------------------------------------------
		// Decode/install gates.
		// ------------------------------------------------------------
		//
		// These say what the CPU map actually installed for this cartridge.
		// We use the same gates for optional handler installation and for the
		// CPU open-bus range table.

		const bool cart_reads_h =
			std::find(std::begin(r_h_pcbs), std::end(r_h_pcbs), pcb_id) != std::end(r_h_pcbs);

		const bool cart_writes_ex =
			std::find(std::begin(w_ex_pcbs), std::end(w_ex_pcbs), pcb_id) != std::end(w_ex_pcbs);

		const bool cart_reads_ex =
			std::find(std::begin(rw_ex_pcbs), std::end(rw_ex_pcbs), pcb_id) != std::end(rw_ex_pcbs);


		// ------------------------------------------------------------
		// $8000-$FFFF optional cart read handler.
		// ------------------------------------------------------------
		//
		// Normal games read through the installed PRG banks.  Only special boards
		// with protection/address scrambling/etc. install read_h().
		if (cart_reads_h)
		{
			logerror("read_h installed!\n");
			space.install_read_handler(0x8000, 0xffff, read8sm_delegate(*m_cartslot, FUNC(nes_cart_slot_device::read_h)));
		}


		// ------------------------------------------------------------
		// $4020-$40FF expansion handlers.
		// ------------------------------------------------------------
		//
		// Only install these for boards that really decode the expansion area.
		// Do not install read_ex just to return open bus.  If read_ex is not installed,
		// we mark $4020-$40FF open bus in the CPU range table below.
		if (cart_writes_ex && !cart_reads_ex)
		{
			logerror("write_ex installed!\n");
			space.install_write_handler(0x4020, 0x40ff, write8sm_delegate(*m_cartslot, FUNC(nes_cart_slot_device::write_ex)));
		}

		if (cart_reads_ex)
		{
			logerror("read_ex & write_ex installed!\n");
			space.install_read_handler(0x4020, 0x40ff, read8sm_delegate(*m_cartslot, FUNC(nes_cart_slot_device::read_ex)));
			space.install_write_handler(0x4020, 0x40ff, write8sm_delegate(*m_cartslot, FUNC(nes_cart_slot_device::write_ex)));
		}

		// ------------------------------------------------------------
		// Finish cart startup.
		// ------------------------------------------------------------
		//
		// pcb_start() sets up PRG banks, CHR, nametable mirroring,
		// RAM save registration, and cart state.
		//
		// PRG RAM / battery RAM sizes are already known from load time,
		// but doing this before publishing the open-bus ranges keeps the
		// cartridge fully initialized before the CPU starts using them.
		m_cartslot->pcb_start(m_ciram.get());


		// ------------------------------------------------------------
		// Pass open-bus ranges to the CPU.
		// ------------------------------------------------------------
		//
		// For UxROM/1943 with no PRG RAM, this should produce:
		//
		//   OPEN BUS RANGE: $4020-$40FF
		//   OPEN BUS RANGE: $6000-$7FFF
		//
		// If a board installed read_ex(), $4020-$40FF is not marked open bus.
		// If a cart has PRG RAM or battery RAM, $6000-$7FFF is not marked open bus.
		auto ranges = compute_open_bus_ranges(m_cartslot, cart_reads_ex);

		uint32_t packed[16];
		int count = 0;

		for (auto &r : ranges)
		{
			if (count >= std::size(packed))
				break;

			logerror("open-bus read installed for range: $%04X-$%04X\n", r.start, r.end);
			osd_printf_info("OPEN BUS RANGE: $%04X-$%04X\n", r.start, r.end);

			packed[count] = (uint32_t(r.start) << 16) | uint32_t(r.end);
			count++;
		}

		if (m_maincpu6502)
			m_maincpu6502->set_open_bus_ranges(packed, count);

		m_cartslot->m_cart->pcb_reg_postload(machine());
	}

	// register saves
	save_item(NAME(m_last_frame_flip));
	save_pointer(NAME(m_ciram), 0x800);
	
	machine_reset();
}


//-------------------------------------------------
//  INPUTS
//-------------------------------------------------
uint8_t nes_base_state::nes_in0_r()
{
	uint8_t ret = m_maincpu6502->get_open_bus() & 0xE0;

	uint8_t const pad1 = m_ctrl1->read_bit0();
	ret |= pad1;

	if (m_ctrl2)
		ret |= m_ctrl2->read_bit34();

	//logerror("4016 R pad1=%02x ret=%02x\n", pad1, ret);

	//static bool s_p1_a_prev = false;
	bool const a_now = BIT(pad1, 0);

	if (a_now && !m_p1_a_prev)
		g_nes_p1_a_pressed_edge = true;

	m_p1_a_prev = a_now;

	return ret;
}

uint8_t nes_base_state::nes_in1_r()
{
	uint8_t ret = m_maincpu6502->get_open_bus() & 0xE0;
	ret |= m_ctrl2->read_bit0();
	ret |= m_ctrl2->read_bit34();
	return ret;
}

void nes_base_state::nes_in0_w(uint8_t data)
{
	//logerror("4016 W data=%02x\n", data);
	m_ctrl1->write(data & 1);
	m_ctrl2->write(data & 1);
}


uint8_t nes_state::fc_in0_r()
{
	uint8_t ret = 0x40;
	// bit 0 from controller port
	ret |= m_ctrl1->read_bit0();

	// bit 2 from P2 controller microphone
	ret |= m_ctrl2->read_bit2();

	// and bit 1 comes from expansion port
	ret |= m_exp->read_exp(0);
	return ret;
}

uint8_t nes_state::fc_in1_r()
{
	uint8_t ret = 0x40;
	// bit 0 from controller port
	ret |= m_ctrl2->read_bit0();

	// bits 1-4 from expansion port (in theory bit 0 also can be read on AV Famicom when controller is unplugged)
	ret |= m_exp->read_exp(1);
	return ret;
}

void nes_state::fc_in0_w(uint8_t data)
{
	m_ctrl1->write(data);
	m_ctrl2->write(data);
	m_exp->write(data);
}


void nes_state::init_famicom()
{
	// setup alt input handlers for additional FC input devices
	address_space &space = m_maincpu->space(AS_PROGRAM);
	space.install_read_handler(0x4016, 0x4016, read8smo_delegate(*this, FUNC(nes_state::fc_in0_r)));
	space.install_write_handler(0x4016, 0x4016, write8smo_delegate(*this, FUNC(nes_state::fc_in0_w)));
	space.install_read_handler(0x4017, 0x4017, read8smo_delegate(*this, FUNC(nes_state::fc_in1_r)));
}
