// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
#ifndef MAME_BUS_NES_MMC5_H
#define MAME_BUS_NES_MMC5_H

#pragma once

#include "nxrom.h"

#include "sound/mmc5snd.h"
#include "sound/nes_apu.h"
#include "video/ppu2c0x.h"  // this has to be included so that IRQ functions can access ppu2c0x_device::BOTTOM_VISIBLE_SCANLINE


// ======================> nes_exrom_device
class m6502_device;
class nes_exrom_device : public nes_nrom_device
{
public:
	// construction/destruction
	nes_exrom_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	~nes_exrom_device();

	virtual uint8_t read_l(offs_t offset) override;
	virtual uint8_t read_m(offs_t offset) override;
	virtual uint8_t read_h(offs_t offset) override;
	virtual void write_l(offs_t offset, uint8_t data) override;
	virtual void write_m(offs_t offset, uint8_t data) override;
	virtual void write_h(offs_t offset, uint8_t data) override;

	virtual uint8_t chr_r(offs_t offset) override;
	virtual uint8_t nt_r(offs_t offset) override;
	virtual void nt_w(offs_t offset, uint8_t data) override;

	//virtual void hblank_irq(int scanline, bool vblank, bool blanked) override;
	virtual void pcb_reset() override;
	
	virtual void ppu_to_mapper(int scanline, unsigned dot) override;
	void mmc5_cpu_cycle();
	void mmc5_end_frame();
	void mmc5_clock_ppu_read(offs_t ppu_addr_bus, bool is_nt_fetch, bool is_at_fetch, bool is_bg_pattern, bool is_spr_pattern);
	void mmc5_reset_scanline_irq_state();
	void mmc5_recompute_ppu_latches();
	void mmc5_real_ppuctrl_write(uint8_t data);
	void mmc5_real_ppumask_write(uint8_t data);
	bool mmc5_substitution_active();
	bool mmc5_split_allowed();
	bool mmc5_exattr_allowed();
	bool mmc5_use_extended_sprite_banking();

private:
	m6502_device* m_maincpu6502 = nullptr;
	
protected:
	// device-level overrides
	virtual void device_add_mconfig(machine_config &config) override;
	virtual void device_start() override;

	void set_mirror(int page, int src);
	void update_prg();

	inline uint8_t base_chr_r(int bank, uint32_t offset);
	inline uint8_t split_chr_r(uint32_t offset);
	inline uint8_t bg_ex1_chr_r(uint32_t offset);
	inline bool in_split();
	void mmc5_clock_irq_detector(offs_t ppu_addr_bus);
	void mmc5_set_in_frame();
	void mmc5_cpu_clock();
	
	uint16_t 	m_irq_count;          		// $5203 IRQ scanline compare value
	int			m_irq_enable;         		// $5204 bit 7 IRQ enable flag

	int      	m_mult1, m_mult2;     		// $5205/$5206 multiplier operands; read result at $5205/$5206

	uint16_t 	m_vrom_bank[12];      		// $5120-$512B CHR bank regs; 10-bit value using $5130 high bits

	int 		m_floodtile;               	// $5106 fill-mode nametable tile byte
	int 		m_floodattr;               	// $5107 fill-mode attribute byte expanded to 00/55/AA/FF

	int 		m_prg_mode;                	// $5100 PRG banking mode: 0=32K, 1=16K/16K, 2=16K/8K/8K, 3=8K
	int 		m_chr_mode;                	// $5101 CHR banking mode: 0=8K, 1=4K, 2=2K, 3=1K
	int 		m_wram_protect_1;          	// $5102 PRG-RAM write protect part 1; must be 0x02 to allow writes
	int 		m_wram_protect_2;          	// $5103 PRG-RAM write protect part 2; must be 0x01 to allow writes
	int 		m_exram_control;           	// $5104 ExRAM mode: 0=extra nametable, 1=extended attr, 2=CPU RAM, 3=CPU read-only
	int 		m_wram_base;               	// $5113 PRG-RAM bank mapped at CPU $6000-$7FFF

	uint8_t 	m_last_chr;            		// Last CHR register group written: LAST_CHR_REG_A or LAST_CHR_REG_B
	uint8_t 	m_ex1_chr;             		// True when an extended-attribute BG tile CHR bank has been latched
	uint8_t 	m_prg_regs[4];         		// $5114-$5117 PRG bank register values, one per 8K CPU slot
	uint8_t 	m_prg_ram_mapped[4];   		// True when corresponding $5114-$5117 slot maps PRG-RAM instead of ROM

	uint8_t 	m_ex1_bank;            		// Extended-attribute latched 4K CHR bank from ExRAM bits 0-5 plus $5130
	uint8_t 	m_ex1_attrib;          		// Extended-attribute latched attribute byte expanded to 00/55/AA/FF

	uint8_t 	m_high_chr;            		// $5130 upper CHR bank bits used by following CHR bank writes / ExAttr

	uint8_t 	m_split_scr;           		// $5200 bit 7: vertical split enable
	uint8_t 	m_split_rev;           		// $5200 bit 6: split side; 0=left side, 1=right side
	uint8_t 	m_split_ctrl;          		// $5200 bits 0-4: split start/stop tile column
	uint8_t 	m_split_yst;           		// $5201 vertical split Y scroll
	uint8_t 	m_split_bank;          		// $5202 vertical split 4K CHR bank

	int      	m_vcount;             		// Current PPU scanline as reported to mapper
	unsigned 	m_dot;                		// Current PPU dot/cycle as reported to mapper

	bool 		irq_pending;              	// MMC5 IRQ pending flag, reported as $5204 bit 7
	int  		scanline_cnt;             	// MMC5 internal scanline counter
	bool 		in_frame;                 	// MMC5 in-frame flag, reported as $5204 bit 6

	uint8_t 	m_split_fetch_tile;    		// Current split-screen tile column selected during NT fetch
	uint8_t 	m_split_fetch_row;     		// Current split-screen fine Y row selected during NT fetch
	bool    	m_split_active_fetch;  		// True when current BG tile fetch is using vertical split data

	int  		m_ppu_idle_dots;          	// Counts CPU cycles/dots without PPU reads for MMC5 in-frame end detection
	bool 		ppu_is_reading;           	// True when PPU performed a mapper-visible read this CPU cycle/window

	// MMC5's own view of fully-decoded PPU register writes.
	uint8_t 	m_mmc5_ppuctrl;        		// Last real write to PPUCTRL $2000
	uint8_t 	m_mmc5_ppumask;        		// Last real write to PPUMASK $2001
	bool    	m_mmc5_subs_enabled;   		// True when MMC5 sees BG or sprite rendering enabled via $2001 bits 3/4
	bool    	m_mmc5_sprite_8x16;    		// True when MMC5 sees PPUCTRL $2000 bit 5 set for 8x16 sprites

	uint8_t  	mmc5_match_count;     		// Consecutive identical $2xxx PPU read match count for scanline detector
	uint16_t 	mmc5_last_ppu_addr;   		// Last PPU address seen by MMC5 scanline detector
	bool     	m_rendering_active;   		// True when MMC5 considers rendering active for ExRAM write gating
	int      	delay_irq;            		// Small deferred IRQ assertion delay used after scanline compare hit

	//const char* m_last_phase;      			// Last PPU fetch phase label: NT/AT/PTL/PTH/SPR_PTL/SPR_PTH/etc.; do not save-state
	// Last decoded PPU fetch phase. This is transient fetch context; do not save-state.
	bool m_phase_nt;
	bool m_phase_at;
	bool m_phase_bg_pattern;
	bool m_phase_spr_pattern;

	// MMC5 contains 1K of internal ExRAM.
	uint8_t 	m_exram[0x400];        		// $5C00-$5FFF ExRAM; also used for ExAttr, split, or extra nametable

	uint8_t 	m_ram_hi_banks[4];     		// Resolved PRG-RAM bank number for each high CPU 8K slot $8000-$FFFF

	required_device<ppu2c0x_device> m_ppu;
	required_device<mmc5snd_device> m_sound;
	
};


// device type definition
DECLARE_DEVICE_TYPE(NES_EXROM, nes_exrom_device)

#endif // MAME_BUS_NES_MMC5_H