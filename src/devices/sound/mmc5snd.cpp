// license:BSD-3-Clause
// copyright-holders:R. Belmont, Fabio Priuli

/***************************************************************************

    mmc5snd.cpp
    Nintendo MMC5 add-on sound

    MMC5 provides two additional pulse channels and a simple PCM DAC.

***************************************************************************/

#include "emu.h"
#include "mmc5snd.h"


DEFINE_DEVICE_TYPE(MMC5SND, mmc5snd_device, "mmc5snd", "Nintendo MMC5 Sound")


//-------------------------------------------------
//  mmc5snd_device - constructor
//-------------------------------------------------

mmc5snd_device::mmc5snd_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: device_t(mconfig, MMC5SND, tag, owner, clock)
	, device_sound_interface(mconfig, *this)
	, m_pcm_mode(0)
	, m_pcm_dac(0)
	, m_frame_accum(0)
	, m_length_clock_phase(false)
	, m_timer_divider(false)
	, m_stream(nullptr)
{
}


//-------------------------------------------------
//  device_start
//-------------------------------------------------

void mmc5snd_device::device_start()
{
	m_stream = stream_alloc(0, 1, clock());

	save_item(STRUCT_MEMBER(m_pulse, control));
	save_item(STRUCT_MEMBER(m_pulse, timer_low));
	save_item(STRUCT_MEMBER(m_pulse, timer_high));
	save_item(STRUCT_MEMBER(m_pulse, timer));
	save_item(STRUCT_MEMBER(m_pulse, duty_step));
	save_item(STRUCT_MEMBER(m_pulse, length_counter));
	save_item(STRUCT_MEMBER(m_pulse, envelope_divider));
	save_item(STRUCT_MEMBER(m_pulse, envelope_decay));
	save_item(STRUCT_MEMBER(m_pulse, envelope_start));
	save_item(STRUCT_MEMBER(m_pulse, enabled));

	save_item(NAME(m_pcm_mode));
	save_item(NAME(m_pcm_dac));
	save_item(NAME(m_frame_accum));
	save_item(NAME(m_length_clock_phase));
	save_item(NAME(m_timer_divider));
	
	logerror("MMC5 Expanded Audio Loaded\n");
}


//-------------------------------------------------
//  device_reset
//-------------------------------------------------

void mmc5snd_device::device_reset()
{
	m_stream->update();

	for (auto &pulse : m_pulse)
	{
		pulse.control = 0;
		pulse.timer_low = 0;
		pulse.timer_high = 0;
		pulse.timer = 0;
		pulse.duty_step = 0;
		pulse.length_counter = 0;
		pulse.envelope_divider = 0;
		pulse.envelope_decay = 0;
		pulse.envelope_start = false;
		pulse.enabled = false;
	}

	m_pcm_mode = 0;
	m_pcm_dac = 0;
	m_frame_accum = 0;
	m_length_clock_phase = false;
	m_timer_divider = false;
}


//-------------------------------------------------
//  period
//-------------------------------------------------

u16 mmc5snd_device::period(int chan) const
{
	return m_pulse[chan].timer_low | ((m_pulse[chan].timer_high & 0x07) << 8);
}


//-------------------------------------------------
//  pulse_output
//-------------------------------------------------

u8 mmc5snd_device::pulse_output(int chan) const
{
	const pulse_t &pulse = m_pulse[chan];
	const u16 timer_period = period(chan);

	if (!pulse.enabled || pulse.length_counter == 0 || timer_period < 8)
		return 0;

	const u8 duty = (pulse.control >> 6) & 0x03;
	if (!BIT(DUTY_TABLE[duty], pulse.duty_step))
		return 0;

	if (pulse.control & 0x10)
		return pulse.control & 0x0f;

	return pulse.envelope_decay & 0x0f;
}


//-------------------------------------------------
//  clock_pulse_timers
//-------------------------------------------------

void mmc5snd_device::clock_pulse_timers()
{
	// NES pulse timers advance on every other CPU cycle.  With the stream
	// running at CPU clock, divide by two here so frequency is CPU / (16 * (period + 1)).
	m_timer_divider = !m_timer_divider;
	if (!m_timer_divider)
		return;

	for (auto &pulse : m_pulse)
	{
		if (pulse.timer == 0)
		{
			pulse.timer = pulse.timer_low | ((pulse.timer_high & 0x07) << 8);
			pulse.duty_step = (pulse.duty_step + 1) & 0x07;
		}
		else
		{
			--pulse.timer;
		}
	}
}


//-------------------------------------------------
//  clock_envelopes
//-------------------------------------------------

void mmc5snd_device::clock_envelopes()
{
	for (auto &pulse : m_pulse)
	{
		const u8 volume = pulse.control & 0x0f;
		const bool loop = BIT(pulse.control, 5);

		if (pulse.envelope_start)
		{
			pulse.envelope_start = false;
			pulse.envelope_decay = 15;
			pulse.envelope_divider = volume;
		}
		else if (pulse.envelope_divider == 0)
		{
			pulse.envelope_divider = volume;

			if (pulse.envelope_decay != 0)
				--pulse.envelope_decay;
			else if (loop)
				pulse.envelope_decay = 15;
		}
		else
		{
			--pulse.envelope_divider;
		}
	}
}


//-------------------------------------------------
//  clock_length_counters
//-------------------------------------------------

void mmc5snd_device::clock_length_counters()
{
	for (auto &pulse : m_pulse)
	{
		if (!BIT(pulse.control, 5) && pulse.length_counter != 0)
			--pulse.length_counter;
	}
}


//-------------------------------------------------
//  clock_frame_sequencer
//-------------------------------------------------

void mmc5snd_device::clock_frame_sequencer()
{
	// Approximate the APU frame sequencer timing at 240 Hz for envelope clocks.
	// Length counters clock every other envelope tick, i.e. 120 Hz.
	m_frame_accum += 240;

	while (m_frame_accum >= clock())
	{
		m_frame_accum -= clock();

		clock_envelopes();

		m_length_clock_phase = !m_length_clock_phase;
		if (m_length_clock_phase)
			clock_length_counters();
	}
}


//-------------------------------------------------
//  sound_stream_update
//-------------------------------------------------

void mmc5snd_device::sound_stream_update(sound_stream &stream, std::vector<read_stream_view> const &inputs, std::vector<write_stream_view> &outputs)
{
	for (int i = 0; i < outputs[0].samples(); i++)
	{
		clock_frame_sequencer();
		clock_pulse_timers();

		// Two 4-bit pulse channels plus the 7-bit PCM DAC.
		// Keep this intentionally conservative; external route gain can be tuned per board.
		const s32 tmp = (pulse_output(0) + pulse_output(1)) * 4 + (m_pcm_dac & 0x7f);

		outputs[0].put_int(i, tmp, 512);
	}
}


//-------------------------------------------------
//  read
//-------------------------------------------------

u8 mmc5snd_device::read(offs_t offset)
{
	m_stream->update();

	switch (offset & 0x1f)
	{
		case 0x15:
			return (m_pulse[0].length_counter ? 0x01 : 0x00) |
				   (m_pulse[1].length_counter ? 0x02 : 0x00);

		default:
			return 0x00;
	}
}


//-------------------------------------------------
//  write
//-------------------------------------------------

void mmc5snd_device::write(offs_t offset, u8 data)
{
	m_stream->update();

	switch (offset & 0x1f)
	{
		case 0x00:
			m_pulse[0].control = data;
			break;

		case 0x02:
			m_pulse[0].timer_low = data;
			break;

		case 0x03:
			m_pulse[0].timer_high = data;
			m_pulse[0].timer = period(0);
			m_pulse[0].duty_step = 0;
			m_pulse[0].envelope_start = true;
			if (m_pulse[0].enabled)
				m_pulse[0].length_counter = LENGTH_TABLE[(data >> 3) & 0x1f];
			break;

		case 0x04:
			m_pulse[1].control = data;
			break;

		case 0x06:
			m_pulse[1].timer_low = data;
			break;

		case 0x07:
			m_pulse[1].timer_high = data;
			m_pulse[1].timer = period(1);
			m_pulse[1].duty_step = 0;
			m_pulse[1].envelope_start = true;
			if (m_pulse[1].enabled)
				m_pulse[1].length_counter = LENGTH_TABLE[(data >> 3) & 0x1f];
			break;

		case 0x10:
			m_pcm_mode = data;
			break;

		case 0x11:
			m_pcm_dac = data & 0x7f;
			break;

		case 0x15:
			m_pulse[0].enabled = BIT(data, 0);
			m_pulse[1].enabled = BIT(data, 1);

			if (!m_pulse[0].enabled)
				m_pulse[0].length_counter = 0;
			if (!m_pulse[1].enabled)
				m_pulse[1].length_counter = 0;
			break;

		default:
			break;
	}
}