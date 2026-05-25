// license:GPL-2.0+
// copyright-holders:Matthew Conte
/*****************************************************************************

  MAME/MESS NES APU CORE

  Based on the Nofrendo/Nosefart NES RP2A03 sound emulation core written by
  Matthew Conte (matt@conte.com) and redesigned for use in MAME/MESS by
  Who Wants to Know? (wwtk@mail.com)

  This core is written with the advise and consent of Matthew Conte and is
  released under the GNU Public License.  This core is freely available for
  use in any freeware project, subject to the following terms:

  Any modifications to this code must be duly noted in the source and
  approved by Matthew Conte and myself prior to public submission.

 *****************************************************************************

   NES_DEFS.H

   NES APU internal type definitions and constants.

 *****************************************************************************/

#ifndef MAME_SOUND_NES_DEFS_H
#define MAME_SOUND_NES_DEFS_H

#pragma once

/* APU type */
struct apu_t
{
	
	/* CHANNEL TYPE DEFINITIONS */

	/* Square Wave */
	struct square_t
	{
		// Range 0-15
		// (Potentially) affected by
		//   - volume updates,
		//   - length counter updates,
		//   - period updates,
		//   - and waveform position updates
		unsigned output_level;

		bool     enabled = false;

		bool     const_vol;
		unsigned duty;
		unsigned waveform_pos;
		unsigned len_cnt;
		unsigned period;
		unsigned period_cnt;
		bool     sweep_enabled;
		bool     sweep_negate;
		unsigned sweep_period;
		unsigned sweep_period_cnt;
		unsigned sweep_shift;
		bool     sweep_reload_flag;
		unsigned vol;

		unsigned env_div_cnt;
		unsigned env_vol;
		bool     halt_len_loop_env;
		bool 	 envelope_loop;
		bool     env_start_flag;
		
		u8 output = 0;

		// Recalculated whenever anything happens that might affect the sweep
		// target period. Not sure if this optimization is still worthwhile.
		int sweep_target_period;
	};

	/* REGISTER DEFINITIONS */
	static constexpr unsigned WRA0    = 0x00;
	static constexpr unsigned WRA1    = 0x01;
	static constexpr unsigned WRA2    = 0x02;
	static constexpr unsigned WRA3    = 0x03;
	static constexpr unsigned WRB0    = 0x04;
	static constexpr unsigned WRB1    = 0x05;
	static constexpr unsigned WRB2    = 0x06;
	static constexpr unsigned WRB3    = 0x07;
	static constexpr unsigned WRC0    = 0x08;
	static constexpr unsigned WRC2    = 0x0A;
	static constexpr unsigned WRC3    = 0x0B;
	static constexpr unsigned WRD0    = 0x0C;
	static constexpr unsigned WRD2    = 0x0E;
	static constexpr unsigned WRD3    = 0x0F;
	static constexpr unsigned WRE0    = 0x10;
	static constexpr unsigned WRE1    = 0x11;
	static constexpr unsigned WRE2    = 0x12;
	static constexpr unsigned WRE3    = 0x13;
	static constexpr unsigned SMASK   = 0x15;
	static constexpr unsigned IRQCTRL = 0x17;

	/* Sound channels */
	square_t   pulse[2];
};

/* CONSTANTS */

// Length counter look-up table
uint8_t const len_table[] = {
	  10, 254, 20,  2, 40,  4, 80,  6, 160,  8, 60, 10, 14, 12, 26, 14,
	  12,  16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30 };

uint16_t const ntsc_noise_periods[] =
	{ 4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068 };
uint16_t const pal_noise_periods[]  =
	{ 4, 8, 14, 30, 60, 88, 118, 148, 188, 236, 354, 472, 708,  944, 1890, 3778 };

uint16_t const ntsc_dmc_periods[] =
	{ 428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106,  84,  72,  54 };
uint16_t const pal_dmc_periods[] =
	{ 398, 354, 316, 298, 276, 236, 210, 198, 176, 148, 132, 118,  98,  78,  66,  50 };

/* ratios of pos/neg pulse for square waves */
/* 2/16 = 12.5%, 4/16 = 25%, 8/16 = 50%, 12/16 = 75% */
static uint8_t const pulse_duties[4][8] =
      { { 0, 1, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 1, 0, 0, 0 },
        { 1, 0, 0, 1, 1, 1, 1, 1 } };

// Premultiply by three to save multiplication during mixing
uint8_t const tri_waveform_steps[32] =
  { 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,  5,  4,  3,  2,  1,  0,
     0,  1,  2,  3,  4,  5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	 
	 
struct mmc5_sound_t
{
	/* CHANNEL TYPE DEFINITIONS */

	/* Square Wave */
	struct square_t
	{
		square_t()
		{
			for (auto & elem : regs)
				elem = 0;
		}

		u8 regs[4];
		s32 freq = 0;
		float phaseacc = 0.0;
		float env_phase = 0.0;
		u8 adder = 0;
		u8 env_vol = 0;
		bool enabled = false;
		u8 output = 0;
	};

	/* DPCM Wave */
	struct pcm_t
	{
		pcm_t()
		{
			for (auto & elem : regs)
				elem = 0;
		}

		u8 regs[2];
		bool irq_enabled = false;
		bool irq_line = false;
		u8 output = 0;
	};


	/* REGISTER DEFINITIONS */
	static constexpr unsigned WRA0    = 0x00;
	static constexpr unsigned WRA1    = 0x01;
	static constexpr unsigned WRA2    = 0x02;
	static constexpr unsigned WRA3    = 0x03;
	static constexpr unsigned WRB0    = 0x04;
	static constexpr unsigned WRB1    = 0x05;
	static constexpr unsigned WRB2    = 0x06;
	static constexpr unsigned WRB3    = 0x07;
	static constexpr unsigned WRE0    = 0x10;
	static constexpr unsigned WRE1    = 0x11;
	static constexpr unsigned SMASK   = 0x15;

	/* Sound channels */
	square_t   squ[2];
	pcm_t      pcm;
};
#endif // MAME_SOUND_NES_DEFS_H
