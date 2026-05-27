// license:BSD-3-Clause
// copyright-holders:Ernesto Corvi, Brad Oliver, Fabio Priuli
/******************************************************************************

    Nintendo 2C0x PPU emulation.

    Written by Ernesto Corvi.
    This code is heavily based on Brad Oliver's MESS implementation.

    2009-04: Changed NES PPU to be a device (Nathan Woods)
    2009-07: Changed NES PPU to use a device memory map (Robert Bohms)
	2026-04: Total rewrite to be cycle accurate

******************************************************************************/

#include "emu.h"
#include "video/ppu2c0x.h"
#include "cpu/m6502/m6502.h"
#include "sound/nes_apu.h"
#include "bus/nes/mmc5.h"
#include "bus/nes/mmc3.h"
#include "bus/nes/mmc1.h"

#include "screen.h"

//**************************************************************************
//  GLOBAL VARIABLES
//**************************************************************************

// devices
DEFINE_DEVICE_TYPE(PPU_2C02,    ppu2c02_device,       "ppu2c02",    "2C02 PPU")
DEFINE_DEVICE_TYPE(PPU_2C03B,   ppu2c03b_device,      "ppu2c03b",   "2C03B PPC")
DEFINE_DEVICE_TYPE(PPU_2C04,    ppu2c04_device,       "ppu2c04",    "2C04 PPU")
DEFINE_DEVICE_TYPE(PPU_2C07,    ppu2c07_device,       "ppu2c07",    "2C07 PPU")
DEFINE_DEVICE_TYPE(PPU_PALC,    ppupalc_device,       "ppupalc",    "Generic PAL Clone PPU")
DEFINE_DEVICE_TYPE(PPU_2C05_01, ppu2c05_01_device,    "ppu2c05_01", "2C05_01 PPU")
DEFINE_DEVICE_TYPE(PPU_2C05_02, ppu2c05_02_device,    "ppu2c05_02", "2C05_02 PPU")
DEFINE_DEVICE_TYPE(PPU_2C05_03, ppu2c05_03_device,    "ppu2c05_03", "2C05_03 PPU")
DEFINE_DEVICE_TYPE(PPU_2C05_04, ppu2c05_04_device,    "ppu2c05_04", "2C05_04 PPU")
DEFINE_DEVICE_TYPE(PPU_2C04C,   ppu2c04_clone_device, "ppu2c04c",   "2C04 Clone PPU")

// default address map
void ppu2c0x_device::ppu2c0x(address_map& map)
{
	if (!has_configured_map(0))
	{
		map(0x0000, 0x3eff).ram();
		map(0x3f00, 0x3fff).rw(FUNC(ppu2c0x_device::palette_read), FUNC(ppu2c0x_device::palette_write));
	}
}

//-------------------------------------------------
//  memory_space_config - return a description of
//  any address spaces owned by this device
//-------------------------------------------------

device_memory_interface::space_config_vector ppu2c0x_device::memory_space_config() const
{
	return space_config_vector
	{
		std::make_pair(0, &m_space_config)
	};
}


//-------------------------------------------------
//  ppu2c0x_device - constructor
//-------------------------------------------------

void ppu2c0x_device::device_config_complete()
{
	/* reset the callbacks */
	m_scanline_callback_proc.set(nullptr);
	m_hblank_callback_proc.set(nullptr);
	m_vidaccess_callback_proc.set(nullptr);
	m_latch.set(nullptr);
	m_ppu_to_mapper.set(nullptr);
}

ppu2c0x_device::ppu2c0x_device(const machine_config& mconfig, device_type type, const char* tag, device_t* owner, uint32_t clock, address_map_constructor internal_map) :
	device_t(mconfig, type, tag, owner, clock),
	device_memory_interface(mconfig, *this),
	device_video_interface(mconfig, *this),
	m_space_config("videoram", ENDIANNESS_LITTLE, 8, 17, 0, internal_map),
	m_cpu(*this, finder_base::DUMMY_TAG),
	m_scanline(0),  // reset the scanline count
	m_videoram_addr_mask(0x3fff),
	m_global_refresh_mask(0x7fff),
	m_line_write_increment_large(32),
	m_paletteram_in_ppuspace(false),
	m_tile_page(0),
	m_back_color(0),
	m_refresh_data(0),
	m_x_fine(0),
	m_toggle(0),
	m_tilecount(0),
	m_latch(*this),
	m_ppu_to_mapper(*this),
	m_scanline_callback_proc(*this),
	m_hblank_callback_proc(*this),
	m_vidaccess_callback_proc(*this),
	m_int_callback(*this),
	m_refresh_latch(0),
	m_add(1),
	m_videomem_addr(0),
	m_data_latch(0),
	m_buffered_data(0),
	m_sprite_page(0),
	m_scan_scale(1) // set the scan scale (this is for dual monitor vertical setups)
{
	for (auto& elem : m_regs)
		elem = 0;

	m_scanlines_per_frame = NTSC_SCANLINES_PER_FRAME;
	m_vblank_first_scanline = VBLANK_FIRST_SCANLINE;
	m_prerender_line = NTSC_SCANLINES_PER_FRAME - 1;

	/* usually, no security value... */
	m_security_value = 0;
}

ppu2c0x_device::ppu2c0x_device(const machine_config& mconfig, device_type type, const char* tag, device_t* owner, uint32_t clock) :
	ppu2c0x_device(mconfig, type, tag, owner, clock, address_map_constructor(FUNC(ppu2c0x_device::ppu2c0x), this))
{
	m_paletteram_in_ppuspace = true;
}

ppu2c0x_rgb_device::ppu2c0x_rgb_device(const machine_config& mconfig, device_type type, const char* tag, device_t* owner, uint32_t clock) :
	ppu2c0x_device(mconfig, type, tag, owner, clock),
	m_palette_data(*this, "palette")
{
}

// NTSC NES
ppu2c02_device::ppu2c02_device(const machine_config& mconfig, const char* tag, device_t* owner, uint32_t clock) :
	ppu2c0x_device(mconfig, PPU_2C02, tag, owner, clock)
{
}

// Playchoice 10
ppu2c03b_device::ppu2c03b_device(const machine_config& mconfig, const char* tag, device_t* owner, uint32_t clock) :
	ppu2c0x_rgb_device(mconfig, PPU_2C03B, tag, owner, clock)
{}

// Vs. Unisystem
ppu2c04_device::ppu2c04_device(const machine_config& mconfig, const char* tag, device_t* owner, uint32_t clock) :
	ppu2c0x_rgb_device(mconfig, PPU_2C04, tag, owner, clock)
{}

// PAL NES
ppu2c07_device::ppu2c07_device(const machine_config& mconfig, const char* tag, device_t* owner, uint32_t clock) :
	ppu2c0x_device(mconfig, PPU_2C07, tag, owner, clock)
{}

// PAL clones
ppupalc_device::ppupalc_device(const machine_config& mconfig, const char* tag, device_t* owner, uint32_t clock) :
	ppu2c0x_device(mconfig, PPU_PALC, tag, owner, clock)
{}

// The PPU_2C05 variants have different protection value, set at device start, but otherwise are all the same...
// Vs. Unisystem (Ninja Jajamaru Kun)
ppu2c05_01_device::ppu2c05_01_device(const machine_config& mconfig, const char* tag, device_t* owner, uint32_t clock) :
	ppu2c0x_rgb_device(mconfig, PPU_2C05_01, tag, owner, clock)
{}

// Vs. Unisystem (Mighty Bomb Jack)
ppu2c05_02_device::ppu2c05_02_device(const machine_config& mconfig, const char* tag, device_t* owner, uint32_t clock) :
	ppu2c0x_rgb_device(mconfig, PPU_2C05_02, tag, owner, clock)
{}

// Vs. Unisystem (Gumshoe)
ppu2c05_03_device::ppu2c05_03_device(const machine_config& mconfig, const char* tag, device_t* owner, uint32_t clock) :
	ppu2c0x_rgb_device(mconfig, PPU_2C05_03, tag, owner, clock)
{}

// Vs. Unisystem (Top Gun)
ppu2c05_04_device::ppu2c05_04_device(const machine_config& mconfig, const char* tag, device_t* owner, uint32_t clock) :
	ppu2c0x_rgb_device(mconfig, PPU_2C05_04, tag, owner, clock)
{}

// Vs. Unisystem (Super Mario Bros. bootlegs)
ppu2c04_clone_device::ppu2c04_clone_device(const machine_config& mconfig, const char* tag, device_t* owner, uint32_t clock) :
	ppu2c0x_device(mconfig, PPU_2C04C, tag, owner, clock),
	m_palette_data(*this, "palette")
{}

//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void ppu2c0x_device::init_runtime_reset_state()
{
	fine_x = t = v = 0;

	memset(oam, 0x00, sizeof(oam));
	memset(sec_oam, 0x00, sizeof(sec_oam));

	// $2000
    v_inc           = 1;
    sprite_pat_addr = bg_pat_addr = 0x0000;
    sprite_size     = EIGHT_BY_EIGHT;
    nmi_on_vblank   = false;
	suppress_vblank_flag  = false;
	// $2001
	grayscale_color_mask = 0x3F; // Grayscale off
    show_bg_left_8       = show_sprites_left_8 = false;
    tint_bits            = 0;
    bg_clip_comp         = sprite_clip_comp = 256;

	// $2002
	sprite_overflow = false;
	sprite_zero_hit = false;
	in_vblank = false;

	// OAM regs
	oam_eval_addr = 0;
	oam_addr = 0;
	sec_oam_addr = 0;
	oam_data = 0;

	// Misc regs/helpers
	write_flip_flop = false;
	ppu_data_reg = 0;
	s0_on_next_scanline = false;
	s0_on_cur_scanline = false;
	ppu_addr_bus = 0;
	dot = 0;
	scanline = 0;
	m_scanline = 0;

	// Render pipeline buffers / shift registers
	nt_byte = 0;
	at_byte = 0;
	bg_byte_l = 0;
	bg_byte_h = 0;
	bg_shift_l = 0;
	bg_shift_h = 0;
	at_shift_l = 0;
	at_shift_h = 0;
	at_latch_l = 0;
	at_latch_h = 0;

	// Sprite units
	sprite_y = 0;
	sprite_index = 0;
	sprite_in_range = false;
	sprite_eval_in_range = false;
	sprite0_eval_addr = 0;
	sprite0_pat = 0;
	sprite_go_this_line = true;
	sprite_go_next_line = true;
	sprite_force_immediate_applied = false;
	sprite_sl0_early_shift_pending = false;
	sl0_stale_s0_loaded = false;
	sl0_stale_sprite0_identity = false;

	memset(sprite_attribs, 0x00, sizeof(sprite_attribs));
	memset(sprite_x, 0x00, sizeof(sprite_x));
	memset(sprite_pat_l, 0x00, sizeof(sprite_pat_l));
	memset(sprite_pat_h, 0x00, sizeof(sprite_pat_h));

	sec_oam_full = false;
	oam_2004_latch = 0xFF;
	sec_oam_last_write = 0xFF;
	sprite_eval_initialized = false;

	oam_copy_done = false;
	oam_addr_overflow = false;
	sec_oam_addr_overflow = false;
	overflow_detection = false;
	overflow_bug_counter = 0;

	// NMI / PPUCTRL state
	nmi_delay = 0;
	nmi_on_vblank = false;
	suppress_vblank_flag = false;
	nmi_pending = false;

	// Decoded/render state
	bg_output_enabled = 0;
	spr_output_enabled = 0;
	bg_pipeline_enabled = 0;
	spr_pipeline_enabled = 0;
	retro_ppumask_color = false;
	retro_ppumask_render = false;

	// Fetch helpers
	inhibit_bg_shift_one_dot = false;

	bg_pat_addr_tile = 0;
	sprite_nt_fetch_v = 0;
	sprite_nt_fetch_v_new = 0;
	sprite_addr_h = 0;
	sprite_addr_l = 0;
	m_bgfetch_nt_addr = 0;
	m_bgfetch_at_addr = 0;

	// Previous pixel latch state
	prev_pixel_valid = false;
	prev_pixel_scanline = 0;
	prev_pixel_x = 0;
	prev_bg_pixel_pat = 0;
	prev_attr_bits = 0;
	prev_spr_pat = 0;
	prev_spr_pal = 0;
	prev_spr_behind_bg = false;
	prev_spr_is_s0 = false;
	prev_sprite0_pat = 0;
	prev_backdrop_pal_index = 0;

	// Pending delayed effects for $2000, $2001, $2004, and $2006.
	pending_2000.has_pending = false;
	pending_2000.value = 0;
	pending_2000.value16 = 0;
	pending_2000.apply_dot = 0;
	pending_2000.apply_scanline = 0;
	pending_2000.apply_ppu = 0;

	pending_2001.has_pending = false;
	pending_2001.value = 0;
	pending_2001.value16 = 0;
	pending_2001.apply_dot = 0;
	pending_2001.apply_scanline = 0;
	pending_2001.apply_ppu = 0;

	pending_2004.has_pending = false;
	pending_2004.value = 0;
	pending_2004.value16 = 0;
	pending_2004.apply_dot = 0;
	pending_2004.apply_scanline = 0;
	pending_2004.apply_ppu = 0;

	pending_2006.has_pending = false;
	pending_2006.value = 0;
	pending_2006.value16 = 0;
	pending_2006.apply_dot = 0;
	pending_2006.apply_scanline = 0;
	pending_2006.apply_ppu = 0;

	// OAM corruption
	oam_corrupt_pending = false;
	oam_corrupt_seed = 0;

	// $2007 read/write state
	ppu2007_buffer_fill_armed = false;
	ppu2007_buffer_fill_arm_pending = false;
	ppu_bus_read_can_fill_2007 = false;

	ppu2007_post_bump_pending = false;
	ppu2007_post_bump_delay = 0;

	m_2007_write.pending = false;
	m_2007_write.delay = 0;
	m_2007_write.addr = 0;
	m_2007_write.data = 0;

	m_2007_read.pending = false;
	m_2007_read.delay = 0;
	m_2007_read.addr = 0;
	m_2007_read.use_next_ppu_read_for_refill = false;
	m_2007_read.waiting_for_refill_bus_read = false;
	
	scroll_inc_h_pending = false;
	scroll_inc_v_pending = false;
	scroll_copy_h_pending = false;
	scroll_copy_v_pending = false;
	scroll_copy_conflict_pending = false;
	scroll_copy_conflict_h_pending = false;
	scroll_copy_conflict_v_pending = false;
}

void ppu2c0x_device::init_startup_only_state()
{
	// One-time / startup-only state
	m_mapper_number = -1;
	m_mmc5 = nullptr;
	m_mmc3 = nullptr;
	m_mmc1_sxrom = nullptr;
	
	m_has_mmc3_a12 = false;
	m_has_mmc5_ppu = false;
	m_has_mmc1_phase = false;
	m_has_chr_latch = false;

	m_security_value = 0;
	m_tile_page = 0;
	m_prerender_line = 0;
	m_planebuf[0] = 0;
	m_planebuf[1] = 0;

	frame = 0;
	skip_dot = false;
	s_after_wrap = false;
	odd_frame = false;
	m_nmi = false;

	memset(m_sec_is_sprite0, 0, sizeof(m_sec_is_sprite0));
	memset(sprite_x_cnt, 0, sizeof(sprite_x_cnt));
	memset(sprite_shift_count, 0, sizeof(sprite_shift_count));
}

void ppu2c0x_device::reset()
{
	init_runtime_reset_state();
}

void ppu2c0x_device::start_nopalram()
{
	/* allocate a screen bitmap, videomem and spriteram, a dirtychar array and the monochromatic colortable */
	m_bitmap = std::make_unique<bitmap_rgb32>(VISIBLE_SCREEN_WIDTH, VISIBLE_SCREEN_HEIGHT);
	init_palette_tables();
	
	// register for state saving
	save_item(NAME(m_scanline));
	save_item(NAME(m_refresh_data));
	save_item(NAME(m_x_fine));
	save_item(NAME(m_toggle));
	save_item(NAME(m_back_color));
	save_item(NAME(m_scan_scale));
	save_item(NAME(m_scanlines_per_frame));
	save_item(NAME(m_vblank_first_scanline));
	save_item(NAME(m_regs));
	save_item(NAME(*m_bitmap));
	
	init_startup_only_state();
	init_runtime_reset_state();
}

void ppu2c0x_device::device_start()
{
	start_nopalram();
	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");
	m_mmc5 = machine().root_device().subdevice<nes_exrom_device>("nes_slot:exrom");
	// pass blargg_ppu_tests/power_up_palette.nes
	m_palette_ram =
	{
		0x09, 0x01, 0x00, 0x01, 0x00, 0x02, 0x02, 0x0D,
		0x08, 0x10, 0x08, 0x24, 0x00, 0x00, 0x04, 0x2C,
		0x09, 0x01, 0x34, 0x03, 0x00, 0x04, 0x00, 0x14,
		0x08, 0x3A, 0x00, 0x02, 0x00, 0x20, 0x2C, 0x08
	};	
							
	save_item(NAME(m_palette_ram));
	machine().save().register_presave(save_prepost_delegate(FUNC(ppu2c0x_device::presave), this));
	machine().save().register_postload(save_prepost_delegate(FUNC(ppu2c0x_device::postload), this));
	// --- core PPU state ---
	save_item(NAME(m_mapper_number));
	save_item(NAME(m_security_value));
	save_item(NAME(m_tile_page));
	save_item(NAME(bg_pat_addr_tile));
	save_item(NAME(m_planebuf));
	save_item(NAME(m_prerender_line));
	save_item(NAME(frame));
	save_item(NAME(m_has_mmc3_a12));
	save_item(NAME(m_has_mmc5_ppu));
	save_item(NAME(m_has_mmc1_phase));
	save_item(NAME(m_has_chr_latch));

	save_item(NAME(oam));
	save_item(NAME(sec_oam));
	save_item(NAME(m_sec_is_sprite0));

	save_item(NAME(t));
	save_item(NAME(v));
	save_item(NAME(fine_x));
	save_item(NAME(v_inc));
	save_item(NAME(sprite_pat_addr));
	save_item(NAME(bg_pat_addr));
	save_item(NAME(m_save_sprite_size));
	save_item(NAME(nmi_on_vblank));
	save_item(NAME(m_nmi));
	save_item(NAME(nmi_pending));

	save_item(NAME(grayscale_color_mask));
	save_item(NAME(show_bg_left_8));
	save_item(NAME(show_sprites_left_8));
	save_item(NAME(tint_bits));
	save_item(NAME(bg_clip_comp));
	save_item(NAME(sprite_clip_comp));

	save_item(NAME(sprite_overflow));
	save_item(NAME(sprite_zero_hit));
	save_item(NAME(in_vblank));

	save_item(NAME(oam_addr));
	save_item(NAME(sec_oam_addr));
	save_item(NAME(oam_data));
	save_item(NAME(sprite_addr_h));
	save_item(NAME(sprite_addr_l));
	save_item(NAME(oam_copy_done));
	save_item(NAME(overflow_bug_counter));
	save_item(NAME(sprite_eval_in_range));
	save_item(NAME(oam_addr_overflow));
	save_item(NAME(sec_oam_addr_overflow));
	save_item(NAME(overflow_detection));

	save_item(NAME(write_flip_flop));
	save_item(NAME(ppu_data_reg));
	save_item(NAME(odd_frame));
	save_item(NAME(scanline));
	save_item(NAME(dot));

	save_item(NAME(nt_byte));
	save_item(NAME(at_byte));
	save_item(NAME(bg_byte_l));
	save_item(NAME(bg_byte_h));
	save_item(NAME(bg_shift_l));
	save_item(NAME(bg_shift_h));
	save_item(NAME(at_shift_l));
	save_item(NAME(at_shift_h));
	save_item(NAME(at_latch_l));
	save_item(NAME(at_latch_h));

	save_item(NAME(sprite_attribs));
	save_item(NAME(sprite_x));
	save_item(NAME(sprite_x_cnt));
	save_item(NAME(sprite_pat_l));
	save_item(NAME(sprite_pat_h));
	save_item(NAME(sprite_shift_count));

	save_item(NAME(s0_on_next_scanline));
	save_item(NAME(s0_on_cur_scanline));
	save_item(NAME(sprite_y));
	save_item(NAME(sprite_index));
	save_item(NAME(sprite_in_range));

	save_item(NAME(ppu_addr_bus));
	save_item(NAME(nmi_delay));
	save_item(NAME(suppress_vblank_flag));

	// --- delayed register writes / reads ---
	save_item(NAME(pending_2000.has_pending));
	save_item(NAME(pending_2000.value));
	save_item(NAME(pending_2000.value16));
	save_item(NAME(pending_2000.apply_dot));
	save_item(NAME(pending_2000.apply_scanline));
	save_item(NAME(pending_2000.apply_ppu));

	save_item(NAME(pending_2001.has_pending));
	save_item(NAME(pending_2001.value));
	save_item(NAME(pending_2001.value16));
	save_item(NAME(pending_2001.apply_dot));
	save_item(NAME(pending_2001.apply_scanline));
	save_item(NAME(pending_2001.apply_ppu));

	save_item(NAME(pending_2006.has_pending));
	save_item(NAME(pending_2006.value));
	save_item(NAME(pending_2006.value16));
	save_item(NAME(pending_2006.apply_dot));
	save_item(NAME(pending_2006.apply_scanline));
	save_item(NAME(pending_2006.apply_ppu));

	save_item(NAME(pending_2004.has_pending));
	save_item(NAME(pending_2004.value));
	save_item(NAME(pending_2004.value16));
	save_item(NAME(pending_2004.apply_dot));
	save_item(NAME(pending_2004.apply_scanline));
	save_item(NAME(pending_2004.apply_ppu));

	// --- sprite corruption / masking / mapper feed ---
	save_item(NAME(oam_eval_addr));
	save_item(NAME(oam_corrupt_pending));
	save_item(NAME(oam_corrupt_seed));
	save_item(NAME(bg_output_enabled));
	save_item(NAME(spr_output_enabled));
	save_item(NAME(bg_pipeline_enabled));
	save_item(NAME(spr_pipeline_enabled));
	save_item(NAME(inhibit_bg_shift_one_dot));
	save_item(NAME(skip_dot));

	save_item(NAME(sprite0_eval_addr));
	save_item(NAME(sprite0_pat));
	save_item(NAME(sprite_go_this_line));
	save_item(NAME(sprite_go_next_line));
	save_item(NAME(sprite_force_immediate_applied));
	save_item(NAME(sprite_sl0_early_shift_pending));

	save_item(NAME(oam_latch_addr));
	save_item(NAME(sec_oam_full));
	save_item(NAME(oam_2004_latch));
	save_item(NAME(sec_oam_last_write));
	save_item(NAME(s_after_wrap));
	save_item(NAME(sl0_stale_s0_loaded));
	save_item(NAME(sl0_stale_sprite0_identity));
	save_item(NAME(overflow_eval_phase));
	save_item(NAME(overflow_finish_bytes));

	// --- retroactive PPUMASK previous-pixel state ---
	save_item(NAME(prev_pixel_valid));
	save_item(NAME(prev_pixel_scanline));
	save_item(NAME(prev_pixel_x));
	save_item(NAME(prev_bg_pixel_pat));
	save_item(NAME(prev_attr_bits));
	save_item(NAME(prev_spr_pat));
	save_item(NAME(prev_spr_pal));
	save_item(NAME(prev_spr_behind_bg));
	save_item(NAME(prev_spr_is_s0));
	save_item(NAME(prev_sprite0_pat));
	save_item(NAME(prev_backdrop_pal_index));
	save_item(NAME(retro_ppumask_color));
	save_item(NAME(retro_ppumask_render));

	// --- delayed fine-X and scroll side effects ---
	save_item(NAME(pending_fine_x));
	save_item(NAME(pending_fine_x_valid));
	save_item(NAME(pending_fine_x_delay));
	save_item(NAME(scroll_inc_h_pending));
	save_item(NAME(scroll_inc_v_pending));
	save_item(NAME(scroll_copy_h_pending));
	save_item(NAME(scroll_copy_v_pending));
	save_item(NAME(scroll_copy_conflict_pending));
	save_item(NAME(scroll_copy_conflict_h_pending));
	save_item(NAME(scroll_copy_conflict_v_pending));

	// --- PPU internal I/O latch/open-bus decay ---
	save_item(NAME(m_ppu_io_db));
	save_item(NAME(m_ppu_io_db_decay_at));

	// --- $2007 delayed read/write machinery ---
	save_item(NAME(ppu2007_buffer_fill_armed));
	save_item(NAME(ppu2007_buffer_fill_arm_pending));
	save_item(NAME(ppu_bus_read_can_fill_2007));
	save_item(NAME(ppu2007_post_bump_pending));
	save_item(NAME(ppu2007_post_bump_delay));
	save_item(NAME(sprite_nt_fetch_v));
	save_item(NAME(sprite_nt_fetch_v_new));

	save_item(NAME(m_2007_write.pending));
	save_item(NAME(m_2007_write.delay));
	save_item(NAME(m_2007_write.addr));
	save_item(NAME(m_2007_write.data));

	save_item(NAME(m_2007_read.pending));
	save_item(NAME(m_2007_read.delay));
	save_item(NAME(m_2007_read.addr));
	save_item(NAME(m_2007_read.use_next_ppu_read_for_refill));
	save_item(NAME(m_2007_read.waiting_for_refill_bus_read));
	
	// --- replacements for function-local statics ---
	save_item(NAME(m_eval_wrap_byte));
	save_item(NAME(m_eval_prev_oam_latch_addr));
	save_item(NAME(m_bgfetch_v_nt));
	save_item(NAME(m_bgfetch_v_at));
	save_item(NAME(m_bgfetch_v_pt));
	save_item(NAME(m_bgfetch_pat_pt));
	save_item(NAME(m_bgfetch_nt_addr));
	save_item(NAME(m_bgfetch_at_addr));
	save_item(NAME(sprite_eval_initialized));
}

void ppu2c0x_device::presave()
{
	m_save_sprite_size = int(sprite_size);
}

void ppu2c0x_device::postload()
{
	sprite_size = Sprite_size(m_save_sprite_size);
	resolve_mapper_ppu_devices();
}

//**************************************************************************
//  INLINE HELPERS
//**************************************************************************

//-------------------------------------------------
//  readbyte - read a byte at the given address
//-------------------------------------------------

uint8_t ppu2c0x_device::readbyte(offs_t address)
{	
	return space().read_byte(address); // works
}

inline uint8_t ppu2c0x_device::readbyte(uint16_t bus_addr, uint16_t mem_addr)
{
    return space().read_byte(mem_addr);
}

//-------------------------------------------------
//  writebyte - write a byte at the given address
//-------------------------------------------------

inline void ppu2c0x_device::writebyte(offs_t address, uint8_t data)
{
	space().write_byte(address, data);
}


inline uint16_t ppu2c0x_device::apply_grayscale_and_emphasis(uint8_t color)
{
	return (color & grayscale_color_mask) | tint_bits;
}

/***************************************************************************
    IMPLEMENTATION
***************************************************************************/

/*************************************
 *
 *  PPU Palette Initialization
 *
 *************************************/

void ppu2c0x_device::apply_color_emphasis_and_clamp(bool is_pal_or_dendy, int color_emphasis, double& R, double& G, double& B)
{
	if (is_pal_or_dendy) // PAL machines swap the colour emphasis bits, this means the red/blue highlighting on rampart tally bar doesn't look as good
	{
		color_emphasis = bitswap<3>(color_emphasis, 2, 0, 1);
	}

	static constexpr double rgb_mod[8][3] =
	{
		//  R      G      B
		{ 1.0,   1.0,   1.0   },
		{ 1.24,  0.915, 0.743 },
		{ 0.794, 1.09,  0.882 },
		{ 0.905, 1.03,  1.28  },
		{ 0.741, 0.987, 1.0   },
		{ 1.02,  0.908, 0.979 },
		{ 1.02,  0.98,  0.653 },
		{ 0.75,  0.75,  0.75  }
	};

	// Clipping, in case of saturation
	R = std::clamp(R * rgb_mod[color_emphasis][0], 0.0, 255.0);
	G = std::clamp(G * rgb_mod[color_emphasis][1], 0.0, 255.0);
	B = std::clamp(B * rgb_mod[color_emphasis][2], 0.0, 255.0);
}

rgb_t ppu2c0x_device::nespal_to_RGB(int color_intensity, int color_num, int color_emphasis, bool is_pal_or_dendy)
{
	const double tint = 0.22; /* adjust to taste */
	const double hue = 287.0;

	const double Kr = 0.2989;
	const double Kb = 0.1145;
	const double Ku = 2.029;
	const double Kv = 1.140;

	static const double brightness[3][4] =
	{
		{ 0.50, 0.75, 1.0, 1.0 },
		{ 0.29, 0.45, 0.73, 0.9 },
		{ 0, 0.24, 0.47, 0.77 }
	};

	double sat;
	double y, u, v;
	double rad;

	switch (color_num)
	{
	case 0:
		sat = 0; rad = 0;
		y = brightness[0][color_intensity];
		break;

	case 13:
		sat = 0; rad = 0;
		y = brightness[2][color_intensity];
		break;

	case 14:
	case 15:
		sat = 0; rad = 0; y = 0;
		break;

	default:
		sat = tint;
		rad = M_PI * ((color_num * 30 + hue) / 180.0);
		y = brightness[1][color_intensity];
		break;
	}

	u = sat * cos(rad);
	v = sat * sin(rad);

	/* Transform to RGB */
	double R = (y + Kv * v) * 255.0;
	double G = (y - (Kb * Ku * u + Kr * Kv * v) / (1 - Kb - Kr)) * 255.0;
	double B = (y + Ku * u) * 255.0;

	apply_color_emphasis_and_clamp(is_pal_or_dendy, color_emphasis, R, G, B);

	return rgb_t(floor(R + .5), floor(G + .5), floor(B + .5));
}

///this is the color for regular nes
void ppu2c0x_device::init_palette_tables()
{
	const bool is_pal = m_scanlines_per_frame != NTSC_SCANLINES_PER_FRAME;

	// Build the NES palette using a transformation from YUV to RGB.
	//
	// Layout:
	//   8 emphasis modes
	//   4 luminance/intensity levels
	//   16 colors per intensity
	int entry = 0;

	for (int color_emphasis = 0; color_emphasis < 8; ++color_emphasis)
	{
		for (int color_intensity = 0; color_intensity < 4; ++color_intensity)
		{
			for (int color_num = 0; color_num < 16; ++color_num)
				m_nespens[entry++] = uint32_t(nespal_to_RGB(color_intensity, color_num, color_emphasis, is_pal));
		}
	}
}

void ppu2c0x_rgb_device::init_palette_tables()
{
	// Build 8 emphasis variants of the 64-entry RGB palette.
	//
	// Each base color is stored as 3 packed 3-bit components in m_palette_data:
	//   R, G, B.
	//
	// For RGB PPUs, emphasis forces the selected channel to max intensity.
	int entry = 0;

	for (int color_emphasis = 0; color_emphasis < 8; ++color_emphasis)
	{
		const bool emph_r = (color_emphasis & 1) != 0;
		const bool emph_g = (color_emphasis & 2) != 0;
		const bool emph_b = (color_emphasis & 4) != 0;

		for (int color_num = 0; color_num < 64; ++color_num)
		{
			const int base = color_num * 3;

			const int r = emph_r ? 7 : m_palette_data[base + 0];
			const int g = emph_g ? 7 : m_palette_data[base + 1];
			const int b = emph_b ? 7 : m_palette_data[base + 2];

			m_nespens[entry++] =
				(pal3bit(r) << 16) |
				(pal3bit(g) << 8)  |
				 pal3bit(b);
		}
	}
}

/*************************************
 *
 *  PPU Bus Helpers
 *
 *************************************/

void ppu2c0x_device::ppu_bus_address_drive(uint16_t addr)
{
	addr &= 0x3FFF;

	ppu_addr_bus = addr;

	// Mapper-specific observers of the raw PPU address bus/access.
	if (m_has_mmc3_a12 && m_mmc3)
		m_mmc3->observe_ppu_a12(ppu_addr_bus, m_cpu->total_cycles());
}

uint8_t ppu2c0x_device::ppu_bus_read(uint16_t addr, ppu_fetch_phase phase)
{
	addr &= 0x3FFF;
	ppu_bus_address_drive(addr);

	// Classify the current PPU fetch phase once.
	//
	// This feeds the mapper hooks below without repeatedly comparing the enum.
	// Keep the meaning the same as the old code:
	//   NT      = background nametable fetch
	//   AT      = background attribute fetch
	//   PTL/PTH = background pattern fetches
	//   SPR_*   = sprite pattern fetches
	//
	// Dummy fetches intentionally leave all of these false unless a mapper
	// explicitly needs to observe them elsewhere.
	bool is_nt = false;
	bool is_at = false;
	bool is_bg_pattern = false;
	bool is_spr_pattern = false;

	switch (phase)
	{
		case ppu_fetch_phase::NT:
			is_nt = true;
			break;

		case ppu_fetch_phase::AT:
			is_at = true;
			break;

		case ppu_fetch_phase::PTL:
		case ppu_fetch_phase::PTH:
			is_bg_pattern = true;
			break;

		case ppu_fetch_phase::SPR_PTL:
		case ppu_fetch_phase::SPR_PTH:
			is_spr_pattern = true;
			break;

		default:
			break;
	}

	const bool chr_fetch = is_bg_pattern || is_spr_pattern;

	// MMC5 needs to observe the full PPU read stream, including NT/AT/pattern
	// classification. Keep this before the actual read, same as your current path.
	//
	// Note: dummy fetches currently pass all phase booleans as false, matching the
	// old boolean classification behavior unless you later decide MMC5 needs a
	// separate dummy-read signal.
	if (m_has_mmc5_ppu && m_mmc5)
	{
		m_mmc5->mmc5_clock_ppu_read(
			addr,
			is_nt,
			is_at,
			is_bg_pattern,
			is_spr_pattern);
	}

	// Actual PPU memory read.
	//
	// Keep this after the MMC5 observe call because the mapper may need to see
	// the bus address/phase before data is returned.
	const uint8_t data = readbyte(addr);

	if (addr < 0x2000)
	{
		// MMC2/MMC4 latch boards react to CHR pattern address fetches.
		// If you know your dummy CHR fetches should not clock MMC2/MMC4, keep
		// this under chr_fetch instead. But for MMC1 board-side CHR-line effects,
		// the actual address on the PPU bus is what matters.
		if (chr_fetch && !m_latch.isnull())
			m_latch(addr);

		// Special MMC1 SxROM-family boards need to know which CHR half was fetched.
		// This is an address-line observation, not really a "render phase" observation.
		// If the PPU puts a CHR address on the bus, the board sees CHR A12/A16/etc.
		if (m_has_mmc1_phase && m_mmc1_sxrom)
		{
			const bool upper_chr = (addr & 0x1000) != 0;
			m_mmc1_sxrom->mmc1_ppu_phase(upper_chr, addr & 0x1FFF);
		}
	}

	// Once the delayed $2007 refill is armed, the VERY NEXT actual PPU bus read
	// provides the refill value, regardless of whether it is BG, sprite, or dummy NT.
	//
	// ppu_bus_read_can_fill_2007 is still the external gate that decides which
	// bus reads are allowed to satisfy the delayed refill.
	if (ppu2007_buffer_fill_armed && ppu_bus_read_can_fill_2007)
	{
		ppu_data_reg = data;
		ppu2007_buffer_fill_armed = false;

		// A rendering $2007 read matures first, then the next allowed real PPU
		// bus read supplies the internal read-buffer refill.
		if (m_2007_read.pending && m_2007_read.waiting_for_refill_bus_read)
		{
			m_2007_read.pending = false;
			m_2007_read.waiting_for_refill_bus_read = false;
		}
	}

	return data;
}

void ppu2c0x_device::schedule_2007_read(uint16_t addr, int delay, bool use_next_ppu_read_for_refill)
{
	addr &= 0x3FFF;

	m_2007_read.pending = true;
	m_2007_read.delay = (delay > 0) ? delay : 0;
	m_2007_read.addr = addr;
	m_2007_read.use_next_ppu_read_for_refill = use_next_ppu_read_for_refill;
	m_2007_read.waiting_for_refill_bus_read = false;
}

void ppu2c0x_device::schedule_2007_write(uint16_t addr, uint8_t data, int delay)
{
	addr &= 0x3FFF;

	m_2007_write.pending = true;
	m_2007_write.delay = (delay > 0) ? delay : 0;
	m_2007_write.addr = addr;
	m_2007_write.data = data;
}

void ppu2c0x_device::schedule_2007_post_access_bump()
{
    const bool rendering_now =
        (bg_pipeline_enabled || spr_pipeline_enabled) &&
        ((scanline < 240) || (scanline == m_prerender_line));

    if (rendering_now)
    {
        ppu2007_post_bump_pending = true;
    }
    else
    {
        do_2007_post_access_bump();
    }
}

void ppu2c0x_device::set_mapper(int mapper)
{
	m_mapper_number = mapper;

	// Start optimistic.  resolve_mapper_ppu_devices() will only leave this true
	// if the loaded NES slot contains a nes_txrom_device-derived PCB.
	m_has_mmc3_a12 = true;

	m_has_mmc5_ppu = mapper == 5;

	// Start conservative. resolve_mapper_ppu_devices() will turn this off if no
	// SxROM-family device is actually present.
	m_has_mmc1_phase = mapper == 1;

	resolve_mapper_ppu_devices();
}

void ppu2c0x_device::resolve_mapper_ppu_devices()
{
	// MMC5 has special PPU read observation for scanline detection / ExROM behavior.
	if (m_has_mmc5_ppu && !m_mmc5)
		m_mmc5 = machine().root_device().subdevice<nes_exrom_device>("nes_slot:exrom");

	// MMC3-family boards watch PPU A12.  This includes official TxROM/MMC6
	// boards and many bootleg/multicart MMC3 clones.
	//
	// Do not resolve this by hardcoded board tags.  The NES slot contains the
	// loaded PCB device, and MMC3 clones derive from nes_txrom_device.  Walk the
	// children of "nes_slot" and find the active nes_txrom_device.
	if (m_has_mmc3_a12 && !m_mmc3)
	{
		device_t *const slot = machine().root_device().subdevice("nes_slot");

		if (slot)
		{
			for (device_t &dev : slot->subdevices())
			{
				m_mmc3 = dynamic_cast<nes_txrom_device *>(&dev);
				if (m_mmc3)
					break;
			}
		}
	}

	// If no TxROM/MMC3-family PCB resolved, disable the A12 observer so the hot
	// PPU path does not keep doing useless checks.
	if (m_has_mmc3_a12 && !m_mmc3) {
		m_has_mmc3_a12 = false;
	}	

	// MMC1 special SxROM-family phase feed.
	//
	// Only boards that repurpose MMC1 CHR register bits for PRG-RAM enable/banking
	// need to know which CHR register was last active on the PPU side.
	//
	// Plain SxROM/SLROM does normal MMC1 CHR banking through set_chr(); it does not
	// need per-fetch PPU phase feedback.
	if (m_has_mmc1_phase && !m_mmc1_sxrom)
	{
		m_mmc1_sxrom = machine().root_device().subdevice<nes_sxrom_device>("nes_slot:snrom");
		if (!m_mmc1_sxrom) m_mmc1_sxrom = machine().root_device().subdevice<nes_sxrom_device>("nes_slot:sorom");
		if (!m_mmc1_sxrom) m_mmc1_sxrom = machine().root_device().subdevice<nes_sxrom_device>("nes_slot:surom");
		if (!m_mmc1_sxrom) m_mmc1_sxrom = machine().root_device().subdevice<nes_sxrom_device>("nes_slot:sxrom_ext");
		if (!m_mmc1_sxrom) m_mmc1_sxrom = machine().root_device().subdevice<nes_sxrom_device>("nes_slot:szrom");
	}

	// Plain MMC1 boards don't need the phase feed. Turn it off if no special
	// SxROM-family device was found.
	if (m_has_mmc1_phase && !m_mmc1_sxrom)
		m_has_mmc1_phase = false;
	
	m_has_chr_latch = !m_latch.isnull();
}

void ppu2c0x_device::ppu_bus_a12_observe(uint16_t addr)
{
	addr &= 0x3FFF;

	if (m_has_mmc3_a12 && m_mmc3)
		m_mmc3->observe_ppu_a12(addr, m_cpu->total_cycles());
}

void ppu2c0x_device::tick() {
	
	// --------------------------------------------------
	// Delayed $2007 write
	//
	// Rendering $2007 writes do not hit VRAM immediately.  The CPU access places
	// the address on the PPU bus now, but the effective write reaches the PPU bus
	// after the delay recorded in m_2007_write.
	//
	// Non-rendering writes still happen immediately in the $2007 write handler;
	// this path is only for delayed rendering writes.
	// --------------------------------------------------
	if (m_2007_write.pending)
	{
		if (m_2007_write.delay > 0)
		{
			--m_2007_write.delay;
		}
		else
		{
			ppu_bus_address_drive(m_2007_write.addr);
			writebyte(ppu_addr_bus, m_2007_write.data);

			// The delayed rendering write has now become effective.  Only now arm
			// the post-access bump, so the increment is delayed relative to the
			// actual PPU bus write rather than the original CPU register write.
			ppu2007_post_bump_pending = true;
			ppu2007_post_bump_delay = 1;

			m_2007_write.pending = false;
		}
	}

	// --------------------------------------------------
	// Delayed $2007 read/refill
	//
	// CPU $2007 reads return immediately from either:
	//   - ppu_data_reg for normal VRAM reads, or
	//   - palette RAM for palette reads.
	//
	// The internal read buffer refill happens later:
	//   - outside rendering, refill directly from the accessed address;
	//   - during rendering, wait for the next allowed real PPU fetch.
	// --------------------------------------------------
	if (m_2007_read.pending && !m_2007_read.waiting_for_refill_bus_read)
	{
		if (m_2007_read.delay > 0)
		{
			--m_2007_read.delay;
		}
		else
		{
			const uint16_t delayed_bus_addr = m_2007_read.addr & 0x3FFF;

			// Palette reads return palette data immediately, but the internal buffer
			// refills from the mirrored non-palette address.
			const uint16_t read_addr =
				((delayed_bus_addr >= 0x3F00 && delayed_bus_addr <= 0x3FFF)
					? (delayed_bus_addr & 0x2FFF)
					: delayed_bus_addr) & 0x3FFF;

			if (m_2007_read.use_next_ppu_read_for_refill)
			{
				// Rendering $2007 read:
				// Mature now, but do not let a fetch that already happened earlier
				// in this PPU dot refill the buffer. Arm the bus-fill after this dot.
				m_2007_read.waiting_for_refill_bus_read = true;
				ppu2007_buffer_fill_arm_pending = true;

				// Rendering $2007 access causes the H+V increment glitch when the
				// delayed access matures.
				schedule_2007_post_access_bump();
			}
			else
			{
				// Non-rendering $2007 read:
				// Delayed direct refill from the accessed address.
				ppu_bus_address_drive(read_addr);
				ppu_data_reg = readbyte(read_addr);

				// Restore the visible bus address to current v after the refill.
				ppu_bus_address_drive(v);

				m_2007_read.pending = false;
			}
		}
	}

	//log when a key is pressed, helps debugging of test roms
	/*if (g_nes_p1_a_pressed_edge)
	{
		logerror("[PPU] A pressed at sl=%d dot=%d\n", scanline, dot);
		g_nes_p1_a_pressed_edge = false;
	}*/
	
	// Start-of-scanline bookkeeping
	// dot 0 bookkeeping
	if (dot == 0) {
		sprite_eval_initialized = false;
		ppu_bus_read_can_fill_2007 = false;

		sprite_go_this_line = sprite_go_next_line;
		sprite_force_immediate_applied = false;

		prev_pixel_valid = false;
		retro_ppumask_color = false;
		retro_ppumask_render = false;

		// One-shot scanline-0 stale sprite0 identity.
		// This is only armed by prerender sprite loading.
		if (scanline == 0)
		{
			if (sl0_stale_sprite0_identity)
				s0_on_cur_scanline = true;

			sl0_stale_sprite0_identity = false;
			sl0_stale_s0_loaded = false;
		}
	}
	
	if (pending_2000.has_pending)
	{
		if (pending_2000.apply_ppu > 0)
			pending_2000.apply_ppu--;

		if (pending_2000.apply_ppu <= 0)
		{
			apply_delayed_2000(pending_2000.value);
			pending_2000.has_pending = false;
		}
	}
	
	if (pending_2006.has_pending)
	{
		if (pending_2006.apply_ppu > 0)
			pending_2006.apply_ppu--;

		if (pending_2006.apply_ppu <= 0)
		{
			const uint16_t ppuaddr_reload = pending_2006.value16 & 0x7FFF;

			const bool rendering_now =
				(bg_pipeline_enabled || spr_pipeline_enabled) &&
				((scanline < 240) || (scanline == m_prerender_line));

			if (rendering_now)
			{
				t = ppuaddr_reload;

				scroll_copy_h_pending = true;
				scroll_copy_v_pending = true;

				// Mapper-only observation for MMC3 manual A12 clocking.
				// Do not change ppu_addr_bus here, or games can get visible timing errors.
				ppu_bus_a12_observe(ppuaddr_reload);
			}
			else
			{
				v = ppuaddr_reload;

				// Outside rendering, your old behavior really did expose v on the bus.
				ppu_bus_address_drive(v);
			}

			pending_2006.has_pending = false;
		}
	}
	
	if (pending_fine_x_valid)
	{
		if (pending_fine_x_delay > 0)
			pending_fine_x_delay--;

		if (pending_fine_x_delay <= 0)
		{
			const uint8_t val = pending_fine_x;
			fine_x = val & 7;
			t      = (t & 0x7FE0) | ((val & 0xF8) >> 3);
			pending_fine_x_valid = false;
		}
	}
	
	// Apply pending $2001 writes using a local countdown.
	if (pending_2001.has_pending)
	{
		if (pending_2001.apply_ppu > 0)
			pending_2001.apply_ppu--;

		if (pending_2001.apply_ppu <= 0)
		{
			apply_delayed_2001(pending_2001.value);
			pending_2001.has_pending = false;
		}
	}

	// Apply OAM corruption exactly once:
	// on the first PPU cycle that occurs with rendering enabled on a visible/pre-render line.
	if (oam_corrupt_pending && (bg_pipeline_enabled || spr_pipeline_enabled)) {
		if (scanline <= 239 || scanline == 261) {

			uint8_t row = oam_corrupt_seed & 0x1F;

			if (row != 0) {
				memcpy(oam + row * 8, oam, 8);   // copy row 0 -> row 'row'
			}

			// Secondary OAM side effect mentioned by the test
			sec_oam[row] = sec_oam[0];

			oam_corrupt_pending = false;
		}
	}

	const bool any_scroll_op = scroll_inc_h_pending | scroll_inc_v_pending | scroll_copy_h_pending | scroll_copy_v_pending;

	if (any_scroll_op)
	{
		//componentwise v increment/copy overlap; AND behavior may apply.
		scroll_copy_conflict_h_pending = scroll_inc_h_pending && scroll_copy_h_pending;
		scroll_copy_conflict_v_pending = scroll_inc_v_pending && scroll_copy_v_pending;
		if(scroll_copy_conflict_h_pending || scroll_copy_conflict_v_pending) {
			logerror("[PPU SCROLL CONFLICT] sl=%d dot=%d H=%d V=%d "
				 "v=%04X t=%04X inc_h=%d inc_v=%d copy_h=%d copy_v=%d\n",
				 scanline,
				 dot,
				 scroll_copy_conflict_h_pending ? 1 : 0,
				 scroll_copy_conflict_v_pending ? 1 : 0,
				 v & 0x7FFF,
				 t & 0x7FFF,
				 scroll_inc_h_pending ? 1 : 0,
				 scroll_inc_v_pending ? 1 : 0,
				 scroll_copy_h_pending ? 1 : 0,
				 scroll_copy_v_pending ? 1 : 0);
		}
		apply_scroll_ops();
	}

	if(nmi_delay > 0 && --nmi_delay == 0) {
		if(nmi_pending) {
			m_maincpu6502->queue_delayed_nmi(1);
			nmi_pending = false;
		}
	}
	
	switch (scanline) {
		case 0 ... 239     : run_visible_scanline_dot();   	break;
		case 241           : run_scanline_241_dot();       	break;
		case 261	   	   : run_prerender_scanline_dot(); 	break;
	}
	
	//mmc3/mmc6 needs no variables but still needs the call to countdown IRQ
	//mmc5 needs scanline, (bg_output_enabled || spr_output_enabled) and dot
	//vrc6 needs no variables but still needs the call to countdown IRQ
	// Only mapper devices with a PPU tick callback need this.
	// Avoid call_mapper() overhead for plain carts.
	if (!m_ppu_to_mapper.isnull())
		m_ppu_to_mapper(scanline, dot);

	if (ppu2007_post_bump_pending)
	{
		if (ppu2007_post_bump_delay > 0)
		{
			--ppu2007_post_bump_delay;
		}
		else
		{
			do_2007_post_access_bump();
			ppu2007_post_bump_pending = false;
			ppu2007_post_bump_delay = 0;
		}
	}
		
	// Odd-frame skip.
	// At this point the current dot's work has already executed.
	// If we are sitting at prerender dot 340 on an odd rendered frame,
	// skip the normal dot++/wrap path and jump straight to scanline 0 dot 0.
	if (scanline == 261 && dot == 339 && odd_frame && (bg_pipeline_enabled || spr_pipeline_enabled)) {
		sprite_sl0_early_shift_pending = true;   // applies to scanline 0, pixel 0

		dot = 0;
		scanline = 0;
		suppress_vblank_flag = false;
		odd_frame = !odd_frame;
		screen().reset_origin(scanline, dot);
		frame++;
	} else {
		++dot;
	}

	// Rendering $2007 reads mature at the start of the tick, but must not be
	// filled by bus reads that already occurred during this same dot. Arm the
	// fill only after the dot's PPU work is complete.
	if (ppu2007_buffer_fill_arm_pending)
	{
		ppu2007_buffer_fill_arm_pending = false;
		ppu2007_buffer_fill_armed = true;
	}

	//Each scanline lasts for 341 PPU clock cycles, with each clock cycle producing one pixel(dot). 
	if (dot > 340) {
		dot=0;
		++scanline;
		if(scanline == 240) {
			ppu_addr_bus = v & 0x3FFF;
		}
		if(scanline > 261) {
			scanline = 0;
			suppress_vblank_flag = false;
			odd_frame = !odd_frame;
			screen().reset_origin(scanline, dot);
			frame++;
		}
	}

	m_scanline=scanline;

	//Start HBlank
	if (scanline <= 239 && dot==257) {
		if (!m_hblank_callback_proc.isnull())
			m_hblank_callback_proc(scanline, in_vblank, (bg_pipeline_enabled || spr_pipeline_enabled) );
	}
}

/*************************************
 *
 *  Scanline Rendering and Update
 *
 *************************************/
void ppu2c0x_device::retro_fix_previous_pixel_after_ppumask_write()
{
	if (!prev_pixel_valid)
		return;

	if (prev_pixel_scanline != scanline)
		return;

	bitmap_rgb32 &bitmap = *m_bitmap;
	const unsigned pixel = prev_pixel_x;
	unsigned pal_index = 0;

	// Use the current visible PPUMASK state after the write.
	const bool bg_visible =
		bg_output_enabled &&
		(show_bg_left_8 || pixel >= 8);

	const bool spr_visible =
		spr_output_enabled &&
		(show_sprites_left_8 || pixel >= 8);

	const bool rendering_disabled =
		(!bg_output_enabled && !spr_output_enabled);

	// Internal BG path for sprite-zero hit.
	// Do NOT include inhibit_bg_shift_one_dot here.
	const unsigned bg_pat_internal =
		bg_visible ? prev_bg_pixel_pat : 0;

	// Display BG path. This is allowed to suppress the enable-edge stale pixel.
	const bool bg_display_visible =
		bg_visible &&
		!inhibit_bg_shift_one_dot;

	const unsigned bg_pat_display =
		bg_display_visible ? prev_bg_pixel_pat : 0;

	// IMPORTANT:
	// prev_spr_pat must be the raw sprite pixel from get_sprite_pixel(),
	// not the already visibility-masked visible_spr_pat.
	const unsigned spr_pat_display =
		spr_visible ? prev_spr_pat : 0;

	// Re-evaluate sprite 0 hit for the previous pixel under the new PPUMASK state.
	// Use internal BG, not display-suppressed BG.
	const bool right_edge_ok = (pixel != 255);

	if (s0_on_cur_scanline &&
		prev_sprite0_pat &&
		bg_pat_internal &&
		bg_visible &&
		spr_visible &&
		right_edge_ok)
	{
		sprite_zero_hit = true;
	}

	if (rendering_disabled)
	{
		pal_index = prev_backdrop_pal_index;
	}
	else if (spr_pat_display && !(prev_spr_behind_bg && bg_pat_display))
	{
		pal_index = 0x10 + (prev_spr_pal << 2) + spr_pat_display;
	}
	else
	{
		if (!bg_pat_display)
			pal_index = 0;
		else
			pal_index = (prev_attr_bits << 2) | bg_pat_display;
	}

	bitmap.pix(scanline, pixel) =
		m_nespens[apply_grayscale_and_emphasis(palette_read(pal_index))];

	retro_ppumask_color = false;
	retro_ppumask_render = false;
}
/*
void ppu2c0x_device::retro_fix_previous_pixel_after_ppumask_write()
{
	if (!prev_pixel_valid)
		return;

	if (prev_pixel_scanline != scanline)
		return;

	bitmap_rgb32 &bitmap = *m_bitmap;
	unsigned const pixel = prev_pixel_x;
	unsigned pal_index = 0;

	// Current visible PPUMASK state is used here.
	const bool bg_visible =
		bg_output_enabled &&
		(show_bg_left_8 || pixel >= 8);

	const bool spr_visible =
		spr_output_enabled &&
		(show_sprites_left_8 || pixel >= 8);

	const bool rendering_disabled =
		(!bg_output_enabled && !spr_output_enabled);

	// Internal BG exists for sprite-zero logic.
	// Do NOT use inhibit_bg_shift_one_dot here for the internal comparator path.
	const unsigned bg_pat_internal =
		bg_visible ? prev_bg_pixel_pat : 0;

	// Display BG is allowed to suppress the enable-edge stale pixel.
	// This mirrors the split used in do_pixel_output_and_sprite_zero().
	const bool bg_display_visible =
		bg_visible &&
		!inhibit_bg_shift_one_dot;

	const unsigned bg_pat_display =
		bg_display_visible ? prev_bg_pixel_pat : 0;

	const unsigned spr_pat_display =
		spr_visible ? prev_spr_pat : 0;

	// Re-evaluate sprite 0 hit for the PREVIOUS pixel under the NEW render-enable state.
	// Use internal BG, not display-suppressed BG, so AccuracyCoin stale BG can still pass.
	const bool right_edge_ok = (pixel != 255);

	if (s0_on_cur_scanline &&
		prev_sprite0_pat &&
		bg_pat_internal &&
		bg_visible &&
		spr_visible &&
		right_edge_ok)
	{
		sprite_zero_hit = true;
	}

	if (rendering_disabled)
	{
		pal_index = prev_backdrop_pal_index;
	}
	else if (spr_pat_display && !(prev_spr_behind_bg && bg_pat_display))
	{
		pal_index = 0x10 + (prev_spr_pal << 2) + spr_pat_display;
	}
	else
	{
		if (!bg_pat_display)
			pal_index = 0;
		else
			pal_index = (prev_attr_bits << 2) | bg_pat_display;
	}

	bitmap.pix(scanline, pixel) =
		m_nespens[apply_grayscale_and_emphasis(palette_read(pal_index))];

	retro_ppumask_color = false;
	retro_ppumask_render = false;
}*/

void ppu2c0x_device::set_nmi(bool s) {
	if (s)
	{
		if (!nmi_pending && nmi_delay == 0)
		{
			// Queue delayed arrival into CPU.
			// we delay 2 ppu clicks, gives enough time to cancel it
			nmi_pending = true;
			nmi_delay = 2;
		}
	}
	else
	{
		if(nmi_delay > 0 && nmi_pending) {
			m_maincpu6502->cancel_delayed_nmi();
		}
		nmi_pending = false;
		nmi_delay = 0;
	}
}

void ppu2c0x_device::run_bg_fetch_dot()
{
	if (!(bg_pipeline_enabled || spr_pipeline_enabled))
		return;

	switch ((dot - 1) % 8)
	{
		case 0:
		{
			// First dot of nametable fetch:
			// Hardware has already put the low part of the address path in motion.
			//
			// If v changes before the actual read dot, the NT fetch can use:
			//   low  8 bits = old/prepared address
			//   high 6 bits = current/new address
			//
			// In the normal case where v does not change, this collapses to the
			// same address as before.
			m_bgfetch_v_nt = v & 0x7FFF;
			m_bgfetch_nt_addr = (0x2000 | (m_bgfetch_v_nt & 0x0FFF)) & 0x3FFF;
			ppu_addr_bus = m_bgfetch_nt_addr;
			break;
		}

		case 1:
		{
			// Actual nametable read.
			//
			// Mixed-address behavior:
			//   low  8 bits come from the address prepared on the first fetch dot
			//   high 6 bits come from current v at the read dot
			//
			// This only matters if a delayed $2006/$2007/scroll operation changed v
			// between the prepare dot and the read dot.
			/*const uint16_t nt_addr_new = (0x2000 | (v & 0x0FFF)) & 0x3FFF;
			const uint16_t nt_addr_mixed =
				(nt_addr_new & 0x3F00) |
				(m_bgfetch_nt_addr & 0x00FF);

			ppu_addr_bus = nt_addr_mixed & 0x3FFF;
*/
			ppu_bus_read_can_fill_2007 = true;
			nt_byte = ppu_bus_read(ppu_addr_bus, ppu_fetch_phase::NT);
			ppu_bus_read_can_fill_2007 = false;
			break;
		}

		case 2:
		{
			// First dot of attribute fetch:
			// Same mixed-address rule as NT fetches. Prepare the old full AT
			// address now, then use only its low 8 bits on the read dot.
			m_bgfetch_v_at = v & 0x7FFF;
			m_bgfetch_at_addr =
				0x23C0
				| (m_bgfetch_v_at & 0x0C00)
				| ((m_bgfetch_v_at >> 4) & 0x38)
				| ((m_bgfetch_v_at >> 2) & 0x07);

			m_bgfetch_at_addr &= 0x3FFF;
			ppu_addr_bus = m_bgfetch_at_addr;
			break;
		}

		case 3:
		{
			// Actual attribute read.
			//
			// Low 8 bits are from the old/prepared AT address.
			// High 6 bits are from the AT address generated from current v.
			/*const uint16_t at_v_new = v & 0x7FFF;
			const uint16_t at_addr_new =
				(0x23C0
				| (at_v_new & 0x0C00)
				| ((at_v_new >> 4) & 0x38)
				| ((at_v_new >> 2) & 0x07)) & 0x3FFF;

			const uint16_t at_addr_mixed =
				(at_addr_new & 0x3F00) |
				(m_bgfetch_at_addr & 0x00FF);

			ppu_addr_bus = at_addr_mixed & 0x3FFF;
*/
			ppu_bus_read_can_fill_2007 = true;
			at_byte = ppu_bus_read(ppu_addr_bus, ppu_fetch_phase::AT);
			ppu_bus_read_can_fill_2007 = false;

			// Attribute quadrant decode should use the v that started this AT fetch,
			// not the possibly changed v from the read dot.
			at_byte = (at_byte >> (((m_bgfetch_v_at >> 4) & 4) | (m_bgfetch_v_at & 2))) & 0x03;
			break;
		}

		case 4:
		{
			// Pattern fetches are intentionally NOT mixed here.
			// The documented mixed-address effect is important for NT/AT fetches;
			// pattern fetches should keep using the latched tile/fine-y path.
			m_bgfetch_v_pt = v & 0x7FFF;
			m_bgfetch_pat_pt = bg_pat_addr;
			ppu_addr_bus = m_bgfetch_pat_pt + (16 * nt_byte) + (m_bgfetch_v_pt >> 12);
			ppu_addr_bus &= 0x3FFF;
			break;
		}

		case 5:
			ppu_bus_read_can_fill_2007 = true;
			bg_byte_l = ppu_bus_read(ppu_addr_bus, ppu_fetch_phase::PTL);
			ppu_bus_read_can_fill_2007 = false;
			break;

		case 6:
			ppu_addr_bus = m_bgfetch_pat_pt + (16 * nt_byte) + (m_bgfetch_v_pt >> 12) + 8;
			ppu_addr_bus &= 0x3FFF;
			break;

		case 7:
			ppu_bus_read_can_fill_2007 = true;
			bg_byte_h = ppu_bus_read(ppu_addr_bus, ppu_fetch_phase::PTH);
			ppu_bus_read_can_fill_2007 = false;

			scroll_inc_h_pending = true;
			break;
	}
}

void ppu2c0x_device::run_visible_scanline_dot() {
	// AccuracyCoin Stale Sprite Shift Registers:
	//
	// Dot 257 normally transfers the sprite0 identity discovered by evaluation
	// into the current-line sprite unit state.  Forced blank must not perform that
	// transfer or clear the pending identity, because stale sprite shifter contents
	// and their sprite0 identity can survive until rendering is re-enabled.
	//
	// Sprite eval/reload follows the delayed pipeline-enable state, while the
	// sprite pattern shifters follow the immediate output-visible state.
	if (bg_pipeline_enabled || spr_pipeline_enabled) {
        switch (dot) {
            case 0:
                sec_oam_addr = 0;
            break;

            case 1 ... 64:
				// OAM2 init/clear phase:
				// $2004 must see $FF through this whole window.
				oam_2004_latch = 0xFF;
				sec_oam_last_write = 0xFF;
				oam_latch_addr = sec_oam_addr & 0x1F;
				if (dot & 1) {
					oam_data = 0xFF;
				} else {
					// write the forced $FF into secondary OAM
					sec_oam[sec_oam_addr & 0x1F] = 0xFF;
					sec_oam_addr = (sec_oam_addr + 1) & 0x1F;
				}
            break;

            case 65 ... 256:
				do_sprite_evaluation();
            break;
        }
    }
	
	
	if (dot == 257 && (bg_pipeline_enabled || spr_pipeline_enabled)) {
		s0_on_cur_scanline = s0_on_next_scanline;
		s0_on_next_scanline = false;
	}

    if (dot >= 2 && dot <= 257) {
        do_pixel_output_and_sprite_zero();
    }
	
	if ((bg_pipeline_enabled || spr_pipeline_enabled) && ((dot >= 2 && dot <= 257) || (dot >= 322 && dot <= 337))) {
		clock_bg_shifters_only();
	}

    run_render_pipeline_dot();
	

}

void ppu2c0x_device::run_render_pipeline_dot() {
	if (dot == 0) {
		if (bg_pipeline_enabled || spr_pipeline_enabled) {
			ppu_addr_bus = (bg_pat_addr + (16 * nt_byte) + (v >> 12)) & 0x3FFF;
		} else {
			ppu_addr_bus = v & 0x3FFF;
		}
	}
	
	const bool pipe_render   = (bg_pipeline_enabled || spr_pipeline_enabled);
	//const bool immediate_render = (bg_output_enabled || spr_output_enabled);

	switch (dot) {
        
        case 1 ... 256:
        case 321 ... 336:
			if (pipe_render) {
				run_bg_fetch_dot();

				if (dot == 256) {
					scroll_inc_v_pending = true;
				}
				// Micromachines/Scroll Fix:
				// The PPU bus often holds the first sprite's data during the fetch phase for
				// the first two background tiles of the next line.
				//
				// Dot 321 signal quirk: OAM2ADDR receives one extra increment.
				// However, $2004-visible secondary-OAM behavior exposes byte 0 here.
				if (dot == 321)
				{
					sec_oam_addr = (sec_oam_addr + 1) & 0x1F;

					//oam_data       = sec_oam[0];
					//oam_latch_addr = 0;
					//oam_2004_latch  = oam_data;
				}
			}
            break;
		case 257:
		{
			if (pipe_render) {
				// Latch OLD v before horizontal reload
				sprite_nt_fetch_v = v & 0x7FFF;

				// Precompute what v will become after copy_horiz()
				sprite_nt_fetch_v_new = (v & ~0x041F) | (t & 0x041F);
				sprite_nt_fetch_v_new &= 0x7FFF;
			}
			
			if (pipe_render) {
				oam_addr = 0;
				sec_oam_addr = 0;
				//copy_horiz();
				scroll_copy_h_pending = true;
			}

			if (pipe_render) { //immediate_render
				do_sprite_loading();
			}
			break;
		}
  
		case 258 ... 320:
			if (pipe_render) { //immediate_render
				do_sprite_loading();
			}
			break;

        // --- Dummy Nametable Fetches (Dots 337 & 339) ---
        case 337: 
			if (pipe_render) 
				ppu_addr_bus = 0x2000 | (v & 0x0FFF);
			break;
		case 338:
			if (pipe_render) {
				ppu_bus_read_can_fill_2007 = true;
				(void)ppu_bus_read(ppu_addr_bus, ppu_fetch_phase::DUMMY338);
				ppu_bus_read_can_fill_2007 = false;
			}
			break;
        case 339:
			// AccuracyCoin Stale Sprite Shift Registers:
			//
			// Dot 339 is the point where sprite shifter counters are told to
			// enter their normal "counting" mode for the next visible line.
			//
			// This decision must be recorded even when rendering is forced
			// blanked.  If dot 339 occurs during forced blank, do NOT leave the
			// previous value of sprite_go_next_line alive.  Explicitly record
			// false so the stale/halted sprite state can survive and draw
			// immediately when rendering is re-enabled.
			if (scanline <= 239 || scanline == 261) {
				sprite_go_next_line = (bg_pipeline_enabled || spr_pipeline_enabled);
			}

			if (pipe_render)
				ppu_addr_bus = 0x2000 | (v & 0x0FFF);

			break;
		case 340:
		if (pipe_render) {
			ppu_bus_read_can_fill_2007 = true;
			(void)ppu_bus_read(ppu_addr_bus, ppu_fetch_phase::DUMMY340);
			ppu_bus_read_can_fill_2007 = false;
		}
			break;
    }
	
	if (pipe_render && ((dot >= 2 && dot <= 257) || (dot >= 322 && dot <= 337))) {
		reload_bg_shift_registers();
	}

}

unsigned ppu2c0x_device::get_sprite_pixel(unsigned &spr_pal, bool &spr_behind_bg, bool &spr_is_s0)
{
	if (scanline >= 240 || dot < 2 || dot > 257)
	{
		sprite0_pat = 0;
		return 0;
	}
	
	unsigned const pixel = dot - 2;

	// /VIS for sprite pattern shifters is output-visible rendering.
	// Counters may continue through forced blank, but pattern shifters only
	// shift when rendering output is actually visible.
	const bool vis = (bg_output_enabled || spr_output_enabled);

	// Output is still gated by sprite-enable + left-8 rules (your existing behavior)
	const bool output_allowed = spr_output_enabled && (show_sprites_left_8 || pixel >= 8);

	// If rendering was NOT enabled at dot 339 last scanline, Fiskbit says the shifters
	// behave like "already expired" and will output/shift as soon as rendering is enabled.
	if (!sprite_go_this_line && !sprite_force_immediate_applied && vis)
	{
		// Immediate output mode when/if vis becomes true.
		memset(sprite_x_cnt, 0, sizeof(sprite_x_cnt));
		sprite_force_immediate_applied = true;
	}

	// Scanline-0 odd-frame skip glitch:
	// one early shift/output at X=0, then counting effectively offset by 1.
	if (scanline == 0 && dot == 2 && sprite_sl0_early_shift_pending)
	{
		sprite_sl0_early_shift_pending = false;

		if (!vis)
		{
			sprite0_pat = 0;
			return 0;
		}

		uint8_t slot0_pat = 0;
		uint8_t first_pat = 0;
		unsigned first_index = 8;

		for (unsigned i = 0; i < 8; ++i)
		{
			uint8_t pat = 0;

			if (sprite_shift_count[i] < 8)
			{
				// Sample current MSB regardless of X counter.
				// This is the "free" early pixel.
				const unsigned p1 = (sprite_pat_h[i] & 0x80) ? 1 : 0;
				const unsigned p0 = (sprite_pat_l[i] & 0x80) ? 1 : 0;
				pat = uint8_t((p1 << 1) | p0);

				// Clock once because /VIS is asserted.
				sprite_pat_h[i] <<= 1;
				sprite_pat_l[i] <<= 1;
				sprite_shift_count[i]++;

				// Offset the subsequent countdown by +1 so the remaining 7 pixels line up.
				if (sprite_x_cnt[i] > 0)
					sprite_x_cnt[i] = uint8_t(sprite_x_cnt[i] + 1);
			}

			// Sprite0-hit uses slot0's pixel, even if another sprite wins output.
			if (i == 0)
				slot0_pat = pat;

			// Keep first non-transparent sprite by priority, but do not return yet.
			// All sprite shifters/counters above must still be serviced.
			if (pat && first_index == 8)
			{
				first_pat = pat;
				first_index = i;
			}
		}

		sprite0_pat = (s0_on_cur_scanline && output_allowed) ? slot0_pat : 0;

		if (!output_allowed || first_index == 8)
			return 0;

		spr_pal       = sprite_attribs[first_index] & 3;
		spr_behind_bg = sprite_attribs[first_index] & 0x20;
		spr_is_s0     = (first_index == 0);
		return first_pat;
	}

	uint8_t slot0_pat = 0;
	uint8_t first_pat = 0;
	unsigned first_index = 8;

	for (unsigned i = 0; i < 8; ++i)
	{
		uint8_t pat = 0;

		if (sprite_shift_count[i] < 8)
		{
			// X counter counts down even if rendering disabled once started.
			if (sprite_x_cnt[i] > 0)
			{
				sprite_x_cnt[i]--;
			}
			else if (vis)
			{
				// Pattern shifters only clock when /VIS is asserted.
				const unsigned p1 = (sprite_pat_h[i] & 0x80) ? 1 : 0;
				const unsigned p0 = (sprite_pat_l[i] & 0x80) ? 1 : 0;
				pat = uint8_t((p1 << 1) | p0);

				sprite_pat_h[i] <<= 1;
				sprite_pat_l[i] <<= 1;
				sprite_shift_count[i]++;
			}
		}

		// Slot0 pixel for sprite0-hit logic.
		if (i == 0)
			slot0_pat = pat;

		// Keep first non-transparent sprite by priority, but do not return yet.
		// All sprite shifters/counters above must still be serviced.
		if (pat && first_index == 8)
		{
			first_pat = pat;
			first_index = i;
		}
	}

	sprite0_pat = (output_allowed && s0_on_cur_scanline) ? slot0_pat : 0;

	if (!output_allowed || first_index == 8)
		return 0;

	spr_pal       = sprite_attribs[first_index] & 3;
	spr_behind_bg = sprite_attribs[first_index] & 0x20;
	spr_is_s0     = (first_index == 0);
	return first_pat;
}

void ppu2c0x_device::do_pixel_output_and_sprite_zero()
{
    bitmap_rgb32& bitmap = *m_bitmap;
    unsigned pixel = dot - 2;
    unsigned pal_index;

    const bool render_line = (scanline <= 239) || (scanline == 261);
    const bool rendering_disabled = (!bg_output_enabled && !spr_output_enabled);

    // If rendering disabled, we still allow sprite unit bookkeeping to run,
    // but we force the output color.
    if (render_line && rendering_disabled)
    {
        {
            bool     spr_behind_bg = false;
            bool     spr_is_s0 = false;
            unsigned spr_pal = 0;
            (void)get_sprite_pixel(spr_pal, spr_behind_bg, spr_is_s0);
            // Do NOT set sprite_zero_hit here; rendering is disabled.
        }

        // Apply retroactive PPUMASK fix to the PREVIOUS pixel first
        if (retro_ppumask_render || retro_ppumask_color)
            retro_fix_previous_pixel_after_ppumask_write();

        uint16_t va = v & 0x3FFF;
        pal_index = ((va & 0x3F00) == 0x3F00) ? (va & 0x1F) : 0;

        bitmap.pix(scanline, pixel) =
            m_nespens[apply_grayscale_and_emphasis(palette_read(pal_index))];

        // Latch this pixel as the "previous pixel" for any later retroactive fix
        prev_pixel_valid = true;
        prev_pixel_scanline = scanline;
        prev_pixel_x = pixel;

        prev_bg_pixel_pat = 0;
        prev_attr_bits = 0;
        prev_spr_pat = 0;
        prev_spr_pal = 0;
        prev_spr_behind_bg = false;
        prev_spr_is_s0 = false;
        prev_sprite0_pat = sprite0_pat;
        prev_backdrop_pal_index = pal_index;

        return;
    }

    unsigned bg_pixel_pat = 0;
    bool     spr_behind_bg = false;
    bool     spr_is_s0 = false;
    unsigned spr_pal = 0;
    unsigned attr_bits = 0;

    // IMPORTANT: BG visibility must use delayed (architectural) enable
    bool bg_visible = bg_output_enabled && (show_bg_left_8 || pixel >= 8);

    unsigned const spr_pat = get_sprite_pixel(spr_pal, spr_behind_bg, spr_is_s0);

    if (bg_visible)
    {
        bg_pixel_pat = (NTH_BIT(bg_shift_h, 15 - fine_x) << 1) |
                       NTH_BIT(bg_shift_l, 15 - fine_x);

        attr_bits = (NTH_BIT(at_shift_h, 15 - fine_x) << 1) |
                    NTH_BIT(at_shift_l, 15 - fine_x);
    }

    // Sprite zero hit conditions
    bool const left8_ok      = (pixel >= 8) || (show_bg_left_8 && show_sprites_left_8);
    bool const right_edge_ok = (dot != 257);

    // Use sprite0_pat (slot0 pixel), NOT the "winning" sprite pixel.
    if (s0_on_cur_scanline &&
        sprite0_pat &&
        bg_pixel_pat &&
        bg_output_enabled && spr_output_enabled &&
        left8_ok &&
        right_edge_ok)
    {
        sprite_zero_hit = true;
    }

    if (spr_pat && !(spr_behind_bg && bg_pixel_pat))
    {
        pal_index = 0x10 + (spr_pal << 2) + spr_pat;
    }
    else
    {
        if (!bg_pixel_pat)
            pal_index = 0;
        else
            pal_index = (attr_bits << 2) | bg_pixel_pat;
    }

    // Apply retroactive PPUMASK fix to the PREVIOUS pixel first
    if (retro_ppumask_render || retro_ppumask_color)
        retro_fix_previous_pixel_after_ppumask_write();

    bitmap.pix(scanline, pixel) =
        m_nespens[apply_grayscale_and_emphasis(palette_read(pal_index))];

    // Latch this pixel as the "previous pixel" for any later retroactive fix
    prev_pixel_valid = true;
    prev_pixel_scanline = scanline;
    prev_pixel_x = pixel;

    prev_bg_pixel_pat = bg_pixel_pat;
    prev_attr_bits = attr_bits;

    prev_spr_pat = spr_pat;
    prev_spr_pal = spr_pal;
    prev_spr_behind_bg = spr_behind_bg;
    prev_spr_is_s0 = spr_is_s0;

    prev_sprite0_pat = sprite0_pat;

    {
        uint16_t va = v & 0x3FFF;
        prev_backdrop_pal_index = ((va & 0x3F00) == 0x3F00) ? (va & 0x1F) : 0;
    }
}
/*
void ppu2c0x_device::do_pixel_output_and_sprite_zero()
{
	bitmap_rgb32& bitmap = *m_bitmap;
	unsigned pixel = dot - 2;
	unsigned pal_index;

	const bool render_line = (scanline <= 239) || (scanline == 261);
	const bool rendering_disabled = (!bg_output_enabled && !spr_output_enabled);

	// If rendering disabled, we still allow sprite unit bookkeeping to run,
	// but we force the output color.
	if (render_line && rendering_disabled)
	{
		{
			bool     spr_behind_bg = false;
			bool     spr_is_s0 = false;
			unsigned spr_pal = 0;
			(void)get_sprite_pixel(spr_pal, spr_behind_bg, spr_is_s0);
			// Do NOT set sprite_zero_hit here; rendering is disabled.
		}

		// Apply retroactive PPUMASK fix to the PREVIOUS pixel first.
		if (retro_ppumask_render || retro_ppumask_color)
			retro_fix_previous_pixel_after_ppumask_write();

		uint16_t va = v & 0x3FFF;
		pal_index = ((va & 0x3F00) == 0x3F00) ? (va & 0x1F) : 0;

		bitmap.pix(scanline, pixel) =
			m_nespens[apply_grayscale_and_emphasis(palette_read(pal_index))];

		// Latch this pixel as the "previous pixel" for any later retroactive fix.
		prev_pixel_valid = true;
		prev_pixel_scanline = scanline;
		prev_pixel_x = pixel;

		prev_bg_pixel_pat = 0;
		prev_attr_bits = 0;
		prev_spr_pat = 0;
		prev_spr_pal = 0;
		prev_spr_behind_bg = false;
		prev_spr_is_s0 = false;
		prev_sprite0_pat = sprite0_pat;
		prev_backdrop_pal_index = pal_index;

		return;
	}

	unsigned bg_pixel_pat = 0;
	bool     spr_behind_bg = false;
	bool     spr_is_s0 = false;
	unsigned spr_pal = 0;
	unsigned attr_bits = 0;

	// Internal/background visibility.
	// Do NOT include bg_pipeline_enabled or inhibit_bg_shift_one_dot here.
	// Accuracy Coin stale BG test #3 needs sprite zero to see stale BG immediately.
	const bool bg_visible =
		bg_output_enabled &&
		(show_bg_left_8 || pixel >= 8);

	unsigned const spr_pat = get_sprite_pixel(spr_pal, spr_behind_bg, spr_is_s0);

	// Sprite output visibility has its own left-8 mask.
	// get_sprite_pixel() still runs for sprite unit bookkeeping.
	const bool spr_visible =
		spr_output_enabled &&
		(show_sprites_left_8 || pixel >= 8);

	const unsigned visible_spr_pat = spr_visible ? spr_pat : 0;

	if (bg_visible)
	{
		bg_pixel_pat = (NTH_BIT(bg_shift_h, 15 - fine_x) << 1) |
					   NTH_BIT(bg_shift_l, 15 - fine_x);

		attr_bits = (NTH_BIT(at_shift_h, 15 - fine_x) << 1) |
					NTH_BIT(at_shift_l, 15 - fine_x);
	}

	// Presentation-only suppression for the first enable-edge pixel.
	// Do NOT use this for sprite-zero hit logic.
	const bool bg_display_visible =
		bg_visible &&
		!inhibit_bg_shift_one_dot;

	const unsigned display_bg_pat = bg_display_visible ? bg_pixel_pat : 0;

	// Sprite zero hit conditions.
	// Use internal bg_pixel_pat, not display_bg_pat.
	const bool right_edge_ok = (dot != 257);

	if (s0_on_cur_scanline &&
		sprite0_pat &&
		bg_pixel_pat &&
		bg_visible &&
		spr_visible &&
		right_edge_ok)
	{
		sprite_zero_hit = true;
	}

	// Final bitmap composition.
	// Use display_bg_pat here so the display-only suppression is respected
	// by both BG drawing and sprite priority.
	if (visible_spr_pat && !(spr_behind_bg && display_bg_pat))
	{
		pal_index = 0x10 + (spr_pal << 2) + visible_spr_pat;
	}
	else
	{
		if (!display_bg_pat)
			pal_index = 0;
		else
			pal_index = (attr_bits << 2) | display_bg_pat;
	}

	// Apply retroactive PPUMASK fix to the PREVIOUS pixel first.
	if (retro_ppumask_render || retro_ppumask_color)
		retro_fix_previous_pixel_after_ppumask_write();

	bitmap.pix(scanline, pixel) =
		m_nespens[apply_grayscale_and_emphasis(palette_read(pal_index))];

	// Latch this pixel as the "previous pixel" for any later retroactive fix.
	prev_pixel_valid = true;
	prev_pixel_scanline = scanline;
	prev_pixel_x = pixel;

	// Store internal BG for retro sprite-zero/render correction.
	prev_bg_pixel_pat = bg_pixel_pat;
	prev_attr_bits = attr_bits;

	// Store display-visible sprite result.
	prev_spr_pat = visible_spr_pat;
	prev_spr_pal = spr_pal;
	prev_spr_behind_bg = spr_behind_bg;
	prev_spr_is_s0 = spr_is_s0;

	prev_sprite0_pat = sprite0_pat;

	{
		uint16_t va = v & 0x3FFF;
		prev_backdrop_pal_index = ((va & 0x3F00) == 0x3F00) ? (va & 0x1F) : 0;
	}
}
*/
inline void ppu2c0x_device::clock_bg_shifters_only()
{
    bg_shift_l = uint16_t(bg_shift_l << 1); // low plane serial-in is 0
    bg_shift_h = uint16_t((bg_shift_h << 1) | 1);
	
    at_shift_l = uint16_t((at_shift_l << 1) | at_latch_l);
    at_shift_h = uint16_t((at_shift_h << 1) | at_latch_h);
}

// only reloads the upper eight bits and
// the attribute bits every eight pixels
void ppu2c0x_device::reload_bg_shift_registers()
{
    if (!(bg_pipeline_enabled || spr_pipeline_enabled)) {
        return;
	}

    // Reload happens once per tile, after the high pattern byte fetch
    // Using your existing timing: dot % 8 == 1
    if ((dot & 7) == 1)
    {
        // Skip exactly ONE reload when rendering is enabled mid-scanline
        if (inhibit_bg_shift_one_dot)
        {
            inhibit_bg_shift_one_dot = false;
            return;
        }

        // ---- Pattern shifter reload (low 8 bits) ----
        bg_shift_l = (bg_shift_l & 0xFF00) | bg_byte_l;
        bg_shift_h = (bg_shift_h & 0xFF00) | bg_byte_h;

        // ---- Attribute latch + reload (8 copies) ----
        at_latch_l = at_byte & 1;
        at_latch_h = (at_byte >> 1) & 1;

        at_shift_l = (at_shift_l & 0xFF00) | (at_latch_l ? 0xFF : 0x00);
        at_shift_h = (at_shift_h & 0xFF00) | (at_latch_h ? 0xFF : 0x00);
    }
}

void ppu2c0x_device::do_sprite_evaluation()
{
	// ------------------------------------------------------------------------
	// Evaluation setup (dot 65)
	//
	// This is the start of the sprite-evaluation band for the scanline.
	// We initialize per-scanline evaluation state here.
	// ------------------------------------------------------------------------
	if (!sprite_eval_initialized)
	{
		sprite_eval_initialized = true;
		s0_on_next_scanline = false;
		// sprite_overflow is intentionally NOT cleared here.
		// It is the PPUSTATUS-visible flag and is cleared elsewhere.

		sprite_eval_in_range       = false;
		sec_oam_addr        = 0;
		overflow_bug_counter  = 0;
		oam_copy_done         = false;

		oam_eval_addr = oam_addr;

		sprite_addr_h = (oam_eval_addr >> 2) & 0x3F;
		sprite_addr_l =  oam_eval_addr       & 0x03;

		sprite0_eval_addr = oam_addr;
		sec_oam_full         = false;
		sec_oam_last_write   = 0xFF;

		s_after_wrap            = false;
		m_eval_wrap_byte             = 0x00;
		m_eval_prev_oam_latch_addr   = 0xFF;

		overflow_eval_phase   = 0;
		overflow_finish_bytes = 0;
	}

	// ------------------------------------------------------------------------
	// Odd dots: read from primary OAM.
	//
	// In this implementation, odd dots perform the primary OAM read and drive
	// the internal OAM bus / $2004-visible latch.
	// ------------------------------------------------------------------------
	if (dot & 0x01)
	{
		oam_latch_addr = (uint8_t)oam_eval_addr;
		oam_data       = oam[oam_eval_addr];

		// Primary OAM drives the bus on odd dots.
		oam_2004_latch = oam_data;

		// Capture the byte read at $FC so post-wrap behavior can replay it.
		if (oam_latch_addr == 0xFC)
			m_eval_wrap_byte = oam_2004_latch;

		// Detect internal OAM address wrap from $FC -> $00.
		if (m_eval_prev_oam_latch_addr == 0xFC && oam_latch_addr == 0x00)
			s_after_wrap = true;

		m_eval_prev_oam_latch_addr = oam_latch_addr;
		return;
	}

	// ------------------------------------------------------------------------
	// Even dots: act on the previously-read primary OAM byte.
	// ------------------------------------------------------------------------
	if (s_after_wrap)
	{
		// After FC->00 wrap, the even-dot bus/latch is forced to the captured
		// wrap byte.
		oam_latch_addr  = 0;
		oam_2004_latch   = m_eval_wrap_byte;
		sec_oam_last_write = oam_2004_latch;
	}

	uint8_t const orig_oam_data   = oam_data;
	int const     sprite_check_y  = scanline & 0xFF;
	int const     spr_h           = (sprite_size == EIGHT_BY_EIGHT) ? 8 : 16;

	uint8_t const startH          = (sprite0_eval_addr >> 2) & 0x3F;
	uint8_t const startL          =  sprite0_eval_addr       & 0x03;
	bool const    misaligned_start = (startL != 0);

	// ------------------------------------------------------------------------
	// End-of-copy handling
	//
	// Once copying is done, evaluation does not necessarily stop immediately.
	// If secondary OAM filled, the PPU continues the failed-copy / OAM2-readback
	// behavior until HBlank.
	// ------------------------------------------------------------------------
	if (oam_copy_done)
	{
		if (sec_oam_full)
		{
			// Keep the OAM2 side visible on $2004 during the remaining eval cycles.
			oam_latch_addr  = 0x00;
			oam_2004_latch   = sec_oam[0];
			sec_oam_last_write = oam_2004_latch;

			// Failed-copy phase scans Y bytes only: n++, m=0
			sprite_addr_h = (sprite_addr_h + 1) & 0x3F;
			sprite_addr_l = 0;

			oam_eval_addr = (sprite_addr_l & 0x03) | ((sprite_addr_h & 0x3F) << 2);
			return;
		}

		sprite_addr_h = (sprite_addr_h + 1) & 0x3F;
		oam_eval_addr = (sprite_addr_l & 0x03) | ((sprite_addr_h & 0x3F) << 2);
		return;
	}

	// ------------------------------------------------------------------------
	// Secondary OAM FULL path
	//
	// Once OAM2 is full, writes become OAM2 readback and evaluation enters the
	// sprite-overflow search / tail behavior.
	// ------------------------------------------------------------------------
	if (sec_oam_addr >= 0x20)
	{
		sec_oam_full = true;

		// With OAM2 full, the OAM2 side becomes readback instead of write.
		// For this logic, keep $2004 seeing OAM2[0]'s Y byte.
		oam_latch_addr  = 0x00;
		oam_2004_latch   = sec_oam[0];
		sec_oam_last_write = oam_2004_latch;

		switch (overflow_eval_phase)
		{
			// ------------------------------------------------------------
			// Phase 0: diagonal overflow search
			//
			// Every current primary OAM byte is treated as a potential Y byte.
			// If not in range: increment n and m without carry.
			// If in range: set overflow and move to phase 1.
			// ------------------------------------------------------------
			case 0:
			{
				// Attribute byte (byte 2) is compared as readback-visible data.
				uint8_t compare_data = orig_oam_data;
				if (sprite_addr_l == 2)
					compare_data &= 0xE3;

				bool const nowInRange =
					(sprite_check_y >= compare_data) &&
					(sprite_check_y < (int(compare_data) + spr_h));

				if (nowInRange)
				{
					sprite_overflow      = true;
					overflow_eval_phase  = 1;
					overflow_finish_bytes = 3;

					// Move to the next entry after the one that matched.
					sprite_addr_l = (sprite_addr_l + 1) & 0x03;
					if (sprite_addr_l == 0)
						sprite_addr_h = (sprite_addr_h + 1) & 0x3F;
				}
				else
				{
					// Hardware bug: increment n and m without carry.
					sprite_addr_h = (sprite_addr_h + 1) & 0x3F;
					sprite_addr_l = (sprite_addr_l + 1) & 0x03;
				}
				break;
			}

			// ------------------------------------------------------------
			// Phase 1: finish the found sprite's next 3 entries
			// ------------------------------------------------------------
			case 1:
			{
				if (overflow_finish_bytes > 0)
					overflow_finish_bytes--;

				sprite_addr_l = (sprite_addr_l + 1) & 0x03;
				if (sprite_addr_l == 0)
					sprite_addr_h = (sprite_addr_h + 1) & 0x3F;

				if (overflow_finish_bytes == 0)
				{
					// After those 3 entries, switch to failed-copy scan.
					overflow_eval_phase = 2;
					sprite_addr_l = 0;
				}
				break;
			}

			// ------------------------------------------------------------
			// Phase 2: failed-copy scan (Y-only)
			//
			// Attempt and fail to copy OAM[n][0], then increment n only.
			// ------------------------------------------------------------
			case 2:
			default:
			{
				sprite_addr_h = (sprite_addr_h + 1) & 0x3F;
				sprite_addr_l = 0;
				break;
			}
		}

		if (sprite_addr_h == 0)
			oam_copy_done = true;

		oam_eval_addr = (sprite_addr_l & 0x03) | ((sprite_addr_h & 0x3F) << 2);
		return;
	}

	// ------------------------------------------------------------------------
	// NOT FULL path
	//
	// Normal sprite evaluation while OAM2 still has room.
	// ------------------------------------------------------------------------
	if (!sprite_eval_in_range)
	{
		bool const is_first_eval_byte =
			(sprite_addr_h == startH) && (sprite_addr_l == startL);

		bool const do_compare =
			is_first_eval_byte ||
			(!misaligned_start ? (sprite_addr_l == 0) : (sprite_addr_l == startL));

		if (do_compare)
		{
			bool const nowInRange =
				(sprite_check_y >= orig_oam_data) &&
				(sprite_check_y < (int(orig_oam_data) + spr_h));

			if (nowInRange)
			{
				sprite_eval_in_range      = true;
				overflow_bug_counter = 0;

				// Sprite 0 is identified when the very first evaluation entry matches.
				if (is_first_eval_byte)
					s0_on_next_scanline = true;
			}
		}
	}

	// ------------------------------------------------------------------------
	// In-range sprite: copy 4 bytes into secondary OAM.
	// ------------------------------------------------------------------------
	if (sprite_eval_in_range)
	{
		oam_latch_addr = (uint8_t)sec_oam_addr;

		if (sprite_addr_l == 2)
		{
			uint8_t const w = orig_oam_data & 0xE3;
			sec_oam[sec_oam_addr] = w;
			sec_oam_last_write       = w;
			oam_2004_latch         = w;
		}
		else
		{
			sec_oam[sec_oam_addr] = orig_oam_data;
			sec_oam_last_write       = orig_oam_data;
			oam_2004_latch         = orig_oam_data;
		}

		sec_oam_addr++;

		if (!sec_oam_full && sec_oam_addr >= 0x20)
			sec_oam_full = true;

		sprite_addr_l++;
		if (sprite_addr_l >= 4)
		{
			sprite_addr_l = 0;
			sprite_addr_h = (sprite_addr_h + 1) & 0x3F;
		}

		overflow_bug_counter++;
		if (overflow_bug_counter >= 4)
		{
			sprite_eval_in_range      = false;
			overflow_bug_counter = 0;

			if (sprite_addr_h == 0)
				oam_copy_done = true;
		}
	}
	else
	{
		// --------------------------------------------------------------------
		// Not in range: repeatedly overwrite the current OAM2 slot without
		// incrementing sec_oam_addr.
		// --------------------------------------------------------------------
		if (!s_after_wrap)
		{
			uint8_t w = orig_oam_data;
			if (sprite_addr_l == 2)
				w &= 0xE3;

			oam_latch_addr        = (uint8_t)sec_oam_addr;
			sec_oam[sec_oam_addr] = w;
			sec_oam_last_write       = w;
			oam_2004_latch         = w;
		}

		sprite_addr_h = (sprite_addr_h + 1) & 0x3F;
		if (misaligned_start)
			sprite_addr_l = (sprite_addr_l + 1) & 0x03;
		else
			sprite_addr_l = 0;

		if (sprite_addr_h == 0)
			oam_copy_done = true;
	}

	// Rebuild the flat primary OAM address from n/m.
	// sprite_addr_h/L are just cached bitfields of the internal eval address:
	//   H = bits 7..2
	//   L = bits 1..0
	oam_eval_addr = (sprite_addr_l & 0x03) | ((sprite_addr_h & 0x3F) << 2);
}

// Returns 'true' if the sprite is in range
bool ppu2c0x_device::calc_sprite_tile_addr(uint8_t y, uint8_t index, uint8_t attrib, bool is_high)
{
    unsigned const diff = (scanline & 0xFF) - y;

    if (sprite_size == EIGHT_BY_EIGHT)
    {
        if (diff >= 8) {
            ppu_addr_bus = sprite_pat_addr + (16 * index) + (8 * is_high);
			ppu_addr_bus &= 0x3FFF;
            return false;
        }

        unsigned const row = (attrib & 0x80) ? (7 - diff) : diff;
        ppu_addr_bus = sprite_pat_addr + (16 * index) + (8 * is_high) + row;
		ppu_addr_bus &= 0x3FFF;
        return true;
    }
    else
    {
        if (diff >= 16) {
           ppu_addr_bus = 0x1000 * (index & 1) + (16 * (index & 0xFE)) + (8 * is_high);
			ppu_addr_bus &= 0x3FFF;
            return false;
        }

        unsigned const row = (attrib & 0x80) ? (15 - diff) : diff;

       ppu_addr_bus =
		0x1000 * (index & 1) +
		(16 * (index & 0xFE)) +
		((row & 8) << 1) +
		(8 * is_high) +
		(row & 7);
	ppu_addr_bus &= 0x3FFF;

        return true;
    }
}

// Initializes the sprite output units with the sprites that were copied into
// the secondary OAM during sprite evaluation
void ppu2c0x_device::do_sprite_loading()
{
	const unsigned sprite_n = (dot - 257) / 8;
	const unsigned base = (sprite_n * 4) & 0x1F;

	// --------------------------------------------------
	// OAM2 visibility / $2004 latch side
	// --------------------------------------------------
	//
	// During sprite fetch dots 257-320:
	//   - OAM2 is read every dot.
	//   - OAM2ADDR increments when ((dot - 1) & 4) == 0.
	//   - dot 257 clears OAM2ADDR and suppresses that dot's increment.
	//
	// Important:
	// This models OAM2 read/latch visibility only.
	// Do NOT use this timing to decide when the sprite render units receive
	// Y/tile/attr/X, or sprite-0 timing breaks.
	const uint8_t oam2_read = sec_oam[sec_oam_addr & 0x1F];

	oam_data       = oam2_read;
	oam_latch_addr = sec_oam_addr & 0x1F;
	oam_2004_latch  = oam2_read;

	switch ((dot - 1) & 7)
	{
		// dot 257, 265, 273...
		// Set up garbage NT fetch #1.
		//
		// SPECIAL MIXED ADDRESS:
		// low 8 bits from OLD v, high 6 bits from horizontally reloaded v.
		case 0:
		{
			// Only the VERY FIRST garbage NT fetch, dot 257 setup / dot 258 read,
			// uses the special mixed address.
			//
			// Build real nametable addresses first, then mix:
			//   low  8 bits = old/pre-reload NT address
			//   high 6 bits = new/post-horizontal-reload NT address
			//
			// Do not mix raw v values directly, because raw v does not include the
			// forced $2000 nametable base. Mixing raw v can accidentally put the bus
			// in $0000-$0FFF pattern space.
			if (dot == 257)
			{
				const uint16_t old_nt_addr = (0x2000 | (sprite_nt_fetch_v     & 0x0FFF)) & 0x3FFF;
				const uint16_t new_nt_addr = (0x2000 | (sprite_nt_fetch_v_new & 0x0FFF)) & 0x3FFF;

				ppu_addr_bus = (new_nt_addr & 0x3F00) | (old_nt_addr & 0x00FF);
				ppu_addr_bus &= 0x3FFF;
			}
			else
			{
				ppu_addr_bus = (0x2000 | (sprite_nt_fetch_v_new & 0x0FFF)) & 0x3FFF;
			}

			// Render-unit load timing preserved from the old working path.
			// Do not use sec_oam_addr here; it now models OAM2 visibility.
			sprite_y = sec_oam[(base + 0) & 0x1F];
			break;
		}

		// dot 258, 266, 274...
		// Read garbage NT fetch #1.
		case 1:
		{
			ppu_bus_read_can_fill_2007 = true;
			(void)ppu_bus_read(ppu_addr_bus, ppu_fetch_phase::SPR_NT);
			ppu_bus_read_can_fill_2007 = false;

			// Render-unit load timing preserved from the old working path.
			sprite_index = sec_oam[(base + 1) & 0x1F];
			break;
		}

		// dot 259, 267, 275...
		// Set up garbage NT fetch #2.
		//
		// This uses the normal upcoming-scanline NT address, not old v.
		case 2:
		{
			ppu_addr_bus = (0x2000 | (sprite_nt_fetch_v_new & 0x0FFF)) & 0x3FFF;

			// Render-unit load timing preserved from the old working path.
			sprite_attribs[sprite_n] = sec_oam[(base + 2) & 0x1F];
			break;
		}

		// dot 260, 268, 276...
		// Read garbage NT fetch #2 / AT-style fetch slot.
		case 3:
		{
			ppu_bus_read_can_fill_2007 = true;
			(void)ppu_bus_read(ppu_addr_bus, ppu_fetch_phase::SPR_AT);
			ppu_bus_read_can_fill_2007 = false;

			// Render-unit load timing preserved from the old working path.
			sprite_x[sprite_n] = sec_oam[(base + 3) & 0x1F];
			sprite_x_cnt[sprite_n] = sprite_x[sprite_n];
			break;
		}

		// dot 261, 269, 277...
		// Set up sprite pattern low fetch.
		case 4:
		{
			sprite_in_range = calc_sprite_tile_addr(
				sprite_y,
				sprite_index,
				sprite_attribs[sprite_n],
				false);
			break;
		}

		// dot 262, 270, 278...
		// Read sprite pattern low fetch.
		case 5:
		{
			ppu_bus_read_can_fill_2007 = true;
			uint8_t const pat = ppu_bus_read(ppu_addr_bus, ppu_fetch_phase::SPR_PTL);
			ppu_bus_read_can_fill_2007 = false;

			sprite_pat_l[sprite_n] = sprite_in_range ? pat : 0;

			if (sprite_in_range && (sprite_attribs[sprite_n] & 0x40))
				sprite_pat_l[sprite_n] = rev_byte(sprite_pat_l[sprite_n]);

			break;
		}

		// dot 263, 271, 279...
		// Set up sprite pattern high fetch.
		case 6:
		{
			sprite_in_range = calc_sprite_tile_addr(
				sprite_y,
				sprite_index,
				sprite_attribs[sprite_n],
				true);
			break;
		}

		// dot 264, 272, 280...
		// Read sprite pattern high fetch.
		case 7:
		{
			ppu_bus_read_can_fill_2007 = true;
			uint8_t const pat = ppu_bus_read(ppu_addr_bus, ppu_fetch_phase::SPR_PTH);
			ppu_bus_read_can_fill_2007 = false;

			sprite_pat_h[sprite_n] = sprite_in_range ? pat : 0;

			if (scanline == 261 && sprite_n == 0 && sprite_in_range)
			{
				sl0_stale_s0_loaded = true;
				sl0_stale_sprite0_identity = true;
			}

			if (sprite_in_range && (sprite_attribs[sprite_n] & 0x40))
				sprite_pat_h[sprite_n] = rev_byte(sprite_pat_h[sprite_n]);

			sprite_shift_count[sprite_n] = 0;
			break;
		}

		default:
			break;
	}

	// --------------------------------------------------
	// OAM2ADDR increment timing during sprite loading
	// --------------------------------------------------
	//
	// Read/latch happens first. Then OAM2ADDR advances for the next dot.
	//
	// The $2004-visible OAM buffer should see, per 8-dot sprite-loading slot:
	//
	//   byte 0, byte 1, byte 2, byte 3, byte 3, byte 3, byte 3, byte 3
	//
	// That means:
	//   phase 0: read byte 0, advance to byte 1
	//   phase 1: read byte 1, advance to byte 2
	//   phase 2: read byte 2, advance to byte 3
	//   phase 3: read byte 3, hold
	//   phase 4: read byte 3, hold
	//   phase 5: read byte 3, hold
	//   phase 6: read byte 3, hold
	//   phase 7: read byte 3, advance to next sprite's byte 0
	//
	// Without the phase-7 advance, the next sprite slot starts by reading the
	// previous sprite's byte 3 one extra time.
	const unsigned spr_load_phase = (dot - 1) & 7;

	if (spr_load_phase < 3 || spr_load_phase == 7)
		sec_oam_addr = (sec_oam_addr + 1) & 0x1F;
}

// Called for dots on the pre-render line
void ppu2c0x_device::run_prerender_scanline_dot() {
	if (dot == 1) {
		sprite_overflow = sprite_zero_hit =  false;
		in_vblank = false;
		set_nmi(false);
	}
			
	// Sprite0 identity latch (one-shot) at dot 257.
	// Evaluation on scanline N determines identity for scanline N+1.
	if (dot == 257) {
		s0_on_cur_scanline = false;
		s0_on_next_scanline = false;
	}

	// === BG shift clocking happens regardless of rendering (composite) ===
	if ((bg_pipeline_enabled || spr_pipeline_enabled) && ((dot >= 2 && dot <= 257) || (dot >= 322 && dot <= 337))) // (bg_pipeline_enabled || spr_pipeline_enabled)  (bg_output_enabled || spr_output_enabled)
	{ 
		 clock_bg_shifters_only();
	}
		 
	run_render_pipeline_dot();
	if (bg_pipeline_enabled || spr_pipeline_enabled) {		//(bg_output_enabled || spr_output_enabled) { //(bg_pipeline_enabled || spr_pipeline_enabled) {			
		switch (dot) {
			case 65 ... 256:
				do_prerender_oam_sweep();
			break;
		}

		if (dot >= 280 && dot <= 304) {
			scroll_copy_v_pending = true;
		}
	}
}

void ppu2c0x_device::do_prerender_oam_sweep()
{
    // /VIS is deasserted for the whole pre-render scanline (Fiskbit).
    // So: OAM2 selected, automatic writes become reads, and OAM1 is not touched.

    if (dot == 65) {
        // Don't touch oam_addr / oam_eval_addr at all.
        // Just start a harmless OAM2 read sweep if you want.
        sec_oam_addr = 0;
    }

    // Read OAM2 every dot in this phase.
    oam_data = sec_oam[sec_oam_addr & 0x1F];
	oam_2004_latch = oam_data;
	oam_latch_addr = sec_oam_addr;
	
    // Optionally advance OAM2ADDR occasionally to "sweep" and refresh for decay behavior.
    // The exact increment cadence is analog-weird and probably not test-critical,
    // but advancing every other dot is a reasonable approximation.
    if (!(dot & 1)) {
        sec_oam_addr = (sec_oam_addr + 1) & 0x1F;
    }
}


// Called for dots on line 241
void ppu2c0x_device::run_scanline_241_dot()
{
    if (!suppress_vblank_flag)
    {
        if (dot == 1 && !in_vblank)
        {
            in_vblank = true;

            if (nmi_on_vblank)
                set_nmi(true);
        }
    }
}

void ppu2c0x_device::do_2007_post_access_bump() {
	if ((bg_pipeline_enabled || spr_pipeline_enabled) && (scanline < 240 || scanline == m_prerender_line)) {
        // Accessing $2007 during rendering performs this glitch. Used by Young
        // Indiana Jones Chronicles to shake the screen.
		scroll_inc_h_pending = true;
		scroll_inc_v_pending = true;
    }
    // The incrementation operation can touch the high bit even though it's not
    // used for addressing (it's the high bit of fine y)
    else {
        v = (v + v_inc) & 0x7FFF;
        // The PPU address bus mirrors v outside of rendering
       ppu_bus_address_drive(v);
	}
}

void ppu2c0x_device::write_oam_dma_byte(uint8_t val)
{
    oam[oam_addr] = val;
    oam_addr = (oam_addr + 1) & 0xFF;
}

void ppu2c0x_device::write_oam_data_reg(uint8_t val)
{
	// $2004 writes during rendering:
    //   - visible scanlines 0-239
    //   - pre-render scanline 261
    //   - only if BG or sprites are enabled
    //
    // They do not write to primary OAM, but they perform the glitchy
    // OAMADDR increment. For this test/model, bump the sprite index by one:
    // +4, then force byte index to 0 with & $FC.
    const bool rendering_on = (bg_pipeline_enabled || spr_pipeline_enabled);
    const bool render_line  = (scanline < 240) || (scanline == 261);

    if (render_line && rendering_on) {
		oam_2004_latch = val;
        oam_addr = (uint8_t)(oam_addr + 4);
        oam_addr &= 0xFC;
        return;
    }

    oam[oam_addr] = val;
    oam_addr = (uint8_t)(oam_addr + 1);
}

void ppu2c0x_device::apply_scroll_ops()
{
	if (!scroll_inc_h_pending &&
		!scroll_inc_v_pending &&
		!scroll_copy_h_pending &&
		!scroll_copy_v_pending)
		return;

	// Resolve all queued scroll operations for this PPU dot.
	//
	// Important:
	// Increment operations are applied first, giving the "incremented v" input.
	// Copy operations use t as the other input.
	//
	// If an increment and t->v copy affect the same component on the same dot,
	// hardware bus-conflicts that component only:
	//
	//   result = incremented_v_component & t_copy_component
	//
	// The conflicted component is written back into BOTH v and t.
	//
	// Horizontal component mask:
	//   coarse X bits + horizontal nametable bit = 0x041F
	//
	// Vertical component mask:
	//   coarse Y + fine Y + vertical nametable bit = 0x7BE0
	static constexpr uint16_t H_MASK = 0x041F;
	static constexpr uint16_t V_MASK = 0x7BE0;

	if (scroll_inc_h_pending)
		bump_horiz();

	if (scroll_inc_v_pending)
		bump_vert();

	const uint16_t inc_v = v & 0x7FFF;
	uint16_t new_v = inc_v;

	if (scroll_copy_h_pending)
	{
		const uint16_t t_h = t & H_MASK;

		if (scroll_copy_conflict_h_pending)
		{
			const uint16_t conflicted_h = (inc_v & t_h) & H_MASK;

			new_v = (new_v & ~H_MASK) | conflicted_h;

			// The bus conflict corrupts the copied component in both v and t.
			t = (t & ~H_MASK) | conflicted_h;
		}
		else
		{
			new_v = (new_v & ~H_MASK) | t_h;
		}
	}

	if (scroll_copy_v_pending)
	{
		const uint16_t t_v = t & V_MASK;

		if (scroll_copy_conflict_v_pending)
		{
			const uint16_t conflicted_v = (inc_v & t_v) & V_MASK;

			new_v = (new_v & ~V_MASK) | conflicted_v;

			// The bus conflict corrupts the copied component in both v and t.
			t = (t & ~V_MASK) | conflicted_v;
		}
		else
		{
			new_v = (new_v & ~V_MASK) | t_v;
		}
	}

	v = new_v & 0x7FFF;

	scroll_inc_h_pending = false;
	scroll_inc_v_pending = false;
	scroll_copy_h_pending = false;
	scroll_copy_v_pending = false;
	scroll_copy_conflict_pending = false;
	scroll_copy_conflict_h_pending = false;
	scroll_copy_conflict_v_pending = false;
}

//The coarse X component of v needs to be incremented when the next tile is reached. Bits 0-4 are incremented, with overflow toggling bit 10. 
//This means that bits 0-4 count from 0 to 31 across a single nametable, and bit 10 selects the current nametable horizontally.
void ppu2c0x_device::bump_horiz() {
	// Coarse x equal to 31?
    if ((v & 0x1F) == 0x1F) {
        // Set coarse x to 0 and switch horizontal nametable. The bit twiddling
        // to clear the lower five bits relies on them being 1.
        v ^= 0x041F;
    } else { ++v; }
	v &= 0x7FFF; // <<< REQUIRED
}

//If rendering is enabled, fine Y is incremented at dot 256 of each scanline, overflowing to coarse Y, and finally adjusted to wrap among the nametables vertically
//Bits 12-14 are fine Y. Bits 5-9 are coarse Y. Bit 11 selects the vertical nametable.
void ppu2c0x_device::bump_vert() {
	// Fine y equal to 7?
    if ((v & 0x7000) == 0x7000) {
        // Check coarse y
        switch (v & 0x03E0) {
			// Coarse y equal to 29. Switch vertical nametable (XOR by 0x0800) and
			// clear fine y and coarse y in the same operation (possible since we
			// know their value).
			case 29 << 5: v ^= 0x7800 | (29 << 5); break;

			// Coarse y equal to 31. Clear fine y and coarse y without switching
			// vertical nametable (this occurs for vertical scroll values > 240).
			case 31 << 5: v &= ~0x73E0; break;

			// Clear fine y and increment coarse y
			default: v = (v & ~0x7000) + 0x0020;
			}
	} else {
        // Bump fine y
        v += 0x1000;
	}
	v &= 0x7FFF; // <<< REQUIRED
}

// Restores the horizontal bits in v from t at the end of each scanline during
// rendering
void ppu2c0x_device::copy_horiz() {
     // v: ... .H.. ...E DCBA = t: ... .H.. ...E DCBA
    v = (v & ~0x041F) | (t & 0x041F);
	v &= 0x7FFF;
}

// Initializes the vertical bits in v from t on the pre-render line
void ppu2c0x_device::copy_vert() {
     // v: IHG F.ED CBA. .... = t: IHG F.ED CBA. ....
    v = (v & ~0x7BE0) | (t & 0x7BE0);
	v &= 0x7FFF;
}


/*************************************
*
*   PPU Memory functions
*
*************************************/

void ppu2c0x_device::palette_write(offs_t offset, uint8_t val) {

	// 1. Address Masking: Mask the address to the 32-entry palette RAM range ($3F00-$3F1F).
	offset &= 0x1F;
	
	// 2. Value Masking: Mask the value to the available 6 bits (64 colors total).
	val &= 0x3F;
	
	// 3. Perform the primary write to the target address.
	m_palette_ram[offset] = val;
	
	// 4. Handle Backdrop Mirroring: 
	// Check if the offset is a backdrop address (multiples of 4: xF00, xF04, xF08, xF0C, xF10, xF14, xF18, xF1C).
	// These addresses are electrically linked.
	if( !(offset & 0x3) )
		// Mirror the written value to its linked address (XOR 0x10 toggles between 
		// the Background Palette 0x0X and the Sprite Palette 0x1X spaces).
		// This ensures $3F00 and $3F10 (and their mirrors) always contain the same universal color.
		m_palette_ram[offset ^ 0x10] = val;
}

uint8_t ppu2c0x_device::palette_read(offs_t offset) {
	// 1. Address Masking: Mask the address to the 32-entry palette RAM range ($3F00-$3F1F).
	offset &= 0x1f;
	
	// 2. Sprite Backdrop Mirroring:
	// If the address is a Sprite Backdrop entry (0x10, 0x14, 0x18, 0x1C), 
	// the hardware forces a read from the corresponding Background Palette index (0x00, 0x04, etc.).
	// Clearing bit 4 (0x10) performs this mapping.
	// Note: This logic relies on the palette_write function to ensure all backdrop entries are linked.
	if(offset == 0x10 || offset == 0x14 || offset == 0x18 || offset == 0x1C)
		offset &= ~0x10;
		
	// 3. Perform the read and apply grayscale if enabled (grayscale_color_mask is set by $2001 bit 0).
	//return m_palette_ram[offset] & grayscale_color_mask;
	return m_palette_ram[offset] & 0x3F;
}


/*************************************
 *
 *  PPU Registers Read
 *
 *************************************/

uint8_t ppu2c0x_device::read(offs_t offset)
{
	switch (offset & 7)
	{
		case 0: case 1: case 3: case 5: case 6: return ppu_open_bus_peek();
		case PPU_STATUS: /* 2 */
		{
			// Read-start sample for vblank
			bool vblank_read = in_vblank;
			bool spr0_read = sprite_zero_hit;
			bool ovf_read  = sprite_overflow;

			// Scheduler convention: dot is the next PPU dot to execute.
			// dot == 1 here means hardware dot 0 has completed, but dot 1 has not run yet.
			// This models the $2002 read-on-241,0 case that suppresses the upcoming vblank set.
			if (scanline == 241 && dot == 1) {
				vblank_read = false;
				suppress_vblank_flag = true;
			}
			// Readback quirk: at prerender clear edge, sprite flags read as cleared here
			if (scanline == 261 && dot == 1) {
				spr0_read = false;
				ovf_read = false;
			}

			write_flip_flop = false;

			const uint8_t old_bus = ppu_open_bus_peek();

			const uint8_t ret =
				(vblank_read ? 0x80 : 0x00) |
				(spr0_read   ? 0x40 : 0x00) |
				(ovf_read    ? 0x20 : 0x00) |
				(old_bus & 0x1f);

			in_vblank = false;
			set_nmi(false);

			// $2002 read drives only status bits 7-5.
			// Low bits 4-0 are open bus and must keep their existing decay timers.
			ppu_open_bus_drive_masked(ret, 0xe0);

			return ret;
		}


		case PPU_SPRITE_DATA: /* $2004 */
		{
			const bool render_line = (scanline < 240) || (scanline == 261);
			const bool rendering_enabled = (bg_pipeline_enabled || spr_pipeline_enabled);//(bg_output_enabled || spr_output_enabled);
			
			uint8_t ret = 0;

			if (render_line && rendering_enabled)
			{
				if (dot == 1) {
					//Reading from $2004 (with rendering enabled) on dot 0 should return Secondary_OAM Index 0.
					ret = sec_oam[0];
				} else if (dot >= 2 && dot <= 65) {
					//Reading from $2004 (with rendering enabled) from dots 1 through dots 64 should return #$FF.
					ret = oam_2004_latch;
				} else if (dot >= 66 && dot <= 257) {
					//Reading from $2004 (with rendering enabled) from dots 65 through 256 should read from the "OAM Latch" used during OAM Evaluation.
					ret = oam_2004_latch;
					if ((oam_latch_addr & 3) == 2) 
						ret &= 0xE3;
				} else if (dot >= 258 && dot <= 321) {
					//Reading from $2004 (with rendering enabled) from dots 257 through 320 should read from secondary OAM.
					ret = oam_2004_latch;
				} else { 
					//A: Reading from $2004 (with rendering enabled) from dots 321 through 340 should read from index 0 of secondary OAM.
					ret = sec_oam[0];
				}
			}
			else {
				// Non-rendering: CPU reads primary OAM at OAMADDR
				ret = oam[oam_addr];
				if ((oam_addr & 3) == 2) ret &= 0xE3;
			}
			
			// $2004 read drives the PPU internal data bus.
			ppu_open_bus_drive(ret);

			return ppu_open_bus_peek();
		}
		
		case PPU_DATA: /* $2007 */
		{
			const uint16_t bus_addr = v & 0x3FFF;

			// CPU access places the current VRAM address on the PPU address bus.
			ppu_bus_address_drive(bus_addr);

			uint8_t ret;

			if (bus_addr >= 0x3F00 && bus_addr <= 0x3FFF)
			{
				const uint8_t old_bus = ppu_open_bus_peek();

				ret = (old_bus & 0xC0) |
					  (m_palette_ram[bus_addr & 0x1F] & grayscale_color_mask);

				// Palette reads drive only bits 5-0.
				// Bits 7-6 remain open bus.
				ppu_open_bus_drive_masked(ret, 0x3F);
			}
			else
			{
				// Normal reads return the existing buffered value.
				ret = ppu_data_reg;
				ppu_open_bus_drive(ret);
			}

			const bool rendering_for_access =
				(bg_pipeline_enabled || spr_pipeline_enabled) &&
				((scanline < 240) || (scanline == m_prerender_line));

			if (!rendering_for_access)
			{
				// Non-rendering:
				// The CPU access increments v now.
				do_2007_post_access_bump();

				// The internal read buffer refills later from the accessed address.
				schedule_2007_read(bus_addr, 5, false);
			}
			else
			{
				// Rendering:
				// Do not perform a hidden memory read here.
				// This is only for CPU $2007 reads during rendering; normal rendering fetches
				// do not update ppu_data_reg unless a pending $2007 refill is armed.
				// The rendering H+V increment glitch is scheduled when the delayed read matures.
				schedule_2007_read(bus_addr, 4, true);
			}

			ppu_bus_address_drive(v);

			return ppu_open_bus_peek();
		}
			
		default:
			logerror("invalid ppu read at $%04X\n", offset);
		break;
	}
	return ppu_open_bus_peek();
}
	
void ppu2c0x_device::apply_delayed_2000(uint8_t val) {
	// Only apply the PPUCTRL bits that affect delayed fetch/render pipeline state.
	
	// Bit 3: sprite pattern table select for 8x8 sprites.
	sprite_pat_addr = (val & 0x08) ? 0x1000 : 0x0000;
	// Bit 4: background pattern table select.
	bg_pat_addr     = (val & 0x10) ? 0x1000 : 0x0000;
	// Bit 5: sprite size.
	sprite_size     = (val & 0x20) ? EIGHT_BY_SIXTEEN : EIGHT_BY_EIGHT;
}

void ppu2c0x_device::apply_delayed_2001(uint8_t val)
{
	// Fetch pipeline enable.
	bool const prev_pipe = (bg_pipeline_enabled || spr_pipeline_enabled);

	const bool new_bg_on  = (val & 0x08) != 0;
	const bool new_spr_on = (val & 0x10) != 0;

	bg_pipeline_enabled  = new_bg_on;
	spr_pipeline_enabled = new_spr_on;

	// Output ON is delayed to the same edge as the pipe.
	// Output OFF already happened immediately in the $2001 write handler.
	if (new_bg_on)
		bg_output_enabled = true;

	if (new_spr_on)
		spr_output_enabled = true;

	// Clip vars follow output domain.
	bg_clip_comp     = !bg_output_enabled  ? 256 : show_bg_left_8      ? 0 : 8;
	sprite_clip_comp = !spr_output_enabled ? 256 : show_sprites_left_8 ? 0 : 8;

	bool const new_pipe = (bg_pipeline_enabled || spr_pipeline_enabled);

	// --------------------------------------------------
	// BGSerialIn behavior:
	// If rendering is enabled mid-scanline, the next BG reload slot must be
	// skipped once, so the shifters keep shifting and the serial input becomes visible.
	// --------------------------------------------------
	if (!prev_pipe && new_pipe)
	{
		const bool render_line = (scanline <= 239) || (scanline == 261);
		const bool in_fetch_region =
			((dot >= 2 && dot <= 257) || (dot >= 322 && dot <= 337));

		if (render_line && in_fetch_region)
			inhibit_bg_shift_one_dot = true;
	}

	// --------------------------------------------------
	// OAM row corruption seed.
	// --------------------------------------------------
	if (prev_pipe && !new_pipe)
	{
		inhibit_bg_shift_one_dot = false;

		const bool render_line = (scanline <= 239) || (scanline == 261);
		const bool early_window = (dot >= 1 && dot <= 64);
		const bool late_window  = (dot >= 257 && dot <= 320);

		if (render_line && (early_window || late_window))
		{
			uint8_t seed = 0;

			if (early_window)
			{
				seed = (uint8_t)((dot >> 1) & 0x1F);
			}
			else
			{
				static constexpr uint8_t late_row_by_dot[64] = {
					0, 1, 2, 3, 3, 3, 3, 3,
					4, 5, 6, 7, 7, 7, 7, 7,
					8, 9,10,11,11,11,11,11,
				   12,13,14,15,15,15,15,15,
				   16,17,18,19,19,19,19,19,
				   20,21,22,23,23,23,23,23,
				   24,25,26,27,27,27,27,27,
				   28,29,30,31,31,31,31,31
				};

				seed = late_row_by_dot[dot - 257] & 0x1F;
			}

			oam_corrupt_seed    = seed;
			oam_corrupt_pending = true;
		}
	}
}

/*************************************
 *
 *  PPU Registers Write
 *
 *************************************/
void ppu2c0x_device::write(offs_t offset, uint8_t val)
{
	ppu_open_bus_drive(val);

	switch (offset & 7)
	{
		case PPU_CONTROL0: /* 0 */
		{
			t               = (t & 0x73FF) | ((val & 0x03) << 10);
			v_inc           = (val & 0x04) ? 32 : 1;
			
			bool new_nmi_on_vblank = val & 0x80;

			// In this scheduler, $2000 writes are handled before the per-dot PPU work,
			// so a write on the exact pre-render clear tick can still see in_vblank = true
			// even though hardware would already be past the enable window.
			bool vblank_seen_for_enable = in_vblank;
			if (scanline == 261 && dot == 1) { //needed for nmi "on" timing
				vblank_seen_for_enable = false;
			}

			// Enabling NMI while VBL is already set retriggers NMI, except on the clear-edge tick.
			if (new_nmi_on_vblank && !nmi_on_vblank && vblank_seen_for_enable) {
				nmi_pending = false;
				nmi_delay = 0;
				m_maincpu6502->queue_delayed_nmi(2);
			}

			// Disabling NMI near the VBL-set edge suppresses the pending NMI for one more
			// scheduler tick than the raw hardware dot because writes are processed before
			// the PPU edge in this core.
			if (!new_nmi_on_vblank && nmi_on_vblank && scanline == 241 && dot >= 1 && dot <= 3) {
				set_nmi(false);
			}

			nmi_on_vblank = new_nmi_on_vblank;
			m_regs[PPU_CONTROL0] = val;
			pending_2000.has_pending = true;
			pending_2000.value = val;
			pending_2000.apply_ppu = 3;
		break;
		}
		
		case PPU_CONTROL1:
		{
			uint8_t old_ppumask = m_regs[PPU_CONTROL1];

			const bool new_bg_on  = (val & 0x08) != 0;
			const bool new_spr_on = (val & 0x10) != 0;

			// Delay pipeline/render-domain change by 3 PPU cycles.
			pending_2001.has_pending = true;
			pending_2001.value       = val;
			pending_2001.apply_ppu   = 2;

			// Output OFF is immediate.
			// Output ON is delayed and happens in apply_delayed_2001() with the pipe.
			if (!new_bg_on)
				bg_output_enabled = false;

			if (!new_spr_on)
				spr_output_enabled = false;

			// Immediate visual-only properties.
			m_regs[PPU_CONTROL1] = val;

			show_bg_left_8      = (val & 0x02) != 0;
			show_sprites_left_8 = (val & 0x04) != 0;

			grayscale_color_mask = (val & PPU_CONTROL1_DISPLAY_MONO) ? 0x30 : 0x3F;
			tint_bits = uint16_t(val & PPU_CONTROL1_COLOR_EMPHASIS) << 1;

			// Clip vars follow output domain.
			bg_clip_comp     = !bg_output_enabled  ? 256 : show_bg_left_8      ? 0 : 8;
			sprite_clip_comp = !spr_output_enabled ? 256 : show_sprites_left_8 ? 0 : 8;

			// Retroactive color/render correction.
			if (scanline <= 239 && dot >= 2 && dot <= 257)
			{
				uint8_t diff = old_ppumask ^ val;
				const int late_delta = dot - prev_pixel_x;

				const bool late_prev_pixel =
					prev_pixel_valid &&
					prev_pixel_scanline == scanline &&
					prev_pixel_x >= 0 &&
					prev_pixel_x < 256 &&
					late_delta == 3;

				if ((diff & (0x01 | 0xE0)) && late_prev_pixel)
					retro_ppumask_color = true;

				if ((diff & 0x18) && odd_frame && late_prev_pixel)
					retro_ppumask_render = true;
			}

			break;
		}

		case 2:	break;
			
		case PPU_SPRITE_ADDRESS: /* 3 */
			m_regs[PPU_SPRITE_ADDRESS] = val;
			oam_addr = val;
			break;

		case PPU_SPRITE_DATA: 
			write_oam_data_reg(val);
			break;

		case PPU_SCROLL: /* 5 */
		{
			//const bool rendering_now = (bg_pipeline_enabled || spr_pipeline_enabled) && ((scanline <= 239) || (scanline == 261));
			if (!write_flip_flop) {
				// First write
				// fine_x = val: .... .ABC
				// t: ... .... ...D EFGH = val: DEFG H...
				fine_x = val & 7;
				t      = (t & 0x7FE0) | ((val & 0xF8) >> 3);
			} else {
				// Second write
				// t: ABC ..DE FGH. .... = val: DEFG HABC
				t = (t & 0x0C1F) | ((val & 0xF8) << 2) | ((val & 7) << 12);
			}
			write_flip_flop = !write_flip_flop;
        break;
		}
		
		case PPU_ADDRESS: /* $2006 */
		{
			if (!write_flip_flop) {
				t = (t & 0x00FF) | ((val & 0x3F) << 8);
			}
			else {
				t = (t & 0x7F00) | val;
				pending_2006.has_pending = true;
				pending_2006.value16 = t & 0x7FFF;
				pending_2006.apply_ppu = 4;
			}

			write_flip_flop = !write_flip_flop;
			break;
		}
		
		
		case PPU_DATA: /* 7 */
		{
			uint16_t bus_addr = v & 0x3FFF;

			// Bus sees the access address now.
			ppu_bus_address_drive(bus_addr);

			const bool rendering_for_access =
				(bg_pipeline_enabled || spr_pipeline_enabled) &&
				((scanline < 240) || (scanline == m_prerender_line));

			if (!rendering_for_access)
			{
				// Non-rendering:
				// $2007 write takes effect at the current address now.
				writebyte(bus_addr & 0x3FFF, val);

				// The $2007 post-access increment is NOT immediate.
				// Hardware leaves the current palette/address visible briefly after
				// the write becomes effective. This lets a palette write show the newly
				// written color for 1 dot before v advances.
				ppu2007_post_bump_pending = true;
				ppu2007_post_bump_delay = 1;

				// Keep the bus at the access address until the delayed bump runs.
				ppu_bus_address_drive(bus_addr);
			}
			else
			{
				// Rendering:
				// The write itself is delayed. Do NOT bump v here.
				// The bump is armed when the delayed write actually reaches the PPU bus,
				// so the increment is delayed relative to the effective write, not merely
				// relative to the CPU register write.
				schedule_2007_write(bus_addr & 0x3FFF, val, 5);

				// Keep the bus at the access address for now.
				ppu_bus_address_drive(bus_addr);
			}
			break;
		}

		default:
			logerror("invalid ppu write: $%02X at $%04X\n", val, offset);
			/* ignore other registers writes */
			break;
	}
}

uint64_t ppu2c0x_device::ppudecay_now() const
{
	// Use whatever your CPU-cycle source is here.
	// In your tree this may be m_maincpu6502->total_cycles().
	return m_maincpu6502 ? m_maincpu6502->total_cycles() : 0;
}

uint64_t ppu2c0x_device::ppudecay_count_to_cpu_cycles(uint16_t count) const
{
	// The inner loop is roughly 15 CPU cycles per successful compare loop.
	// The +8 is the small setup/read-position fudge needed because the first
	// CMP does not happen immediately after STA $2002.
	return uint64_t(count) * 15 + 8;
}

void ppu2c0x_device::ppu_open_bus_drive(uint8_t data)
{
	const uint64_t now = ppudecay_now();

	m_ppu_io_db = data;

	for (int bit = 0; bit < 8; bit++)
	{
		const uint8_t mask = 1U << bit;

		if (data & mask)
			m_ppu_io_db_decay_at[bit] = now + ppudecay_count_to_cpu_cycles(ppudecay_count_for_bit[bit]);
		else
			m_ppu_io_db_decay_at[bit] = 0;
	}
}

uint8_t ppu2c0x_device::ppu_open_bus_peek()
{
	const uint64_t now = ppudecay_now();

	for (int bit = 0; bit < 8; bit++)
	{
		const uint8_t mask = 1U << bit;

		if ((m_ppu_io_db & mask) && m_ppu_io_db_decay_at[bit] && now >= m_ppu_io_db_decay_at[bit])
			m_ppu_io_db &= ~mask;
	}

	return m_ppu_io_db;
}

void ppu2c0x_device::ppu_open_bus_drive_masked(uint8_t data, uint8_t mask)
{
	const uint64_t now = ppudecay_now();

	// Only masked bits are newly driven/refreshed.
	m_ppu_io_db = (m_ppu_io_db & ~mask) | (data & mask);

	for (int bit = 0; bit < 8; bit++)
	{
		const uint8_t b = 1U << bit;

		if (!(mask & b))
			continue;

		if (data & b)
			m_ppu_io_db_decay_at[bit] = now + ppudecay_count_to_cpu_cycles(ppudecay_count_for_bit[bit]);
		else
			m_ppu_io_db_decay_at[bit] = 0;
	}
}

uint16_t ppu2c0x_device::get_vram_dest()
{
	return v;
}

void ppu2c0x_device::set_vram_dest(uint16_t dest)
{
	v=dest;
}

/*************************************
 *
 *  PPU Rendering
 *
 *************************************/

/*void ppu2c0x_device::render(bitmap_rgb32& bitmap, int flipx, int flipy, int sx, int sy, const rectangle& cliprect)
{
	copybitmap(bitmap, *m_bitmap, flipx, flipy, sx, sy, cliprect);
}*/

void ppu2c0x_device::render(bitmap_rgb32& bitmap, int flipx, int flipy, int sx, int sy, const rectangle& cliprect)
{
//	rectangle inner = cliprect;

	// Hide 8 pixels on each side
	//inner.min_x = std::max(inner.min_x, 8);
	//inner.max_x = std::min(inner.max_x, 255 - 8);

	// Hide top and bottom 8 lines
//	inner.min_y = std::max(inner.min_y, 1);
	//inner.max_y = std::min(inner.max_y, 239 - 8);

//	if (inner.min_x <= inner.max_x && inner.min_y <= inner.max_y)
//		copybitmap(bitmap, *m_bitmap, flipx, flipy, sx, sy, inner);
	copybitmap(bitmap, *m_bitmap, flipx, flipy, sx, sy, cliprect);
}

uint32_t ppu2c0x_device::screen_update(screen_device& screen, bitmap_rgb32& bitmap, const rectangle& cliprect)
{
	render(bitmap, 0, 0, 0, 0, cliprect);
	return 0;
}

void ppu2c0x_device::read_tile_plane_data(int address, int color) {}
void ppu2c0x_device::shift_tile_plane_data(uint8_t& pix) {}
void ppu2c0x_device::draw_tile_pixel(uint8_t pix, int color, uint32_t back_pen, uint32_t*& dest) {}
void ppu2c0x_device::draw_tile(uint8_t* line_priority, int color_byte, int color_bits, int address, int start_x, uint32_t back_pen, uint32_t*& dest) {}
void ppu2c0x_device::draw_background(uint8_t* line_priority) {}
void ppu2c04_clone_device::draw_background(uint8_t* line_priority) {}
void ppu2c0x_device::draw_back_pen(uint32_t* dest, int back_pen) {}
void ppu2c0x_device::draw_background_pen() {}
void ppu2c0x_device::read_sprite_plane_data(int address) {}
void ppu2c0x_device::make_sprite_pixel_data(uint8_t& pixel_data, int flipx) {}
void ppu2c0x_device::draw_sprite_pixel(int sprite_xpos, int color, int pixel, uint8_t pixel_data, bitmap_rgb32& bitmap) {}
void ppu2c04_clone_device::draw_sprite_pixel(int sprite_xpos, int color, int pixel, uint8_t pixel_data, bitmap_rgb32 &bitmap) {}
void ppu2c0x_device::read_extra_sprite_bits(int sprite_index) {}
bool ppu2c0x_device::is_spritepixel_opaque(int pixel_data, int color) {return false;}
void ppu2c0x_device::draw_sprite_pixel_low(bitmap_rgb32& bitmap, int pixel_data, int pixel, int sprite_xpos, int color, int sprite_index, uint8_t* line_priority){}
void ppu2c0x_device::draw_sprite_pixel_high(bitmap_rgb32& bitmap, int pixel_data, int pixel, int sprite_xpos, int color, int sprite_index, uint8_t* line_priority) {}
int ppu2c0x_device::apply_sprite_pattern_page(int index1, int size) {return 0;}
void ppu2c0x_device::draw_sprites(uint8_t* line_priority) {}
void ppu2c04_clone_device::draw_sprites(uint8_t *line_priority) {}
void ppu2c0x_device::render_scanline() {}
void ppu2c0x_device::scanline_increment_fine_ycounter(){}
void ppu2c0x_device::update_visible_enabled_scanline() {}
void ppu2c0x_device::update_visible_disabled_scanline() {}
void ppu2c0x_device::update_visible_scanline() {}
void ppu2c0x_device::update_scanline() {}
void ppu2c04_clone_device::write(offs_t offset, uint8_t data) {}
void ppu2c04_clone_device::device_start() {}
void ppu2c04_clone_device::init_palette_tables() {}
uint8_t ppu2c04_clone_device::read(offs_t offset) {return 0;}