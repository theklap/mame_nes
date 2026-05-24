// license
// copyright-holders Priuli
/***********************************************************************************************************

NES/Famicom cartridge emulation for Nintendo MMC-1 PCBs

Here we emulate the Nintendo SxROM / MMC-1 PCBs + older variants without WRAM protect bit

Known issues on specific mappers:

001 Yoshi flashes in-game.

001 Back to the Future have heavily corrupted graphics (since forever).

TODO:

Combine 2 versions of set_prg in SxROM base class. This means dealing with
variant boards SNROM, SUROM, etc which repurpose bits in the MMC1 regs.

Determine if "MMC1" marked chips, the earliest version, ignores WRAM
enable/disable bit like its first revision, MMC1A. Also determine if MMC1C
really exists. It's described by kevtris, but it's not in BootGod's DB.

***********************************************************************************************************/

#include "emu.h"
#include "mmc1.h"
#include "cpu/m6502/m6502.h"
#include "video/ppu2c0x.h"

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

nes_sxrom_device::nes_sxrom_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock)
: nes_nrom_device(mconfig, type, tag, owner, clock)
, m_last_mmc1_write_cpu_cycle(0)
, m_last_mmc1_write_valid(false)
, m_powered(false)
, m_mmc1_upper_chr(false)
, m_mmc1_ppu_addr(0)
, m_latch(0)
, m_count(0)
, m_maincpu6502(nullptr)
{
	memset(m_reg, 0, sizeof(m_reg));
}

nes_sxrom_ext_device::nes_sxrom_ext_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
: nes_sxrom_device(mconfig, NES_SXROM_EXT, tag, owner, clock)
{
}

nes_sxrom_device::nes_sxrom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
: nes_sxrom_device(mconfig, NES_SXROM, tag, owner, clock)
{
}

nes_snrom_device::nes_snrom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
: nes_sxrom_device(mconfig, NES_SNROM, tag, owner, clock)
{
}

nes_sorom_device::nes_sorom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
: nes_sxrom_device(mconfig, NES_SOROM, tag, owner, clock)
{
}

nes_surom_device::nes_surom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
: nes_sxrom_device(mconfig, NES_SUROM, tag, owner, clock)
{
}

nes_szrom_device::nes_szrom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
: nes_sxrom_device(mconfig, NES_SZROM, tag, owner, clock)
{
}

void nes_sxrom_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
	save_item(NAME(m_count));
	save_item(NAME(m_reg));
	save_item(NAME(m_powered));
	save_item(NAME(m_last_mmc1_write_cpu_cycle));
	save_item(NAME(m_last_mmc1_write_valid));
	save_item(NAME(m_mmc1_upper_chr));
	save_item(NAME(m_mmc1_ppu_addr));
	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");
	if (auto *ppu = machine().root_device().subdevice<ppu2c0x_device>("ppu"))
			ppu->set_mapper(1);
}

void nes_snrom_device::device_start()
{
	nes_sxrom_device::device_start();
}

void nes_sxrom_device::pcb_reset()
{
m_latch = 0;
m_count = 0;
m_mmc1_upper_chr = false;
m_mmc1_ppu_addr = 0;
m_last_mmc1_write_cpu_cycle = 0;
m_last_mmc1_write_valid = false;

if (!m_powered)
{
	m_reg[0] = 0x0f;

	// For CHR RAM boards, keep initial CHR banking at bank 0 so
	// CHR RAM uploads done before mapper init land in the visible banks.
	if (m_chr_source == CHRRAM)
	{
		m_reg[1] = 0x00;
		m_reg[2] = 0x00;
	}
	else
	{
		m_reg[1] = machine().rand() & 0x1f;
		m_reg[2] = machine().rand() & 0x1f;
	}

	m_reg[3] = machine().rand() & 0x1f;
	m_powered = true;
}

set_chr();
set_prg();
set_mirror();
}

void nes_surom_device::set_prg()
{
const u8 active = mmc1_active_chr_reg();

const u8 prg_base = (m_prg_chunks > 16 && BIT(active, 4)) ? 0x10 : 0x00;

nes_sxrom_device::set_prg(prg_base, 0x0f);
}

void nes_surom_device::set_chr()
{
// SUROM has 8 KiB CHR-RAM.
// CHR register bit 4 is PRG-ROM A18, not CHR banking.
if (BIT(m_reg[0], 4))
{
chr4_0(m_reg[1] & 0x01, m_chr_source);
chr4_4(m_reg[2] & 0x01, m_chr_source);
}
else
{
chr8(0, m_chr_source);
}
}

void nes_surom_device::update_regs(int reg)
{
	switch (reg)
	{
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



u8 nes_surom_device::prgram_bank() const
{
return 0;
}

/*-------------------------------------------------
mapper specific handlers
-------------------------------------------------*/

/*-------------------------------------------------

SxROM (MMC1 based) board emulation

iNES: mapper 1 (and 155 for the MMC1A variant which does not
have WRAM disable bit)

-------------------------------------------------*/

void nes_sxrom_device::mmc1_ppu_phase(bool upper_chr, uint16_t ppu_addr)
{
m_mmc1_upper_chr = upper_chr;
m_mmc1_ppu_addr = ppu_addr & 0x1fff;
}

u8 nes_sxrom_device::mmc1_active_chr_reg() const
{
// In 4 KB CHR mode:
// PPU $0000-$0FFF uses R1
// PPU $1000-$1FFF uses R2

// In 8 KB CHR mode:
// R1 controls the 8 KB bank; R2 is ignored.
if (BIT(m_reg[0], 4) && m_mmc1_upper_chr)
return m_reg[2];

return m_reg[1];
}

void nes_sxrom_ext_device::update_regs(int reg)
{
	switch (reg)
	{
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

// Standard MMC1 PRG banking with base and mask (to support multicarts, etc)
void nes_sxrom_device::set_prg(int prg_base, int prg_mask)
{
if (get_submapper() == 5)
{
prg32(0);
return;
}

u8 bank = prg_base | (m_reg[3] & prg_mask);

switch (BIT(m_reg[0], 2, 2))
{
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

void nes_sxrom_device::set_prg()
{
if (get_submapper() == 5)
{
prg32(0);
return;
}

const u8 prg_mode = m_reg[0] & 0x0c;
const u8 bank = (m_prg_chunks > 0) ? ((m_reg[3] & 0x0f) % m_prg_chunks) : 0;
const u8 last = (m_prg_chunks > 0) ? (m_prg_chunks - 1) : 0;

switch (prg_mode)
{
    case 0x00:
    case 0x04:
        prg32(bank >> 1);
        break;

    case 0x08:
        prg16_89ab(0);
        prg16_cdef(bank);
        break;

    case 0x0c:
        prg16_89ab(bank);
        prg16_cdef(last);
        break;
}
}

// Standard MMC1 CHR banking with base and mask (to support multicarts, etc)
void nes_sxrom_device::set_chr(int chr_base, int chr_mask)
{
if (BIT(m_reg[0], 4))
{
chr4_0(chr_base | (m_reg[1] & chr_mask), m_chr_source);
chr4_4(chr_base | (m_reg[2] & chr_mask), m_chr_source);
}
else
{
chr8((chr_base | (m_reg[1] & chr_mask)) >> 1, m_chr_source);
}
}

void nes_sxrom_device::set_mirror()
{
	static constexpr u8 mirr[4] = { PPU_MIRROR_LOW, PPU_MIRROR_HIGH, PPU_MIRROR_VERT, PPU_MIRROR_HORZ };
	set_nt_mirroring(mirr[m_reg[0] & 0x03]);
}

// this allows for easier implementation of MMC1 subclasses
void nes_sxrom_device::update_regs(int reg)
{
	switch (reg)
	{
		case 0:
			set_chr();
			set_prg();
			set_mirror();
			break;

		case 1:
		case 2:
			set_chr();
			break;

		case 3:
			set_prg();
			break;
	}
}

bool nes_sxrom_device::mmc1_ignore_serial_write(u64 now_cpu) const
{
if (!m_last_mmc1_write_valid)
return false;

return (now_cpu - m_last_mmc1_write_cpu_cycle) == 1;
}

void nes_sxrom_device::mmc1_record_write_cycle(u64 now_cpu)
{
m_last_mmc1_write_cpu_cycle = now_cpu;
m_last_mmc1_write_valid = true;
}

bool nes_sxrom_device::prgram_enabled() const
{
// MMC1A ignores PRG-RAM disable bit
if (m_mmc1_type == mmc1_type::MMC1A)
return true;

// MMC1B/MMC1-compatible behavior:
// PRG register bit 4 disables PRG-RAM when set
return !BIT(m_reg[3], 4);
}

u8 nes_sxrom_device::prgram_bank() const
{
// Base SxROM logic currently uses CHR register high bits for banking
//return BIT(m_reg[1], 2, 2);
return 0;
}

void nes_sxrom_device::write_h(offs_t offset, u8 data)
{
LOG("sxrom write_h, offset: %04x, data: %02x\n", offset, data);

if (!m_maincpu6502)
{
	logerror("Cannot find CPU in mmc1.cpp->write_h()\n");
	return;
}

const u64 now_cpu = m_maincpu6502->total_cycles();

if (data & 0x80)
{
	m_latch = 0;
	m_count = 0;
	m_reg[0] |= 0x0c;

	mmc1_record_write_cycle(now_cpu);
	set_prg();
	return;
}

if (mmc1_ignore_serial_write(now_cpu))
{
	logerror("sxrom write_h ignored (consecutive serial write): now=%llu last=%llu data=%02x\n",
		(unsigned long long)now_cpu,
		(unsigned long long)m_last_mmc1_write_cpu_cycle,
		data);
	return;
}

mmc1_record_write_cycle(now_cpu);

m_latch >>= 1;
m_latch |= (data & 0x01) << 4;
++m_count;

if (m_count == 5)
{
	const int reg = BIT(offset, 13, 2);
	m_reg[reg] = m_latch & 0x1f;

	m_latch = 0;
	m_count = 0;

	update_regs(reg);
}
}

void nes_sxrom_device::write_m(offs_t offset, u8 data)
{
const u8 bank = prgram_bank();
LOG("sxrom write_m, offset: %04x, data: %02x\n", offset, data);

if (prgram_enabled())
{
	if (!m_battery.empty())
		m_battery[((bank * 0x2000) + offset) & (m_battery.size() - 1)] = data;
	if (!m_prgram.empty())
		m_prgram[((bank * 0x2000) + offset) & (m_prgram.size() - 1)] = data;
}
}

u8 nes_sxrom_device::read_m(offs_t offset)
{
const u8 bank = prgram_bank();
LOG("sxrom read_m, offset: %04x\n", offset);

if (prgram_enabled())
{
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

void nes_sxrom_ext_device::set_prg()
{
const u8 active = mmc1_active_chr_reg();

const u8 prg_base = (m_prg_chunks > 16 && BIT(active, 4)) ? 0x10 : 0x00;

nes_sxrom_device::set_prg(prg_base, 0x0f);
}

void nes_sxrom_ext_device::set_chr()
{
// True SXROM has CHR-RAM.
// High CHR bits are reused for PRG-ROM/PRG-RAM banking,
// so do not let bit 3/4 affect CHR-RAM banking.
if (BIT(m_reg[0], 4))
{
chr4_0(m_reg[1] & 0x01, m_chr_source);
chr4_4(m_reg[2] & 0x01, m_chr_source);
}
else
{
chr8(0, m_chr_source);
}
}

u8 nes_sxrom_ext_device::prgram_bank() const
{
const u8 active = mmc1_active_chr_reg();

if ((!m_battery.empty() && m_battery.size() > 0x2000) ||
	(!m_prgram.empty() && m_prgram.size() > 0x2000))
	return BIT(active, 2, 2);

return 0;
}

/*-------------------------------------------------

SNROM (MMC1 based) board emulation

iNES: mapper 1

Variant of SxROM where MMC1 output lines are repurposed
for board-specific WRAM control instead of using only the
generic MMC1 PRG-RAM disable behavior.

-------------------------------------------------*/
void nes_snrom_device::set_prg()
{
u8 prg_mode = m_reg[0] & 0x0c;
u8 bank = m_reg[3] & 0x0f;

// SNROM does NOT use reg[1] bit 4 as PRG offset.
switch (prg_mode)
{
	case 0x00:
	case 0x04:
		prg32(bank >> 1);
		break;

	case 0x08:
		prg16_89ab(0);
		prg16_cdef(bank);
		break;

	case 0x0c:
		prg16_89ab(bank);
		prg16_cdef(0x0f);
		break;
}
}

void nes_snrom_device::set_chr()
{
// On SNROM, bit 4 of R1/R2 is repurposed for WRAM /CE via CHR A16.
// It must NOT affect CHR bank selection.
if (BIT(m_reg[0], 4))
{
chr4_0(m_reg[1] & 0x0f, m_chr_source);
chr4_4(m_reg[2] & 0x0f, m_chr_source);
}
else
{
chr8((m_reg[1] & 0x0f) >> 1, m_chr_source);
}
}

bool nes_snrom_device::prgram_enabled() const
{
// Keep simple for now; actual access is handled directly in read_m/write_m
return true;
}

u8 nes_snrom_device::prgram_bank() const
{
return 0;
}

u8 nes_snrom_device::read_m(offs_t offset)
{
bool enabled = true;

if (m_mmc1_type != mmc1_type::MMC1A)
	enabled = !BIT(mmc1_active_chr_reg(), 4);

LOG("snrom read_m off=%04x enabled=%d upper_chr=%d ppu_addr=%04x r1=%02x r2=%02x active=%02x\n",
	offset, enabled ? 1 : 0, m_mmc1_upper_chr ? 1 : 0, m_mmc1_ppu_addr,
	m_reg[1], m_reg[2], mmc1_active_chr_reg());

if (enabled)
{
	if (!m_battery.empty())
		return m_battery[offset & (m_battery.size() - 1)];
	if (!m_prgram.empty())
		return m_prgram[offset & (m_prgram.size() - 1)];
}

return get_open_bus();
}

void nes_snrom_device::write_m(offs_t offset, u8 data)
{
bool enabled = true;

if (m_mmc1_type != mmc1_type::MMC1A)
	enabled = !BIT(mmc1_active_chr_reg(), 4);

LOG("snrom write_m off=%04x data=%02x enabled=%d upper_chr=%d ppu_addr=%04x r1=%02x r2=%02x active=%02x\n",
	offset, data, enabled ? 1 : 0, m_mmc1_upper_chr ? 1 : 0, m_mmc1_ppu_addr,
	m_reg[1], m_reg[2], mmc1_active_chr_reg());

if (enabled)
{
	if (!m_battery.empty())
		m_battery[offset & (m_battery.size() - 1)] = data;
	if (!m_prgram.empty())
		m_prgram[offset & (m_prgram.size() - 1)] = data;
}
}

/*-------------------------------------------------

SOROM (MMC1 based) board emulation

iNES: mapper 1

Variant of SxROM with 16 KiB of PRG-RAM split into
two 8 KiB banks. One bank is normal WRAM and the other
is battery-backed WRAM.

CHR register bit 3 is repurposed as the PRG-RAM bank
select line instead of being used for normal CHR banking.

-------------------------------------------------*/
void nes_sorom_device::write_m(offs_t offset, u8 data)
{
const u8 bank = BIT(mmc1_active_chr_reg(), 3);

LOG("sorom write_m, offset: %04x, data: %02x bank=%d r1=%02x r3=%02x\n",
	offset, data, bank, m_reg[1], m_reg[3]);

if (!BIT(m_reg[3], 4) || m_mmc1_type == mmc1_type::MMC1A)
{
	if (bank)
	{
		if (!m_battery.empty())
			m_battery[offset & (m_battery.size() - 1)] = data;
	}
	else
	{
		if (!m_prgram.empty())
			m_prgram[offset & (m_prgram.size() - 1)] = data;
	}
}
}

u8 nes_sorom_device::read_m(offs_t offset)
{
const u8 bank = BIT(mmc1_active_chr_reg(), 3);

LOG("sorom read_m, offset: %04x bank=%d r1=%02x r3=%02x\n",
	offset, bank, m_reg[1], m_reg[3]);

if (!BIT(m_reg[3], 4) || m_mmc1_type == mmc1_type::MMC1A)
{
	if (bank)
	{
		if (!m_battery.empty())
			return m_battery[offset & (m_battery.size() - 1)];
	}
	else
	{
		if (!m_prgram.empty())
			return m_prgram[offset & (m_prgram.size() - 1)];
	}
}

return get_open_bus();
}

void nes_sorom_device::set_chr()
{
if (BIT(m_reg[0], 4))
{
chr4_0(m_reg[1] & 0x01, m_chr_source);
chr4_4(m_reg[2] & 0x01, m_chr_source);
}
else
{
chr8(0, m_chr_source);
}
}

/*-------------------------------------------------

SZROM (MMC1 based) board emulation

iNES: mapper 1

Variant of SxROM with CHR-ROM banking and 16 KiB of
PRG-RAM split into two 8 KiB banks. One bank is normal
WRAM and the other is battery-backed WRAM.

CHR register bit 4 / CHR A16 is repurposed as the
PRG-RAM bank select line, while normal MMC1 CHR-ROM
banking remains active.

-------------------------------------------------*/
u8 nes_szrom_device::prgram_bank() const
{
return BIT(mmc1_active_chr_reg(), 4);
}

void nes_szrom_device::write_m(offs_t offset, u8 data)
{
const u8 bank = prgram_bank();

LOG("szrom write_m off=%04x data=%02x bank=%d r0=%02x r1=%02x r2=%02x r3=%02x\n",
	offset, data, bank, m_reg[0], m_reg[1], m_reg[2], m_reg[3]);

if (!BIT(m_reg[3], 4) || m_mmc1_type == mmc1_type::MMC1A)
{
	if (bank)
	{
		if (!m_battery.empty())
			m_battery[offset & (m_battery.size() - 1)] = data;
	}
	else
	{
		if (!m_prgram.empty())
			m_prgram[offset & (m_prgram.size() - 1)] = data;
	}
}
}

u8 nes_szrom_device::read_m(offs_t offset)
{
const u8 bank = prgram_bank();

LOG("szrom read_m off=%04x bank=%d r0=%02x r1=%02x r2=%02x r3=%02x\n",
	offset, bank, m_reg[0], m_reg[1], m_reg[2], m_reg[3]);

if (!BIT(m_reg[3], 4) || m_mmc1_type == mmc1_type::MMC1A)
{
	if (bank)
	{
		if (!m_battery.empty())
			return m_battery[offset & (m_battery.size() - 1)];
	}
	else
	{
		if (!m_prgram.empty())
			return m_prgram[offset & (m_prgram.size() - 1)];
	}
}

return get_open_bus();
}
