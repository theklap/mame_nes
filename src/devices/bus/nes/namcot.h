// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
#ifndef MAME_BUS_NES_NAMCOT_H
#define MAME_BUS_NES_NAMCOT_H

#pragma once

#include "nxrom.h"
#include "sound/namco_163.h"


// ======================> nes_namcot3433_device
class m6502_device;
class nes_namcot3433_device : public nes_nrom_device
{
public:
	// construction/destruction
	nes_namcot3433_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	void dxrom_write(offs_t offset, uint8_t data);
	virtual void write_h(offs_t offset, uint8_t data) override { dxrom_write(offset, data); }

	virtual void pcb_reset() override;

protected:
	nes_namcot3433_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock);

	// device-level overrides
	virtual void device_start() override;

private:
	uint8_t m_latch;
};


// ======================> nes_namcot3446_device

class nes_namcot3446_device : public nes_nrom_device
{
public:
	// construction/destruction
	nes_namcot3446_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual void write_h(offs_t offset, uint8_t data) override;

	virtual void pcb_reset() override;

protected:
	// device-level overrides
	virtual void device_start() override;

private:
	uint8_t m_latch;
};


// ======================> nes_namcot3425_device

class nes_namcot3425_device : public nes_nrom_device
{
public:
	// construction/destruction
	nes_namcot3425_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual void write_h(offs_t offset, uint8_t data) override;

	virtual void pcb_reset() override;

protected:
	// device-level overrides
	virtual void device_start() override;

private:
	uint8_t m_latch;
	uint8_t m_reg[4];
};


// ======================> nes_namcot340_device

class nes_namcot340_device : public nes_nrom_device
{
public:
	// construction/destruction
	nes_namcot340_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	uint8_t n340_loread(offs_t offset);
	void n340_lowrite(offs_t offset, uint8_t data);
	void n340_hiwrite(offs_t offset, uint8_t data);

	virtual uint8_t read_l(offs_t offset) override { return n340_loread(offset); }
	virtual void write_l(offs_t offset, uint8_t data) override { n340_lowrite(offset, data); }
	virtual void write_h(offs_t offset, uint8_t data) override { n340_hiwrite(offset, data); }

	virtual void ppu_to_mapper(int scanline, unsigned dot, int ppu_tick) override;
	virtual void pcb_reset() override;

protected:
	nes_namcot340_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock);

	// device-level overrides
	virtual void device_start() override;

	TIMER_CALLBACK_MEMBER(irq_timer_tick);

	uint16_t m_irq_count;
	int m_irq_enable;

	// Mapper-side IRQ publication delay.
	int delay_irq;

	emu_timer *irq_timer;

	// Cached CPU pointer for delayed mapper IRQ queue/cancel.
	m6502_device *m_maincpu6502;

	// Namcot-163 chip contains 8K of internal ram, possibly battery backed up (not emulated yet)
	// was it also present in 175 & 340 chips?
	uint8_t m_n163_ram[0x2000];
};


// ======================> nes_namcot175_device

class nes_namcot175_device : public nes_namcot340_device
{
public:
	// construction/destruction
	nes_namcot175_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// device-level overrides
	virtual void device_start() override;
	virtual uint8_t read_m(offs_t offset) override;
	virtual void write_m(offs_t offset, uint8_t data) override;
	virtual void write_h(offs_t offset, uint8_t data) override;

	virtual void pcb_reset() override;

private:
	uint8_t m_wram_protect;
};


// ======================> nes_namcot163_device

class nes_namcot163_device : public nes_namcot340_device
{
public:
	// construction/destruction
	nes_namcot163_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual uint8_t read_l(offs_t offset) override;
	virtual uint8_t read_m(offs_t offset) override;
	virtual void write_l(offs_t offset, uint8_t data) override;
	virtual void write_m(offs_t offset, uint8_t data) override;
	virtual void write_h(offs_t offset, uint8_t data) override;

	// We overwrite these to allow CIRAM to be mapped into the pattern tables.
	virtual uint8_t chr_r(offs_t offset) override;
	virtual void chr_w(offs_t offset, uint8_t data) override;

	virtual void pcb_reset() override;

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_add_mconfig(machine_config &config) override;

private:
	void set_mirror(uint8_t page, uint8_t data);

	uint8_t m_wram_protect;
	uint8_t m_latch;
	uint8_t m_chr_bank[8];

	required_device<namco_163_sound_device> m_namco163snd;
};


// device type definition
DECLARE_DEVICE_TYPE(NES_NAMCOT3433, nes_namcot3433_device)
DECLARE_DEVICE_TYPE(NES_NAMCOT3446, nes_namcot3446_device)
DECLARE_DEVICE_TYPE(NES_NAMCOT3425, nes_namcot3425_device)
DECLARE_DEVICE_TYPE(NES_NAMCOT340,  nes_namcot340_device)
DECLARE_DEVICE_TYPE(NES_NAMCOT175,  nes_namcot175_device)
DECLARE_DEVICE_TYPE(NES_NAMCOT163,  nes_namcot163_device)

#endif // MAME_BUS_NES_NAMCOT_H
