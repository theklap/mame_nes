// license:BSD-3-Clause
// copyright-holders:Brad Oliver,Fabio Priuli
/***************************************************************************

  nes.cpp

  Driver file to handle emulation of the Nintendo Entertainment System (Famicom).

  MESS driver by Brad Oliver (bradman@pobox.com), NES sound code by Matt Conte.
  Based in part on the old xNes code, by Nicolas Hamel, Chuck Mason, Brad Oliver,
  Richard Bannister and Jeff Mitchell.

***************************************************************************/

#include "emu.h"

#include "bus/nes/disksys.h"
#include "bus/nes/nes_slot.h"
#include "bus/nes/nes_carts.h"
#include "bus/nes_ctrl/ctrl.h"
#include "cpu/m6502/rp2a03.h"
#include "video/ppu2c0x.h"
#include "screen.h"
#include "softlist_dev.h"
#include "speaker.h"

namespace {

/***************************************************************************
    TYPE DEFINITIONS
***************************************************************************/

class nes_base_state : public driver_device
{
public:
	nes_base_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_ctrl1(*this, "ctrl1"),
		m_ctrl2(*this, "ctrl2")
	{ }

	required_device<cpu_device> m_maincpu;
	optional_device<nes_control_port_device> m_ctrl1;
	optional_device<nes_control_port_device> m_ctrl2;
	m6502_device *m_maincpu6502 = nullptr;
	bool first = false;
	int64_t last_2016_read = 0;
	bool m_p1_a_prev = false;
	bool m_p1_a_pressed_edge = false;

	uint8_t nes_in0_r();
	uint8_t nes_in1_r();
	void nes_in0_w(uint8_t data);
};

bool g_nes_p1_a_pressed_edge = false;

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

	if (pcb_id == CAMERICA_ALADDIN)
	{
		ranges.push_back({ 0x4100, 0x5fff });
		ranges.push_back({ 0x6000, 0x7fff });
		return ranges;
	}

	if (pcb_id == BATMAP_SRRX)
	{
		ranges.push_back({ 0x4100, 0x47ff });
		return ranges;
	}

	// MMC5 only drives specific readable registers in $4100-$5FFF.
	// ExRAM at $5C00-$5FFF is runtime-dependent and handled separately.
	if (pcb_id == STD_EXROM) {
		ranges.push_back({ 0x4100, 0x500f });
		ranges.push_back({ 0x5011, 0x5014 });
		ranges.push_back({ 0x5016, 0x5203 });
		ranges.push_back({ 0x5207, 0x5bff });
		return ranges;
	}

	if (pcb_id == UNL_SMB2J) {
		ranges.push_back({ 0x4100, 0x4fff });
		return ranges;
	}

	if (pcb_id == BTL_SMB2JA) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == BTL_SMB2JB) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == BMC_N32_4IN1) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == BTL_0353) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == BTL_09034A) {
		ranges.push_back({ 0x4020, 0x4041 });
		ranges.push_back({ 0x4056, 0x5fff });
		return ranges;
	}

	if (pcb_id == BTL_L001) {
		ranges.push_back({ 0x4100, 0x7fff });
		return ranges;
	}

	if (pcb_id == BTL_BATMANFS) {
		ranges.push_back({ 0x4100, 0x7fff });
		return ranges;
	}

	if (pcb_id == BTL_PALTHENA) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == BTL_TOBIDASE) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == UNL_LH32) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == UNL_LH42) {
		ranges.push_back({ 0x4100, 0x5fff });
		ranges.push_back({ 0x6000, 0x7fff });
		return ranges;
	}

	if (pcb_id == UNL_LG25) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == UNL_LH10) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == UNL_LH51) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == UNL_LH53) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == UNL_2708) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == UNL_AC08) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == UNL_MMALEE) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == CNE_FSB) {
		ranges.push_back({ 0x4100, 0x5fff });
		ranges.push_back({ 0x6000, 0x67ff });
		ranges.push_back({ 0x7000, 0x7fff });
		return ranges;
	}

	if (pcb_id == CONY_BOARD || pcb_id == CONY1K_BOARD) {
		ranges.push_back({ 0x4100, 0x4fff });
		return ranges;
	}

	if (pcb_id == YOKO_BOARD) {
		ranges.push_back({ 0x4100, 0x4fff });
		return ranges;
	}

	if (pcb_id == BANDAI_DATACH) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == FARID_SLROM8IN1) {
		ranges.push_back({ 0x4100, 0x5fff });
		ranges.push_back({ 0x6000, 0x7fff });
		return ranges;
	}

	if (pcb_id == UNL_158B)
	{
		ranges.push_back({ 0x4100, 0x4fff });
		return ranges;
	}

	if (pcb_id == GOUDER_37017) {
		ranges.push_back({ 0x4100, 0x57ff });
		ranges.push_back({ 0x6000, 0x77ff });
		return ranges;
	}

	if (pcb_id == SACHEN_SA9602B) {
		ranges.push_back({ 0x4100, 0x5fff });
		ranges.push_back({ 0x6000, 0x7fff });
		return ranges;
	}

	if (pcb_id == SACHEN_SHERO) {
		ranges.push_back({ 0x4101, 0x5fff });
		return ranges;
	}

	if (pcb_id == SACHEN_ZGDH) {
		ranges.push_back({ 0x4100, 0x5fff });
		return ranges;
	}

	if (pcb_id == BMC_HIK8IN1) {
		ranges.push_back({ 0x4100, 0x4fff });
		return ranges;
	}

	if (pcb_id == SMD133_BOARD) {
		ranges.push_back({ 0x4100, 0x4fff });
		return ranges;
	}

	if (pcb_id == KAY_BOARD)
		ranges.push_back({ 0x4100, 0x4fff });

	if (pcb_id == KAISER_KS7017) {
		ranges.push_back({ 0x4020, 0x402f });
		ranges.push_back({ 0x4031, 0x40ff });
		ranges.push_back({ 0x4100, 0x5fff });
	}

	if (pcb_id == STD_EVENT2) {
		ranges.push_back({ 0x4100, 0x4fff });
	}

	// Namcot 163 drives its sound-data and IRQ registers from $4800-$5FFF.
	// Nothing drives reads from $4100-$47FF.
	if (pcb_id == NAMCOT_163)
		ranges.push_back({ 0x4100, 0x47ff });

	// J.Y. ASIC boards decode registers throughout $5000-$5FFF.
	// Only $4100-$4FFF is completely undriven.
	if (pcb_id == JYCOMPANY_A || pcb_id == JYCOMPANY_B || pcb_id == JYCOMPANY_C)
		ranges.push_back({ 0x4100, 0x4fff });

	// Mario Baby only drives $4030 in cartridge expansion space.
	// The remaining expansion addresses read as CPU open bus.
	if (pcb_id == BTL_MARIOBABY) {
		ranges.push_back({ 0x4020, 0x402f });
		ranges.push_back({ 0x4031, 0x40ff });
	}

	if (pcb_id == UNL_KOF96)
		ranges.push_back({ 0x4100, 0x4fff });

	switch (pcb_id) {
		case AVE_NINA01:
		case AVE_NINA06:
		case AVE_MAXI15:
		case BANDAI_OEKAKIDS:
		case BANDAI_FCG:
		case BANDAI_LZ93:
		case BANDAI_LZ93EX1:
		case BANDAI_LZ93EX2:
		case BANDAI_FJUMP2:
		case BANDAI_DATACH:
		case BANDAI_KARAOKE:
		case BATMAP_000:
		case BMC_BENSHIENG:
		case BTL_AISENSHINICOL:
		case BTL_MARIOBABY:
		case BTL_SMB3:
		case BTL_CONTRAJ:
		case BTL_DRAGONNINJA:
		case CAMERICA_ALADDIN:
		case CAMERICA_BF9093:
		case CAMERICA_BF9096:
		case CAMERICA_BF9096_ALT:
		case CAMERICA_GOLDENFIVE:
		case CNE_DECATHLON:
		case CNE_SHLZ:
		case DIS_74X161X161X32:
		case DIS_74X139X74:
		case DIS_74X377:
		case DIS_74X161X138:
		case HENGG_SRICH:
		case HENGG_XHZS:	//legacy need to move mapper 179 to mapper 176
		case HES_BOARD:
		case IREM_LROG017:
		case IREM_HOLYDIVR:
		case IREM_TAM_S1:
		case IREM_G101:
		case IREM_H3001:
		case JALECO_JF11:
		case JALECO_JF13:
		case JALECO_JF16:
		case JALECO_JF17:
		case JALECO_JF17_ADPCM:
		case JALECO_JF19:
		case JALECO_JF19_ADPCM:
		case JALECO_SS88006:
		case JALECO_JF23:
		case JALECO_JF24:
		case JALECO_JF29:
		case JALECO_JF33:
		case JNCOTA_KT1001:
		case KAISER_KS106C:
		case KAISER_KS7058:
		case KAISER_KS7022:
		case KAISER_KS7032:
		case KAISER_KS202:
		case KAISER_KS7016:
		case KAISER_KS7016B:
		case KAISER_KS7021A:
		case KAISER_KS7010:
		case KAISER_KS7012:
		case KAISER_KS7013B:
		case KAISER_KS7030:
		case KAISER_KS7031:
		case KAISER_KS7037:
		case KAISER_KS7057:
		case NAMCOT_34X3:
		case NAMCOT_3446:
		case NAMCOT_3425:
		case NAMCOT_175:
		case NAMCOT_340:
		case STD_EVENT:
		case STD_DISKSYS:
		case STD_PXROM:
		case STD_SXROM:
		case STD_SNROM:
		case STD_SOROM:
		case STD_SUROM:
		case STD_SXROM_EXT:
		case STD_SZROM:
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
		case STD_TXROM:
		case STD_TXSROM:
		case STD_TKROM:
		case STD_TQROM:
		case STD_HKROM:
		case NES_QJ:
		case PAL_ZZ:
		case KONAMI_VRC1:
		case KONAMI_VRC2:
		case KONAMI_VRC3:
		case KONAMI_VRC4:
		case KONAMI_VRC6:
		case KONAMI_VRC7:
		case SACHEN_SA0037:
		case SUNSOFT_1:
		case SUNSOFT_2:
		case SUNSOFT_3:
		case TENGEN_800032:
		case TENGEN_800037:
		case UNL_ACTION53:
		case UNL_DH08:
		case UNL_LE05:
		case UNL_LH28_LH54:
		case UNL_LH31:
		case UNL_RT01:
		case UNL_NINJARYU:
		case BMC_JY012005:
		case BMC_JY820845C:
		case BMC_SRPG_5IN1:
		case TXC_22110:
		case STD_FXROM:
		case NITRA_TDA:
		case UNL_BMW8544:
		case UNL_FS6:
		case BTL_SBROS11:
		case UNL_MALISB:
		case BMC_FAMILY_4646:
		case BTL_PIKACHUY2K:
		case UNL_8237:
		case UNL_8237A:
		case KASING_BOARD:
		case SUPERGAME_LIONKING:
		case SUPERGAME_BOOGERMAN:
		case UNL_H2288:
		case TXC_TW:
		case UNL_KOF97:
		case UNL_SF3:
		case COCOMA_BOARD:
		case UNL_A9746:
		case BMC_NT639:
		case BMC_S24IN1SC03:
		case BMC_SUPERBIG_7IN1:
		case BMC_JY302:
		case BMC_SFC12:
		case BMC_A88S1:
		case BMC_830832C:
		case BMC_YY841101C:
		case BMC_YY841155C:
		case BMC_PJOY84:
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

	// MMC6/HKROM contains 1KB of internal RAM decoded in $7000-$7FFF.
	// The cartridge does not drive reads from $6000-$6FFF.
	if (pcb_id == STD_HKROM)
		ranges.push_back({ 0x6000, 0x6fff });

	const u32 prg_ram_size = cart->get_prgram_size() + cart->get_battery_size();

	if (pcb_id == KONAMI_VRC4 && prg_ram_size == 0x0800)
		ranges.push_back({ 0x7000, 0x7fff });

	const bool no_prg_ram = prg_ram_size == 0;

	//const bool no_prg_ram = !cart->get_prgram_size() && !cart->get_battery_size();

	if (no_prg_ram)
	{
		bool range_6000_is_special = false;

		switch (pcb_id)
		{
			case KONAMI_VRC2:
				range_6000_is_special = true;
				ranges.push_back({ 0x7000, 0x7fff });
				break;
			//case STD_EXROM:
			case BANDAI_LZ93EX1:
			case BANDAI_LZ93EX2:
			case BANDAI_KARAOKE:
			case STD_HKROM:
			//case STD_SZROM:
			case UNL_BMW8544:
			case SUNSOFT_FME7:
			case SUNSOFT_5:
			case BTL_AISENSHINICOL:
			case BTL_MARIOBABY:
			case BTL_YUNG08:
			case JYCOMPANY_A:
			case JYCOMPANY_B:
			case JYCOMPANY_C:
			case SVISION16_BOARD:
			case UNL_DH08:
			case UNL_LE05:
			case UNL_LH28_LH54:
			case UNL_LH31:
			case UNL_2708:
			case UNL_AC08:
			case UNL_MMALEE:
			case GG_NROM:
			case KAISER_KS7032:
			case KAISER_KS7016:
			case KAISER_KS7016B:
			case KAISER_KS7010:
			case KAISER_KS7030:
			case KAISER_KS7031:
			case KAISER_KS7037:
			case KAISER_KS7057:
			case KASING_BOARD:
			case SUPERGAME_LIONKING:
			case SUPERGAME_BOOGERMAN:
			case UNL_603_5052:
			case BMC_FK23C:
			case BMC_FK23CA:
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

class nes_state : public nes_base_state
{
public:
	nes_state(const machine_config &mconfig, device_type type, const char *tag) :
		nes_base_state(mconfig, type, tag),
		m_mainram(*this, "mainram"),
		m_ppu(*this, "ppu"),
		m_screen(*this, "screen"),
		m_exp(*this, "exp"),
		m_special(*this, "special"),
		m_cartslot(*this, "nes_slot"),
		m_disk(*this, "disk"),
		m_prg_bank(*this, "prg%u", 0U)
	{ }

	uint8_t fc_in0_r();
	uint8_t fc_in1_r();
	void fc_in0_w(uint8_t data);
	virtual void machine_start() override ATTR_COLD;
	virtual void machine_reset() override ATTR_COLD;
	virtual void video_start() override ATTR_COLD;
	uint32_t screen_update_nes(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	void screen_vblank_nes(int state);

	void init_famicom();

	// these are needed until we modernize the FDS controller
	DECLARE_MACHINE_START(fds);
	DECLARE_MACHINE_START(famitwin);
	DECLARE_MACHINE_RESET(fds);
	DECLARE_MACHINE_RESET(famitwin);
	DECLARE_MACHINE_RESET(famitvc1);
	void setup_disk(nes_disksys_device *slot);

	void suborkbd(machine_config &config);
	void famipalc(machine_config &config);
	void famicom(machine_config &config);
	void famicomo(machine_config &config);
	void famitvc1(machine_config &config);
	void famitwin(machine_config &config);
	void fctitler(machine_config &config);
	void nespal(machine_config &config);
	void nespalc(machine_config &config);
	void nes(machine_config &config);
	void fds(machine_config &config);
	void nes_map(address_map &map) ATTR_COLD;

private:
	// video-related
	int m_last_frame_flip = 0;

	// misc
	ioport_port       *m_io_disksel = nullptr;

	std::unique_ptr<uint8_t[]>    m_ciram; // PPU nametable RAM - external to PPU!

	required_shared_ptr<uint8_t> m_mainram;

	required_device<ppu2c0x_device> m_ppu;
	required_device<screen_device> m_screen;
	optional_device<nes_control_port_device> m_exp;
	optional_device<nes_control_port_device> m_special;
	optional_device<nes_cart_slot_device> m_cartslot;
	optional_device<nes_disksys_device> m_disk;
	memory_bank_array_creator<4> m_prg_bank;
};

/***************************************************************************
    FUNCTIONS
***************************************************************************/

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
	for (int i = 0; i < 0x800; i++)
		m_mainram[i] = machine().rand() & 0xff;

	// CIRAM (Character Internal RAM)
	// NES has 2KB of internal RAM which can be used to fill the 4x1KB banks of PPU RAM at $2000-$2fff
	// Line A10 is exposed to the carts, so that games can change CIRAM mapping in PPU (we emulate this with the set_nt_mirroring
	// function). CIRAM can also be disabled by the game (if e.g. VROM or cart RAM has to be used in PPU...
	m_ciram = std::make_unique<uint8_t[]>(0x800);
	for (int i = 0; i < 0x800; i += 2)
	{
		m_ciram[i] = 0x00;
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
		m_ppu->set_latch(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::ppu_latch));
		m_ppu->set_ppu_to_mapper(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::ppu_to_mapper));

		m_ppu->set_ppu_bus_address(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::ppu_bus_address));
		m_ppu->set_ppu_odd_frame_skip(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::ppu_odd_frame_skip));
		m_ppu->set_mmc1_ppu_phase(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::mmc1_ppu_phase));
		const int callback_pcb_id = m_cartslot->get_pcb_id();

		if (callback_pcb_id == STD_EXROM || callback_pcb_id == GG_NROM)
		{
			m_ppu->set_mmc5_ppu_read(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::mmc5_clock_ppu_read));
			m_ppu->set_mmc5_reset_scanline_irq(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::mmc5_reset_scanline_irq_state));
			m_maincpu6502->set_mmc5_reset_scanline_irq(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::mmc5_reset_scanline_irq_state));
			m_maincpu6502->set_mmc5_ppuctrl_write(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::mmc5_real_ppuctrl_write));
			m_maincpu6502->set_mmc5_ppumask_write(*m_cartslot->m_cart, FUNC(device_nes_cart_interface::mmc5_real_ppumask_write));
		}

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
			CNE_FSB,
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
			UNL_LH53,
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
			ACTENT_ACT52,
			BMC_FK23C,
			BMC_FK23CA,
			BTL_09034A,
			BTL_MARIOBABY,
			KAISER_KS7017,
			GG_NROM,
			STD_DISKSYS,
			UNL_603_5052
		};

		int pcb_id = m_cartslot->get_pcb_id();

		const bool cart_reads_h = std::find(std::begin(r_h_pcbs), std::end(r_h_pcbs), pcb_id) != std::end(r_h_pcbs);
		const bool cart_writes_ex = std::find(std::begin(w_ex_pcbs), std::end(w_ex_pcbs), pcb_id) != std::end(w_ex_pcbs);
		const bool cart_reads_ex = std::find(std::begin(rw_ex_pcbs), std::end(rw_ex_pcbs), pcb_id) != std::end(rw_ex_pcbs);

		if (cart_reads_h)
		{
			logerror("read_h installed!\n");
			space.install_read_handler(0x8000, 0xffff, read8sm_delegate(*m_cartslot, FUNC(nes_cart_slot_device::read_h)));
		}

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

		m_cartslot->pcb_start(m_ciram.get());

		auto ranges = compute_open_bus_ranges(m_cartslot, cart_reads_ex);
		uint32_t packed[16];
		int count = 0;
		for (auto &r : ranges)
		{
			if (count >= std::size(packed))
				break;
			logerror("open-bus read installed for range: $%04X-$%04X\n", r.start, r.end);
			osd_printf_info("OPEN BUS RANGE: $%04X-$%04X\n", r.start, r.end);
			packed[count++] = (uint32_t(r.start) << 16) | uint32_t(r.end);
		}
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
	uint8_t ret = m_maincpu6502->get_open_bus() & 0xe0;
	uint8_t const pad1 = m_ctrl1->read_bit0();
	ret |= pad1;
	if (m_ctrl2)
		ret |= m_ctrl2->read_bit34();
	bool const a_now = BIT(pad1, 0);
	if (a_now && !m_p1_a_prev)
		g_nes_p1_a_pressed_edge = true;
	m_p1_a_prev = a_now;
	return ret;
}

uint8_t nes_base_state::nes_in1_r()
{
	uint8_t ret = m_maincpu6502->get_open_bus() & 0xe0;
	ret |= m_ctrl2->read_bit0();
	ret |= m_ctrl2->read_bit34();
	return ret;
}

void nes_base_state::nes_in0_w(uint8_t data)
{
	m_ctrl1->write(data & 1);
	m_ctrl2->write(data & 1);
}


uint8_t nes_state::fc_in0_r()
{
	uint8_t ret = 0x40 | (m_maincpu6502->get_open_bus() & 0xb8);

	ret |= m_ctrl1->read_bit0();
	ret |= m_ctrl2->read_bit2();
	ret |= m_exp->read_exp(0);

	return ret;
}

uint8_t nes_state::fc_in1_r()
{
	uint8_t ret = 0x40 | (m_maincpu6502->get_open_bus() & 0xa0);

	ret |= m_ctrl2->read_bit0();
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


void nes_state::video_start()
{
	m_last_frame_flip =  0;
}


/***************************************************************************

  Display refresh

***************************************************************************/

uint32_t nes_state::screen_update_nes(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	// render the ppu
	m_ppu->render(bitmap, 0, 0, 0, 0, cliprect);
	return 0;
}

void nes_state::screen_vblank_nes(int state)
{
	// on rising edge
	if (!state)
	{
		// if this is a disk system game, check for the flip-disk key
		if ((m_cartslot && m_cartslot->exists() && (m_cartslot->get_pcb_id() == STD_DISKSYS))   // first scenario = disksys in m_cartslot (= famicom)
				|| m_disk)  // second scenario = disk via fixed internal disk option (fds & famitwin)
		{
			if (m_io_disksel)
			{
				// latch this input so it doesn't go at warp speed
				if ((m_io_disksel->read() & 0x01) && (!m_last_frame_flip))
				{
					if (m_disk)
						m_disk->disk_flip_side();
					else
						m_cartslot->disk_flip_side();
					m_last_frame_flip = 1;
				}

				if (!(m_io_disksel->read() & 0x01))
					m_last_frame_flip = 0;
			}
		}
	}
}


void nes_state::nes_map(address_map &map)
{
	map(0x0000, 0x07ff).ram().mirror(0x1800).share("mainram");                              // RAM
	map(0x2000, 0x3fff).rw(m_ppu, FUNC(ppu2c0x_device::read), FUNC(ppu2c0x_device::write)); // PPU registers
	// $4014 OAM DMA is handled by the RP2A03 core.
	map(0x4016, 0x4016).rw(FUNC(nes_state::nes_in0_r), FUNC(nes_state::nes_in0_w));         // IN0 - input port 1
	map(0x4017, 0x4017).r(FUNC(nes_state::nes_in1_r));                                      // IN1 - input port 2
	// 0x4100-0x5fff -> LOW HANDLER defined on a pcb base
	// 0x6000-0x7fff -> MID HANDLER defined on a pcb base
	// 0x8000-0xffff -> HIGH HANDLER defined on a pcb base
}

static INPUT_PORTS_START( nes )
	// input devices go through slot options
INPUT_PORTS_END

static INPUT_PORTS_START( famicom )
	// input devices go through slot options
	PORT_START("FLIPDISK") // fake key
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("Change Disk Side") PORT_CODE(KEYCODE_SPACE)
INPUT_PORTS_END


void nes_state::nes(machine_config &config)
{
	// basic machine hardware
	rp2a03_device &maincpu(RP2A03G(config, m_maincpu, NTSC_APU_CLOCK));
	maincpu.set_addrmap(AS_PROGRAM, &nes_state::nes_map);

	SCREEN(config, m_screen);
	m_screen->set_raw(21'477'272.0 / 4.0, 341, 0, 256, 262, 0, 240);
	m_screen->set_size(32*8, 262);
	m_screen->set_visarea(0*8, 32*8-1, 0*8, 30*8-1);
	m_screen->set_screen_update(FUNC(nes_state::screen_update_nes));
	m_screen->screen_vblank().set(FUNC(nes_state::screen_vblank_nes));

	PPU_2C02(config, m_ppu);
	m_ppu->set_cpu_tag(m_maincpu);
	m_ppu->int_callback().set_inputline(m_maincpu, INPUT_LINE_NMI);

	// sound hardware
	SPEAKER(config, "mono").front_center();
	maincpu.add_route(ALL_OUTPUTS, "mono", 0.90);

	NES_CONTROL_PORT(config, m_ctrl1, nes_control_port1_devices, "joypad").set_screen_tag(m_screen);
	NES_CONTROL_PORT(config, m_ctrl2, nes_control_port2_devices, "joypad").set_screen_tag(m_screen);
	NES_CONTROL_PORT(config, m_special, nes_control_special_devices, nullptr).set_screen_tag(m_screen);

	NES_CART_SLOT(config, m_cartslot, NTSC_APU_CLOCK, nes_cart, nullptr).set_must_be_loaded(true);
	SOFTWARE_LIST(config, "cart_list").set_original("nes").set_filter("!EXP");
	SOFTWARE_LIST(config, "ade_list").set_original("nes_ade");         // Camerica/Codemasters Aladdin Deck Enhancer mini-carts
	SOFTWARE_LIST(config, "ntb_list").set_original("nes_ntbrom");      // Sunsoft Nantettate! Baseball mini-carts
	SOFTWARE_LIST(config, "kstudio_list").set_original("nes_kstudio"); // Bandai Karaoke Studio expansion carts
	SOFTWARE_LIST(config, "datach_list").set_original("nes_datach");   // Bandai Datach Joint ROM System mini-carts
	SOFTWARE_LIST(config, "famibox_list").set_compatible("famibox");   // FamicomBox/FamicomStation carts
}

void nes_state::nespal(machine_config &config)
{
	nes(config);

	// basic machine hardware
	m_maincpu->set_clock(PAL_APU_CLOCK);

	PPU_2C07(config.replace(), m_ppu);
	m_ppu->set_cpu_tag(m_maincpu);
	m_ppu->int_callback().set_inputline(m_maincpu, INPUT_LINE_NMI);

	m_cartslot->set_clock(PAL_APU_CLOCK);

	// PAL NES / 2C07 composite timing
	m_screen->set_raw(26'601'712.0 / 5.0, 341, 8, 248, 312, 8, 232);
}

void nes_state::famicom(machine_config &config)
{
	nes(config);

	downcast<m6502_device &>(*m_maincpu).set_famicom_controller_timing(true);

	NES_CONTROL_PORT(config.replace(), m_ctrl1, fc_control_port1_devices, "joypad").set_screen_tag(m_screen);
	NES_CONTROL_PORT(config.replace(), m_ctrl2, fc_control_port2_devices, "joypad").set_screen_tag(m_screen);
	NES_CONTROL_PORT(config, m_exp, fc_expansion_devices, nullptr).set_screen_tag(m_screen);

	subdevice<software_list_device>("cart_list")->set_filter(nullptr);
	SOFTWARE_LIST(config, "flop_list").set_original("famicom_flop");
	SOFTWARE_LIST(config, "cass_list").set_original("famicom_cass");
}

void nes_state::famicomo(machine_config &config)
{
	famicom(config);

	// basic machine hardware
	rp2a03_device &maincpu(RP2A03(config.replace(), m_maincpu, NTSC_APU_CLOCK));
	maincpu.set_addrmap(AS_PROGRAM, &nes_state::nes_map);
	maincpu.set_famicom_controller_timing(true);

	// sound hardware
	maincpu.add_route(ALL_OUTPUTS, "mono", 0.90);
}

void nes_state::nespalc(machine_config &config)
{
	nespal(config);

	m_maincpu->set_clock(PALC_APU_CLOCK);
	m_maincpu->set_addrmap(AS_PROGRAM, &nes_state::nes_map);

	// UMC 6538 and friends -- extends time for rendering dummy scanlines
	PPU_PALC(config.replace(), m_ppu);
	m_ppu->set_cpu_tag(m_maincpu);
	m_ppu->int_callback().set_inputline(m_maincpu, INPUT_LINE_NMI);

	m_cartslot->set_clock(PALC_APU_CLOCK);

	// video hardware
	m_screen->set_refresh_hz(50.0070);
	m_screen->set_vblank_time(ATTOSECONDS_IN_USEC((113.66/(PALC_APU_CLOCK.dvalue()/1000000)) *
							 (ppu2c0x_device::VBLANK_LAST_SCANLINE_PAL-ppu2c0x_device::VBLANK_FIRST_SCANLINE_PALC+1+2)));
}

void nes_state::famipalc(machine_config &config)
{
	nespalc(config);

	NES_CONTROL_PORT(config.replace(), m_ctrl1, fc_control_port1_devices, "joypad").set_screen_tag(m_screen);
	NES_CONTROL_PORT(config.replace(), m_ctrl2, fc_control_port2_devices, "joypad").set_screen_tag(m_screen);
	NES_CONTROL_PORT(config, m_exp, fc_expansion_devices, nullptr).set_screen_tag(m_screen);

	SOFTWARE_LIST(config, "cass_list").set_original("famicom_cass");
}

void nes_state::suborkbd(machine_config &config)
{
	famipalc(config);

	// TODO: emulate the parallel port bus!
	m_exp->set_default_option("subor_keyboard");
	m_exp->set_fixed(true);
}

void nes_state::setup_disk(nes_disksys_device *slot)
{
	if (slot)
	{
		address_space &space = m_maincpu->space(AS_PROGRAM);

		// Set up memory handlers
		space.install_read_handler(0x4020, 0x40ff, read8sm_delegate(*slot, FUNC(nes_disksys_device::read_ex)));
		space.install_write_handler(0x4020, 0x40ff, write8sm_delegate(*slot, FUNC(nes_disksys_device::write_ex)));
		space.install_read_handler(0x4100, 0x5fff, read8sm_delegate(*slot, FUNC(device_nes_cart_interface::read_l)));
		space.install_write_handler(0x4100, 0x5fff, write8sm_delegate(*slot, FUNC(device_nes_cart_interface::write_l)));
		space.install_read_handler(0x6000, 0x7fff, read8sm_delegate(*slot, FUNC(nes_disksys_device::read_m)));
		space.install_write_handler(0x6000, 0x7fff, write8sm_delegate(*slot, FUNC(nes_disksys_device::write_m)));
		space.install_read_handler(0x8000, 0xffff, read8sm_delegate(*slot, FUNC(nes_disksys_device::read_h)));
		space.install_write_handler(0x8000, 0xffff, write8sm_delegate(*slot, FUNC(nes_disksys_device::write_h)));

		slot->vram_alloc(0x2000);
		slot->prgram_alloc(0x8000);

		slot->pcb_start(machine(), m_ciram.get(), false);
		m_ppu->space(AS_PROGRAM).install_readwrite_handler(0, 0x1fff, read8sm_delegate(*slot, FUNC(device_nes_cart_interface::chr_r)), write8sm_delegate(*slot, FUNC(device_nes_cart_interface::chr_w)));
		m_ppu->space(AS_PROGRAM).install_readwrite_handler(0x2000, 0x3eff, read8sm_delegate(*slot, FUNC(device_nes_cart_interface::nt_r)), write8sm_delegate(*slot, FUNC(device_nes_cart_interface::nt_w)));
		m_ppu->set_scanline_callback(*slot, FUNC(device_nes_cart_interface::scanline_irq));
		m_ppu->set_hblank_callback(*slot, FUNC(nes_disksys_device::hblank_irq));
		m_ppu->set_latch(*slot, FUNC(device_nes_cart_interface::ppu_latch));
		m_ppu->set_ppu_to_mapper(*slot, FUNC(device_nes_cart_interface::ppu_to_mapper));
		m_ppu->set_ppu_bus_address(*slot, FUNC(device_nes_cart_interface::ppu_bus_address));
	}
}

MACHINE_START_MEMBER( nes_state, fds )
{
	m_ciram = std::make_unique<uint8_t[]>(0x800);
	m_io_disksel = ioport("FLIPDISK");
	setup_disk(m_disk);

	// register saves
	save_item(NAME(m_last_frame_flip));
	save_pointer(NAME(m_ciram), 0x800);
}

MACHINE_RESET_MEMBER( nes_state, fds )
{
	// Reset the mapper variables
	m_disk->pcb_reset();

	// the rest is the same as for nes/famicom/dendy
	m_maincpu->reset();
}

void nes_state::fds(machine_config &config)
{
	famicom(config);

	MCFG_MACHINE_START_OVERRIDE(nes_state, fds)
	MCFG_MACHINE_RESET_OVERRIDE(nes_state, fds)

	config.device_remove("nes_slot");
	NES_DISKSYS(config, "disk", NTSC_APU_CLOCK);

	config.device_remove("cart_list");
	config.device_remove("cass_list");
	config.device_remove("ade_list");
	config.device_remove("ntb_list");
	config.device_remove("kstudio_list");
	config.device_remove("datach_list");
	config.device_remove("famibox_list");
}

MACHINE_RESET_MEMBER( nes_state, famitvc1 )
{
	// TODO: supposedly the C1 used the cartridge connector audio pins to detect
	// the presence of a cart and picks the builtin ROM accordingly. If so, the C1
	// should not support Famicom expansion audio chips.

	// Reset the mapper variables. Will also mark the char-gen ram as dirty
	if (m_cartslot->exists())
	{
		m_cartslot->pcb_reset();
	}
	else
	{
		m_maincpu->space(AS_PROGRAM).install_rom(0x8000, 0x9fff, 0x6000, memregion("canvas_prg")->base());
		m_ppu->space(AS_PROGRAM).install_rom(0x0000, 0x1fff, memregion("canvas_chr")->base());
	}

	m_maincpu->reset();
}

void nes_state::famitvc1(machine_config &config)
{
	famicomo(config); // has an RP2A03 like the original Famicom

	MCFG_MACHINE_RESET_OVERRIDE( nes_state, famitvc1 )

	PPU_2C03B(config.replace(), m_ppu);
	m_ppu->set_cpu_tag(m_maincpu);
	m_ppu->int_callback().set_inputline(m_maincpu, INPUT_LINE_NMI);

	m_cartslot->set_must_be_loaded(false);
}

MACHINE_START_MEMBER( nes_state, famitwin )
{
	// start the base nes stuff
	machine_start();

	// if there is no cart inserted, setup the disk expansion instead
	if (!m_cartslot->exists())
	{
		setup_disk(m_disk);

		// replace the famicom disk ROM with the twin famicom one (until we modernize the floppy drive)
		m_maincpu->space(AS_PROGRAM).install_rom(0xe000, 0xffff, memregion("maincpu")->base() + 0xe000);
	}
	machine().scheduler().perfect_quantum(attotime::from_ticks(1, m_maincpu->clock() * 3));
}

MACHINE_RESET_MEMBER( nes_state, famitwin )
{
	// Reset the mapper variables. Will also mark the char-gen ram as dirty
	m_cartslot->pcb_reset();
	// if there is no cart inserted, initialize the disk expansion instead
	if (!m_cartslot->exists())
		m_disk->pcb_reset();

	// the rest is the same as for nes/famicom/dendy
	m_maincpu->reset();
}

void nes_state::famitwin(machine_config &config)
{
	famicom(config);

	MCFG_MACHINE_START_OVERRIDE( nes_state, famitwin )
	MCFG_MACHINE_RESET_OVERRIDE( nes_state, famitwin )

	m_cartslot->set_must_be_loaded(false);

	NES_DISKSYS(config, "disk", NTSC_APU_CLOCK);
}

void nes_state::fctitler(machine_config &config)
{
	famicom(config);

	// PPU is really RC2C05-99, but it can't be like the other 2C05s since they swap PPUCTRL and PPUMASK registers
	PPU_2C03B(config.replace(), m_ppu);
	m_ppu->set_cpu_tag(m_maincpu);
	m_ppu->int_callback().set_inputline(m_maincpu, INPUT_LINE_NMI);
}


ROM_START( nes )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASE00 )  // Main RAM
ROM_END

ROM_START( nespal )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASE00 )  // Main RAM
ROM_END

ROM_START( famicom )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASE00 )  // Main RAM
ROM_END

ROM_START( famicomo )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASE00 )  // Main RAM
ROM_END

ROM_START( famitvc1 )
	ROM_REGION( 0x2000, "canvas_prg", 0 )
	ROM_LOAD( "ix0402ce.ic109", 0x0000, 0x2000, CRC(96456b13) SHA1(a4dcb3c4f2be5077f0d197e870a26414287ce189) ) // dump needs verified

	ROM_REGION( 0x2000, "canvas_chr", 0 )
	ROM_LOAD( "ix0403ce.ic110", 0x0000, 0x2000, CRC(9cba4524) SHA1(2bd833f8049bf7a14ce337c3cde35f2140242a18) ) // dump needs verified

	ROM_REGION( 0xc0, "ppu:palette", 0 )
	ROM_LOAD( "rp2c0x.pal", 0x00, 0xc0, CRC(48de65dc) SHA1(d10acafc8da9ff479c270ec01180cca61efe62f5) )
ROM_END

#define rom_fds rom_famicom

ROM_START( famitwin )
	ROM_REGION( 0x10000, "maincpu", 0 )  // Main RAM
	ROM_LOAD( "rp2c33a-02.bin", 0xe000, 0x2000, CRC(4df24a6c) SHA1(e4e41472c454f928e53eb10e0509bf7d1146ecc1) ) // "Famicom" logo instead of Nintendo logo
ROM_END

ROM_START( fctitler )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASE00 )  // Main RAM

	// builtin has its own MMC1B1, 8K PRGRAM (battery backed), and 8K CHRRAM
	// TODO: add switch that selects which boots: builtin vs cart
	ROM_REGION( 0x80000, "builtin_prg", 0 )
	ROM_LOAD( "x1252ce.prg", 0x00000, 0x80000, CRC(696712f9) SHA1(1b9475f569ea9943122676ce65165dc82d11ef38) )

	ROM_REGION( 0xc0, "ppu:palette", 0 )
	ROM_LOAD( "rp2c0x.pal", 0x00, 0xc0, CRC(48de65dc) SHA1(d10acafc8da9ff479c270ec01180cca61efe62f5) )
ROM_END

// see http://www.disgruntleddesigner.com/chrisc/drpcjr/index.html
// and http://www.disgruntleddesigner.com/chrisc/drpcjr/DrPCJrMemMap.txt
ROM_START( drpcjr )
	ROM_REGION( 0x18000, "maincpu", 0 )  // Main RAM + program banks
	// 4 banks to be mapped in 0xe000-0xffff (or 8 banks to be mapped in 0xe000-0xefff & 0xf000-0xffff).
	// Banks selected by writing at 0x4180
	ROM_LOAD("drpcjr_bios.bin", 0x10000, 0x8000, CRC(c8fbef89) SHA1(2cb0a817b31400cdf27817d09bae7e69f41b062b) ) // bios vers. 1.0a
	// Not sure if we should support this: hacked version 1.5a by Chris Covell with bugfixes and GameGenie support
//  ROM_LOAD("drpcjr_v1_5_gg.bin", 0x10000, 0x8000, CRC(98f2033b) SHA1(93c114da787a19279d1a46667c2f69b49e25d4f1) )
ROM_END

ROM_START( iq501 )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASE00 )  // Main RAM
ROM_END

ROM_START( iq502 )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASE00 )  // Main RAM
ROM_END

ROM_START( dendy )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASE00 )  // Main RAM
ROM_END

ROM_START( dendy2 )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASE00 )  // Main RAM
ROM_END

ROM_START( gchinatv )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASE00 )  // Main RAM
ROM_END

ROM_START( sb486 )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASE00 )  // Main RAM
ROM_END

} // anonymous namespace

/***************************************************************************

  Game driver(s)

***************************************************************************/

//    YEAR  NAME      PARENT   COMPAT  MACHINE   INPUT    CLASS      INIT          COMPANY          FULLNAME

// Nintendo Entertainment System hardware
CONS( 1985, nes,      0,       0,      nes,      nes,     nes_state, empty_init,   "Nintendo",      "Nintendo Entertainment System / Famicom (NTSC)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
CONS( 1987, nespal,   nes,     0,      nespal,   nes,     nes_state, empty_init,   "Nintendo",      "Nintendo Entertainment System (PAL)",            MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )

// Famicom hardware
CONS( 1983, famicom,  0,       nes,    famicom,  famicom, nes_state, init_famicom, "Nintendo",      "Famicom",                         MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
CONS( 1983, famicomo, famicom, 0,      famicomo, famicom, nes_state, init_famicom, "Nintendo",      "Famicom (earlier, with RP2A03)",  MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
CONS( 1983, famitvc1, famicom, 0,      famitvc1, famicom, nes_state, init_famicom, "Sharp",         "My Computer Terebi C1",           MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
CONS( 1986, fds,      famicom, 0,      fds,      famicom, nes_state, init_famicom, "Nintendo",      "Famicom (w/ Disk System add-on)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
CONS( 1986, famitwin, famicom, 0,      famitwin, famicom, nes_state, init_famicom, "Sharp",         "Twin Famicom",                    MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
CONS( 1989, fctitler, famicom, 0,      fctitler, famicom, nes_state, init_famicom, "Sharp",         "Famicom Titler",                  MACHINE_IMPERFECT_GRAPHICS | MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )

// Clone hardware
// Many knockoffs using derivatives of the UMC board design, later incorporated into single CMOS chips, were manufactured before and past the end of the Famicom's timeline.

// !! PAL clones documented here !!
// Famicom-based
CONS( 1992, iq501,    0,       nes,    famipalc, nes,     nes_state, init_famicom, "Micro Genius",  "IQ-501",               MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
CONS( 1992, iq502,    0,       nes,    famipalc, nes,     nes_state, init_famicom, "Micro Genius",  "IQ-502",               MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
CONS( 1992, dendy,    iq501,   0,      famipalc, nes,     nes_state, init_famicom, "Steepler",      "Dendy Classic 1",      MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
CONS( 1992, dendy2,   iq502,   0,      famipalc, nes,     nes_state, init_famicom, "Steepler",      "Dendy Classic 2",      MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
CONS( 198?, gchinatv, 0,       nes,    famipalc, nes,     nes_state, init_famicom, "Golden China",  "Golden China TV Game", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )

// Subor/Xiao Ba Wang hardware and derivatives
// These clones implement a keyboard and a parallel port for printing from a word processor. Later models have mice, PS/2 ports, serial ports and a floppy drive.
CONS( 1993, sb486,    0,       nes,    suborkbd, nes,     nes_state, init_famicom, "Subor",         "SB-486", MACHINE_IMPERFECT_GRAPHICS | MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )

// !! NTSC clones documented here !!
// Famicom-based
// Bung hardware
// Mice, keyboard, etc, including a floppy drive that allows you to run games with a selection of 4 internal "mappers" available on the system.
CONS( 1996, drpcjr,   0,       nes,    famicom,  famicom, nes_state, init_famicom, "Bung",          "Doctor PC Jr", MACHINE_IMPERFECT_GRAPHICS | MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
