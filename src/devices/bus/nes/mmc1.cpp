// license
// copyright-holders Priuli
/***********************************************************************************************************

NES/Famicom cartridge emulation for Nintendo MMC-1 PCBs

Here we emulate the Nintendo SxROM / MMC-1 PCBs + older variants without WRAM protect bit

The base implementation handles the MMC1 serial port, consecutive-cycle
write suppression, mirroring, standard PRG/CHR banking, and the different
$E000 bit 4 behavior of MMC1A and MMC1B-family ASICs. Board subclasses
handle the output lines repurposed by SNROM, SOROM, SUROM, SXROM, and SZROM.

The two PRG banking paths have not been combined. The standard path accepts
an outer-bank base and mask for extended boards, while the other preserves
MMC1A's special $E000 bit 4 behavior. Any future consolidation must retain
that MMC1A bypass of the fixed-bank logic.

Remaining hardware questions: it is not known whether chips marked simply
"MMC1" ignore the PRG-RAM disable bit like MMC1A. The reported MMC1C
revision also remains unverified.

***********************************************************************************************************/

#include "emu.h"
#include "mmc1.h"
#include "cpu/m6502/m6502.h"

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"

//-------------------------------------------------
// constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_SXROM, nes_sxrom_device, "nes_sxrom", "NES Cart SxROM (MMC-1) PCB")
DEFINE_DEVICE_TYPE(NES_SXROM_EXT, nes_sxrom_ext_device, "nes_sxrom_ext", "NES Cart SXROM Extended MMC-1 PCB")
DEFINE_DEVICE_TYPE(NES_SNROM, nes_snrom_device, "nes_snrom", "NES Cart SNROM (MMC-1) PCB")
DEFINE_DEVICE_TYPE(NES_SOROM, nes_sorom_device, "nes_sorom", "NES Cart SOROM (MMC-1) PCB")
DEFINE_DEVICE_TYPE(NES_SUROM, nes_surom_device, "nes_surom", "NES Cart SUROM (MMC-1) PCB")
DEFINE_DEVICE_TYPE(NES_SZROM, nes_szrom_device, "nes_szrom", "NES Cart SZROM (MMC-1) PCB")

nes_sxrom_device::nes_sxrom_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock) : nes_nrom_device(mconfig, type, tag, owner, clock), m_powered(false), m_mmc1_upper_chr(false), m_mmc1_ppu_addr(0), m_latch(0), m_count(0), m_maincpu6502(nullptr) {
	memset(m_reg, 0, sizeof(m_reg));
}

nes_sxrom_ext_device::nes_sxrom_ext_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock) : nes_sxrom_device(mconfig, NES_SXROM_EXT, tag, owner, clock) {
}

nes_sxrom_device::nes_sxrom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock) : nes_sxrom_device(mconfig, NES_SXROM, tag, owner, clock) {
}

nes_snrom_device::nes_snrom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock) : nes_sxrom_device(mconfig, NES_SNROM, tag, owner, clock) {
}

nes_sorom_device::nes_sorom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock) : nes_sxrom_device(mconfig, NES_SOROM, tag, owner, clock) {
}

nes_surom_device::nes_surom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock) : nes_sxrom_device(mconfig, NES_SUROM, tag, owner, clock) {
}

nes_szrom_device::nes_szrom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock) : nes_sxrom_device(mconfig, NES_SZROM, tag, owner, clock) {
}

void nes_sxrom_device::device_start() {
	common_start();

	save_item(NAME(m_latch));
	save_item(NAME(m_count));
	save_item(NAME(m_reg));
	save_item(NAME(m_powered));
	save_item(NAME(m_mmc1_upper_chr));
	save_item(NAME(m_mmc1_ppu_addr));

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");
}

void nes_sxrom_device::pcb_reset() {
	m_latch = 0;
	m_count = 0;
	m_mmc1_upper_chr = false;
	m_mmc1_ppu_addr = 0;

	if (!m_powered) {
		m_reg[0] = 0x0c;

		// For CHR RAM boards, keep initial CHR banking at bank 0 so
		// CHR RAM uploads done before mapper init land in the visible banks.
		if (m_chr_source == CHRRAM) {
			m_reg[1] = 0x00;
			m_reg[2] = 0x00;
		}
		else {
			m_reg[1] = machine().rand() & 0x1f;
			m_reg[2] = machine().rand() & 0x1f;
		}

		m_reg[3] = machine().rand() & 0x1f;
		m_powered = true;
	}

	// Reset preserves the mirroring selection but forces the MMC1
	// into 16 KiB PRG mode with the final bank fixed at $C000.
	m_reg[0] |= 0x0c;

	set_chr();
	set_prg();
	set_mirror();
}

/*-------------------------------------------------
mapper specific handlers
-------------------------------------------------*/

/*-------------------------------------------------

SxROM (MMC1 based) board emulation

iNES: mapper 1 (and 155 for the MMC1A variant which does not have WRAM disable bit)

-------------------------------------------------*/

void nes_sxrom_device::mmc1_ppu_phase(bool upper_chr, uint16_t ppu_addr) {
	const bool phase_changed = m_mmc1_upper_chr != upper_chr;

	m_mmc1_upper_chr = upper_chr;
	m_mmc1_ppu_addr = ppu_addr & 0x1fff;

	// In 4 KiB CHR mode, R1 and R2 are selected by PPU A12.
	// SUROM and SXROM connect CHR A16 to PRG-ROM A18, so
	// differing R1/R2 bit 4 values bankswitch PRG-ROM as the
	// PPU moves between the two pattern tables.
	if (phase_changed && BIT(m_reg[0], 4) && m_prg_chunks > 16)
		set_prg();
}

u8 nes_sxrom_device::mmc1_active_chr_reg() const {
	// In 4 KB CHR mode:
	// PPU $0000-$0FFF uses R1
	// PPU $1000-$1FFF uses R2

	// In 8 KB CHR mode:
	// R1 controls the 8 KB bank; R2 is ignored.
	if (BIT(m_reg[0], 4) && m_mmc1_upper_chr)
		return m_reg[2];

	return m_reg[1];
}

// Standard MMC1 PRG banking with base and mask (to support multicarts, etc)
void nes_sxrom_device::set_prg(int prg_base, int prg_mask) {
	// NES 2.0 mapper 1 submapper 5 describes boards whose 32 KiB PRG-ROM
	// is fixed even though an MMC1-compatible register interface is present.
	//if (get_submapper() == 5) {
	if (m_prg_chunks <= 2 || get_submapper() == 5) {
		prg32(0);
		return;
	}

	u8 bank = prg_base | (m_reg[3] & prg_mask);

	switch (BIT(m_reg[0], 2, 2)) {
		case 0:
		case 1:
			prg32(bank >> 1);
			break;
		case 2:
			prg16_89ab(prg_base);
			prg16_cdef(bank);
			break;
		case 3:
			prg16_89ab(bank);
			prg16_cdef(prg_base | prg_mask);
			break;
	}
}

void nes_sxrom_device::set_prg() {
	//if (get_submapper() == 5) {
	if (m_prg_chunks <= 2 || get_submapper() == 5) {
		prg32(0);
		return;
	}

	const u8 prg_mode = m_reg[0] & 0x0c;
	const u8 bank = (m_prg_chunks > 0) ? ((m_reg[3] & 0x0f) % m_prg_chunks) : 0;

	u8 first = 0;
	u8 last = (m_prg_chunks > 0) ? (m_prg_chunks - 1) : 0;

	if (m_mmc1_type == mmc1_type::MMC1A && BIT(m_reg[3], 4)) {
		first = m_reg[3] & 0x08;
		last = first | 0x07;
	}

	switch (prg_mode) {
		case 0x00:
		case 0x04:
			prg32(bank >> 1);
			break;

		case 0x08:
			prg16_89ab(first);
			prg16_cdef(bank);
			break;

		case 0x0c:
			prg16_89ab(bank);
			prg16_cdef(last);
			break;
	}
}

// Standard MMC1 CHR banking with base and mask.
void nes_sxrom_device::set_chr(int chr_base, int chr_mask) {
	if (BIT(m_reg[0], 4)) {
		chr4_0(chr_base | (m_reg[1] & chr_mask), m_chr_source);
		chr4_4(chr_base | (m_reg[2] & chr_mask), m_chr_source);
	}
	else {
		chr8((chr_base | (m_reg[1] & chr_mask)) >> 1, m_chr_source);
	}
}

void nes_sxrom_device::set_mirror() {
	static constexpr u8 mirr[4] = {
		PPU_MIRROR_LOW, PPU_MIRROR_HIGH, PPU_MIRROR_VERT, PPU_MIRROR_HORZ};
	set_nt_mirroring(mirr[m_reg[0] & 0x03]);
}

// this allows for easier implementation of MMC1 subclasses
void nes_sxrom_device::update_regs(int reg) {
	switch (reg) {
		case 0:
			set_chr();
			set_prg();
			set_mirror();
			break;

		case 1:
			set_chr();
			set_prg();
			break;
			
		case 2:
			set_chr();
			break;

		case 3:
			set_prg();
			break;
	}
}

bool nes_sxrom_device::prgram_enabled() const {
	// MMC1A has no PRG-RAM disable function.
	if (m_mmc1_type == mmc1_type::MMC1A)
		return true;

	// On the MMC1B family, PRG register bit 4 disables PRG-RAM when set.
	return !BIT(m_reg[3], 4);
}

u8 nes_sxrom_device::prgram_bank() const {
	// Standard SxROM boards expose one 8 KiB PRG-RAM bank.
	return 0;
}

void nes_sxrom_device::write_h(offs_t offset, u8 data) {
	LOG("sxrom write_h, offset: %04x, data: %02x\n", offset, data);

	const u64 now_cpu = m_maincpu6502->get_last_cpu_write_cycle();
	const u64 previous_cpu_write = m_maincpu6502->get_previous_cpu_write_cycle();

	// A write with D7 set always resets the serial port, including
	// when it occurs on the second write cycle of an RMW instruction.
	if (BIT(data, 7)) {
		m_latch = 0;
		m_count = 0;

		// Reset forces 16 KiB PRG mode with the final bank fixed
		// at $C000. Mirroring and CHR banking are unchanged.
		m_reg[0] |= 0x0c;

		set_prg();
		return;
	}

	// Consecutive-cycle filtering applies only to serial data writes.
	if (previous_cpu_write + 1 == now_cpu) {
		logerror("MMC1 ignored consecutive serial write, cycle=%llu previous=%llu address=%04X data=%02X\n", (unsigned long long)now_cpu, (unsigned long long)previous_cpu_write, unsigned(offset + 0x8000), data);
		return;
	}

	m_latch >>= 1;
	m_latch |= BIT(data, 0) << 4;
	++m_count;

	if (m_count == 5) {
		const int reg = BIT(offset, 13, 2);

		m_reg[reg] = m_latch & 0x1f;
		m_latch = 0;
		m_count = 0;

		update_regs(reg);
	}
}

void nes_sxrom_device::write_m(offs_t offset, u8 data) {
	const u8 bank = prgram_bank();
	LOG("sxrom write_m, offset: %04x, data: %02x\n", offset, data);

	if (prgram_enabled()) {
		if (!m_battery.empty())
			m_battery[((bank * 0x2000) + offset) & (m_battery.size() - 1)] = data;
		if (!m_prgram.empty())
			m_prgram[((bank * 0x2000) + offset) & (m_prgram.size() - 1)] = data;
	}
}

u8 nes_sxrom_device::read_m(offs_t offset) {
	const u8 bank = prgram_bank();
	LOG("sxrom read_m, offset: %04x\n", offset);

	if (prgram_enabled()) {
		if (!m_battery.empty())
			return m_battery[((bank * 0x2000) + offset) & (m_battery.size() - 1)];
		if (!m_prgram.empty())
			return m_prgram[((bank * 0x2000) + offset) & (m_prgram.size() - 1)];
	}

	return get_open_bus();
}

/*-------------------------------------------------

SXROM Extended (MMC1 based) board emulation

iNES: mapper 1

Variant of SxROM with 32 KiB of PRG-RAM and, on some
boards, 512 KiB of PRG-ROM. It combines the SUROM-style
outer PRG-ROM bank select with larger PRG-RAM banking.

CHR register bit 4 / CHR A16 can be repurposed as
PRG-ROM A18 to select the outer 256 KiB PRG-ROM half.
CHR register bits 2-3 are repurposed as PRG-RAM bank
select lines for four 8 KiB PRG-RAM banks.

-------------------------------------------------*/

void nes_sxrom_ext_device::set_prg() {
	const u8 active = mmc1_active_chr_reg();

	const u8 prg_base = (m_prg_chunks > 16 && BIT(active, 4)) ? 0x10 : 0x00;

	nes_sxrom_device::set_prg(prg_base, 0x0f);
}

void nes_sxrom_ext_device::update_regs(int reg) {
	switch (reg) {
		case 0:
			set_chr();
			set_prg();
			set_mirror();
			break;

		case 1:
		case 2:
			set_chr();
			set_prg();
			break;

		case 3:
			set_prg();
			break;
	}
}

void nes_sxrom_ext_device::set_chr() {
	// SXROM addresses only 8 KiB of CHR-ROM or CHR-RAM.
	// Bits 2-4 are reused for PRG-RAM and PRG-ROM banking,
	// while bit 1 is unused. Only bit 0 reaches CHR A12
	// in 4 KiB CHR mode.
	if (BIT(m_reg[0], 4)) {
		chr4_0(m_reg[1] & 0x01, m_chr_source);
		chr4_4(m_reg[2] & 0x01, m_chr_source);
	}
	else {
		chr8(0, m_chr_source);
	}
}

u8 nes_sxrom_ext_device::prgram_bank() const {
	const u8 active = mmc1_active_chr_reg();

	if ((!m_battery.empty() && m_battery.size() > 0x2000) || (!m_prgram.empty() && m_prgram.size() > 0x2000))
		return BIT(active, 2, 2);

	return 0;
}

/*-------------------------------------------------

SNROM (MMC1 based) board emulation

iNES: mapper 1

SNROM addresses 8 KiB of CHR-ROM or CHR-RAM. CHR A16,
normally controlled by bit 4 of the active CHR register,
is instead connected to PRG-RAM /CE.

The active CHR register is R1 in 8 KiB CHR mode. In
4 KiB CHR mode, PPU A12 selects R1 or R2, allowing
PRG-RAM to become enabled or disabled as the PPU
moves between pattern tables.

-------------------------------------------------*/

void nes_snrom_device::set_chr() {
	// Only CHR register bit 0 reaches CHR A12.
	// Bits 1-3 are disconnected and bit 4 controls PRG-RAM /CE.
	if (BIT(m_reg[0], 4)) {
		chr4_0(m_reg[1] & 0x01, m_chr_source);
		chr4_4(m_reg[2] & 0x01, m_chr_source);
	}
	else {
		chr8(0, m_chr_source);
	}
}

bool nes_snrom_device::prgram_enabled() const {
	// MMC1B disables PRG-RAM when $E000 bit 4 is set.
	// MMC1A ignores that disable bit.
	//
	// Every MMC1 revision on SNROM additionally disables
	// PRG-RAM when CHR A16 from the active CHR register is set.
	return nes_sxrom_device::prgram_enabled() && !BIT(mmc1_active_chr_reg(), 4);
}

/*-------------------------------------------------

SOROM (MMC1 based) board emulation

iNES: mapper 1

SOROM contains 16 KiB of PRG-RAM divided between two
8 KiB chips. Bank 0 is volatile PRG-RAM and bank 1 is
battery-backed PRG-RAM.

Bit 3 of the active CHR register drives PRG-RAM A13.
In 4 KiB CHR mode, PPU A12 selects which CHR register
provides this line. Only CHR register bit 0 reaches the
8 KiB CHR-ROM or CHR-RAM.

-------------------------------------------------*/

u8 nes_sorom_device::read_m(offs_t offset) {
	if (!prgram_enabled())
		return get_open_bus();

	const u8 bank = BIT(mmc1_active_chr_reg(), 3);

	if (bank) {
		if (!m_battery.empty())
			return m_battery[offset & (m_battery.size() - 1)];
	}
	else {
		if (!m_prgram.empty())
			return m_prgram[offset & (m_prgram.size() - 1)];
	}

	return get_open_bus();
}

void nes_sorom_device::write_m(offs_t offset, u8 data) {
	if (!prgram_enabled())
		return;

	const u8 bank = BIT(mmc1_active_chr_reg(), 3);

	if (bank) {
		if (!m_battery.empty())
			m_battery[offset & (m_battery.size() - 1)] = data;
	}
	else {
		if (!m_prgram.empty())
			m_prgram[offset & (m_prgram.size() - 1)] = data;
	}
}

void nes_sorom_device::set_chr() {
	// Only bit 0 reaches CHR A12. Bit 3 selects PRG-RAM,
	// while bits 1, 2 and 4 are disconnected.
	if (BIT(m_reg[0], 4)) {
		chr4_0(m_reg[1] & 0x01, m_chr_source);
		chr4_4(m_reg[2] & 0x01, m_chr_source);
	}
	else {
		chr8(0, m_chr_source);
	}
}

/*-------------------------------------------------

SUROM (MMC1 based) board emulation

iNES: mapper 1

SUROM supports up to 512 KiB of PRG-ROM, one 8 KiB
PRG-RAM bank, and 8 KiB of CHR-ROM or CHR-RAM.

Bit 4 of the active CHR register drives PRG-ROM A18,
selecting the outer 256 KiB PRG-ROM half. Only bit 0
reaches CHR A12 in 4 KiB CHR mode.

In 4 KiB CHR mode, PPU A12 selects which CHR register
provides PRG-ROM A18. In 8 KiB CHR mode, only the first
CHR register is used.

-------------------------------------------------*/

void nes_surom_device::set_prg() {
	const u8 active = mmc1_active_chr_reg();
	const u8 prg_base = (m_prg_chunks > 16 && BIT(active, 4)) ? 0x10 : 0x00;

	nes_sxrom_device::set_prg(prg_base, 0x0f);
}

void nes_surom_device::set_chr() {
	// Bits 1-3 are disconnected and bit 4 drives PRG-ROM A18.
	// Only bit 0 reaches CHR A12.
	if (BIT(m_reg[0], 4)) {
		chr4_0(m_reg[1] & 0x01, m_chr_source);
		chr4_4(m_reg[2] & 0x01, m_chr_source);
	}
	else {
		chr8(0, m_chr_source);
	}
}

void nes_surom_device::update_regs(int reg) {
	switch (reg) {
		case 0:
			set_chr();
			set_prg();
			set_mirror();
			break;

		case 1:
		case 2:
			set_chr();
			set_prg();
			break;

		case 3:
			set_prg();
			break;
	}
}

u8 nes_surom_device::prgram_bank() const {
	return 0;
}

/*-------------------------------------------------

SZROM (MMC1 based) board emulation

iNES: mapper 1

SZROM contains 16 KiB of PRG-RAM divided between two
8 KiB chips. Bank 0 is volatile PRG-RAM and bank 1 is
battery-backed PRG-RAM.

Bit 4 of the active CHR register drives PRG-RAM A13.
The lower four bits retain normal MMC1 CHR banking,
allowing up to 64 KiB of CHR-ROM or CHR-RAM.

In 4 KiB CHR mode, PPU A12 selects which CHR register
provides the PRG-RAM bank line. In 8 KiB CHR mode,
only the first CHR register is used.

-------------------------------------------------*/

u8 nes_szrom_device::prgram_bank() const {
	return BIT(mmc1_active_chr_reg(), 4);
}

u8 nes_szrom_device::read_m(offs_t offset) {
	if (!prgram_enabled())
		return get_open_bus();

	const u8 bank = prgram_bank();

	if (bank) {
		if (!m_battery.empty())
			return m_battery[offset & (m_battery.size() - 1)];
	}
	else {
		if (!m_prgram.empty())
			return m_prgram[offset & (m_prgram.size() - 1)];
	}

	return get_open_bus();
}

void nes_szrom_device::write_m(offs_t offset, u8 data) {
	if (!prgram_enabled())
		return;

	const u8 bank = prgram_bank();

	if (bank) {
		if (!m_battery.empty())
			m_battery[offset & (m_battery.size() - 1)] = data;
	}
	else {
		if (!m_prgram.empty())
			m_prgram[offset & (m_prgram.size() - 1)] = data;
	}
}

void nes_szrom_device::set_chr() {
	// Bits 0-3 select CHR banks. Bit 4 selects PRG-RAM.
	if (BIT(m_reg[0], 4)) {
		chr4_0(m_reg[1] & 0x0f, m_chr_source);
		chr4_4(m_reg[2] & 0x0f, m_chr_source);
	}
	else {
		chr8((m_reg[1] & 0x0f) >> 1, m_chr_source);
	}
}