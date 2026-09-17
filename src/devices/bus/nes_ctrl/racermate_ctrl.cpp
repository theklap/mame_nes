// license:BSD-3-Clause
// copyright-holders:Matt Sutton
/**********************************************************************

    RacerMate CompuTrainer bicycle training system

    The NES connector box occupies both controller ports.  OUT0 from
    $4016 is sent to both trainers.  Trainer 1 returns RX on $4016 D0
    and trainer 2 returns RX on $4017 D0.

    Each trainer exchanges 48 serial bits over two video frames.  The
    useful trainer-to-NES data is carried by the 24 even-numbered bits.
    The preceding odd-numbered bit is the inverse of each data bit.

**********************************************************************/

#include "emu.h"
#include "racermate_ctrl.h"


DEFINE_DEVICE_TYPE(NES_RACERMATE_P1, nes_racermate_p1_device, "nes_racermate_p1", "RacerMate CompuTrainer Player 1")
DEFINE_DEVICE_TYPE(NES_RACERMATE_P2, nes_racermate_p2_device, "nes_racermate_p2", "RacerMate CompuTrainer Player 2")


static INPUT_PORTS_START( nes_racermate_p1 )
	PORT_START("BUTTONS")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_BUTTON1)
		PORT_NAME("P1 RacerMate RESET")
		PORT_PLAYER(1)
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_START1)
		PORT_NAME("P1 RacerMate F1 / Start")
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_SELECT)
		PORT_NAME("P1 RacerMate F3 / Set")
		PORT_PLAYER(1)
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP)
		PORT_NAME("P1 RacerMate Plus / Up")
		PORT_PLAYER(1)
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_BUTTON2)
		PORT_NAME("P1 RacerMate F2 / Display")
		PORT_PLAYER(1)
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN)
		PORT_NAME("P1 RacerMate Minus / Down")
		PORT_PLAYER(1)

	PORT_START("SPEED")
	PORT_BIT(0x0fff, 0x0000, IPT_AD_STICK_Y)
		PORT_NAME("P1 RacerMate Speed")
		PORT_SENSITIVITY(100)
		PORT_KEYDELTA(32)
		PORT_MINMAX(0x0000, 0x0fff)
		PORT_PLAYER(1)

	PORT_START("WATTS")
	PORT_BIT(0x0fff, 0x0000, IPT_AD_STICK_Z)
		PORT_NAME("P1 RacerMate Watts")
		PORT_SENSITIVITY(100)
		PORT_KEYDELTA(32)
		PORT_MINMAX(0x0000, 0x0fff)
		PORT_PLAYER(1)

	PORT_START("PULSE")
	PORT_BIT(0x00ff, 0x0000, IPT_AD_STICK_X)
		PORT_NAME("P1 RacerMate Pulse")
		PORT_SENSITIVITY(100)
		PORT_KEYDELTA(2)
		PORT_MINMAX(0x0000, 0x00ff)
		PORT_PLAYER(1)
INPUT_PORTS_END


static INPUT_PORTS_START( nes_racermate_p2 )
	PORT_START("BUTTONS")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_BUTTON1)
		PORT_NAME("P2 RacerMate RESET")
		PORT_PLAYER(2)
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_START2)
		PORT_NAME("P2 RacerMate F1 / Start")
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_SELECT)
		PORT_NAME("P2 RacerMate F3 / Set")
		PORT_PLAYER(2)
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP)
		PORT_NAME("P2 RacerMate Plus / Up")
		PORT_PLAYER(2)
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_BUTTON2)
		PORT_NAME("P2 RacerMate F2 / Display")
		PORT_PLAYER(2)
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN)
		PORT_NAME("P2 RacerMate Minus / Down")
		PORT_PLAYER(2)

	PORT_START("SPEED")
	PORT_BIT(0x0fff, 0x0000, IPT_AD_STICK_Y)
		PORT_NAME("P2 RacerMate Speed")
		PORT_SENSITIVITY(100)
		PORT_KEYDELTA(32)
		PORT_MINMAX(0x0000, 0x0fff)
		PORT_PLAYER(2)

	PORT_START("WATTS")
	PORT_BIT(0x0fff, 0x0000, IPT_AD_STICK_Z)
		PORT_NAME("P2 RacerMate Watts")
		PORT_SENSITIVITY(100)
		PORT_KEYDELTA(32)
		PORT_MINMAX(0x0000, 0x0fff)
		PORT_PLAYER(2)

	PORT_START("PULSE")
	PORT_BIT(0x00ff, 0x0000, IPT_AD_STICK_X)
		PORT_NAME("P2 RacerMate Pulse")
		PORT_SENSITIVITY(100)
		PORT_KEYDELTA(2)
		PORT_MINMAX(0x0000, 0x00ff)
		PORT_PLAYER(2)
INPUT_PORTS_END


ioport_constructor nes_racermate_p1_device::device_input_ports() const {
	return INPUT_PORTS_NAME(nes_racermate_p1);
}


ioport_constructor nes_racermate_p2_device::device_input_ports() const {
	return INPUT_PORTS_NAME(nes_racermate_p2);
}


nes_racermate_ctrl_device::nes_racermate_ctrl_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock)
	: device_t(mconfig, type, tag, owner, clock)
	, device_nes_control_port_interface(mconfig, *this)
	, m_buttons(*this, "BUTTONS")
	, m_speed(*this, "SPEED")
	, m_watts(*this, "WATTS")
	, m_pulse(*this, "PULSE")
	, m_rx_data(0)
	, m_rx_bit(0)
	, m_index(1)
	, m_tx(1)
{
}


nes_racermate_p1_device::nes_racermate_p1_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_racermate_ctrl_device(mconfig, NES_RACERMATE_P1, tag, owner, clock)
{
}


nes_racermate_p2_device::nes_racermate_p2_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_racermate_ctrl_device(mconfig, NES_RACERMATE_P2, tag, owner, clock)
{
}


void nes_racermate_ctrl_device::device_start() {
	save_item(NAME(m_rx_data));
	save_item(NAME(m_rx_bit));
	save_item(NAME(m_index));
	save_item(NAME(m_tx));
}


void nes_racermate_ctrl_device::device_reset() {
	m_rx_data = 0;
	m_rx_bit = 0;
	m_index = 1;
	m_tx = 1;

	prepare_packet();
}


void nes_racermate_ctrl_device::prepare_packet() {
	u16 data;

	switch (m_index) {
		case 1:
			data = m_speed->read() & 0x0fff;
			break;

		case 2:
			data = m_watts->read() & 0x0fff;
			break;

		case 3:
			data = m_pulse->read() & 0x00ff;
			break;

		default:
			data = 0;
			break;
	}

	m_rx_data = m_buttons->read() & 0x3f;
	m_rx_data |= u32(data & 0x00ff) << 8;
	m_rx_data |= u32((m_index << 4) | ((data >> 8) & 0x0f)) << 16;
}


u8 nes_racermate_ctrl_device::read_bit0() {
	const u8 data_bit = BIT(m_rx_data, m_rx_bit >> 1);
	const u8 result = BIT(m_rx_bit, 0) ? data_bit : !data_bit;

	m_rx_bit++;

	if (m_rx_bit >= 48) {
		m_rx_bit = 0;
		m_index++;

		if (m_index > 3) {
			m_index = 1;
		}

		prepare_packet();
	}

	return result;
}


void nes_racermate_ctrl_device::write(u8 data) {
	m_tx = BIT(data, 0);
}
