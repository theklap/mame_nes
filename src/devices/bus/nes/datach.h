// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
#ifndef MAME_BUS_NES_DATACH_H
#define MAME_BUS_NES_DATACH_H

#pragma once

#include "bandai.h"
#include "imagedev/cartrom.h"
#include "machine/bcreader.h"
#include "machine/i2cmem.h"


class nes_datach_device;


// ======================> datach_cart_interface

class datach_cart_interface : public device_interface
{
	friend class nes_datach_device;

public:
	virtual ~datach_cart_interface();

	uint8_t read(offs_t offset);
	uint8_t *get_cart_base() { return m_rom; }

	void write_prg_bank(uint8_t bank) { m_bank = bank; }

protected:
	datach_cart_interface(const machine_config &mconfig, device_t &device);

	optional_device<i2cmem_device> m_external_eeprom;

	uint8_t *m_rom = nullptr;
	uint8_t m_bank = 0;
};


// ======================> nes_datach_slot_device

class nes_datach_slot_device : public device_t,
	public device_cartrom_image_interface,
	public device_single_card_slot_interface<datach_cart_interface>
{
	friend class nes_datach_device;

public:
	template <typename T>
	nes_datach_slot_device(machine_config const &mconfig, char const *tag, device_t *owner, uint32_t clock, T &&opts)
		: nes_datach_slot_device(mconfig, tag, owner, clock)
	{
		option_reset();
		opts(*this);
		set_default_option(nullptr);
		set_fixed(false);
	}

	nes_datach_slot_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	virtual ~nes_datach_slot_device();

	virtual std::pair<std::error_condition, std::string> call_load() override;

	virtual bool is_reset_on_load() const noexcept override { return true; }
	virtual const char *image_interface() const noexcept override { return "datach_cart"; }
	virtual const char *file_extensions() const noexcept override { return "nes,bin,prg"; }

	virtual std::string get_default_card_software(get_default_card_software_hook &hook) const override;

	uint8_t read(offs_t offset);

	void write_prg_bank(uint8_t bank)
	{
		if (m_cart) {
			m_cart->write_prg_bank(bank);
		}
	}

protected:
	virtual void device_start() override;

	datach_cart_interface *m_cart = nullptr;
};

DECLARE_DEVICE_TYPE(NES_DATACH_SLOT, nes_datach_slot_device)


// ======================> nes_datach_rom_device

class nes_datach_rom_device : public device_t, public datach_cart_interface
{
public:
	nes_datach_rom_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

protected:
	nes_datach_rom_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock);

	virtual void device_start() override;
	virtual void device_reset() override;
	virtual const tiny_rom_entry *device_rom_region() const override;
};


// ======================> nes_datach_24c01_device

// The identifier retains its historical name, but the physical
// EEPROM used by Battle Rush is an X24C01.
class nes_datach_24c01_device : public nes_datach_rom_device
{
public:
	nes_datach_24c01_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

protected:
	virtual void device_add_mconfig(machine_config &config) override;
};

DECLARE_DEVICE_TYPE(NES_DATACH_ROM,   nes_datach_rom_device)
DECLARE_DEVICE_TYPE(NES_DATACH_24C01, nes_datach_24c01_device)


// ======================> nes_datach_device

class nes_datach_device : public nes_lz93d50_device
{
public:
	nes_datach_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual uint8_t read_m(offs_t offset) override;
	virtual uint8_t read_h(offs_t offset) override;
	virtual void write_h(offs_t offset, uint8_t data) override;

	virtual void pcb_reset() override;

protected:
	virtual void device_start() override;
	virtual void device_add_mconfig(machine_config &config) override;

	TIMER_CALLBACK_MEMBER(serial_tick);

	uint8_t m_datach_latch = 0;
	bool m_i2c_dir = false;

	required_device<i2cmem_device> m_internal_eeprom;
	required_device<barcode_reader_device> m_reader;
	required_device<nes_datach_slot_device> m_subslot;

	emu_timer *m_serial_timer = nullptr;
};

DECLARE_DEVICE_TYPE(NES_DATACH, nes_datach_device)

#endif // MAME_BUS_NES_DATACH_H