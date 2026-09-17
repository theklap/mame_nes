// license:BSD-3-Clause
// copyright-holders:Matt Sutton
/**********************************************************************

    RacerMate CompuTrainer bicycle training system

**********************************************************************/

#ifndef MAME_BUS_NES_CTRL_RACERMATE_CTRL_H
#define MAME_BUS_NES_CTRL_RACERMATE_CTRL_H

#pragma once

#include "ctrl.h"


// ======================> nes_racermate_ctrl_device

class nes_racermate_ctrl_device : public device_t,
								   public device_nes_control_port_interface
{
public:
	virtual u8 read_bit0() override;
	virtual void write(u8 data) override;

protected:
	nes_racermate_ctrl_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock);

	virtual void device_start() override ATTR_COLD;
	virtual void device_reset() override ATTR_COLD;

private:
	void prepare_packet();

	required_ioport m_buttons;
	required_ioport m_speed;
	required_ioport m_watts;
	required_ioport m_pulse;

	u32 m_rx_data;
	u8 m_rx_bit;
	u8 m_index;
	u8 m_tx;
};


// ======================> nes_racermate_p1_device

class nes_racermate_p1_device : public nes_racermate_ctrl_device
{
public:
	nes_racermate_p1_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);

protected:
	virtual ioport_constructor device_input_ports() const override ATTR_COLD;
};


// ======================> nes_racermate_p2_device

class nes_racermate_p2_device : public nes_racermate_ctrl_device
{
public:
	nes_racermate_p2_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);

protected:
	virtual ioport_constructor device_input_ports() const override ATTR_COLD;
};


DECLARE_DEVICE_TYPE(NES_RACERMATE_P1, nes_racermate_p1_device)
DECLARE_DEVICE_TYPE(NES_RACERMATE_P2, nes_racermate_p2_device)

#endif // MAME_BUS_NES_CTRL_RACERMATE_CTRL_H
