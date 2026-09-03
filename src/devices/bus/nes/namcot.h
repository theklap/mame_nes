// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
#ifndef MAME_BUS_NES_NAMCOT_H
#define MAME_BUS_NES_NAMCOT_H

#pragma once

#include "nxrom.h"
#include "sound/namco_163.h"

class m6502_device;


// ======================> nes_namcot3433_device

class nes_namcot3433_device : public nes_nrom_device {
public:
	nes_namcot3433_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	void dxrom_write(offs_t offset, uint8_t data);
	virtual void write_h(offs_t offset, uint8_t data) override { dxrom_write(offset, data); }

	virtual void pcb_reset() override;

protected:
	nes_namcot3433_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock);

	virtual void device_start() override;

private:
	uint8_t m_latch;
};


// ======================> nes_namcot3446_device

class nes_namcot3446_device : public nes_nrom_device {
public:
	nes_namcot3446_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual void write_h(offs_t offset, uint8_t data) override;
	virtual void pcb_reset() override;

protected:
	virtual void device_start() override;

private:
	uint8_t m_latch;
};


// ======================> nes_namcot3425_device

class nes_namcot3425_device : public nes_nrom_device {
public:
	nes_namcot3425_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual void write_h(offs_t offset, uint8_t data) override;
	virtual void pcb_reset() override;

protected:
	virtual void device_start() override;

private:
	void update_mirroring();

	uint8_t m_latch;
	uint8_t m_reg[2];
};


// ======================> nes_namcot340_device

class nes_namcot340_device : public nes_nrom_device {
public:
	nes_namcot340_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual void write_h(offs_t offset, uint8_t data) override { n340_hiwrite(offset, data); }
	virtual void pcb_reset() override;

protected:
	nes_namcot340_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock);

	virtual void device_start() override;

	void n340_hiwrite(offs_t offset, uint8_t data);
};


// ======================> nes_namcot175_device

class nes_namcot175_device : public nes_namcot340_device {
public:
	nes_namcot175_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual uint8_t read_m(offs_t offset) override;
	virtual void write_m(offs_t offset, uint8_t data) override;
	virtual void write_h(offs_t offset, uint8_t data) override;

	virtual void pcb_reset() override;

protected:
	virtual void device_start() override;

private:
	uint8_t m_wram_protect;
};


// ======================> nes_namcot163_device

class nes_namcot163_device : public nes_namcot340_device {
public:
	nes_namcot163_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual uint8_t read_l(offs_t offset) override;
	virtual uint8_t read_m(offs_t offset) override;
	virtual void write_l(offs_t offset, uint8_t data) override;
	virtual void write_m(offs_t offset, uint8_t data) override;
	virtual void write_h(offs_t offset, uint8_t data) override;

	virtual uint8_t chr_r(offs_t offset) override;
	virtual void chr_w(offs_t offset, uint8_t data) override;

	virtual void ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address) override;
	virtual void pcb_reset() override;

protected:
	virtual void device_start() override;
	virtual void device_add_mconfig(machine_config &config) override;

private:
	void set_mirror(uint8_t page, uint8_t data);

	TIMER_CALLBACK_MEMBER(irq_timer_tick);

	uint16_t m_irq_count;
	int m_irq_enable;
	int delay_irq;

	emu_timer *irq_timer;
	m6502_device *m_maincpu6502;

	uint8_t m_wram_protect;
	uint8_t m_latch;
	uint8_t m_chr_bank[8];

	required_device<namco_163_sound_device> m_namco163snd;
};


DECLARE_DEVICE_TYPE(NES_NAMCOT3433, nes_namcot3433_device)
DECLARE_DEVICE_TYPE(NES_NAMCOT3446, nes_namcot3446_device)
DECLARE_DEVICE_TYPE(NES_NAMCOT3425, nes_namcot3425_device)
DECLARE_DEVICE_TYPE(NES_NAMCOT340,  nes_namcot340_device)
DECLARE_DEVICE_TYPE(NES_NAMCOT175,  nes_namcot175_device)
DECLARE_DEVICE_TYPE(NES_NAMCOT163,  nes_namcot163_device)

#endif // MAME_BUS_NES_NAMCOT_H