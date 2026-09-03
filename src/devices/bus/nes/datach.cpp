// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/**************************************************************************************************

 NES/Famicom cartridge emulation for the Bandai Datach Joint ROM System

 iNES: mapper 157

 The Datach base unit contains a Bandai LZ93D50 ASIC, an internal
 24C02 EEPROM, a barcode reader, 8 KiB of CHR-RAM and a slot for
 a 256 KiB PRG-ROM minicart.

 Most minicarts contain only PRG-ROM. Battle Rush also contains
 an external X24C01 EEPROM.

**************************************************************************************************/

#include "emu.h"
#include "datach.h"

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif

#include "logmacro.h"


//-------------------------------------------------
//  Datach minicart interface
//-------------------------------------------------

datach_cart_interface::datach_cart_interface(const machine_config &mconfig, device_t &device)
	: device_interface(device, "datachcart")
	, m_external_eeprom(*this, "i2cmem")
{
}

datach_cart_interface::~datach_cart_interface()
{
}

uint8_t datach_cart_interface::read(offs_t offset)
{
	if (!m_rom) {
		return 0xff;
	}

	if (offset < 0x4000) {
		return m_rom[(m_bank * 0x4000) + (offset & 0x3fff)];
	}

	return m_rom[(0x0f * 0x4000) + (offset & 0x3fff)];
}


//-------------------------------------------------
//  Datach minicart slot
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_DATACH_SLOT, nes_datach_slot_device, "nes_datach_slot", "NES Datach Cartridge Slot")

nes_datach_slot_device::nes_datach_slot_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: device_t(mconfig, NES_DATACH_SLOT, tag, owner, clock)
	, device_cartrom_image_interface(mconfig, *this)
	, device_single_card_slot_interface<datach_cart_interface>(mconfig, *this)
{
}

nes_datach_slot_device::~nes_datach_slot_device()
{
}

void nes_datach_slot_device::device_start()
{
	m_cart = get_card_device();
}

uint8_t nes_datach_slot_device::read(offs_t offset)
{
	if (m_cart) {
		return m_cart->read(offset);
	}

	return 0xff;
}

std::pair<std::error_condition, std::string> nes_datach_slot_device::call_load()
{
	if (!m_cart) {
		return std::make_pair(
			image_error::INTERNAL,
			"Datach minicart device is missing");
	}

	uint8_t *const rom = m_cart->get_cart_base();

	if (!rom) {
		return std::make_pair(
			image_error::INTERNAL,
			"Datach minicart ROM region is missing");
	}

	if (loaded_through_softlist()) {
		if (get_software_region_length("rom") != 0x40000) {
			return std::make_pair(
				image_error::INVALIDLENGTH,
				"Unsupported cartridge size (must be 256K)");
		}

		const uint8_t *const source = get_software_region("rom");

		if (!source) {
			return std::make_pair(
				image_error::INTERNAL,
				"Datach software-list ROM region is missing");
		}

		memcpy(rom, source, 0x40000);

		return std::make_pair(std::error_condition(), std::string());
	}

	const uint64_t file_size = length();

	if (file_size != 0x40000 && file_size != 0x40010) {
		return std::make_pair(
			image_error::INVALIDLENGTH,
			"Unsupported cartridge size (must be 256K raw or 256K iNES)");
	}

	std::vector<uint8_t> temp(file_size);

	fread(temp.data(), file_size);

	const size_t shift = file_size - 0x40000;

	if (file_size == 0x40010) {
		uint8_t mapper = (temp[6] & 0xf0) >> 4;

		mapper |= temp[7] & 0xf0;

		if (mapper != 157 && mapper != 16) {
			return std::make_pair(
				image_error::INVALIDIMAGE,
				util::string_format(
					"Unsupported iNES mapper %u (must be 16 or 157)",
					mapper));
		}
	}

	memcpy(rom, temp.data() + shift, 0x40000);

	return std::make_pair(std::error_condition(), std::string());
}

std::string nes_datach_slot_device::get_default_card_software(get_default_card_software_hook &hook) const
{
	return software_get_default_slot("datach_rom");
}


//-------------------------------------------------
//  Datach minicarts
//
//  Two known minicart PCB configurations exist:
//
//  * PRG-ROM only, used by most games.
//  * PRG-ROM with an X24C01 EEPROM, used by
//    Battle Rush.
//-------------------------------------------------

ROM_START(datach_rom)
	ROM_REGION(0x40000, "datachrom", ROMREGION_ERASEFF)
ROM_END

DEFINE_DEVICE_TYPE(NES_DATACH_ROM,   nes_datach_rom_device,   "nes_datach_rom", "NES Datach ROM")
DEFINE_DEVICE_TYPE(NES_DATACH_24C01, nes_datach_24c01_device, "nes_datach_ep1", "NES Datach + X24C01 PCB")

nes_datach_rom_device::nes_datach_rom_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock)
	: device_t(mconfig, type, tag, owner, clock)
	, datach_cart_interface(mconfig, *this)
{
}

nes_datach_rom_device::nes_datach_rom_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_datach_rom_device(mconfig, NES_DATACH_ROM, tag, owner, clock)
{
}

nes_datach_24c01_device::nes_datach_24c01_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_datach_rom_device(mconfig, NES_DATACH_24C01, tag, owner, clock)
{
}

void nes_datach_rom_device::device_start()
{
	m_rom = reinterpret_cast<uint8_t *>(memregion("datachrom")->base());

	save_item(NAME(m_bank));
}

void nes_datach_rom_device::device_reset()
{
	m_bank = 0;
}

const tiny_rom_entry *nes_datach_rom_device::device_rom_region() const
{
	return ROM_NAME(datach_rom);
}

void nes_datach_24c01_device::device_add_mconfig(machine_config &config)
{
	I2C_X24C01(config, m_external_eeprom);
}


//-------------------------------------------------
//  Datach base unit
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_DATACH, nes_datach_device, "nes_datach", "NES Cart Bandai Datach PCB")

nes_datach_device::nes_datach_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_lz93d50_device(mconfig, NES_DATACH, tag, owner, clock)
	, m_internal_eeprom(*this, "i2cmem")
	, m_reader(*this, "datach")
	, m_subslot(*this, "datach_slot")
{
}

void nes_datach_device::device_start()
{
	nes_fcg_device::device_start();

	m_serial_timer = timer_alloc(FUNC(nes_datach_device::serial_tick), this);
	m_serial_timer->adjust(attotime::zero, 0, clocks_to_attotime(1000));

	save_item(NAME(m_datach_latch));
	save_item(NAME(m_i2c_dir));
}

void nes_datach_device::pcb_reset()
{
	nes_fcg_device::pcb_reset();

	m_datach_latch = 0;
	m_i2c_dir = false;
}


/*-------------------------------------------------

 Bandai Datach Joint ROM System

 The base unit contains an LZ93D50 ASIC, an internal
 24C02 EEPROM, a barcode reader, 8 KiB of CHR-RAM
 and a slot for a 256 KiB PRG-ROM minicart.

 Most minicarts contain only PRG-ROM. Battle Rush
 also contains an external X24C01 EEPROM. Its SDA
 line is shared with the internal EEPROM, while
 each EEPROM has a separate clock line.

 Reads from $6000-$7FFF return barcode data on bit 3
 and EEPROM data on bit 4. The other data lines retain
 CPU open bus.

 iNES: mapper 157

 In MAME: Supported.

 -------------------------------------------------*/

uint8_t nes_datach_device::read_m(offs_t offset)
{
	LOG("Datach read_m, offset: %04x\n", offset);

	uint8_t result = get_open_bus() & 0xe7;

	if (m_datach_latch) {
		result |= 0x08;
	}

	if (m_i2c_dir) {
		int eeprom_sda = m_internal_eeprom->read_sda();

		if (m_subslot->m_cart && m_subslot->m_cart->m_external_eeprom) {
			eeprom_sda &= m_subslot->m_cart->m_external_eeprom->read_sda();
		}

		result |= (eeprom_sda & 0x01) << 4;
	}

	return result;
}

uint8_t nes_datach_device::read_h(offs_t offset)
{
	LOG("Datach read_h, offset: %04x\n", offset);

	if (m_subslot->m_cart) {
		return m_subslot->m_cart->read(offset);
	}

	return get_open_bus();
}

void nes_datach_device::write_h(offs_t offset, uint8_t data)
{
	LOG("Datach write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x0f) {
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x03:
			// $8000-$8003: external X24C01 clock.
			if (m_subslot->m_cart && m_subslot->m_cart->m_external_eeprom) {
				m_subslot->m_cart->m_external_eeprom->write_scl(BIT(data, 3));
			}
			break;

		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
			// PA12 and PA13 are grounded in the Datach base unit.
			break;

		case 0x08:
			m_subslot->write_prg_bank(data & 0x0f);
			break;

		case 0x0d:
		{
			m_i2c_dir = BIT(data, 7);

			const int sda = m_i2c_dir ? 1 : BIT(data, 6);

			m_internal_eeprom->write_sda(sda);

			if (m_subslot->m_cart && m_subslot->m_cart->m_external_eeprom) {
				m_subslot->m_cart->m_external_eeprom->write_sda(sda);
			}

			m_internal_eeprom->write_scl(BIT(data, 5));
			break;
		}

		default:
			fcg_write(offset & 0x0f, data);
			break;
	}
}


//-------------------------------------------------
//  Barcode reader, minicart slot and EEPROMs
//-------------------------------------------------

static void datach_cart(device_slot_interface &device)
{
	device.option_add_internal("datach_rom", NES_DATACH_ROM);
	device.option_add_internal("datach_ep1", NES_DATACH_24C01);
}

void nes_datach_device::device_add_mconfig(machine_config &config)
{
	BARCODE_READER(config, m_reader, 0);
	NES_DATACH_SLOT(config, m_subslot, 0, datach_cart);
	I2C_24C02(config, m_internal_eeprom);
}


//-------------------------------------------------
//  Barcode serial input
//-------------------------------------------------

TIMER_CALLBACK_MEMBER(nes_datach_device::serial_tick)
{
	m_datach_latch = m_reader->read_pixel() << 3;
}