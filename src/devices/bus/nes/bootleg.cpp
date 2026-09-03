// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Bootleg PCBs


 Here we emulate the PCBs used in FDS2NES conversions which are common in the Taiwanese & HK markets
 Notice that many of these have unusual PRG sizes (32KB+8KB, 32KB+24KB) partially mapped in the WRAM
 area, or WRAM overlapping the usual PRG area, so that we often skip the usual bankswitching mechanisms
 in favor of direct handling of the PRG accesses

 TODO:
 - review all PCBs and fix the starting banks (which are often the main problem of not working games)
 - SMB2 bootlegs all seem to have timing issues. This is apparent on worlds A-D as the bottom of the
   world letter scrolls. Hardware footage of the mapper 50 version shows the letter bottoms flickering
   (though it could be a video/compression artifact).

 ***********************************************************************************************************/


#include "emu.h"
#include "bootleg.h"
#include "cpu/m6502/m6502.h"
#include "video/ppu2c0x.h"      // this has to be included so that IRQ functions can access ppu2c0x_device::BOTTOM_VISIBLE_SCANLINE

#define LOG_HIFREQ (1U << 1)

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_MARIOBABY,      nes_mbaby_device,     "nes_mbaby",     "NES Cart Mario Baby Bootleg PCB")
DEFINE_DEVICE_TYPE(NES_ASN,            nes_asn_device,       "nes_asn",       "NES Cart Ai Senshi Nicol Bootleg PCB")
DEFINE_DEVICE_TYPE(NES_SMB3PIRATE,     nes_smb3p_device,     "nes_smb3p",     "NES Cart Super Mario Bros. 3 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_BTL_CONTRAJ,    nes_btl_cj_device,    "nes_btl_cj",    "NES Cart Contra Japan Pirate PCB")
DEFINE_DEVICE_TYPE(NES_BTL_DNINJA,     nes_btl_dn_device,    "nes_btl_dn",    "NES Cart DragonNinja Pirate PCB")
DEFINE_DEVICE_TYPE(NES_SMB2J,          nes_smb2j_device,     "nes_smb2j",     "NES Cart Super Mario Bros. 2 Jpn PCB")
DEFINE_DEVICE_TYPE(NES_SMB2JA,         nes_smb2ja_device,    "nes_smb2ja",    "NES Cart Super Mario Bros. 2 Jpn (Alt) PCB")
DEFINE_DEVICE_TYPE(NES_SMB2JB,         nes_smb2jb_device,    "nes_smb2jb",    "NES Cart Super Mario Bros. 2 Jpn (Alt 2) PCB")
DEFINE_DEVICE_TYPE(NES_N32_4IN1,       nes_n32_4in1_device,  "nes_n32_4in1",  "NES Cart N-32 4 in 1 PCB")
DEFINE_DEVICE_TYPE(NES_0353,           nes_0353_device,      "nes_0353",      "NES Cart 0353 PCB")
DEFINE_DEVICE_TYPE(NES_09034A,         nes_09034a_device,    "nes_09034a",    "NES Cart 09-034A PCB")
DEFINE_DEVICE_TYPE(NES_L001,           nes_l001_device,      "nes_l001",      "NES Cart L-001 PCB")
DEFINE_DEVICE_TYPE(NES_BATMANFS,       nes_batmanfs_device,  "nes_batmanfs",  "NES Cart Batman Pirate PCB")
DEFINE_DEVICE_TYPE(NES_PALTHENA,       nes_palthena_device,  "nes_palthena",  "NES Cart Palthena no Kagami Pirate PCB")
DEFINE_DEVICE_TYPE(NES_TOBIDASE,       nes_tobidase_device,  "nes_tobidase",  "NES Cart Tobidase Daisakusen Pirate PCB")
DEFINE_DEVICE_TYPE(NES_DH08,           nes_dh08_device,      "nes_dh08",      "NES Cart DH-08 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_LE05,           nes_le05_device,      "nes_le05",      "NES Cart LE05 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_LG25,           nes_lg25_device,      "nes_lg25",      "NES Cart LG25 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_LH10,           nes_lh10_device,      "nes_lh10",      "NES Cart LH10 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_LH28_LH54,      nes_lh28_lh54_device, "nes_lh28_lh54", "NES Cart LH28/LH54 Pirate PCBs")
DEFINE_DEVICE_TYPE(NES_LH31,           nes_lh31_device,      "nes_lh31",      "NES Cart LH31 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_LH32,           nes_lh32_device,      "nes_lh32",      "NES Cart LH32 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_LH42,           nes_lh42_device,      "nes_lh42",      "NES Cart LH42 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_LH51,           nes_lh51_device,      "nes_lh51",      "NES Cart LH51 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_LH53,           nes_lh53_device,      "nes_lh53",      "NES Cart LH53 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_2708,           nes_2708_device,      "nes_2708",      "NES Cart BTL-2708 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_AC08,           nes_ac08_device,      "nes_ac08",      "NES Cart AC08 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_MMALEE,         nes_mmalee_device,    "nes_mmalee",    "NES Cart Super Mario Bros. Malee 2 Pirate PCB")
DEFINE_DEVICE_TYPE(NES_RT01,           nes_rt01_device,      "nes_rt01",      "NES Cart RT-01 PCB")
DEFINE_DEVICE_TYPE(NES_YUNG08,         nes_yung08_device,    "nes_yung08",    "NES Cart Super Mario Bros. 2 YUNG-08 PCB")

nes_mbaby_device::nes_mbaby_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_MARIOBABY, tag, owner, clock), m_irq_count(0), m_irq_enable(0), m_irq_delay(0), m_latch(0), irq_timer(nullptr), m_maincpu6502(nullptr)
{
}

nes_asn_device::nes_asn_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_ASN, tag, owner, clock), m_latch(0)
{
}

nes_smb3p_device::nes_smb3p_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_SMB3PIRATE, tag, owner, clock)
	, m_irq_count(0)
	, m_irq_enable(0)
	, m_irq_delay(0)
	, irq_timer(nullptr)
	, m_maincpu6502(nullptr)
{
}

nes_btl_cj_device::nes_btl_cj_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_BTL_CONTRAJ, tag, owner, clock)
{
}

nes_btl_dn_device::nes_btl_dn_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_BTL_DNINJA, tag, owner, clock)
	, m_irq_count(0)
	, m_irq_delay(0)
	, m_a12_low_ticks(0)
	, m_maincpu6502(nullptr)
{
}

nes_smb2j_device::nes_smb2j_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_SMB2J, tag, owner, clock),
	m_irq_count(0),
	m_irq_enable(0),
	m_irq_delay(0),
	irq_timer(nullptr),
	m_maincpu6502(nullptr) {
}

nes_smb2ja_device::nes_smb2ja_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_SMB2JA, tag, owner, clock),
	m_irq_count(0),
	m_irq_enable(0),
	m_irq_delay(0),
	irq_timer(nullptr),
	m_maincpu6502(nullptr) {
}

nes_smb2jb_device::nes_smb2jb_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock, u8 bank67)
	: nes_nrom_device(mconfig, type, tag, owner, clock),
	m_irq_count(0),
	m_irq_enable(0),
	m_irq_delay(0),
	m_reg(0),
	irq_timer(nullptr),
	m_maincpu6502(nullptr),
	m_bank67(bank67) {
}

nes_smb2jb_device::nes_smb2jb_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_smb2jb_device(mconfig, NES_SMB2JB, tag, owner, clock, 0x0f) {
}

nes_n32_4in1_device::nes_n32_4in1_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_smb2jb_device(mconfig, NES_N32_4IN1, tag, owner, clock, 0x07)
	, m_smb2j_mode(false)
{
}

nes_0353_device::nes_0353_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_0353, tag, owner, clock), m_reg(0)
{
}

nes_09034a_device::nes_09034a_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_09034A, tag, owner, clock)
	, m_irq_count(0)
	, m_irq_enable(0)
	, m_reg(0)
	, m_irq_delay(0)
	, irq_timer(nullptr)
	, m_maincpu6502(nullptr)
{
}

nes_l001_device::nes_l001_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_L001, tag, owner, clock), m_irq_count(0), m_irq_delay(0), irq_timer(nullptr), m_maincpu6502(nullptr)
{
}

nes_batmanfs_device::nes_batmanfs_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_BATMANFS, tag, owner, clock), m_irq_count(0), m_irq_enable(0), m_irq_delay(0), irq_timer(nullptr), m_maincpu6502(nullptr)
{
}

nes_palthena_device::nes_palthena_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_PALTHENA, tag, owner, clock), m_reg(0)
{
}

nes_tobidase_device::nes_tobidase_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_TOBIDASE, tag, owner, clock), m_latch(0)
{
}

nes_whirlwind_device::nes_whirlwind_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, type, tag, owner, clock), m_reg(0)
{
}

nes_dh08_device::nes_dh08_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_whirlwind_device(mconfig, NES_DH08, tag, owner, clock)
{
}

nes_le05_device::nes_le05_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_whirlwind_device(mconfig, NES_LE05, tag, owner, clock)
{
}

nes_lh28_lh54_device::nes_lh28_lh54_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_whirlwind_device(mconfig, NES_LH28_LH54, tag, owner, clock)
{
}

nes_lh31_device::nes_lh31_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_whirlwind_device(mconfig, NES_LH31, tag, owner, clock)
{
}

nes_lh32_device::nes_lh32_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_LH32, tag, owner, clock), m_latch(0)
{
}

nes_lh42_device::nes_lh42_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_LH42, tag, owner, clock), m_latch(0)
{
}

nes_lg25_device::nes_lg25_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_LG25, tag, owner, clock), m_latch(0)
{
}

nes_lh10_device::nes_lh10_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_LH10, tag, owner, clock), m_latch(0)
{
}

nes_lh51_device::nes_lh51_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_LH51, tag, owner, clock)
{
}

nes_lh53_device::nes_lh53_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_LH53, tag, owner, clock), m_irq_count(0), m_irq_enable(0), m_reg(0), irq_timer(nullptr)
{
}

nes_2708_device::nes_2708_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_2708, tag, owner, clock)
{
}

nes_ac08_device::nes_ac08_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_AC08, tag, owner, clock), m_latch(0)
{
}

nes_mmalee_device::nes_mmalee_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_MMALEE, tag, owner, clock)
{
}

nes_rt01_device::nes_rt01_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_RT01, tag, owner, clock)
{
}

nes_yung08_device::nes_yung08_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_YUNG08, tag, owner, clock)
	, m_irq_count(0)
	, m_irq_latch(0)
	, m_irq_delay(0)
	, irq_timer(nullptr)
	, m_maincpu6502(nullptr)
{
}

void nes_mbaby_device::device_start()
{
	common_start();
	irq_timer = timer_alloc(FUNC(nes_mbaby_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_latch));
	save_item(NAME(m_irq_delay));
}

void nes_mbaby_device::pcb_reset()
{
	prg8_89(0x0c);
	prg8_ab(0x0d);
	prg8_cd(0x0e);
	prg8_ef(0x0f);
	chr8(0, m_chr_source);

	m_irq_enable = 0;
	m_irq_count = 0;
	m_latch = 0;
	m_irq_delay = 0;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}

void nes_asn_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_asn_device::pcb_reset()
{
	prg32((m_prg_chunks - 1) >> 1);
	chr8(0, m_chr_source);

	m_latch = 0;
}

void nes_smb3p_device::device_start() {
	common_start();

	irq_timer = timer_alloc(FUNC(nes_smb3p_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_delay));
}

void nes_smb3p_device::pcb_start(running_machine &machine, u8 *ciram_ptr, bool cart_mounted)
{
	device_nes_cart_interface::pcb_start(machine, ciram_ptr, cart_mounted);
	// registers reliably boot up with all 1s according to hardware tests
	for (int i = 0; i < 4; i++)
		prg8_x(i, 0x1f);
}

void nes_smb3p_device::pcb_reset()
{
	// registers not cleared or initialized at reset
}

void nes_batmanfs_device::device_start()
{
	common_start();

	irq_timer = timer_alloc(FUNC(nes_batmanfs_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_delay));
}

void nes_batmanfs_device::pcb_reset()
{
	prg32((m_prg_chunks >> 1) - 1);
	chr8(0, CHRROM);

	m_irq_count = 0;
	m_irq_enable = 0;
	m_irq_delay = 0;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}

void nes_btl_cj_device::pcb_reset()
{
	prg32((m_prg_chunks >> 1) - 1);    // Last 8K bank is fixed, the rest are swappable
	chr8(0, CHRROM);
}

void nes_btl_dn_device::device_start() {
	common_start();

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_delay));
	save_item(NAME(m_a12_low_ticks));
}

void nes_btl_dn_device::pcb_reset() {
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	m_irq_count = 0;
	m_irq_delay = 0;
	m_a12_low_ticks = 0;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}

void nes_smb2j_device::device_start() {
	common_start();

	irq_timer = timer_alloc(FUNC(nes_smb2j_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_delay));
}

void nes_smb2j_device::pcb_reset() {
	prg8_89(1);
	prg8_ab(0);
	prg8_cd(4);
	prg8_ef(9);
	chr8(0, CHRROM);

	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_delay = 0;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}

void nes_smb2ja_device::device_start() {
	common_start();

	irq_timer = timer_alloc(FUNC(nes_smb2ja_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_delay));
}

void nes_smb2ja_device::pcb_reset() {
	prg8_89(4);
	prg8_ab(5);
	prg8_cd(0);
	prg8_ef(7);
	chr8(0, CHRROM);

	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_delay = 0;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}

void nes_smb2jb_device::device_start() {
	common_start();

	irq_timer = timer_alloc(FUNC(nes_smb2jb_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_delay));
	save_item(NAME(m_reg));
}

void nes_smb2jb_device::pcb_reset() {
	prg8_89(0x08);
	prg8_ab(0x09);
	prg8_cd(0);
	prg8_ef(0x0b);

	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_delay = 0;
	m_reg = 0;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}

void nes_n32_4in1_device::device_start() {
	nes_smb2jb_device::device_start();

	save_item(NAME(m_smb2j_mode));
}

void nes_n32_4in1_device::pcb_reset() {
	// Powers up in menu, but soft reset does not touch banks so each
	// game returns to its own title.

	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_delay = 0;
	m_reg = 0;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}

void nes_0353_device::device_start()
{
	common_start();
	save_item(NAME(m_reg));
}

void nes_0353_device::pcb_reset()
{
	prg32((m_prg_chunks >> 1) - 1);    // fixed 32K bank

	m_reg = 0;
}

void nes_09034a_device::device_start() {
	common_start();

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	irq_timer = timer_alloc(FUNC(nes_09034a_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_enable));
	save_item(NAME(m_reg));
	save_item(NAME(m_irq_delay));
}

void nes_09034a_device::pcb_reset()
{
	prg32(0);
	chr8(0, m_chr_source);

	m_irq_enable = 0;
	m_irq_count = 0;
	m_reg = 0;
	m_irq_delay = 0;
}

void nes_l001_device::device_start()
{
	common_start();

	irq_timer = timer_alloc(FUNC(nes_l001_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_delay));
}

void nes_l001_device::pcb_reset()
{
	prg32((m_prg_chunks >> 1) - 1);
	chr8(0, CHRROM);

	m_irq_count = 0;
	m_irq_delay = 0;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}

void nes_palthena_device::device_start()
{
	common_start();
	save_item(NAME(m_reg));
}

void nes_palthena_device::pcb_reset()
{
	prg8_89(0x0c);
	// 0xa000-0xbfff switchable bank
	prg16_cdef(m_prg_chunks - 1);

	m_reg = 0;
}

void nes_tobidase_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_tobidase_device::pcb_reset()
{
	prg32(2);

	m_latch = 0;
}

void nes_whirlwind_device::device_start()
{
	common_start();
	save_item(NAME(m_reg));
}

void nes_whirlwind_device::pcb_reset()
{
	prg32((m_prg_chunks >> 1) - 1);      // upper PRG: banks are always fixed
	chr8(0, m_chr_source);

	m_reg = (m_prg_chunks << 1) - 1;     // lower PRG: ProWres needs this fixed, others modify it
}

void nes_lh32_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_lh32_device::pcb_reset()
{
	prg32((m_prg_chunks - 1) >> 1);
	// 0xc000-0xdfff reads/writes WRAM
	m_latch = 0xf;
}

void nes_lh42_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_lh42_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);    // Last 16K is fixed

	m_latch = 0;
}

void nes_lg25_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_lg25_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);    // Last 16K is fixed

	m_latch = 0;
}

void nes_lh10_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_lh10_device::pcb_reset()
{
	prg8_89(0);
	prg8_ab(0);
	// 0xc000-0xdfff reads/writes WRAM
	prg8_ef((m_prg_chunks << 1) - 1);

	m_latch = 0;
}

void nes_lh51_device::pcb_reset()
{
	prg32((m_prg_chunks >> 1) - 1);    // first 8K is switchable, the rest fixed
}

void nes_lh53_device::device_start()
{
	common_start();
	irq_timer = timer_alloc(FUNC(nes_lh53_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));
	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_irq_delay));
	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_reg));
}

void nes_lh53_device::pcb_reset()
{
	prg32(3);
	chr8(0, CHRRAM);

	m_reg = 0;
	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_delay = 0;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}

void nes_2708_device::device_start()
{
	common_start();
	save_item(NAME(m_reg));
}

void nes_2708_device::pcb_reset()
{
	prg32(7);
	// the upper PRG banks never change, but there are 8K of WRAM overlaid to the ROM area based on reg1
	m_reg[0] = 0;
	m_reg[1] = 0;
}

void nes_ac08_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_ac08_device::pcb_reset()
{
	prg32(0xff);
	set_nt_mirroring(PPU_MIRROR_VERT);

	m_latch = 0x0f;
}

void nes_rt01_device::pcb_reset()
{
	chr2_0(0, CHRROM);
	chr2_2(0, CHRROM);
	chr2_4(0, CHRROM);
	chr2_6(0, CHRROM);
	prg16_89ab(0);
	prg16_cdef(0);
}

void nes_yung08_device::device_start()
{
	common_start();

	irq_timer = timer_alloc(FUNC(nes_yung08_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_latch));
	save_item(NAME(m_irq_delay));
}

void nes_yung08_device::pcb_reset()
{
	prg8_89(1);
	prg8_ab(0);
	prg8_cd(0);
	prg8_ef(8);
	chr8(0, CHRROM);

	m_irq_count = 0;
	m_irq_latch = 0;
	m_irq_delay = 0;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
}


/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 BTL-MARIOBABY

 Game: Mario Baby

 iNES: mapper 42

 This FDS conversion provides a switchable 8KB PRG
 bank at $6000-$7FFF and a 15-bit CPU M2 IRQ counter.

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_mbaby_device::irq_timer_tick)
{
	if (m_irq_enable) {
		m_irq_count = (m_irq_count + 1) & 0x7fff;

		if (m_irq_count == 0x6000) {
			m_irq_delay = 2;
		}
		else if (!m_irq_count) {
			m_irq_delay = 0;
			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
		}
	}
}

void nes_mbaby_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address)
{
	if (m_irq_delay) {
		m_irq_delay--;

		if (!m_irq_delay) {
			m_maincpu6502->queue_delayed_mapper_irq(2);
		}
	}
}

void nes_mbaby_device::write_h(offs_t offset, u8 data)
{
	LOG("Mario Baby write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x6003) {
		case 0x6000:
			// $E000: 8 KiB PRG bank at CPU $6000-$7FFF.
			m_latch = data & 0x0f;
			break;

		case 0x6001:
			// $E001: nametable mirroring.
			set_nt_mirroring(BIT(data, 3) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			break;

		case 0x6002:
			// $E002: IRQ control. Bit 1 enables the 15-bit M2 counter.
			m_irq_enable = BIT(data, 1);

			if (!m_irq_enable) {
				m_irq_count = 0;
				m_irq_delay = 0;
				set_irq_line(CLEAR_LINE);
				m_maincpu6502->cancel_delayed_mapper_irq();
			}
			break;
	}
}

u8 nes_mbaby_device::read_m(offs_t offset)
{
	LOG("Mario Baby read_m, offset: %04x\n", offset);

	return m_prg[((m_latch & 0x0f) * 0x2000 + (offset & 0x1fff)) & (m_prg_size - 1)];
}

u8 nes_mbaby_device::read_ex(offs_t offset)
{
	LOG("Mario Baby read_ex, offset: %04x\n", offset);

	switch (offset) {
		case 0x10:
			// $4030: FDS IRQ status used by this FDS conversion's startup code.
			set_irq_line(CLEAR_LINE);
			return 0x01;
	}

	return get_open_bus();
}

void nes_mbaby_device::write_ex(offs_t offset, u8 data)
{
	LOG("Mario Baby write_ex, offset: %04x, data: %02x\n", offset, data);

	switch (offset) {
		case 0x00:
			// $4020: FDS IRQ latch low, ignored by this cartridge mapper.
			break;

		case 0x01:
			// $4021: FDS IRQ latch high, ignored by this cartridge mapper.
			break;

		case 0x03:
			// $4023: FDS IRQ/disk control, ignored.
			break;

		case 0x06:
			// $4026: FDS external connector, ignored.
			break;

		case 0x60:
			// $4080: FDS wavetable volume/envelope, ignored.
			break;

		case 0x6a:
			// $408A: FDS envelope speed, ignored.
			break;
	}
}

/*-------------------------------------------------

 BTL-AISENSHINICOL

 Game: Ai Senshi Nicol

 iNES: mapper 42

 This variant has switchable 8KB CHR-ROM and an
 8KB PRG-ROM bank at $6000-$7FFF. It does not
 provide the Mario Baby IRQ or mirroring controls.

 -------------------------------------------------*/

void nes_asn_device::write_h(offs_t offset, uint8_t data)
{
	LOG("Ai Senshi Nicol write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x6003) {
		case 0x0000:
			// $8000-$9FFF, mirrored every four bytes:
			// 8 KiB CHR-ROM bank at PPU $0000-$1FFF.
			chr8(data, m_chr_source);
			break;

		case 0x6000:
			// $E000-$FFFF, mirrored every four bytes:
			// 8 KiB PRG-ROM bank at CPU $6000-$7FFF.
			m_latch = data & 0x0f;
			break;
	}
}

uint8_t nes_asn_device::read_m(offs_t offset)
{
	LOG("Ai Senshi Nicol read_m, offset: %04x\n", offset);

	return m_prg[((m_latch & 0x0f) * 0x2000 + (offset & 0x1fff)) & (m_prg_size - 1)];
}

/*-------------------------------------------------

 BTL-SMB3

 Game: Super Mario Bros. 3 pirate

 iNES: mapper 106

 This discrete-logic board provides 8KB of PRG RAM,
 eight 1KB CHR banks, four 8KB PRG banks and a
 saturating 16-bit CPU M2 IRQ counter.

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_smb3p_device::irq_timer_tick) {
	// The counter runs even while interrupts are disabled and stops at $FFFF.
	if (m_irq_count != 0xffff) {
		m_irq_count++;

		if (m_irq_enable && m_irq_count == 0xffff) {
			m_irq_delay = 2;
		}
	}
}

void nes_smb3p_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) {
	if (m_irq_delay) {
		m_irq_delay--;

		if (!m_irq_delay) {
			m_maincpu6502->queue_delayed_mapper_irq(2);
		}
	}
}

void nes_smb3p_device::write_h(offs_t offset, u8 data) {
	LOG("btl_smb3_w, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x0f) {
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x03:
			chr1_x(offset & 0x07, (data & 0x7e) | BIT(offset, 0), CHRROM);
			break;

		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
			chr1_x(offset & 0x07, data & 0x7f, CHRROM);
			break;

		case 0x08:
		case 0x0b:
			prg8_x(offset & 0x03, (data | 0x10) & 0x1f);
			break;

		case 0x09:
		case 0x0a:
			prg8_x(offset & 0x03, data & 0x1f);
			break;

		case 0x0c:
			set_nt_mirroring(BIT(data, 0) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			break;

		case 0x0d:
			// Reset the counter, disable IRQs and acknowledge a pending IRQ.
			m_irq_count = 0;
			m_irq_enable = 0;
			m_irq_delay = 0;

			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			break;

		case 0x0e:
			// Low byte of the running counter.
			m_irq_count = (m_irq_count & 0xff00) | data;

			if (m_irq_enable && m_irq_count == 0xffff) {
				m_irq_delay = 2;
			}
			break;

		case 0x0f:
			// High byte of the running counter; writing enables IRQs.
			m_irq_count = (m_irq_count & 0x00ff) | (data << 8);
			m_irq_enable = 1;

			if (m_irq_count == 0xffff) {
				m_irq_delay = 2;
			}
			break;
	}
}

/*-------------------------------------------------

 BTL-CONTRAJ

 Game: Contra (Japan) pirate

 NES 2.0: mapper 326

 This board provides switchable 8KB PRG-ROM banks
 at $8000-$9FFF, $A000-$BFFF and $C000-$DFFF.
 The final 8KB bank at $E000-$FFFF is fixed.

 It also provides eight switchable 1KB CHR-ROM
 banks and independently selectable CIRAM pages.

 The board has no PRG RAM, leaving $6000-$7FFF
 undriven as CPU open bus.

 -------------------------------------------------*/

void nes_btl_cj_device::write_h(offs_t offset, u8 data) {
	LOG("btl_cj write_h, offset: %04x, data: %02x\n", offset, data);

	if (BIT(offset, 4)) {
		if (BIT(offset, 3)) {
			set_nt_page(offset & 0x03, CIRAM, data & 0x01, 1);
		}
		else {
			chr1_x(offset & 0x07, data & 0x7f, CHRROM);
		}
	}
	else {
		offset = BIT(offset, 13, 2);

		if (offset != 3) {
			prg8_x(offset, data & 0x0f);
		}
	}
}

/*-------------------------------------------------

 BTL-DRAGONNINJA

 Games: Dragon Ninja pirate, Super Mario Bros. 8

 iNES: mapper 222

 This board provides two switchable 8KB PRG-ROM
 banks, eight switchable 1KB CHR-ROM banks,
 mapper-controlled mirroring and a PPU A12 IRQ
 counter with a terminal count of 240.

 The board has no PRG RAM, leaving $6000-$7FFF
 undriven as CPU open bus.

 -------------------------------------------------*/

void nes_btl_dn_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) {
	if (m_irq_delay) {
		m_irq_delay--;

		if (!m_irq_delay) {
			m_maincpu6502->queue_delayed_mapper_irq(2);
		}
	}

	if (!BIT(ppu_address, 12)) {
		if (m_a12_low_ticks != 0xff) {
			m_a12_low_ticks++;
		}

		return;
	}

	if (m_a12_low_ticks > 10) {
		if (m_irq_count) {
			m_irq_count++;

			if (m_irq_count >= 240) {
				m_irq_count = 0;
				m_irq_delay = 2;
			}
		}
	}

	m_a12_low_ticks = 0;
}

void nes_btl_dn_device::write_h(offs_t offset, uint8_t data) {
	LOG("btl_dn write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x7003) {
		case 0x0000:
			prg8_89(data);
			break;

		case 0x1000:
			set_nt_mirroring(BIT(data, 0) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			break;

		case 0x2000:
			prg8_ab(data);
			break;

		case 0x3000:
		case 0x3002:
		case 0x4000:
		case 0x4002:
		case 0x5000:
		case 0x5002:
		case 0x6000:
		case 0x6002:
			chr1_x(2 * (BIT(offset, 12, 3) - 3) + BIT(offset, 1), data, CHRROM);
			break;

		case 0x7000:
			m_irq_count = data;
			m_irq_delay = 0;
			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			break;
	}
}

/*-------------------------------------------------

 BOOTLEG FC VERSIONS OF FDS GAMES

 -------------------------------------------------*/

/*-------------------------------------------------

 Whirlwind Manu FDS conversion boards

 Boards: DH-08, LE05, LH28, LH31 and LH54

 Games: Bubble Bobble alt 1, ProWres,
        Meikyuu Jiin Dababa, Bubble Bobble alt 2
        and Falsion

 iNES: mapper 108
 NES 2.0 submappers 1-4

 All variants map a selectable 8KB PRG-ROM bank at
 $6000-$7FFF and use hard-wired nametable mirroring.
 They differ in CHR memory, banking behavior and
 register decoding.

 -------------------------------------------------*/

u8 nes_whirlwind_device::read_m(offs_t offset) {
	LOG("whirlwind read_m, offset: %04x\n", offset);

	return m_prg[((m_reg & m_prg_mask) * 0x2000) + offset];
}

void nes_dh08_device::write_h(offs_t offset, u8 data) {
	LOG("dh08 write_h, offset: %04x, data: %02x\n", offset, data);

	// Submapper 1: PRG register at $F000-$FFFF.
	if (offset >= 0x7000) {
		m_reg = data;
	}
}

void nes_le05_device::write_h(offs_t offset, u8 data) {
	LOG("le05 write_h, offset: %04x, data: %02x\n", offset, data);

	// Submapper 4: CHR register throughout $8000-$FFFF.
	chr8(data & 0x01, m_chr_source);
}

void nes_lh28_lh54_device::write_h(offs_t offset, u8 data) {
	LOG("lh28_lh54 write_h, offset: %04x, data: %02x\n", offset, data);

	// Submapper 3: PRG register throughout $8000-$FFFF.
	m_reg = data;
}

void nes_lh31_device::write_h(offs_t offset, u8 data) {
	LOG("lh31 write_h, offset: %04x, data: %02x\n", offset, data);

	// Submapper 2: combined PRG and CHR register at $E000-$FFFF.
	if (offset >= 0x6000) {
		m_reg = data;
		chr8(data, m_chr_source);
	}
}

/*-------------------------------------------------

 UNL-SMB2J

 Games: Super Mario Bros. 2 pirates
        (TONY-I and YS-612)

 iNES: mapper 43

 These FDS conversions map PRG ROM into $5000-$7FFF
 and provide a switchable 8 KiB PRG bank at
 $C000-$DFFF.

 The IRQ uses a 12-bit CPU M2 counter. TONY-I
 controls it through $4122, while YS-612 also
 decodes $8122.

 In MAME: Supported.

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_smb2j_device::irq_timer_tick) {
	if (m_irq_enable) {
		m_irq_count = (m_irq_count + 1) & 0x0fff;

		if (!m_irq_count) {
			m_irq_delay = 2;
		}
	}
}

void nes_smb2j_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) {
	if (m_irq_delay) {
		m_irq_delay--;

		if (!m_irq_delay) {
			m_maincpu6502->queue_delayed_mapper_irq(2);
		}
	}
}

void nes_smb2j_device::update_irq(u8 data) {
	m_irq_enable = BIT(data, 0);

	if (!m_irq_enable) {
		m_irq_count = 0;
		m_irq_delay = 0;

		set_irq_line(CLEAR_LINE);
		m_maincpu6502->cancel_delayed_mapper_irq();
	}
}

void nes_smb2j_device::write_45(offs_t offset, u8 data) {
	static constexpr u8 bank[8] = { 4, 3, 4, 4, 4, 7, 5, 6 };

	switch (offset & 0x71ff) {
		case 0x4022:
			prg8_cd(bank[data & 0x07]);
			break;

		case 0x4122:
			update_irq(data);
			break;
	}
}

void nes_smb2j_device::write_ex(offs_t offset, u8 data) {
	LOG("smb2j write_ex, offset: %04x, data: %02x\n", offset, data);
	write_45(offset + 0x4020, data);
}

void nes_smb2j_device::write_l(offs_t offset, u8 data) {
	LOG("smb2j write_l, offset: %04x, data: %02x\n", offset, data);
	write_45(offset + 0x4100, data);
}

void nes_smb2j_device::write_h(offs_t offset, u8 data) {
	LOG("smb2j write_h, offset: %04x, data: %02x\n", offset, data);

	// $8122 is also an IRQ-control register on YS-612.
	// The complete address-decode mask is unknown.
	if (offset == 0x0122) {
		update_irq(data);
	}
}

u8 nes_smb2j_device::read_l(offs_t offset) {
	LOG("smb2j read_l, offset: %04x\n", offset);

	offset += 0x0100;

	if (offset >= 0x1000) {
		return m_prg[0x10000 + (offset & 0x0fff)];
	}

	return get_open_bus();
}

u8 nes_smb2j_device::read_m(offs_t offset) {
	LOG("smb2j read_m, offset: %04x\n", offset);
	return m_prg[0x4000 + offset];
}

/*-------------------------------------------------

 BTL-SMB2JA

 Games: Super Mario Bros. 2 pirates
        (LF36, Hey Sung and 1990 SMB4)

 iNES: mapper 40

 These FDS conversions map a fixed 8 KiB PRG-ROM
 bank at $6000-$7FFF and a switchable 8 KiB bank
 at $C000-$DFFF.

 The IRQ counter is clocked by CPU M2. It asserts
 after 4096 cycles and automatically clears after
 another 4096 cycles if it is not acknowledged.

 Writes to $8000-$9FFF disable and acknowledge the
 IRQ. Writes to $A000-$BFFF restart and enable the
 counter.

 In MAME: Supported.

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_smb2ja_device::irq_timer_tick) {
	if (m_irq_enable) {
		m_irq_count = (m_irq_count + 1) & 0x1fff;

		if (m_irq_count == 0x1000) {
			m_irq_delay = 2;
		}
		else if (!m_irq_count) {
			m_irq_delay = 0;

			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
		}
	}
}

void nes_smb2ja_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) {
	if (m_irq_delay) {
		m_irq_delay--;

		if (!m_irq_delay) {
			m_maincpu6502->queue_delayed_mapper_irq(2);
		}
	}
}

void nes_smb2ja_device::write_h(offs_t offset, u8 data) {
	LOG("smb2ja write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x6000) {
		case 0x0000:
			// $8000-$9FFF: disable, stop and acknowledge the IRQ.
			m_irq_enable = 0;
			m_irq_count = 0;
			m_irq_delay = 0;

			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			break;

		case 0x2000:
			// $A000-$BFFF: restart and enable the M2 counter.
			m_irq_count = 0;
			m_irq_enable = 1;
			break;

		case 0x6000:
			// $E000-$FFFF: select the PRG bank at $C000-$DFFF.
			prg8_cd(data & 0x07);
			break;
	}
}

u8 nes_smb2ja_device::read_m(offs_t offset) {
	LOG("smb2ja read_m, offset: %04x\n", offset);
	return m_prg[(0x06 * 0x2000) + offset];
}

/*-------------------------------------------------

 BTL-SMB2JB

 Games: Super Mario Bros. 2 pirate
        (N-32)

 PCB: 761214

 iNES: mapper 50

 This FDS conversion maps fixed PRG-ROM banks at
 $6000-$BFFF and $E000-$FFFF. A scrambled register
 selects the 8 KiB bank at $C000-$DFFF.

 The one-shot IRQ counter is clocked by CPU M2.
 It asserts after 4096 cycles and then stops.
 Writing zero to the IRQ-control register disables
 the counter, resets it and acknowledges the IRQ.

 In MAME: Supported.

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_smb2jb_device::irq_timer_tick) {
	if (m_irq_enable) {
		m_irq_count++;

		if (m_irq_count == 0x1000) {
			m_irq_enable = 0;
			m_irq_delay = 2;
		}
	}
}

void nes_smb2jb_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) {
	if (m_irq_delay) {
		m_irq_delay--;

		if (!m_irq_delay) {
			m_maincpu6502->queue_delayed_mapper_irq(2);
		}
	}
}

void nes_smb2jb_device::write_45(offs_t offset, u8 data) {
	switch (offset & 0x4120) {
		case 0x4020:
			// Select the 8 KiB PRG-ROM bank at $C000-$DFFF.
			m_reg = bitswap<4>(data, 3, 0, 2, 1);
			prg8_cd(m_reg);
			break;

		case 0x4120:
			// Bit 0 enables the one-shot CPU M2 IRQ counter.
			m_irq_enable = BIT(data, 0);

			if (!m_irq_enable) {
				m_irq_count = 0;
				m_irq_delay = 0;

				set_irq_line(CLEAR_LINE);
				m_maincpu6502->cancel_delayed_mapper_irq();
			}
			break;
	}
}

void nes_smb2jb_device::write_ex(offs_t offset, u8 data) {
	LOG("smb2jb write_ex, offset: %04x, data: %02x\n", offset, data);
	write_45(offset + 0x4020, data);
}

void nes_smb2jb_device::write_l(offs_t offset, u8 data) {
	LOG("smb2jb write_l, offset: %04x, data: %02x\n", offset, data);
	write_45(offset + 0x4100, data);
}

u8 nes_smb2jb_device::read_m(offs_t offset) {
	LOG("smb2jb read_m, offset: %04x\n", offset);
	return m_prg[((m_bank67 * 0x2000) + offset) & (m_prg_size - 1)];
}

/*-------------------------------------------------

 BMC-N32-4IN1

 Games: 4-in-1 multicart

 NES 2.0: mapper 416

 This multicart combines an N-32-compatible mapper
 50 mode for Super Mario Bros. 2 with conventional
 8, 16 and 32 KiB multicart PRG banking.

 In N-32 mode, the mapper 50 banking registers and
 one-shot CPU M2 IRQ counter are used. The fixed
 PRG-ROM bank at $6000-$7FFF differs from the
 standalone 761214 board.

 In MAME: Supported.

 -------------------------------------------------*/

void nes_n32_4in1_device::write_h(offs_t offset, u8 data) {
	LOG("n32_4in1 write_h, offset: %04x, data: %02x\n", offset, data);

	// The outer multicart register is decoded at $8000-$9FFF.
	if (offset >= 0x2000) {
		return;
	}

	m_smb2j_mode = !BIT(data, 3);

	if (m_smb2j_mode) {
		// N-32-compatible mapper 50 mode.
		prg8_89(0);
		prg8_ab(1);
		prg8_cd(m_reg);
		prg8_ef(3);
	}
	else {
		const u8 bank = bitswap<3>(data, 3, 7, 5);

		if (BIT(data, 7)) {
			// 32 KiB PRG-ROM mode.
			prg16_89ab(bank & ~1);
			prg16_cdef(bank | 1);
		}
		else if (BIT(data, 6)) {
			// 16 KiB PRG-ROM bank mirrored in both CPU windows.
			prg16_89ab(bank);
			prg16_cdef(bank);
		}
		else {
			// 8 KiB PRG-ROM bank mirrored across $8000-$FFFF.
			for (int i = 0; i < 4; i++) {
				prg8_x(i, bank << 1);
			}
		}
	}

	chr8(BIT(data, 1, 2), CHRROM);
	set_nt_mirroring(BIT(data, 2) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
}

void nes_n32_4in1_device::write_ex(offs_t offset, u8 data) {
	LOG("n32_4in1 write_ex, offset: %04x, data: %02x\n", offset, data);

	if (m_smb2j_mode) {
		write_45(offset + 0x4020, data);
	}
}

void nes_n32_4in1_device::write_l(offs_t offset, u8 data) {
	LOG("n32_4in1 write_l, offset: %04x, data: %02x\n", offset, data);

	if (m_smb2j_mode) {
		write_45(offset + 0x4100, data);
	}
}

/*-------------------------------------------------

 BTL-0353

 Games: Lucky Rabbit

 NES 2.0: mapper 415

 This board converts the FDS game Roger Rabbit,
 renamed Lucky Rabbit, to cartridge format.

 Writes throughout $8000-$FFFF select the 8 KiB
 PRG-ROM bank mapped at $6000-$7FFF and control
 horizontal or vertical nametable mirroring.
 PRG-ROM at $8000-$FFFF and CHR-ROM are fixed.

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_0353_device::read_m(offs_t offset) {
	LOGMASKED(LOG_HIFREQ, "0353 read_m, offset: %04x\n", offset);
	return m_prg[(m_reg * 0x2000) + offset];
}

void nes_0353_device::write_h(offs_t offset, u8 data) {
	LOG("0353 write_h, offset: %04x, data: %02x\n", offset, data);

	// D0-D3 select the 8 KiB PRG-ROM bank at $6000-$7FFF.
	m_reg = data & 0x0f;

	// D4 controls nametable mirroring.
	set_nt_mirroring(BIT(data, 4) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
}

/*-------------------------------------------------

 (UNL-)09-034A

 Games: Zanac, Super Mario Bros. 2 and Volleyball
        FDS conversions

 NES 2.0: mapper 304

 These boards use two PRG-ROM chips and map the
 second chip into $6000-$7FFF. Zanac uses CHR-RAM,
 while the Super Mario Bros. 2 and Volleyball
 conversions use CHR-ROM.

 Only Super Mario Bros. 2 uses the IRQ. The IRQ
 circuit is not fully documented, so it is currently
 modeled as a 12-bit CPU M2 counter like several
 other Super Mario Bros. 2 conversion boards.

 Reads from $4042-$4055 return $FF. These are
 presently unidentified hardware reads, but treating
 them as open bus prevents Super Mario Bros. 2 from
 booting.

 In MAME: Supported.

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_09034a_device::irq_timer_tick) {
	if (m_irq_enable) {
		m_irq_count = (m_irq_count + 1) & 0x0fff;

		if (!m_irq_count) {
			m_irq_delay = 2;
		}
	}
}

void nes_09034a_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) {
	if (m_irq_delay) {
		m_irq_delay--;

		if (!m_irq_delay) {
			m_maincpu6502->queue_delayed_mapper_irq(2);
		}
	}
}

void nes_09034a_device::write_ex(offs_t offset, u8 data) {
	const offs_t address = offset + 0x4020;

	LOG("09-034a write_ex, address: %04x, data: %02x\n", address, data);

	switch (address) {
		case 0x4027:
			// Select the 8 KiB PRG-ROM bank at $6000-$7FFF.
			m_reg = data & 0x01;
			break;

		case 0x4068:
			// Bit 0 enables the presumed 12-bit CPU M2 IRQ counter.
			m_irq_enable = BIT(data, 0);

			if (!m_irq_enable) {
				m_irq_count = 0;
				m_irq_delay = 0;

				set_irq_line(CLEAR_LINE);
				m_maincpu6502->cancel_delayed_mapper_irq();
			}
			break;
	}
}

u8 nes_09034a_device::read_ex(offs_t offset) {
	const offs_t address = offset + 0x4020;

	LOGMASKED(LOG_HIFREQ, "09-034a read_ex, address: %04x\n", address);

	// These unidentified reads are driven high rather than left open.
	if (address >= 0x4042 && address <= 0x4055) {
		return 0xff;
	}

	return get_open_bus();
}

u8 nes_09034a_device::read_m(offs_t offset) {
	LOGMASKED(LOG_HIFREQ, "09-034a read_m, offset: %04x\n", offset);

	// The second PRG-ROM chip begins after the first four 8 KiB banks.
	// Smaller PRG-ROM images wrap through the size mask.
	return m_prg[(((m_reg + 4) * 0x2000) + offset) & (m_prg_size - 1)];
}

/*-------------------------------------------------

 Board L-001

 Games: Sangokushi III
        (bootleg of Sangokushi II: Haō no Tairiku)

 NES 2.0: mapper 330

 This board implements banking similar to the Namco
 163 board used by the original game, but does not
 retain its expansion audio.

 It provides three switchable 8 KiB PRG-ROM banks
 at $8000-$DFFF and a fixed final bank at
 $E000-$FFFF. CHR-ROM and CIRAM are independently
 selectable in 1 KiB pages.

 The IRQ uses a 15-bit CPU M2 counter with its
 enable flag stored in bit 15. The counter stops
 when it overflows and generates the IRQ.

 In MAME: Supported.

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_l001_device::irq_timer_tick) {
	if (BIT(m_irq_count, 15)) {
		m_irq_count++;

		if (!m_irq_count) {
			m_irq_delay = 2;
		}
	}
}

void nes_l001_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) {
	if (m_irq_delay) {
		m_irq_delay--;

		if (!m_irq_delay) {
			m_maincpu6502->queue_delayed_mapper_irq(2);
		}
	}
}

void nes_l001_device::write_h(offs_t offset, u8 data) {
	LOG("l-001 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x6400) {
		case 0x0000:
		case 0x2000:
			// $8000-$BFFF: eight 1 KiB CHR-ROM bank registers.
			chr1_x(BIT(offset, 11, 3), data, CHRROM);
			break;

		case 0x0400:
			// $8400: IRQ counter low byte.
			m_irq_count = (m_irq_count & 0xff00) | data;
			break;

		case 0x2400:
			// $A400: IRQ counter high byte and enable flag.
			m_irq_count = (m_irq_count & 0x00ff) | (data << 8);
			m_irq_delay = 0;

			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			break;

		case 0x4000:
			// $C000-$DFFF: four 1 KiB CIRAM page registers.
			set_nt_page(BIT(offset, 11, 2), CIRAM, data & 0x01, 1);
			break;

		case 0x6000:
			// $E000-$F7FF: three switchable 8 KiB PRG-ROM banks.
			// $F800-$FFFF is ignored, leaving the final bank fixed.
			if (offset < 0x7800) {
				prg8_x(BIT(offset, 11, 2), data & 0x1f);
			}
			break;
	}
}

/*-------------------------------------------------

 BTL-BATMANFS

 Games: Batman "Fine Studio" pirate
        RoboCop pirate

 NES 2.0: mapper 417

 Submapper 0 is the Fine Studio copy of Batman. It
 uses a continuously running 10-bit CPU M2 counter
 and four dedicated CIRAM banking registers.

 Submapper 1 is RoboCop. It uses a continuously
 running 12-bit counter, and bit 7 of the first four
 CHR-ROM registers also selects the CIRAM pages.

 Both variants provide three switchable 8 KiB
 PRG-ROM banks, eight switchable 1 KiB CHR-ROM
 banks and a fixed final 8 KiB PRG-ROM bank.

 In MAME: Supported.

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_batmanfs_device::irq_timer_tick) {
	// The counter runs continuously, even while IRQs are disabled.
	const u16 counter_mask = get_submapper() == 1 ? 0x0fff : 0x03ff;

	m_irq_count = (m_irq_count + 1) & counter_mask;

	if (m_irq_enable && !m_irq_count) {
		m_irq_delay = 2;
	}
}

void nes_batmanfs_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) {
	if (m_irq_delay) {
		m_irq_delay--;

		if (!m_irq_delay) {
			m_maincpu6502->queue_delayed_mapper_irq(2);
		}
	}
}

void nes_batmanfs_device::write_h(offs_t offset, u8 data) {
	LOG("batmanfs write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x70) {
		case 0x00:
			// $8000-$8002: three switchable 8 KiB PRG-ROM banks.
			// $8003 is ignored, leaving the final bank fixed.
			if ((offset & 0x03) != 0x03) {
				prg8_x(offset & 0x03, data & 0x0f);
			}
			break;

		case 0x10:
			// $8010-$8013: lower four 1 KiB CHR-ROM banks.
			chr1_x(offset & 0x03, data, CHRROM);

			// On RoboCop, D7 also selects the corresponding CIRAM page.
			if (get_submapper() == 1) {
				set_nt_page(offset & 0x03, CIRAM, BIT(data, 7), 1);
			}
			break;

		case 0x20:
			// $8020-$8023: upper four 1 KiB CHR-ROM banks.
			chr1_x((offset & 0x03) + 4, data, CHRROM);
			break;

		case 0x30:
			// $8030: reset the counter and enable IRQs.
			m_irq_enable = 1;
			m_irq_count = 0;
			m_irq_delay = 0;

			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			break;

		case 0x40:
			// $8040: disable and acknowledge IRQs.
			// The counter continues running.
			m_irq_enable = 0;
			m_irq_delay = 0;

			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			break;

		case 0x50:
			// $8050-$8053: dedicated CIRAM banking on Batman only.
			if (get_submapper() == 0) {
				set_nt_page(offset & 0x03, CIRAM, data & 0x01, 1);
			}
			break;
	}
}

/*-------------------------------------------------

 BTL-PALTHENA

 Games: Palthena no Kagami
        (Kid Icarus FDS conversion)

 NES 2.0: mapper 539

 This board maps fixed 8 KiB PRG-ROM banks at
 $6000-$9FFF and $C000-$FFFF. The 8 KiB PRG-ROM
 bank at $A000-$BFFF is switchable.

 The board contains 8 KiB of non-battery-backed
 PRG-RAM, but only 6 KiB is exposed through several
 overlays in the CPU address space:

 PRG-RAM           CPU address
 $0000-$11FF       $C000-$D1FF
 $1200-$12FF       $8200-$82FF
 $1300-$17FF       not mapped
 $1800-$18FF       $6000-$60FF
 $1900-$19FF       not mapped
 $1A00-$1AFF       $6200-$62FF
 $1B00-$1BFF       not mapped
 $1C00-$1DFF       $6400-$65FF
 $1E00-$1EFF       not mapped
 $1F00-$1FFF       $DF00-$DFFF

 Addresses not covered by these RAM overlays expose
 the underlying PRG ROM.

 The original FDS sound-register writes are retained
 by the software, although the cartridge does not
 contain FDS expansion audio hardware.

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_palthena_device::read_m(offs_t offset) {
	LOGMASKED(LOG_HIFREQ, "palthena read_m, offset: %04x\n", offset);

	switch (offset & 0x1f00) {
		case 0x0000:
			// $6000-$60FF maps PRG-RAM $1800-$18FF.
			return m_prgram[offset | 0x1800];

		case 0x0200:
			// $6200-$62FF maps PRG-RAM $1A00-$1AFF.
			return m_prgram[offset | 0x1800];

		case 0x0400:
		case 0x0500:
			// $6400-$65FF maps PRG-RAM $1C00-$1DFF.
			return m_prgram[offset | 0x1800];

		default:
			// Remaining addresses expose fixed PRG-ROM bank $0D.
			return m_prg[(0x0d * 0x2000) + offset];
	}
}

void nes_palthena_device::write_m(offs_t offset, u8 data) {
	LOG("palthena write_m, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x1f00) {
		case 0x0000:
		case 0x0200:
		case 0x0400:
		case 0x0500:
			m_prgram[offset | 0x1800] = data;
			break;
	}
}

u8 nes_palthena_device::read_h(offs_t offset) {
	LOGMASKED(LOG_HIFREQ, "palthena read_h, offset: %04x\n", offset);

	const u8 page = offset >> 8;

	if ((page >= 0x40 && page <= 0x51) || page == 0x5f) {
		// $C000-$D1FF maps RAM $0000-$11FF.
		// $DF00-$DFFF maps RAM $1F00-$1FFF.
		return m_prgram[offset & 0x1fff];
	}

	if (page == 0x02) {
		// $8200-$82FF maps RAM $1200-$12FF.
		return m_prgram[offset | 0x1000];
	}

	return hi_access_rom(offset);
}

void nes_palthena_device::write_h(offs_t offset, u8 data) {
	LOG("palthena write_h, offset: %04x, data: %02x\n", offset, data);

	const u8 page = offset >> 8;

	if ((page >= 0x40 && page <= 0x51) || page == 0x5f) {
		m_prgram[offset & 0x1fff] = data;
	}
	else if (page == 0x02) {
		m_prgram[offset | 0x1000] = data;
	}
	else if (page >= 0x20 && page <= 0x3f) {
		// $A000-$BFFF selects the switchable 8 KiB PRG-ROM bank.
		prg8_ab(data & 0x0f);
	}
	else if (offset == 0x7fff) {
		// $FFFF selects nametable mirroring.
		set_nt_mirroring(BIT(data, 3) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
	}
}

/*-------------------------------------------------

 BTL-TOBIDASE

 Games: Tobidase Daisakusen (FDS conversion)

 iNES: mapper 120

 This board maps a switchable 8 KiB PRG-ROM bank at
 $6000-$7FFF and hard-wires 32 KiB PRG-ROM bank 2 at
 $8000-$FFFF. The write-only bank register is mirrored
 through $4100-$5FFF using address mask $E100.

 CHR is an unbanked 8 KiB region.

 In MAME: Supported.

 -------------------------------------------------*/

void nes_tobidase_device::write_l(offs_t offset, u8 data) {
	const offs_t address = offset + 0x4100;

	LOG("tobidase write_l, address: %04x, data: %02x\n", address, data);

	if ((address & 0xe100) == 0x4100) {
		m_latch = data & 0x07;
	}
}

u8 nes_tobidase_device::read_m(offs_t offset) {
	LOGMASKED(LOG_HIFREQ, "tobidase read_m, offset: %04x\n", offset);

	return m_prg[(m_latch * 0x2000) + offset];
}

/*-------------------------------------------------

 UNL-LH32

 Games: Monty no Doki Doki Daisassou
        (FDS conversion)

 iNES: mapper 125

 This board maps a switchable 8 KiB PRG-ROM bank at
 $6000-$7FFF. Writes throughout that range select the
 bank using data bits 0-3.

 PRG-ROM banks 12 and 13 are fixed at $8000-$BFFF,
 8 KiB of PRG-RAM is mapped at $C000-$DFFF, and
 PRG-ROM bank 15 is fixed at $E000-$FFFF.

 CHR is an unbanked 8 KiB CHR-RAM region, and
 nametable mirroring is hard-wired vertically.

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_lh32_device::read_m(offs_t offset) {
	LOGMASKED(LOG_HIFREQ, "lh32 read_m, offset: %04x\n", offset);

	return m_prg[(m_latch * 0x2000) + offset];
}

u8 nes_lh32_device::read_h(offs_t offset) {
	LOGMASKED(LOG_HIFREQ, "lh32 read_h, offset: %04x\n", offset);

	if (offset >= 0x4000 && offset < 0x6000) {
		return m_prgram[offset & 0x1fff];
	}

	return hi_access_rom(offset);
}

void nes_lh32_device::write_m(offs_t offset, u8 data) {
	LOG("lh32 write_m, offset: %04x, data: %02x\n", offset, data);

	// The documented address mask is probably $E000, making the
	// bank register writable throughout CPU $6000-$7FFF.
	m_latch = data & 0x0f;
}

void nes_lh32_device::write_h(offs_t offset, u8 data) {
	LOG("lh32 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset >= 0x4000 && offset < 0x6000) {
		m_prgram[offset & 0x1fff] = data;
	}
}

/*-------------------------------------------------

 UNL-LH42

 Games: Highway Star (Whirlwind Manu bootleg)

 NES 2.0: mapper 418

 This board has switchable 8 KiB PRG-ROM banks at
 $8000-$9FFF and $A000-$BFFF. The final 16 KiB of
 PRG-ROM is fixed at $C000-$FFFF.

 The command and data registers are mirrored through
 $8000-$9FFF. Command 5 controls nametable mirroring,
 while commands 6 and 7 select the two PRG-ROM banks.

 CHR is an unbanked 8 KiB CHR-RAM region.

 In MAME: Supported.

 -------------------------------------------------*/

void nes_lh42_device::write_h(offs_t offset, u8 data) {
	LOG("lh42 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset >= 0x2000) {
		return;
	}

	if (!BIT(offset, 0)) {
		m_latch = data & 0x07;
		return;
	}

	switch (m_latch) {
		case 5:
			set_nt_mirroring(BIT(data, 0) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			break;

		case 6:
			prg8_89(data & 0x0f);
			break;

		case 7:
			prg8_ab(data & 0x0f);
			break;
	}
}

/*-------------------------------------------------

 UNL-LG25

 Games: Moero TwinBee: Cinnamon-hakase o Sukue!
        (FDS conversion)

 NES 2.0: mapper 557

 This board maps 8 KiB of WRAM at $6000-$7FFF and
 has switchable 8 KiB PRG-ROM banks at $8000-$9FFF
 and $A000-$BFFF. The final 16 KiB of PRG-ROM is
 fixed at $C000-$FFFF.

 The command and data registers are mirrored through
 $8000-$9FFF. Command 5 controls nametable mirroring,
 while commands 6 and 7 select the two PRG-ROM banks.

 CHR is an unbanked 8 KiB CHR-RAM region.

 In MAME: Supported.

 -------------------------------------------------*/

void nes_lg25_device::write_h(offs_t offset, u8 data) {
	LOG("lg25 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset >= 0x2000) {
		return;
	}

	if (!BIT(offset, 0)) {
		m_latch = data & 0x07;
		return;
	}

	switch (m_latch) {
		case 5:
			set_nt_mirroring(BIT(data, 5) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			break;

		case 6:
			prg8_89(data & 0x0f);
			break;

		case 7:
			prg8_ab(data & 0x0f);
			break;
	}
}

/*-------------------------------------------------

 UNL-LH10

 Games: Fuuun Shourinken (FDS conversion)

 NES 2.0: mapper 522

 This board maps the second-to-last 8 KiB PRG-ROM
 bank at $6000-$7FFF. It has switchable 8 KiB
 PRG-ROM banks at $8000-$9FFF and $A000-$BFFF,
 8 KiB of PRG-RAM at $C000-$DFFF, and the final
 PRG-ROM bank fixed at $E000-$FFFF.

 The command and data registers are mirrored through
 $8000-$9FFF. Commands 0-5 have no function, while
 commands 6 and 7 select the two switchable PRG-ROM
 banks.

 CHR is an unbanked 8 KiB CHR-RAM region, and
 nametable mirroring is hard-wired vertically.

 This is similar to KS7037 but uses a restricted
 MMC3-style command and data register arrangement.

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_lh10_device::read_m(offs_t offset) {
	LOGMASKED(LOG_HIFREQ, "lh10 read_m, offset: %04x\n", offset);

	return m_prg[m_prg_size - 0x4000 + offset];
}

u8 nes_lh10_device::read_h(offs_t offset) {
	LOGMASKED(LOG_HIFREQ, "lh10 read_h, offset: %04x\n", offset);

	if (offset >= 0x4000 && offset < 0x6000) {
		return m_prgram[offset & 0x1fff];
	}

	return hi_access_rom(offset);
}

void nes_lh10_device::write_h(offs_t offset, u8 data) {
	LOG("lh10 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset < 0x2000) {
		if (!BIT(offset, 0)) {
			m_latch = data & 0x07;
			return;
		}

		switch (m_latch) {
			case 6:
				prg8_89(data & 0x0f);
				break;

			case 7:
				prg8_ab(data & 0x0f);
				break;
		}

		return;
	}

	if (offset >= 0x4000 && offset < 0x6000) {
		m_prgram[offset & 0x1fff] = data;
	}
}

/*-------------------------------------------------

 UNL-LH51

 Games: Ai Senshi Nicol
        (Whirlwind Manu FDS conversion)

 NES 2.0: mapper 309

 This board maps 8 KiB of WRAM at $6000-$7FFF and
 a switchable 8 KiB PRG-ROM bank at $8000-$9FFF.
 PRG-ROM banks 13, 14, and 15 are fixed at
 $A000-$FFFF.

 Writes throughout $8000-$9FFF select the switchable
 PRG-ROM bank. Writes throughout $E000-$FFFF select
 vertical or horizontal nametable mirroring.

 CHR is an unbanked 8 KiB CHR-RAM region.

 This conversion's sound routine is broken and does
 not work correctly on real hardware. Although the
 original FDS sound register writes remain in the
 program, the cartridge contains no FDS sound hardware.

 In MAME: Supported.

 -------------------------------------------------*/

void nes_lh51_device::write_h(offs_t offset, u8 data) {
	LOG("lh51 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset < 0x2000) {
		prg8_89(data & 0x0f);
	}
	else if (offset >= 0x6000) {
		set_nt_mirroring(BIT(data, 3) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
	}
}

/*-------------------------------------------------

 UNL-LH53

 Games: Nazo no Murasamejou (FDS conversion)

 NES 2.0: mapper 535

 This board maps a switchable 8 KiB PRG-ROM bank at
 $6000-$7FFF.

 The final 32 KiB of PRG-ROM is mapped at $8000-$FFFF,
 with 8 KiB of PRG-RAM overlaid at $B800-$D7FF.

 Writes throughout $E000-$EFFF control, reset and
 acknowledge a fixed-period M2-cycle IRQ counter.
 Writes throughout $F000-$FFFF select the switchable
 PRG-ROM bank.

 The IRQ period of 7561 M2 cycles comes from FCEUX and
 has not been confirmed on hardware.

 CHR is an unbanked 8 KiB CHR-RAM region. The PRG-RAM
 is not battery-backed, so game progress cannot be saved.

 In MAME: Supported.

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_lh53_device::irq_timer_tick)
{
	if (!m_irq_enable) {
		return;
	}

	m_irq_count++;

	if (m_irq_count == 7561) {
		m_irq_delay = 2;
	}
}

void nes_lh53_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address)
{
	if (!m_irq_delay) {
		return;
	}

	m_irq_delay--;

	if (!m_irq_delay) {
		m_maincpu6502->queue_delayed_mapper_irq(2);
	}
}

uint8_t nes_lh53_device::read_m(offs_t offset)
{
	LOG("lh53 read_m, offset: %04x\n", offset);

	return m_prg[((m_reg * 0x2000) + offset) & (m_prg_size - 1)];
}

uint8_t nes_lh53_device::read_h(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "lh53 read_h, offset: %04x\n", offset);

	if (offset >= 0x3800 && offset < 0x5800) {
		return m_prgram[offset - 0x3800];
	}

	return hi_access_rom(offset);
}

void nes_lh53_device::write_h(offs_t offset, uint8_t data)
{
	LOG("lh53 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset >= 0x3800 && offset < 0x5800) {
		m_prgram[offset - 0x3800] = data;
		return;
	}

	switch (offset & 0x7000) {
		case 0x6000:
			// $E000-$EFFF: acknowledge, reset and configure the IRQ.
			m_irq_enable = BIT(data, 1);
			m_irq_count = 0;
			m_irq_delay = 0;
			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			break;

		case 0x7000:
			// $F000-$FFFF: select the 8 KiB PRG-ROM bank at $6000-$7FFF.
			m_reg = data & 0x0f;
			break;
	}
}

/*-------------------------------------------------

 BTL-2708

 Games: Doki Doki Panic (FDS conversion)

 iNES: mapper 103

 This board contains 16 KiB of WRAM. The first 8 KiB
 is mapped at $6000-$7FFF, and the second 8 KiB is
 overlaid at $B800-$D7FF.

 When $F000-$FFFF bit 4 is set, both WRAM regions are
 disabled for reads. The selected PRG-ROM bank appears
 at $6000-$7FFF, while fixed PRG-ROM appears at
 $B800-$D7FF. Writes always reach WRAM regardless of
 the read mode.

 Writes throughout $8000-$8FFF select the 8 KiB
 PRG-ROM bank at $6000-$7FFF. Writes throughout
 $E000-$EFFF control nametable mirroring.

 The registers are not cleared on reset, and their
 power-on state is unknown. The board has no bus
 conflicts.

 In MAME: Supported.

 -------------------------------------------------*/

uint8_t nes_2708_device::read_m(offs_t offset)
{
	LOG("btl-2708 read_m, offset: %04x\n", offset);

	if (!m_reg[1]) {
		return m_prgram[offset];
	}

	return m_prg[((m_reg[0] * 0x2000) + offset) & (m_prg_size - 1)];
}

void nes_2708_device::write_m(offs_t offset, uint8_t data)
{
	LOG("btl-2708 write_m, offset: %04x, data: %02x\n", offset, data);

	m_prgram[offset] = data;
}

uint8_t nes_2708_device::read_h(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "btl-2708 read_h, offset: %04x\n", offset);

	if (offset >= 0x3800 && offset < 0x5800 && !m_reg[1]) {
		return m_prgram[0x2000 + offset - 0x3800];
	}

	return hi_access_rom(offset);
}

void nes_2708_device::write_h(offs_t offset, uint8_t data)
{
	LOG("btl-2708 write_h, offset: %04x, data: %02x\n", offset, data);

	if (offset >= 0x3800 && offset < 0x5800) {
		m_prgram[0x2000 + offset - 0x3800] = data;
		return;
	}

	switch (offset & 0x7000) {
		case 0x0000:
			// $8000-$8FFF: select the PRG-ROM bank at $6000-$7FFF.
			m_reg[0] = data & 0x0f;
			break;

		case 0x6000:
			// $E000-$EFFF: nametable mirroring.
			set_nt_mirroring(BIT(data, 3) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			break;

		case 0x7000:
			// $F000-$FFFF: bit 4 disables WRAM for reads.
			m_reg[1] = BIT(data, 4);
			break;
	}
}

/*-------------------------------------------------

 UNL-AC08 / LH09

 Games: Green Beret (FDS conversion)

 UNIF: UNL-AC08
 iNES: none

 The board contains 160 KiB of PRG-ROM. The first
 128 KiB is divided between two 64 KiB PRG chips and
 is exposed as one of sixteen switchable 8 KiB banks
 at $6000-$7FFF.

 The final 32 KiB PRG chip is fixed at $8000-$FFFF.

 Writes to $8001, decoded with mask $E001, select the
 switchable PRG-ROM bank. Data bits 1-4 provide the
 four-bit bank number.

 Nametable mirroring is fixed vertical. The retained
 FDS register write at $4025 is not decoded by the
 cartridge.

 In MAME: Supported.

 -------------------------------------------------*/

uint8_t nes_ac08_device::read_m(offs_t offset)
{
	LOG("AC-08 read_m, offset: %04x\n", offset);

	return m_prg[(m_latch * 0x2000) + offset];
}

void nes_ac08_device::write_h(offs_t offset, uint8_t data)
{
	LOG("AC-08 write_h, offset: %04x, data: %02x\n", offset, data);

	if ((offset & 0x6001) == 0x0001) {
		m_latch = BIT(data, 1, 4);
	}
}

/*-------------------------------------------------

 BTL-MARIO1-MALEE2 (aka Genius Merio Bros.)

 Games: Fly Merio Bros., Super Mario Bros. Malee 2,
        Genius Mario

 iNES: mapper 55
 UNIF: BTL-MARIO1-MALEE2

 This board extends an NROM-256 configuration with
 2 KiB of additional PRG-ROM and 2 KiB of WRAM.

 The additional PRG-ROM is mirrored across
 $6000-$6FFF, while the WRAM is mirrored across
 $7000-$7FFF. The original 32 KiB PRG-ROM remains
 fixed at $8000-$FFFF.

 The board has 8 KiB of fixed CHR-ROM and contains
 no banking registers.

 In MAME: Supported.

 -------------------------------------------------*/

uint8_t nes_mmalee_device::read_m(offs_t offset)
{
	LOG("mmalee read_m, offset: %04x\n", offset);

	if (offset < 0x1000) {
		return m_prg[0x8000 + (offset & 0x07ff)];
	}

	if (!m_prgram.empty()) {
		return m_prgram[offset & 0x07ff];
	}

	return get_open_bus();
}

void nes_mmalee_device::write_m(offs_t offset, uint8_t data)
{
	LOG("mmalee write_m, offset: %04x, data: %02x\n", offset, data);

	if (offset >= 0x1000 && !m_prgram.empty()) {
		m_prgram[offset & 0x07ff] = data;
	}
}

/*-------------------------------------------------

 RT-01

 Games: Test Ver. 1.01 Dlya Proverki TV Pristavok

 This board has 16 KiB of PRG-ROM mirrored across
 $8000-$FFFF and 2 KiB of CHR-ROM mirrored across
 $0000-$1FFF.

 The PRG EPROM contains two copy-protection regions
 with weak data bits. Reads from these regions return
 fixed high bits with variable values on D0, D2 and D3.

 NES 2.0: mapper 328

 In MAME: Partially supported.

 -------------------------------------------------*/

uint8_t nes_rt01_device::read_h(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "rt01 read_h, offset: %04x\n", offset);

	const bool weak_data =
		(offset >= 0x4e80 && offset < 0x4f00) ||
		(offset >= 0x7e80 && offset < 0x7f00);

	if (weak_data) {
		// D1 and D4-D7 read high. D0, D2 and D3 are unstable.
		return 0xf2 | (machine().rand() & 0x0d);
	}

	return hi_access_rom(offset);
}

/*-------------------------------------------------

 YUNG-08

 Games: Super Mario Bros. 2 Pirate (YUNG-08)

 This board maps fixed PRG-ROM banks at $6000-$BFFF
 and $E000-$FFFF. The 8 KiB bank at $C000-$DFFF is
 selected by writes to $4022.

 Writes to $4122 control a four-bit readable latch.
 Bit 0 also controls a 12-bit CPU M2 IRQ counter.
 When enabled, the counter increments every M2 cycle
 and requests an IRQ when it wraps from $FFF to $000.

 NES 2.0: mapper 368

 In MAME: Supported.

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_yung08_device::irq_timer_tick)
{
	if (!BIT(m_irq_latch, 0)) {
		return;
	}

	m_irq_count = (m_irq_count + 1) & 0x0fff;

	if (!m_irq_count) {
		m_irq_delay = 2;
	}
}

void nes_yung08_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address)
{
	if (!m_irq_delay) {
		return;
	}

	m_irq_delay--;

	if (!m_irq_delay) {
		m_maincpu6502->queue_delayed_mapper_irq(2);
	}
}

void nes_yung08_device::write_45(offs_t offset, u8 data)
{
	switch (offset & 0x51ff) {
		case 0x4022:
			// $4022, mirrored through $4FFF:
			// select the 8 KiB PRG-ROM bank at $C000-$DFFF.
			if (BIT(data, 0)) {
				prg8_cd(3);
			}
			else {
				prg8_cd(4 + BIT(data, 1, 2));
			}
			break;

		case 0x4122:
			// $4122, mirrored through $4FFF:
			// bits 0, 2, 4 and 5 form a readable latch.
			m_irq_latch = data & 0x35;

			// Bit 0 clears and disables the IRQ counter when zero.
			if (!BIT(m_irq_latch, 0)) {
				m_irq_count = 0;
				m_irq_delay = 0;

				set_irq_line(CLEAR_LINE);
				m_maincpu6502->cancel_delayed_mapper_irq();
			}
			break;
	}
}

void nes_yung08_device::write_ex(offs_t offset, u8 data)
{
	LOG("yung08 write_ex, offset: %04x, data: %02x\n", offset, data);

	write_45(offset + 0x4020, data);
}

void nes_yung08_device::write_l(offs_t offset, u8 data)
{
	LOG("yung08 write_l, offset: %04x, data: %02x\n", offset, data);

	write_45(offset + 0x4100, data);
}

u8 nes_yung08_device::read_l(offs_t offset)
{
	LOG("yung08 read_l, offset: %04x\n", offset);

	offset += 0x100;

	if ((offset & 0x11ff) == 0x0122) {
		return m_irq_latch | 0x8a;
	}

	return get_open_bus();
}

u8 nes_yung08_device::read_m(offs_t offset)
{
	LOG("yung08 read_m, offset: %04x\n", offset);

	return m_prg[(0x02 * 0x2000) + offset];
}