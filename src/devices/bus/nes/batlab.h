// license:BSD-3-Clause
// copyright-holders:kmg
#ifndef MAME_BUS_NES_BATLAB_H
#define MAME_BUS_NES_BATLAB_H

#pragma once

#include "mmc3.h"

class m6502_device;

// ======================> nes_batmap_000_device

class nes_batmap_000_device : public nes_txrom_device
{
public:
	nes_batmap_000_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);

	virtual void write_h(offs_t offset, u8 data) override;
	virtual void pcb_reset() override;
};

// ======================> nes_batmap_srrx_device

class nes_batmap_srrx_device : public nes_nrom_device
{
public:
	nes_batmap_srrx_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);

	virtual u8 read_l(offs_t offset) override;
	virtual u8 read_m(offs_t offset) override;
	virtual u8 read_h(offs_t offset) override;
	virtual void write_h(offs_t offset, u8 data) override;

	virtual void ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) override;
	virtual void pcb_reset() override;

	virtual void ppu_bus_address(uint16_t ppu_address, uint64_t ppu_cycle, int ppu_tick, bool odd_frame) override;

protected:
	virtual void device_start() override;

private:
	u8 read_dpcm();
	void irq_clock();

	u8 m_reg;
	u32 m_dpcm_addr;
	u8 m_dpcm_ctrl;

	u16 m_irq_count;
	u16 m_irq_count_latch;
	int m_irq_enable;
	bool m_irq_reload;
	int delay_irq;

	uint64_t m_last_a12_low_cycle;
	uint16_t m_prev_ppu_addr;
	bool m_a12_low_seen;

	m6502_device *m_maincpu6502;
};

DECLARE_DEVICE_TYPE(NES_BATMAP_000,  nes_batmap_000_device)
DECLARE_DEVICE_TYPE(NES_BATMAP_SRRX, nes_batmap_srrx_device)

#endif // MAME_BUS_NES_BATLAB_H