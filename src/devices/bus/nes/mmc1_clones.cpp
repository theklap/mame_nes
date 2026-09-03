// license:BSD-3-Clause
// copyright-holders:kmg
/***********************************************************************************************************


 NES/Famicom cartridge emulation for MMC-1 clone PCBs


 Here we emulate several pirate PCBs based on MMC-1 boards

 ***********************************************************************************************************/


#include "emu.h"
#include "mmc1_clones.h"


#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_BMC_JY012005,  nes_bmc_jy012005_device,  "nes_bmc_jy012005",  "NES Cart BMC JY012005 PCB")
DEFINE_DEVICE_TYPE(NES_BMC_JY820845C, nes_bmc_jy820845c_device, "nes_bmc_jy820845c", "NES Cart BMC JY820845C PCB")
DEFINE_DEVICE_TYPE(NES_FARID_SLROM,   nes_farid_slrom_device,   "nes_farid_slrom",   "NES Cart Farid SLROM 8 in 1 PCB")
DEFINE_DEVICE_TYPE(NES_NINJARYU,      nes_ninjaryu_device,      "nes_ninjaryu",      "NES Cart Ninja Ryukenden Chinese PCB")
DEFINE_DEVICE_TYPE(NES_RESETSXROM,    nes_resetsxrom_device,    "nes_resetsxrom",    "NES Cart BMC RESET-SXROM PCB")
DEFINE_DEVICE_TYPE(NES_SRPG_5IN1,     nes_srpg5in1_device,      "nes_srpg5in1",      "NES Cart Super RPG 5 in 1 PCB")
DEFINE_DEVICE_TYPE(NES_TXC_22110,     nes_txc_22110_device,     "nes_txc_22110",     "NES Cart TXC 01-22110-000 PCB")


nes_bmc_jy012005_device::nes_bmc_jy012005_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_sxrom_device(mconfig, NES_BMC_JY012005, tag, owner, clock), m_latch0(0)
{
}

nes_bmc_jy820845c_device::nes_bmc_jy820845c_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_sxrom_device(mconfig, NES_BMC_JY820845C, tag, owner, clock), m_latch0(0), m_mode(0)
{
}

nes_farid_slrom_device::nes_farid_slrom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_sxrom_device(mconfig, NES_FARID_SLROM, tag, owner, clock), m_outer(0)
{
}

nes_ninjaryu_device::nes_ninjaryu_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_sxrom_device(mconfig, NES_NINJARYU, tag, owner, clock)
{
}

nes_resetsxrom_device::nes_resetsxrom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_sxrom_device(mconfig, NES_RESETSXROM, tag, owner, clock), m_reset_count(-1)
{
}

nes_srpg5in1_device::nes_srpg5in1_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_sxrom_device(mconfig, NES_SRPG_5IN1, tag, owner, clock), m_outer(0), m_outer_count(0), m_outer_latch(0)
{
}

nes_txc_22110_device::nes_txc_22110_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_sxrom_device(mconfig, NES_TXC_22110, tag, owner, clock), m_latch0(0), m_mode(0)
{
}



void nes_bmc_jy012005_device::device_start()
{
	nes_sxrom_device::device_start();
	save_item(NAME(m_latch0));
}

void nes_bmc_jy012005_device::pcb_reset()
{
	m_latch0 = 0;
	nes_sxrom_device::pcb_reset();
}

void nes_bmc_jy820845c_device::device_start()
{
	nes_sxrom_device::device_start();
	save_item(NAME(m_latch0));
	save_item(NAME(m_mode));
}

void nes_bmc_jy820845c_device::pcb_reset() {
	m_latch0 = 0;
	m_mode = 0;

	nes_sxrom_device::pcb_reset();

	update_banks();
}

void nes_farid_slrom_device::device_start()
{
	nes_sxrom_device::device_start();
	save_item(NAME(m_outer));
}

void nes_farid_slrom_device::pcb_reset()
{
	m_outer = 0;
	nes_sxrom_device::pcb_reset();
}

void nes_resetsxrom_device::device_start()
{
	nes_sxrom_device::device_start();
	save_item(NAME(m_reset_count));
}

void nes_resetsxrom_device::pcb_reset()
{
	m_reset_count = (m_reset_count + 1) & 3;
	nes_sxrom_device::pcb_reset();
}

void nes_srpg5in1_device::device_start()
{
	nes_sxrom_device::device_start();
	save_item(NAME(m_outer));
	save_item(NAME(m_outer_count));
	save_item(NAME(m_outer_latch));
}

void nes_srpg5in1_device::pcb_reset()
{
	m_outer = 0;
	m_outer_count = 0;
	m_outer_latch = 0;
	nes_sxrom_device::pcb_reset();
}

void nes_txc_22110_device::device_start()
{
	nes_sxrom_device::device_start();
	save_item(NAME(m_latch0));
	save_item(NAME(m_mode));
}

void nes_txc_22110_device::pcb_reset()
{
	nes_sxrom_device::pcb_reset();

	m_latch0 = 0;
	m_mode = 0;
	update_banks();
}



/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

UNL-NINJARYU

Game: Ninja Ryukenden Chinese

This board provides four parallel registers resembling
the internal MMC1 registers. Writes do not use the MMC1
five-bit serial protocol.

$8000-$9FFF: Control
$A000-$BFFF: CHR bank 0
$C000-$DFFF: CHR bank 1
$E000-$FFFF: PRG bank and PRG-RAM disable

The board supports 256 KiB of PRG-ROM and 256 KiB of
CHR-ROM. D7 reset behavior has not been verified.

The board was formerly assigned to iNES mapper 111.
Mapper 111 is now assigned to GTROM, so this board is
currently selected through the MAME software list only.

D7 reset behavior has not been verified.

In MAME: Supported.

-------------------------------------------------*/

void nes_ninjaryu_device::write_h(offs_t offset, u8 data) {
	LOG("unl_ninjaryu write_h, offset: %04x, data: %02x\n", offset, data);

	if (!BIT(data, 7)) {
		const u8 reg = BIT(offset, 13, 2);

		m_reg[reg] = data;
		update_regs(reg);
	}
}


/*-------------------------------------------------

 MULTIGAME CARTS BASED ON MMC1

 -------------------------------------------------*/

/*-------------------------------------------------

 BMC-JY012005

 Game: 1998 Super HiK 8 in 1 (JY-021B)

 This board combines an MMC1-compatible mapper with
 an outer banking register written throughout
 $6000-$7FFF.

 Outer register bits 2-0 select the CHR-ROM region.
 Bit 6 selects the PRG-ROM banking mode. When clear,
 bits 2-1 select a 256 KiB PRG-ROM region and bit 0
 is ignored. When set, bits 2-0 select a 128 KiB
 PRG-ROM region.

 Bit 7 locks the outer register until reset.

 NES 2.0: mapper 404

 In MAME: Supported.

 -------------------------------------------------*/

void nes_bmc_jy012005_device::set_prg() {
	const u8 mode = !BIT(m_latch0, 6);
	const int prg_base = (m_latch0 & 0x07 & ~mode) << 3;
	const int prg_mask = (mode << 3) | 0x07;

	nes_sxrom_device::set_prg(prg_base, prg_mask);
}

void nes_bmc_jy012005_device::set_chr() {
	nes_sxrom_device::set_chr((m_latch0 & 0x07) << 5, 0x1f);
}

void nes_bmc_jy012005_device::write_m(offs_t offset, u8 data) {
	LOG("bmc_jy012005 write_m, offset: %04x, data: %02x\n", offset, data);

	if (!BIT(m_latch0, 7)) {
		m_latch0 = data;
		set_prg();
		set_chr();
	}
}

/*-------------------------------------------------

 BMC-JY820845C

 Game: 7 in 1 1993 Chess Series (JY-015)

 This board combines an MMC1-compatible mapper with
 a PAL providing outer banking and an optional
 MHROM-compatible banking mode.

 The outer register is selected by writes throughout
 $7000-$7FFF. Its value comes from CPU address bits
 3-0 rather than the written data:

 A3:    Outer-register lock
 A2-A1: Outer PRG-ROM and CHR-ROM bank
 A2-A1: 0-2 select MHROM mode
 A2-A1: 3 selects MMC1 mode
 A0:    Additional PRG-ROM bank bit in MHROM mode

 In MHROM mode, writes throughout $8000-$FFFF latch
 data bits 4, 1 and 0 for direct PRG-ROM and CHR-ROM
 banking.

 In MMC1 mode, writes throughout $8000-$FFFF are
 forwarded to the MMC1-compatible serial registers.
 Nametable mirroring is therefore controlled by the
 MMC1 while running the included MMC1 games.

 The outer register overlaps 8 KiB of WRAM at
 $6000-$7FFF.

 NES 2.0: mapper 550

 In MAME: Supported.

 -------------------------------------------------*/

void nes_bmc_jy820845c_device::set_prg() {
	if ((m_mode & 0x06) == 0x06) {
		nes_sxrom_device::set_prg(0x18, 0x07);
	}
	else {
		prg32(((m_mode & 0x07) << 1) | BIT(m_latch0, 4));
	}
}

void nes_bmc_jy820845c_device::set_chr() {
	if ((m_mode & 0x06) == 0x06) {
		nes_sxrom_device::set_chr(0x18, 0x07);
	}
	else {
		chr8(((m_mode & 0x06) << 1) | (m_latch0 & 0x03), CHRROM);
	}
}

void nes_bmc_jy820845c_device::update_banks() {
	const u8 prg_bank = ((m_mode & 0x07) << 1) | BIT(m_latch0, 4);
	const u8 chr_bank = ((m_mode & 0x06) << 1) | (m_latch0 & 0x03);

	prg32(prg_bank);
	chr8(chr_bank, CHRROM);
}

void nes_bmc_jy820845c_device::write_m(offs_t offset, u8 data) {
	LOG("bmc_jy820845c write_m, offset: %04x, data: %02x\n", offset, data);

	nes_sxrom_device::write_m(offset, data);

	if (offset >= 0x1000 && !BIT(m_mode, 3)) {
		m_mode = offset & 0x0f;

		if ((m_mode & 0x06) == 0x06) {
			set_prg();
			set_chr();
		}
		else {
			update_banks();
		}
	}
}

void nes_bmc_jy820845c_device::write_h(offs_t offset, u8 data) {
	LOG("bmc_jy820845c write_h, offset: %04x, data: %02x\n", offset, data);

	m_latch0 = data;

	nes_sxrom_device::write_h(offset, data);

	if ((m_mode & 0x06) != 0x06) {
		update_banks();
	}
}

/*-------------------------------------------------

 FARID_SLROM_8-IN-1

 Game: 8 in 1

 This MMC1 clone replaces PRG-RAM at $6000-$7FFF
 with a write-only outer bank register.

 Outer register bits 4-6 select a 128 KiB PRG-ROM
 and CHR-ROM region. Bit 3 locks the register until
 reset. Writes are accepted only while the MMC1
 PRG-RAM enable output is active.

 The outer register is cleared on warm and cold reset.

 NES 2.0: mapper 323
 UNIF: FARID_SLROM_8-IN-1

 In MAME: Supported.

 -------------------------------------------------*/

void nes_farid_slrom_device::write_m(offs_t offset, u8 data) {
	LOG("farid_slrom write_m, offset: %04x, data: %02x\n", offset, data);

	if (!BIT(m_reg[3], 4) && !BIT(m_outer, 3)) {
		m_outer = data;
		set_prg();
		set_chr();
	}
}

/*-------------------------------------------------

 BMC-RESET-SXROM

 Games: 4 in 1 (JY-021, JY-022, JY-051)

 MMC1 clone with outer banks switched by resetting.

 NES 2.0: mapper 374

 In MAME: Supported.

 -------------------------------------------------*/

/*-------------------------------------------------

 BMC-SRPG-5IN1

 Game: Super RPG 5 in 1 CH501

 This MMC1 clone contains an additional four-bit serial
 shift register written through $5000-$5FFF. After four
 writes, the completed value selects the outer PRG-ROM
 bank and affects PRG-RAM banking.

 The cartridge contains eight switchable 8 KiB banks of
 battery-backed PRG-RAM at $6000-$7FFF. MMC1 PRG register
 bit 4 controls whether the selected RAM bank is enabled.

 Outer register bit 3 is stored but its hardware function
 has not been verified. The game does not soft-reset
 properly on the original cartridge.

 NES 2.0: mapper 543

 In MAME: Supported.

 -------------------------------------------------*/

void nes_srpg5in1_device::write_l(offs_t offset, u8 data) {
	LOG("srpg5in1 write_l, offset: %04x, data: %02x\n", offset, data);

	offset += 0x100;

	if (offset >= 0x1000) {
		m_outer_latch = (data & 0x08) | (m_outer_latch >> 1);
		m_outer_count = (m_outer_count + 1) & 0x03;

		if (!m_outer_count) {
			m_outer = m_outer_latch;
			set_prg();
		}
	}
}

void nes_srpg5in1_device::write_m(offs_t offset, u8 data) {
	LOG("srpg5in1 write_m, offset: %04x, data: %02x\n", offset, data);

	const u8 bank = BIT(m_outer, 1)
			? bitswap<3>(m_outer, 1, 2, 0)
			: ((m_outer & 0x01) << 1) | BIT(m_reg[1], 3);

	if (!BIT(m_reg[3], 4) && !m_battery.empty()) {
		m_battery[((bank * 0x2000) + offset) & (m_battery.size() - 1)] = data;
	}
}

u8 nes_srpg5in1_device::read_m(offs_t offset) {
	LOG("srpg5in1 read_m, offset: %04x\n", offset);

	const u8 bank = BIT(m_outer, 1)
			? bitswap<3>(m_outer, 1, 2, 0)
			: ((m_outer & 0x01) << 1) | BIT(m_reg[1], 3);

	if (!BIT(m_reg[3], 4) && !m_battery.empty()) {
		return m_battery[((bank * 0x2000) + offset) & (m_battery.size() - 1)];
	}

	return get_open_bus();
}

/*-------------------------------------------------

 TXC 01-22110-000

 Game: 2 in 1 Uzi Lightgun (MGC-002)

 This board supports two banking modes. Space Shadow
 and the menu use mapper-70-compatible direct banking,
 while Operation Wolf uses an MMC1-compatible mapper.

 Writes throughout $4100-$41FF select the operating
 mode. Bit 0 selects MMC1 mode when set. In direct
 banking mode, bit 1 selects the 128 KiB PRG-ROM
 region.

 In direct banking mode, writes throughout
 $8000-$FFFF use data bits 5-4 to select a switchable
 16 KiB PRG-ROM bank and bits 3-0 to select an 8 KiB
 CHR-ROM bank. The final PRG-ROM bank within the
 selected 128 KiB region remains fixed at $C000-$FFFF.

 MMC1 mode uses the upper 128 KiB of PRG-ROM and
 the upper 128 KiB of CHR-ROM.

 Nametable mirroring is hardwired vertically in both
 modes.

 The cartridge requires the Bandai Hyper Shot
 lightgun connected through the Famicom expansion
 port.

 NES 2.0: mapper 297

 In MAME: Supported.

 -------------------------------------------------*/

void nes_txc_22110_device::update_banks() {
	const u8 outer = (m_mode & 0x02) << 1;

	prg16_89ab(outer | BIT(m_latch0, 4, 2));
	prg16_cdef(outer | 0x03);
	chr8(m_latch0 & 0x0f, CHRROM);
}

void nes_txc_22110_device::write_l(offs_t offset, u8 data) {
	LOG("TXC 22110 write_l, offset: %04x, data: %02x\n", offset, data);

	if (offset < 0x100) {
		m_mode = data;

		if (BIT(m_mode, 0)) {
			set_prg();
			set_chr();
		}
		else {
			update_banks();
		}
	}
}

void nes_txc_22110_device::write_h(offs_t offset, u8 data) {
	LOG("TXC 22110 write_h, offset: %04x, data: %02x\n", offset, data);

	if (BIT(m_mode, 0)) {
		nes_sxrom_device::write_h(offset, data);
	}
	else {
		m_latch0 = data;
		update_banks();
	}
}