// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************

 NES/Famicom cartridge emulation for Nintendo MMC-5 / ExROM

 Here we emulate the Nintendo MMC-5 / ExROM PCBs [mapper 5]

 ***********************************************************************************************************/

#include "emu.h"
#include "mmc5.h"
#include "cpu/m6502/m6502.h"

#include "speaker.h"

#define LOG_UNHANDLED (1U << 1)
#define LOG_BANK_INFO (1U << 2)

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_UNHANDLED | LOG_GENERAL)
#else
#define VERBOSE (LOG_UNHANDLED)
#endif
#include "logmacro.h"

#define LAST_CHR_REG_A 0
#define LAST_CHR_REG_B 1

static const int m_mmc5_attrib[4] = {0x00, 0x55, 0xaa, 0xff};

//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_EXROM, nes_exrom_device, "nes_exrom", "NES Cart ExROM (MMC-5) PCB")

nes_exrom_device::nes_exrom_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_EXROM, tag, owner, clock)
	, m_irq_count(0)
	, m_irq_enable(0)
	, m_mult1(0)
	, m_mult2(0)
	, m_floodtile(0)
	, m_floodattr(0)
	, m_prg_mode(0)
	, m_chr_mode(0)
	, m_wram_protect_1(0)
	, m_wram_protect_2(0)
	, m_exram_control(3)
	, m_wram_base(0)
	, m_last_chr(0)
	, m_ex1_chr(0)
	, m_ex1_bank(0)
	, m_ex1_attrib(0)
	, m_high_chr(0)
	, m_split_scr(0)
	, m_split_rev(0)
	, m_split_ctrl(0)
	, m_split_yst(0)
	, m_split_y_counter(0)
	, m_split_bank(0)
	, irq_pending(false)
	, scanline_cnt(0)
	, in_frame(false)
	, m_split_fetch_tile(0)
	, m_split_fetch_row(0)
	, m_split_active_fetch(false)
	, m_ppu_idle_dots(0)
	, ppu_is_reading(false)
	, m_mmc5_ppuctrl(0)
	, m_mmc5_ppumask(0)
	, m_mmc5_subs_enabled(false)
	, m_mmc5_sprite_8x16(false)
	, mmc5_match_count(0)
	, mmc5_last_ppu_addr(0xffff)
	, delay_irq(0)
	, m_phase_nt(false)
	, m_phase_at(false)
	, m_phase_bg_pattern(false)
	, m_phase_spr_pattern(false)
	, m_sound(*this, "mmc5snd")
	, m_ppu_read_count(0)
	, m_ppu_fetch_locked(false) {
}

nes_exrom_device::~nes_exrom_device() {
}

void nes_exrom_device::device_start() {
	common_start();
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_enable));
	save_item(NAME(m_mult1));
	save_item(NAME(m_mult2));
	save_item(NAME(m_floodtile));
	save_item(NAME(m_floodattr));
	save_item(NAME(m_prg_mode));
	save_item(NAME(m_chr_mode));
	save_item(NAME(m_wram_protect_1));
	save_item(NAME(m_wram_protect_2));
	save_item(NAME(m_exram_control));
	save_item(NAME(m_wram_base));
	save_item(NAME(m_vrom_bank));
	save_item(NAME(m_last_chr));
	save_item(NAME(m_ex1_chr));
	save_item(NAME(m_prg_regs));
	save_item(NAME(m_prg_ram_mapped));
	save_item(NAME(m_ex1_bank));
	save_item(NAME(m_ex1_attrib));
	save_item(NAME(m_high_chr));
	save_item(NAME(m_split_scr));
	save_item(NAME(m_split_rev));
	save_item(NAME(m_split_ctrl));
	save_item(NAME(m_split_yst));
	save_item(NAME(m_split_y_counter));
	save_item(NAME(m_split_bank));
	save_item(NAME(m_exram));
	save_item(NAME(m_ram_hi_banks));
	save_item(NAME(m_mmc5_ppuctrl));
	save_item(NAME(m_mmc5_ppumask));
	save_item(NAME(m_mmc5_subs_enabled));
	save_item(NAME(m_mmc5_sprite_8x16));

	save_item(NAME(irq_pending));
	save_item(NAME(scanline_cnt));
	save_item(NAME(in_frame));
	save_item(NAME(m_split_fetch_tile));
	save_item(NAME(m_split_fetch_row));
	save_item(NAME(m_split_active_fetch));
	save_item(NAME(m_ppu_idle_dots));
	save_item(NAME(ppu_is_reading));
	save_item(NAME(mmc5_match_count));
	save_item(NAME(mmc5_last_ppu_addr));
	save_item(NAME(delay_irq));
	save_item(NAME(m_ppu_read_count));
	save_item(NAME(m_ppu_fetch_locked));

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	m_cpu_cycle_timer = timer_alloc(FUNC(nes_exrom_device::cpu_cycle_tick), this);

	const attotime cpu_cycle = m_maincpu6502->clocks_to_attotime(1);
	m_cpu_cycle_timer->adjust(cpu_cycle, 0, cpu_cycle);

	m_phase_nt = false;
	m_phase_at = false;
	m_phase_bg_pattern = false;
	m_phase_spr_pattern = false;
}

void nes_exrom_device::pcb_reset() {
	prg16_89ab(m_prg_chunks - 2);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	m_irq_count = 0;
	m_irq_enable = 0;

	m_mult1 = m_mult2 = 0xff;

	m_floodtile = m_floodattr = 0;

	m_prg_mode = 3;
	m_chr_mode = 0;
	m_wram_base = 0;
	m_wram_protect_1 = 0x01;
	m_wram_protect_2 = 0x02;
	m_exram_control = 0x03;
	m_maincpu6502->set_m_exram_control(m_exram_control);
	m_high_chr = 0;
	m_split_scr = 0;
	m_split_rev = 0;
	m_split_ctrl = 0;
	m_split_yst = 0;
	m_split_y_counter = 0;
	m_split_bank = 0;
	m_last_chr = LAST_CHR_REG_A;
	m_ex1_chr = 0;
	m_ex1_bank = 0;
	m_ex1_attrib = 0;

	for (auto &elem : m_vrom_bank)
		elem = 0x3ff;

	m_prg_regs[0] = 0xfc;
	m_prg_regs[1] = 0xfd;
	m_prg_regs[2] = 0xfe;
	m_prg_regs[3] = 0xff;
	m_prg_ram_mapped[0] = 0;
	m_prg_ram_mapped[1] = 0;
	m_prg_ram_mapped[2] = 0;
	m_prg_ram_mapped[3] = 0;

	m_ram_hi_banks[0] = 0;
	m_ram_hi_banks[1] = 0;
	m_ram_hi_banks[2] = 0;
	m_ram_hi_banks[3] = 0;

	irq_pending = false;
	scanline_cnt = 0;
	in_frame = false;
	m_split_fetch_tile = 0;
	m_split_fetch_row = 0;
	m_split_active_fetch = false;
	m_ppu_idle_dots = 0;
	ppu_is_reading = false;
	m_mmc5_ppuctrl = 0;
	m_mmc5_ppumask = 0;
	m_mmc5_subs_enabled = false;
	m_mmc5_sprite_8x16 = false;
	mmc5_match_count = 0;
	mmc5_last_ppu_addr = 0xffff;
	delay_irq = 0;
	m_phase_nt = false;
	m_phase_at = false;
	m_phase_bg_pattern = false;
	m_phase_spr_pattern = false;
	
	m_ppu_read_count = 0;
	m_ppu_fetch_locked = false;
}

/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 ExROM (MMC5 based) board emulation

 Games: Castlevania III, Just Breed, many Koei titles

 iNES: mapper 5

 MAME status: Supported

 -------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_exrom_device::cpu_cycle_tick) {
	mmc5_cpu_cycle();
}

void nes_exrom_device::update_prg() {
	int bank0, bank1, bank2, bank3;

	switch (m_prg_mode) {
	case 0: // 32k banks
		bank3 = m_prg_regs[3] >> 2;
		prg32(bank3);
		break;

	case 1: // 16k banks
		bank1 = m_prg_regs[1] >> 1;
		bank3 = m_prg_regs[3] >> 1;

		if (m_prg_ram_mapped[1]) {
			m_ram_hi_banks[0] = ((bank1 << 1) & 0x07);
			m_ram_hi_banks[1] = ((bank1 << 1) & 0x07) | 1;
		} else
			prg16_89ab(bank1);

		prg16_cdef(bank3);
		break;

	case 2: // 16k-8k banks
		bank1 = m_prg_regs[1] >> 1;
		bank2 = m_prg_regs[2];
		bank3 = m_prg_regs[3];

		if (m_prg_ram_mapped[1]) {
			m_ram_hi_banks[0] = ((bank1 << 1) & 0x07);
			m_ram_hi_banks[1] = ((bank1 << 1) & 0x07) | 1;
		} else
			prg16_89ab(bank1);

		if (m_prg_ram_mapped[2])
			m_ram_hi_banks[2] = (bank2 & 0x07);
		else
			prg8_cd(bank2);

		prg8_ef(bank3);
		break;

	case 3: // 8k banks
		bank0 = m_prg_regs[0];
		bank1 = m_prg_regs[1];
		bank2 = m_prg_regs[2];
		bank3 = m_prg_regs[3];

		if (m_prg_ram_mapped[0])
			m_ram_hi_banks[0] = (bank0 & 0x07);
		else
			prg8_89(bank0);

		if (m_prg_ram_mapped[1])
			m_ram_hi_banks[1] = (bank1 & 0x07);
		else
			prg8_ab(bank1);

		if (m_prg_ram_mapped[2])
			m_ram_hi_banks[2] = (bank2 & 0x07);
		else
			prg8_cd(bank2);

		prg8_ef(bank3);
		break;
	}
}

void nes_exrom_device::mmc5_recompute_ppu_latches() {
	// MMC5 substitution features care whether BG or SPR rendering is enabled
	// from MMC5's own view of REAL $2001.
	m_mmc5_subs_enabled = (m_mmc5_ppumask & 0x18) != 0;

	// PPUCTRL bit 5 = 1 means 8x16 sprites
	m_mmc5_sprite_8x16 = BIT(m_mmc5_ppuctrl, 5);
}

void nes_exrom_device::mmc5_real_ppuctrl_write(uint8_t data) {
	m_mmc5_ppuctrl = data;
	mmc5_recompute_ppu_latches();
}

void nes_exrom_device::mmc5_real_ppumask_write(uint8_t data) {
	m_mmc5_ppumask = data;
	mmc5_recompute_ppu_latches();
}

bool nes_exrom_device::mmc5_substitution_active() {
	// MMC5's own gating, not just live PPU rendering.
	return m_mmc5_subs_enabled;
}

bool nes_exrom_device::mmc5_split_allowed() {
	// Split is disabled in ExRAM modes 2/3, and also when MMC5 believes
	// substitutions are off.
	if (!m_mmc5_subs_enabled)
		return false;

	if (m_exram_control & 0x02)
		return false;

	return m_split_scr != 0;
}

bool nes_exrom_device::mmc5_exattr_allowed() {
	return m_mmc5_subs_enabled && (m_exram_control == 1);
}

bool nes_exrom_device::mmc5_use_extended_sprite_banking() {
	return m_mmc5_subs_enabled && m_mmc5_sprite_8x16;
}

void nes_exrom_device::mmc5_set_in_frame() {
	if (!in_frame) {
		in_frame = true;
		scanline_cnt = 0;

		// The vertical-split counter is separate from the IRQ
		// scanline counter and is loaded from $5201 at frame start.
		m_split_y_counter = m_split_yst;

		irq_pending = false;
		set_irq_line(CLEAR_LINE);
		m_maincpu6502->cancel_delayed_mapper_irq();
		delay_irq = 0;
	} else {
		++scanline_cnt;

		// Normal vertical scrolling wraps from 239 to 0.
		// Values 240-255 continue through the attribute-table area
		// and then wrap naturally from 255 to 0.
		if (m_split_y_counter == 239)
			m_split_y_counter = 0;
		else
			++m_split_y_counter;
	}

	if (m_irq_count != 0 && scanline_cnt == m_irq_count) {
		irq_pending = true;

		if (m_irq_enable)
			delay_irq = 2;
	}
}

void nes_exrom_device::mmc5_end_frame() {
	in_frame = false;
	mmc5_last_ppu_addr = 0xffff;
	mmc5_match_count = 0;
	m_ppu_read_count = 0;
	m_ppu_fetch_locked = false;
	m_ppu_idle_dots = 0;
	ppu_is_reading = false;
	m_phase_nt = false;
	m_phase_at = false;
	m_phase_bg_pattern = false;
	m_phase_spr_pattern = false;
	m_split_active_fetch = false;
	m_split_fetch_tile = 0;
	m_split_fetch_row = 0;
}

void nes_exrom_device::mmc5_clock_ppu_read(uint16_t ppu_addr_bus) {
	ppu_is_reading = true;

	const uint16_t addr = ppu_addr_bus & 0x3fff;
	bool scanline_sync = false;

	m_phase_nt = false;
	m_phase_at = false;
	m_phase_bg_pattern = false;
	m_phase_spr_pattern = false;

	// Detect three consecutive reads from the same nametable address.
	if (addr >= 0x2000 && addr <= 0x2fff && addr == mmc5_last_ppu_addr) {
		++mmc5_match_count;

		if (mmc5_match_count == 2) {
			scanline_sync = true;
			mmc5_set_in_frame();
		}
	} else {
		mmc5_match_count = 0;
	}

	mmc5_last_ppu_addr = addr;

	// The third matching nametable read is the first background
	// nametable fetch of the new scanline.
	if (scanline_sync) {
		m_ppu_read_count = 0;
		m_ppu_fetch_locked = true;
	}

	// Until MMC5 recognizes a scanline, reads are treated as PPUDATA or
	// other non-rendering accesses and use the last-written CHR set.
	if (!m_ppu_fetch_locked)
		return;

	// Reads 0-127 are the 32 visible background tile fetches.
	if (m_ppu_read_count < 128) {
		switch (m_ppu_read_count & 0x03) {
			case 0:
				m_phase_nt = true;
				break;

			case 1:
				m_phase_at = true;
				break;

			case 2:
			case 3:
				m_phase_bg_pattern = true;
				break;
		}
	}

	// Reads 128-159 are the eight sprite fetch slots.
	// The first two reads in each slot are garbage nametable reads.
	else if (m_ppu_read_count < 160) {
		switch (m_ppu_read_count & 0x03) {
			case 2:
			case 3:
				m_phase_spr_pattern = true;
				break;
		}
	}

	// Reads 160-167 prefetch the first two background tiles
	// for the following scanline.
	else if (m_ppu_read_count < 168) {
		switch (m_ppu_read_count & 0x03) {
			case 0:
				m_phase_nt = true;
				break;

			case 1:
				m_phase_at = true;
				break;

			case 2:
			case 3:
				m_phase_bg_pattern = true;
				break;
		}
	}

	// Extended attributes are latched during genuine background
	// nametable fetches derived from the MMC5 read counter.
	if (m_phase_nt) {
		if (mmc5_exattr_allowed() && addr >= 0x2000 && addr <= 0x2fff) {
			uint8_t ex = m_exram[addr & 0x03ff];

			m_ex1_chr = 1;
			m_ex1_bank = ex & 0x3f;
			m_ex1_attrib = m_mmc5_attrib[(ex >> 6) & 0x03];
		} else {
			m_ex1_chr = 0;
		}
	}

	++m_ppu_read_count;
}

void nes_exrom_device::mmc5_cpu_cycle() {
	if (ppu_is_reading) {
		m_ppu_idle_dots = 0;
	} else {
		if (in_frame || mmc5_last_ppu_addr != 0xffff || mmc5_match_count != 0) {
			++m_ppu_idle_dots;

			if (m_ppu_idle_dots == 3) {
				mmc5_end_frame();
			}
		}
	}

	ppu_is_reading = false;
}

void nes_exrom_device::mmc5_reset_scanline_irq_state() {
	in_frame = false;
	irq_pending = false;
	scanline_cnt = 0;
	mmc5_last_ppu_addr = 0xffff;
	mmc5_match_count = 0;
	m_ppu_read_count = 0;
	m_ppu_fetch_locked = false;
	m_ppu_idle_dots = 0;
	ppu_is_reading = false;
	m_phase_nt = false;
	m_phase_at = false;
	m_phase_bg_pattern = false;
	m_phase_spr_pattern = false;
	m_split_active_fetch = false;
	m_split_fetch_tile = 0;
	m_split_fetch_row = 0;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();
	delay_irq = 0;
}

void nes_exrom_device::ppu_to_mapper(int, unsigned, int, uint16_t ppu_address) {
	bool queue_irq = false;

	if (delay_irq > 0) {
		--delay_irq;
		if (delay_irq == 0) {
			queue_irq = true;
		}
	}

	if (m_sound->clock_irq_delay()) {
		queue_irq = true;
	}

	if (queue_irq) {
		m_maincpu6502->queue_delayed_mapper_irq(2);
	}
}

void nes_exrom_device::set_mirror(int page, int src) {
	switch (src) {
	case 0:
		set_nt_page(page, CIRAM, 0, 1);
		break;
	case 1:
		set_nt_page(page, CIRAM, 1, 1);
		break;
	case 2:
		set_nt_page(page, EXRAM, 0, 1);
		break;
	case 3:
		set_nt_page(page, MMC5FILL, 0, 0);
		break;
	default:
		fatalerror("This should never happen\n");
	}
}

inline int nes_exrom_device::current_bg_tile() {
	if (!m_ppu_fetch_locked || m_ppu_read_count == 0)
		return -1;

	const uint16_t current_read = m_ppu_read_count - 1;

	// Visible background fetch region. Because of the PPU pipeline,
	// these fetches begin with displayed tile column 2.
	if (current_read < 128 && (current_read & 0x03) == 0)
		return ((current_read >> 2) + 2) & 0x1f;

	// Background prefetch region for tile columns 0 and 1
	// of the following scanline.
	if (current_read >= 160 && current_read < 168 && ((current_read - 160) & 0x03) == 0)
		return (current_read - 160) >> 2;

	return -1;
}

inline bool nes_exrom_device::in_split(int render_tile) {
	if (render_tile < 0 || render_tile >= 32)
		return false;

	if (!m_split_rev)
		return render_tile < m_split_ctrl;

	return render_tile >= m_split_ctrl;
}

uint8_t nes_exrom_device::nt_r(offs_t offset) {
	int page = ((offset & 0xc00) >> 10);
	const bool isNtFetch = m_phase_nt;
	const bool isAttrFetch = m_phase_at;

	if (isNtFetch) {
		m_split_active_fetch = false;
	}

	// MMC5 vertical split handling first
	if (mmc5_split_allowed()) {
		const uint8_t split_tile_y = (m_split_y_counter >> 3) & 0x1f;
		const uint8_t split_row = m_split_y_counter & 0x07;
	
		if (isNtFetch) {
			const int render_tile = current_bg_tile();

			if (in_split(render_tile)) {
				m_split_active_fetch = true;
				m_split_fetch_tile = render_tile & 0x1f;
				m_split_fetch_row = split_row;

				uint16_t exram_nt = ((split_tile_y << 5) | m_split_fetch_tile) & 0x03ff;
				return m_exram[exram_nt];
			}
		} else if (isAttrFetch && m_split_active_fetch) {
			const uint16_t exram_at = 0x03c0 | ((split_tile_y & 0x1c) << 1) | (m_split_fetch_tile >> 2);
			const uint8_t attr = m_exram[exram_at & 0x03ff];
			const uint8_t shift = ((split_tile_y & 0x02) << 1) | (m_split_fetch_tile & 0x02);
			const uint8_t palette = (attr >> shift) & 0x03;

			return m_mmc5_attrib[palette];
		}
	}

	// Extended attribute mode
	if (mmc5_exattr_allowed()) {
		if (isAttrFetch)
			return m_ex1_attrib;
	}

	switch (m_nt_src[page]) {
	case MMC5FILL:
		return ((offset & 0x3ff) >= 0x3c0) ? m_floodattr : m_floodtile;

	case EXRAM:
		if (!BIT(m_exram_control, 1)) // modes 0/1
			return m_exram[offset & 0x3ff];
		else
			return 0x00; // modes 2/3

	case CIRAM:
	default:
		break;
	}

	return m_nt_access[page][offset & 0x3ff];
}

void nes_exrom_device::nt_w(offs_t offset, uint8_t data) {
	int page = BIT(offset, 10, 2);

	if (!m_nt_writable[page])
		return;

	switch (m_nt_src[page]) {
	case EXRAM:
		if (!BIT(m_exram_control, 1))
			m_exram[offset & 0x3ff] = data;
		break;

	case CIRAM:
	default:
		m_nt_access[page][offset & 0x3ff] = data;
		break;
	}
}

inline uint8_t nes_exrom_device::base_chr_r(int bank, uint32_t offset) {
	uint32_t helper = 0;

	switch (m_chr_mode) {
	case 0: // 8 KB
		if (bank < 8)
			helper = ((m_vrom_bank[bank | 7] & 0x3ff) * 0x2000) + (offset & 0x1fff);
		else
			helper = ((m_vrom_bank[bank | 3] & 0x3ff) * 0x2000) + (offset & 0x1fff);
		break;

	case 1: // 4 KB
		helper = ((m_vrom_bank[bank | 3] & 0x3ff) * 0x1000) + (offset & 0x0fff);
		break;

	case 2: // 2 KB
		helper = ((m_vrom_bank[bank | 1] & 0x3ff) * 0x0800) + (offset & 0x07ff);
		break;

	case 3: // 1 KB
		helper = ((m_vrom_bank[bank] & 0x3ff) * 0x0400) + (offset & 0x03ff);
		break;
	}

	return m_vrom[helper & (m_vrom_size - 1)];
}

inline uint8_t nes_exrom_device::split_chr_r(uint32_t offset) {
	uint32_t helper = (m_split_bank * 0x1000) + (offset & 0x3ff8) + (m_split_fetch_row & 0x07);
	return m_vrom[helper & (m_vrom_size - 1)];
}

inline uint8_t nes_exrom_device::bg_ex1_chr_r(uint32_t offset) {
	const uint16_t bank = m_ex1_bank | (m_high_chr << 6);
	const uint32_t helper = (bank * 0x1000) + (offset & 0x0fff);

	return m_vrom[helper & (m_vrom_size - 1)];
}

uint8_t nes_exrom_device::chr_r(offs_t offset) {
	const int bank = offset >> 10;

	const bool isBgPattern = m_phase_bg_pattern;
	const bool isSprPattern = m_phase_spr_pattern;

	// MMC5 split CHR only affects real BG pattern fetches.
	// The split decision was latched during the corresponding NT fetch.
	// Do not recalculate in_split() here, because PTL/PTH occur later in the tile fetch.
	if (mmc5_split_allowed() && m_split_active_fetch && isBgPattern)
		return split_chr_r(offset & 0x0fff);

	// MMC5 ExAttr mode:
	// BG CHR ignores $5101 and ignores $5120-$512B.
	// The ExRAM byte latched during the NT fetch selects a 4KB CHR bank.
	if (mmc5_exattr_allowed() && isBgPattern)
		return bg_ex1_chr_r(offset & 0x0fff);

	// Sprite pattern fetches.
	// Sprites use the normal A register set $5120-$5127.
	if (isSprPattern)
		return base_chr_r(bank & 7, offset & 0x1fff);

	// Background pattern fetches.
	if (isBgPattern) {
		// In 8x16 sprite mode, MMC5 gives BG its second CHR register set:
		// $5128-$512B, mirrored across both pattern tables.
		if (mmc5_use_extended_sprite_banking())
			return base_chr_r((bank & 3) + 8, offset & 0x1fff);

		// Otherwise BG uses the normal A register set.
		return base_chr_r(bank & 7, offset & 0x1fff);
	}

	// CPU $2007 / non-render pattern access fallback.
	// MMC5 uses the last-written CHR register group for PPU data access.
	if (m_last_chr == LAST_CHR_REG_B)
		return base_chr_r((bank & 3) + 8, offset & 0x1fff);

	return base_chr_r(bank & 7, offset & 0x1fff);
}

uint8_t nes_exrom_device::read_l(offs_t offset) {
	LOG("exrom read_l, offset: %04x\n", offset);
	offset += 0x100;

	if ((offset >= 0x1c00) && (offset <= 0x1fff)) {
		// EXRAM
		if (BIT(m_exram_control, 1)) // Modes 2,3 = read
			return m_exram[offset - 0x1c00];
		else {
			 return get_open_bus();   // Modes 0,1 = open bus
		}
	}

	switch (offset) {
	case 0x1010:
	case 0x1015:
		return m_sound->read(offset & 0x1f);

	case 0x1204: {
		uint8_t value = (irq_pending ? 0x80 : 0x00) | (in_frame ? 0x40 : 0x00) | (get_open_bus() & 0x3f);
		if (!machine().side_effects_disabled()) {
			irq_pending = false;
			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			delay_irq = 0;
		}
		return value;
	}

	case 0x1205:
		return (m_mult1 * m_mult2) & 0xff;

	case 0x1206:
		return ((m_mult1 * m_mult2) & 0xff00) >> 8;

	default:
		if (!machine().side_effects_disabled())
			LOGMASKED(LOG_UNHANDLED, "MMC5 uncaught read, offset: %04x\n", offset + 0x4100);
		return get_open_bus();
	}
}

void nes_exrom_device::write_l(offs_t offset, uint8_t data) {
	LOG("exrom write_l, offset: %04x, data: %02x\n", offset, data);
	offset += 0x100;

	if ((offset >= 0x1000) && (offset <= 0x1015)) {
		m_sound->write(offset & 0x1f, data);
		return;
	}

	if ((offset >= 0x1c00) && (offset <= 0x1fff)) {
		const offs_t exram_offs = offset - 0x1c00;

		switch (m_exram_control & 0x03) {
		case 0x00: // ExRAM as extra nametable / write-only during rendering
		case 0x01: // Extended attribute mode / write-only during rendering
			m_exram[exram_offs] = (in_frame && m_mmc5_subs_enabled) ? data : 0x00;
			break;
		case 0x02: // CPU read/write RAM
			m_exram[exram_offs] = data;
			break;

		case 0x03: // CPU read-only
		default:
			break;
		}
		return;
	}

	switch (offset) { // 5100
	case 0x1100:
		m_prg_mode = data & 0x03;
		update_prg();
		LOGMASKED(LOG_BANK_INFO, "MMC5 rom bank mode: %02x\n", data);
		break;

	case 0x1101:
		m_chr_mode = data & 0x03;
		m_ex1_chr = 0;
		LOGMASKED(LOG_BANK_INFO, "MMC5 vrom bank mode: %02x\n", data);
		break;

	case 0x1102:
		m_wram_protect_1 = data & 0x03;
		LOG("MMC5 vram protect 1: %02x\n", data);
		break;

	case 0x1103:
		m_wram_protect_2 = data & 0x03;
		LOG("MMC5 vram protect 2: %02x\n", data);
		break;

	case 0x1104: { // Extra VRAM (EXRAM)
		m_exram_control = data & 0x03;
		m_maincpu6502->set_m_exram_control(m_exram_control);
		break;
	}
	case 0x1105:
		set_mirror(0, (data & 0x03) >> 0);
		set_mirror(1, (data & 0x0c) >> 2);
		set_mirror(2, (data & 0x30) >> 4);
		set_mirror(3, (data & 0xc0) >> 6);
		break;

	case 0x1106:
		m_floodtile = data;
		break;

	case 0x1107:
		m_floodattr = m_mmc5_attrib[data & 3];
		break;

	case 0x1113:
		LOG("MMC5 mid RAM bank select: %02x\n", data & 0x07);
		m_wram_base = data & 0x07;
		break;

	case 0x1114:
	case 0x1115:
	case 0x1116:
	case 0x1117:
		m_prg_regs[offset & 3] = data & 0x7f;
		m_prg_ram_mapped[offset & 3] = !BIT(data, 7); // $5117 is always ROM; slot 3 RAM flag is ignored by read_h/write_h
		update_prg();
		break;

	case 0x1120:
	case 0x1121:
	case 0x1122:
	case 0x1123:
	case 0x1124:
	case 0x1125:
	case 0x1126:
	case 0x1127: {
		const int chr_reg = offset & 0x07;
		m_vrom_bank[chr_reg] = data | (m_high_chr << 8);
		m_last_chr = LAST_CHR_REG_A;
		break;
	}

	case 0x1128:
	case 0x1129:
	case 0x112a:
	case 0x112b:
		m_vrom_bank[offset & 0x0f] = data | (m_high_chr << 8);
		m_last_chr = LAST_CHR_REG_B;
		break;

	case 0x1130:
		m_high_chr = data & 0x03;
		break;

	case 0x1200:
		// in EX2 and EX3 modes, no split screen
		m_split_scr = BIT(data, 7); // Enable vertical split mode
		m_split_rev = BIT(data, 6); // Specify vertical split screen side (0:left; 1:right)
		m_split_ctrl = data & 0x1f; // Specify vertical split start/stop tile
		break;

	case 0x1201:
		m_split_yst = data;
		break;

	case 0x1202:
		m_split_bank = data;
		break;

	case 0x1203:
		m_irq_count = data;
		LOG("MMC5 irq scanline: %d\n", m_irq_count);
		break;

	case 0x1204:
		m_irq_enable = data & 0x80;
		if (m_irq_enable && irq_pending) {
			delay_irq = 2;
		} else {
			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			delay_irq = 0;
		}
		LOG("MMC5 irq enable: %02x\n", data);
		break;

	case 0x1205:
		m_mult1 = data;
		break;

	case 0x1206:
		m_mult2 = data;
		break;

	default:
		LOG("MMC5 uncaught write, offset: %04x, data: %02x\n", offset + 0x4100, data);
		break;
	}
}

// 3 bits are used to access PRG-RAM banks.
// In commercial configs, bits 0-1 select 8K pages within a RAM chip,
// and bit 2 selects between two RAM chips.
// For ETROM, each chip is only 8K, so bits 0-1 mirror within that chip.
uint8_t nes_exrom_device::read_m(offs_t offset) {
	LOG("exrom read_m, offset: %04x\n", offset);

	const int ram_bank = m_wram_base & 0x07;
	const u32 addr = offset & 0x1fff;

	// ETROM: 8K battery RAM + 8K volatile WRAM.
	// MMC5 PRG-RAM bank bit 2 selects the chip.
	// Banks 0-3 mirror battery RAM, banks 4-7 mirror volatile WRAM.
	if (!m_battery.empty() && !m_prgram.empty()) {
		if (ram_bank & 0x04)
			return m_prgram[addr & (m_prgram.size() - 1)];

		return m_battery[addr & (m_battery.size() - 1)];
	}

	// Single-chip volatile WRAM fallback.
	// Commercial single-chip boards normally only respond to banks 0-3.
	// Banks 4-7 select a missing chip, so return open bus.
	if (!m_prgram.empty()) {
		if (ram_bank & 0x04)
			return get_open_bus();

		return m_prgram[((ram_bank & 0x03) * 0x2000 + addr) & (m_prgram.size() - 1)];
	}

	// Single-chip battery WRAM fallback.
	if (!m_battery.empty()) {
		if (ram_bank & 0x04)
			return get_open_bus();

		return m_battery[((ram_bank & 0x03) * 0x2000 + addr) & (m_battery.size() - 1)];
	}

	return get_open_bus();
}

void nes_exrom_device::write_m(offs_t offset, uint8_t data) {
	LOG("exrom write_m, offset: %04x, data: %02x\n", offset, data);

	if (m_wram_protect_1 != 0x02 || m_wram_protect_2 != 0x01)
		return;

	const int ram_bank = m_wram_base & 0x07;
	const u32 addr = offset & 0x1fff;

	// ETROM: 8K battery RAM + 8K volatile WRAM.
	// MMC5 PRG-RAM bank bit 2 selects the chip.
	if (!m_battery.empty() && !m_prgram.empty()) {
		if (ram_bank & 0x04) {
			m_prgram[addr & (m_prgram.size() - 1)] = data;
			return;
		}

		m_battery[addr & (m_battery.size() - 1)] = data;
		return;
	}

	// Single-chip volatile WRAM fallback.
	// Ignore writes when bit 2 selects a missing chip.
	if (!m_prgram.empty()) {
		if (ram_bank & 0x04)
			return;

		m_prgram[((ram_bank & 0x03) * 0x2000 + addr) & (m_prgram.size() - 1)] = data;
		return;
	}

	// Single-chip battery WRAM fallback.
	if (!m_battery.empty()) {
		if (ram_bank & 0x04)
			return;

		m_battery[((ram_bank & 0x03) * 0x2000 + addr) & (m_battery.size() - 1)] = data;
		return;
	}
}

// some games (e.g. Bandit Kings of Ancient China) write to PRG-RAM through 0x8000-0xdfff
uint8_t nes_exrom_device::read_h(offs_t offset) {
	LOG("exrom read_h, offset: %04x\n", offset);

	const int bank = offset / 0x2000;
	bool ram_override = false;

	switch (m_prg_mode) {
	case 0:
		ram_override = false;
		break;

	case 1:
		// $8000-$BFFF controlled by reg 1 as one 16K region.
		ram_override = (bank < 2 && m_prg_ram_mapped[1]);
		break;

	case 2:
		// $8000-$BFFF controlled by reg 1, $C000-$DFFF controlled by reg 2.
		ram_override = (bank < 2 && m_prg_ram_mapped[1]) || (bank == 2 && m_prg_ram_mapped[2]);
		break;

	case 3:
		// $8000, $A000, $C000 individually controlled.
		ram_override = (bank < 3 && m_prg_ram_mapped[bank]);
		break;
	}

	u8 ret = 0;

	if (ram_override) {
		const int ram_bank = m_ram_hi_banks[bank] & 0x07;
		const u32 addr = offset & 0x1fff;

		// ETROM: 8K battery RAM + 8K volatile WRAM.
		// MMC5 PRG-RAM bank bit 2 selects the chip.
		// Banks 0-3 mirror battery RAM, banks 4-7 mirror volatile WRAM.
		if (!m_battery.empty() && !m_prgram.empty()) {
			if (ram_bank & 0x04)
				ret = m_prgram[addr & (m_prgram.size() - 1)];
			else
				ret = m_battery[addr & (m_battery.size() - 1)];
		}
		// Single-chip volatile WRAM fallback.
		// Commercial single-chip boards normally only respond to banks 0-3.
		// Banks 4-7 select a missing chip, so return open bus.
		else if (!m_prgram.empty()) {
			if (ram_bank & 0x04)
				ret = get_open_bus();
			else
				ret = m_prgram[((ram_bank & 0x03) * 0x2000 + addr) & (m_prgram.size() - 1)];
		}
		// Single-chip battery WRAM fallback.
		else if (!m_battery.empty()) {
			if (ram_bank & 0x04)
				ret = get_open_bus();
			else
				ret = m_battery[((ram_bank & 0x03) * 0x2000 + addr) & (m_battery.size() - 1)];
		} else {
			ret = get_open_bus();
		}
	} else {
		ret = hi_access_rom(offset);
	}

	// MMC5 PCM read mode samples CPU reads from $8000-$BFFF only.
	// read_h() offset is CPU $8000-$FFFF as $0000-$7FFF, so bank 0/1 means
	// $8000-$BFFF. This should happen for ROM reads and RAM-override reads.
	if (bank < 2) {
		m_sound->pcm_read(ret);
	}

	return ret;
}

void nes_exrom_device::write_h(offs_t offset, uint8_t data) {
	LOG("exrom write_h, offset: %04x, data: %02x\n", offset, data);

	const int bank = offset / 0x2000;
	bool ram_override = false;

	switch (m_prg_mode) {
	case 0:
		ram_override = false;
		break;

	case 1:
		// $8000-$BFFF controlled by reg 1 as one 16K region.
		ram_override = (bank < 2 && m_prg_ram_mapped[1]);
		break;

	case 2:
		// $8000-$BFFF controlled by reg 1, $C000-$DFFF controlled by reg 2.
		ram_override = (bank < 2 && m_prg_ram_mapped[1]) || (bank == 2 && m_prg_ram_mapped[2]);
		break;

	case 3:
		// $8000, $A000, $C000 individually controlled.
		ram_override = (bank < 3 && m_prg_ram_mapped[bank]);
		break;
	}

	if (m_wram_protect_1 != 0x02 || m_wram_protect_2 != 0x01 || bank == 3 || !ram_override)
		return;

	const int ram_bank = m_ram_hi_banks[bank] & 0x07;
	const u32 addr = offset & 0x1fff;

	// ETROM: 8K battery RAM + 8K volatile WRAM.
	// MMC5 PRG-RAM bank bit 2 selects the chip.
	if (!m_battery.empty() && !m_prgram.empty()) {
		if (ram_bank & 0x04) {
			m_prgram[addr & (m_prgram.size() - 1)] = data;
			return;
		}

		m_battery[addr & (m_battery.size() - 1)] = data;
		return;
	}

	// Single-chip volatile WRAM fallback.
	// Ignore writes when bit 2 selects a missing chip.
	if (!m_prgram.empty()) {
		if (ram_bank & 0x04)
			return;

		m_prgram[((ram_bank & 0x03) * 0x2000 + addr) & (m_prgram.size() - 1)] = data;
		return;
	}

	// Single-chip battery WRAM fallback.
	if (!m_battery.empty()) {
		if (ram_bank & 0x04)
			return;

		m_battery[((ram_bank & 0x03) * 0x2000 + addr) & (m_battery.size() - 1)] = data;
		return;
	}
}

//-------------------------------------------------
//  device_add_mconfig - add device configuration
//-------------------------------------------------
void nes_exrom_device::device_add_mconfig(machine_config &config) {
	SPEAKER(config, "addon").front_center();

	MMC5SND(config, m_sound, XTAL(21'477'272) / 12);
	m_sound->add_route(ALL_OUTPUTS, "addon", 0.90);
}
