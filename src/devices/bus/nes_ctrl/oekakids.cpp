// license:BSD-3-Clause
// copyright-holders:Matt Sutton
/**********************************************************************

    Bandai Oeka Kids drawing tablet

    The tablet connects to the Famicom expansion port.

    $4016 D0:
        0 = latch tablet data
        1 = read mode

    $4016 D1:
        advance control

    $4017 D2:
        MCU acknowledgement

    $4017 D3:
        inverted serial data, MSB first

    Report:
        XXXXXXXX YYYYYYYY T C

**********************************************************************/

#include "emu.h"
#include "oekakids.h"


DEFINE_DEVICE_TYPE(NES_OEKAKIDS_TABLET, nes_oekakids_tablet_device, "nes_oekakids_tablet", "Bandai Oeka Kids Drawing Tablet")


static INPUT_PORTS_START( nes_oekakids_tablet )
	PORT_START("TABLET_X")
	PORT_BIT(0xff, 0x78, IPT_LIGHTGUN_X)
		PORT_NAME("Tablet X")
		PORT_CROSSHAIR(X, 1.0, 0.0, 0)
		PORT_SENSITIVITY(50)
		PORT_KEYDELTA(10)
		PORT_MINMAX(0, 255)
		PORT_PLAYER(1)
		PORT_CODE(MOUSECODE_X)

	PORT_START("TABLET_Y")
	PORT_BIT(0xff, 0x80, IPT_LIGHTGUN_Y)
		PORT_NAME("Tablet Y")
		PORT_CROSSHAIR(Y, 1.0, 0.0, 0)
		PORT_SENSITIVITY(50)
		PORT_KEYDELTA(10)
		PORT_MINMAX(16, 255)
		PORT_PLAYER(1)
		PORT_CODE(MOUSECODE_Y)

	PORT_START("BUTTONS")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_BUTTON1)
		PORT_NAME("Stylus Touch")
		PORT_CODE(MOUSECODE_BUTTON1)

	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_BUTTON2)
		PORT_NAME("Stylus Click")
		PORT_CODE(MOUSECODE_BUTTON2)
INPUT_PORTS_END


ioport_constructor nes_oekakids_tablet_device::device_input_ports() const {
	return INPUT_PORTS_NAME(nes_oekakids_tablet);
}


nes_oekakids_tablet_device::nes_oekakids_tablet_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: device_t(mconfig, NES_OEKAKIDS_TABLET, tag, owner, clock)
	, device_nes_control_port_interface(mconfig, *this)
	, m_x(*this, "TABLET_X")
	, m_y(*this, "TABLET_Y")
	, m_buttons(*this, "BUTTONS")
	, m_latch(0)
	, m_strobe(0)
	, m_advance(0)
	, m_bit(0)
	, m_tablet_x(0x78)
	, m_tablet_y(0x80)
	, m_ack(1)
	, m_ack_delay(0)
{
}


void nes_oekakids_tablet_device::device_start() {
	save_item(NAME(m_latch));
	save_item(NAME(m_strobe));
	save_item(NAME(m_advance));
	save_item(NAME(m_bit));
	save_item(NAME(m_tablet_x));
	save_item(NAME(m_tablet_y));
	save_item(NAME(m_ack));
	save_item(NAME(m_ack_delay));
}


void nes_oekakids_tablet_device::device_reset() {
	m_latch = 0;
	m_strobe = 0;
	m_advance = 0;
	m_bit = 0;
	m_tablet_x = 0x78;
	m_tablet_y = 0x80;
	m_ack = 1;
	m_ack_delay = 0;
}


u8 nes_oekakids_tablet_device::read_exp(offs_t offset) {
	if (offset == 0) {
		return 0;
	}

	if (m_ack_delay) {
		m_ack_delay--;

		if (!m_ack_delay) {
			if (!m_strobe) {
				m_ack = 0;
			}
			else if (m_advance) {
				m_ack = 0;
			}
			else {
				m_ack = 1;
			}
		}
	}

	if (!m_strobe) {
		return 0x00;
	}

	return (m_ack ? 0x04 : 0x00) | (BIT(m_latch, 17 - m_bit) ? 0x00 : 0x08);
}


void nes_oekakids_tablet_device::write(u8 data) {
	if (!BIT(data, 0)) {
		if (m_strobe) {
			/*
			//Raw Read - Crosshair is way high vs where pen draws, bad all over screen/tablet
			*/
			//m_tablet_x = m_x->read();
			//m_tablet_y = m_y->read();
			/*
			//Static Coordinate Block - drifts, does not stay accurate where pen draws,
			//good for middle of screen only
			*/
			//m_tablet_x = std::clamp<int>(m_x->read() - 1, 0, 255);
			//m_tablet_y = std::clamp<int>(m_y->read() - 21, 0, 255);
			/*
			//Reverse the Scale Math and keep crosshair and where pen draws locked
			*/
			m_tablet_x = std::clamp<int>(((int(m_x->read()) + 7) * 16 + 8) / 17, 0, 255);
			m_tablet_y = std::clamp<int>(((int(m_y->read()) - 34) * 16 + 7) / 15, 0, 255);

			m_latch = (u32(m_tablet_x) << 10) | (u32(m_tablet_y) << 2);

			if (BIT(m_buttons->read(), 0)) {
				m_latch |= 0x02;
			}

			if (BIT(m_buttons->read(), 1)) {
				m_latch |= 0x03;
			}

			m_bit = 0;
		}

		m_ack = 0;
		m_ack_delay = 0;
	}

	if (m_advance) {
		if (!BIT(data, 1)) {
			if (BIT(data, 0)) {
				if (m_bit < 17) {
					m_bit++;
				}
			}
		}
	}

	if (BIT(data, 0)) {
		if (!m_strobe) {
			m_ack = 0;
			m_ack_delay = 12;
		}
		else if (BIT(data, 1) != m_advance) {
			if (BIT(data, 1)) {
				m_ack = 1;
				m_ack_delay = 12;
			}
			else {
				m_ack = 0;
				m_ack_delay = 21;
			}
		}
	}

	m_strobe = BIT(data, 0);
	m_advance = BIT(data, 1);
}
