// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
#ifndef MAME_BUS_NES_BENSHIENG_H
#define MAME_BUS_NES_BENSHIENG_H

#pragma once

#include "nxrom.h"

// ======================> nes_benshieng_device

class nes_benshieng_device : public nes_nrom_device
{
public:
	nes_benshieng_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);

	virtual void write_h(offs_t offset, u8 data) override;
	virtual void pcb_reset() override;

protected:
	virtual ioport_constructor device_input_ports() const override;

private:
	required_ioport m_cartdips;
};

DECLARE_DEVICE_TYPE(NES_BENSHIENG, nes_benshieng_device)

#endif // MAME_BUS_NES_BENSHIENG_H