// license:BSD-3-Clause
// copyright-holders:Matt Sutton
/**********************************************************************

    Bandai Oeka Kids drawing tablet

**********************************************************************/

#ifndef MAME_BUS_NES_CTRL_OEKAKIDS_H
#define MAME_BUS_NES_CTRL_OEKAKIDS_H

#pragma once

#include "ctrl.h"


// ======================> nes_oekakids_tablet_device

class nes_oekakids_tablet_device : public device_t,
								   public device_nes_control_port_interface
{
public:
	nes_oekakids_tablet_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);

	virtual u8 read_exp(offs_t offset) override;
	virtual void write(u8 data) override;

protected:
	virtual void device_start() override ATTR_COLD;
	virtual void device_reset() override ATTR_COLD;
	virtual ioport_constructor device_input_ports() const override ATTR_COLD;

private:
	required_ioport m_x;
	required_ioport m_y;
	required_ioport m_buttons;

	u32 m_latch;
	u8 m_strobe;
	u8 m_advance;
	u8 m_bit;
	u8 m_tablet_x;
	u8 m_tablet_y;
	u8 m_ack;
	u8 m_ack_delay;
};


DECLARE_DEVICE_TYPE(NES_OEKAKIDS_TABLET, nes_oekakids_tablet_device)

#endif // MAME_BUS_NES_CTRL_OEKAKIDS_H
