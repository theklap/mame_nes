// license:BSD-3-Clause
// copyright-holders:Fabio Priuli,kmg
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Nintendo NES-EVENT PCBs


 Here we emulate the following PCBs

 * Nintendo NES-EVENT [mapper 105]
 * Nintendo NES-EVENT2 [mapper 555]

 ***********************************************************************************************************/


#include "emu.h"
#include "event.h"

#define LOG_HIFREQ (1U << 1)

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_EVENT,  nes_event_device,  "nes_event",  "NES Cart EVENT PCB")
DEFINE_DEVICE_TYPE(NES_EVENT2, nes_event2_device, "nes_event2", "NES Cart EVENT2 PCB")


nes_event_device::nes_event_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_sxrom_device(mconfig, NES_EVENT, tag, owner, clock)
	, m_dsw(*this, "DIPSW")
	, m_nwc_init(0)
	, event_timer(nullptr)
	, m_timer_count(0)
	, m_timer_on(0)
	, m_timer_enabled(0)
{
}

nes_event2_device::nes_event2_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_tqrom_device(mconfig, NES_EVENT2, tag, owner, clock)
	, m_dsw(*this, "DIPSW")
	, m_tqrom_mode(false)
	, event_timer(nullptr)
	, m_timer_count(0)
	, m_timer_enabled(0)
{
}


void nes_event_device::device_start()
{
	nes_sxrom_device::device_start();

	event_timer = timer_alloc(FUNC(nes_event_device::event_tick), this);
	event_timer->adjust(attotime::never);

	save_item(NAME(m_nwc_init));

	save_item(NAME(m_timer_count));
	save_item(NAME(m_timer_on));
	save_item(NAME(m_timer_enabled));
}

void nes_event_device::pcb_reset()
{
	m_nwc_init = 2;

	nes_sxrom_device::pcb_reset();

	prg32(0);
	chr8(0, CHRRAM);

	m_timer_count = 0;
	m_timer_enabled = 0;
	m_timer_on = 0;

	event_timer->adjust(attotime::never);
	set_irq_line(CLEAR_LINE);
}

void nes_event2_device::device_start()
{
	mmc3_start();

	event_timer = timer_alloc(FUNC(nes_event2_device::event_tick), this);
	event_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	save_item(NAME(m_tqrom_mode));

	save_item(NAME(m_timer_count));
	save_item(NAME(m_timer_enabled));
}

void nes_event2_device::pcb_reset()
{
	m_tqrom_mode = false;
	mmc3_common_initialize(0x07, 0x7f, 0);

	m_timer_count = 0;
	m_timer_enabled = 0;
}



/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 NES-EVENT competition cartridge

 Game: Nintendo World Championships 1990

 This board combines an MMC1-compatible serial
 interface with discrete PRG banking and a 30-bit
 competition timer.

 MMC1 register $A000 is repurposed:

 Bit 4 controls the competition timer. Writing it
 clear resets and acknowledges the timer. Setting it
 afterward starts the timer.

 Bit 3 selects the PRG banking mode. When clear,
 bits 1-2 select one of four 32 KiB banks from the
 first 128 KiB PRG-ROM. When set, normal MMC1 PRG
 banking applies to the second 128 KiB PRG-ROM.

 The first 32 KiB PRG bank remains selected until
 the initial $A000 bit 4 low-to-high sequence.

 The board contains one fixed 8 KiB CHR-RAM region.

 iNES: mapper 105

 In MAME: Supported.

 -------------------------------------------------*/

void nes_event_device::update_regs(int reg)
{
	if (reg != 1) {
		nes_sxrom_device::update_regs(reg);
		return;
	}

	const bool timer_run = BIT(m_reg[1], 4);

	switch (m_nwc_init) {
		case 2:
			if (!timer_run) {
				m_nwc_init = 1;
			}
			break;

		case 1:
			if (timer_run) {
				m_nwc_init = 0;
			}
			break;
	}

	// Apply the $A000 PRG selection after updating the
	// startup state, but before changing the timer.
	nes_sxrom_device::update_regs(reg);

	if (!timer_run) {
		m_timer_count = 0;
		m_timer_enabled = 1;
		m_timer_on = 0;

		event_timer->adjust(attotime::never);
		set_irq_line(CLEAR_LINE);
	}
	else if (!m_timer_on && m_timer_enabled) {
		event_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));
		m_timer_on = 1;
	}
}

void nes_event_device::set_chr()
{
	// CHR-RAM is fixed and is not controlled by the MMC1 CHR registers.
	chr8(0, CHRRAM);
}

void nes_event_device::set_prg()
{
	if (m_nwc_init) {
		prg32(0);
		return;
	}

	if (BIT(m_reg[1], 3)) {
		// Apply normal MMC1 PRG banking to the second 128 KiB.
		nes_sxrom_device::set_prg(0x08, 0x07);
	}
	else {
		// Bits 1-2 select a 32 KiB bank from the first 128 KiB.
		prg32(BIT(m_reg[1], 1, 2));
	}
}

/*-------------------------------------------------

 NES-EVENT2 competition cartridge

 Game: Nintendo Campus Challenge 1991

 This board combines an MMC3-compatible mapper with
 discrete game-selection logic and a competition
 timer.

 The board contains 8 KiB of CHR-RAM and 8 KiB of
 WRAM used by the included games. An additional
 2 KiB WRAM region is mirrored through $5000-$57FF
 and is used by the competition control program.

 Reads from $5800-$5FFF return the timer status on
 bit 7. Writes to $5800-$5BFF select the active game,
 PRG/CHR outer banks and timer state.

 The original board also contains an RJ11 interface
 used to transmit player names and scores to an
 external display. That interface is not emulated.

 The current implementation is based partly on the
 documented RetroZone reproduction board.

 NES 2.0: mapper 555

 In MAME: Supported.

 -------------------------------------------------*/

u8 nes_event2_device::read_l(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "event2 read_l, offset: %04x\n", offset);

	const offs_t address = offset + 0x4100;

	if (address >= 0x5800) {
		const u32 timer_limit = u32(0x10 | m_dsw->read()) << 25;

		return m_timer_count >= timer_limit ? 0x80 : 0x00;
	}

	if (address >= 0x5000 && !m_prgram.empty()) {
		const offs_t ram_address = 0x2000 + (address & 0x07ff);

		return m_prgram[ram_address % m_prgram.size()];
	}

	return get_open_bus();
}

u8 nes_event2_device::read_m(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "event2 read_m, offset: %04x\n", offset);

	if (m_prgram.empty()) {
		return get_open_bus();
	}

	return m_prgram[offset % m_prgram.size()];
}

void nes_event2_device::write_l(offs_t offset, u8 data)
{
	LOG("event2 write_l, offset: %04x, data: %02x\n", offset, data);

	const offs_t address = offset + 0x4100;

	if (address >= 0x5000 && address < 0x5800) {
		if (!m_prgram.empty()) {
			const offs_t ram_address = 0x2000 + (address & 0x07ff);

			m_prgram[ram_address % m_prgram.size()] = data;
		}

		return;
	}

	if (address >= 0x5800 && address < 0x5c00) {
		m_tqrom_mode = (data & 0x06) == 0x02;

		m_prg_base = (data & 0x04) << 3;
		m_prg_mask = ((data & 0x03) << 3) | 0x07;

		set_prg(m_prg_base, m_prg_mask);

		m_chr_base = m_prg_base << 2;

		set_chr(m_chr_source, m_chr_base, m_chr_mask);

		m_timer_enabled = BIT(data, 3);

		if (!m_timer_enabled) {
			m_timer_count = 0;
		}
	}
}

void nes_event2_device::write_m(offs_t offset, u8 data)
{
	LOGMASKED(LOG_HIFREQ, "event2 write_m, offset: %04x, data: %02x\n", offset, data);

	if (!m_prgram.empty()) {
		m_prgram[offset % m_prgram.size()] = data;
	}
}

void nes_event2_device::chr_cb(int start, int bank, int source)
{
	if (m_tqrom_mode) {
		nes_tqrom_device::chr_cb(start, bank, source);
	}
	else {
		nes_txrom_device::chr_cb(start, bank, source);
	}
}

//-------------------------------------------------
//  Competition timer DIP switches
//-------------------------------------------------

static INPUT_PORTS_START(nwc_dsw)
	PORT_START("DIPSW")

	PORT_DIPNAME(0x0f, 0x04, "Timer") PORT_DIPLOCATION("SW:!1,!2,!3,!4")
	PORT_DIPSETTING(0x00, "5:00.4")
	PORT_DIPSETTING(0x01, "5:19.2")
	PORT_DIPSETTING(0x02, "5:38.0")
	PORT_DIPSETTING(0x03, "5:56.7")
	PORT_DIPSETTING(0x04, "6:15.5")
	PORT_DIPSETTING(0x05, "6:34.3")
	PORT_DIPSETTING(0x06, "6:53.1")
	PORT_DIPSETTING(0x07, "7:11.9")
	PORT_DIPSETTING(0x08, "7:30.6")
	PORT_DIPSETTING(0x09, "7:49.4")
	PORT_DIPSETTING(0x0a, "8:08.2")
	PORT_DIPSETTING(0x0b, "8:27.0")
	PORT_DIPSETTING(0x0c, "8:45.8")
	PORT_DIPSETTING(0x0d, "9:04.5")
	PORT_DIPSETTING(0x0e, "9:23.3")
	PORT_DIPSETTING(0x0f, "9:42.1")
INPUT_PORTS_END

ioport_constructor nes_event_device::device_input_ports() const
{
	return INPUT_PORTS_NAME(nwc_dsw);
}

ioport_constructor nes_event2_device::device_input_ports() const
{
	return INPUT_PORTS_NAME(nwc_dsw);
}

//-------------------------------------------------
//  Competition timers
//-------------------------------------------------

TIMER_CALLBACK_MEMBER(nes_event_device::event_tick)
{
	if (!m_timer_on) {
		return;
	}

	const u32 timer_limit = u32(0x10 | m_dsw->read()) << 25;

	m_timer_count++;

	if (m_timer_count >= timer_limit) {
		set_irq_line(ASSERT_LINE);
		event_timer->reset();
	}
}

TIMER_CALLBACK_MEMBER(nes_event2_device::event_tick)
{
	if (m_timer_enabled) {
		m_timer_count++;
	}
}