// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Jaleco PCBs


 Here we emulate the following PCBs

 * Jaleco JF-11 [mapper 140]
 * Jaleco JF-13 [mapper 86]
 * Jaleco JF-16 [mapper 78]
 * Jaleco JF-17 [mapper 72] + its variant with samples
 * Jaleco JF-19 [mapper 92] + its variant with samples
 * Jaleco SS88006 [mapper 18] + its variant(s) with samples

 ***********************************************************************************************************/


#include "emu.h"
#include "jaleco.h"
#include "cpu/m6502/m6502.h"

#include "speaker.h"

#define LOG_UNHANDLED (1U << 1)

#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_UNHANDLED | LOG_GENERAL)
#else
#define VERBOSE (LOG_UNHANDLED)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_JF11,       nes_jf11_device,       "nes_jf11",     "NES Cart Jaleco JF-11 PCB")
DEFINE_DEVICE_TYPE(NES_JF13,       nes_jf13_device,       "nes_jf13",     "NES Cart Jaleco JF-13 PCB")
DEFINE_DEVICE_TYPE(NES_JF16,       nes_jf16_device,       "nes_jf16",     "NES Cart Jaleco JF-16 PCB")
DEFINE_DEVICE_TYPE(NES_JF17,       nes_jf17_device,       "nes_jf17",     "NES Cart Jaleco JF-17 PCB")
DEFINE_DEVICE_TYPE(NES_JF17_ADPCM, nes_jf17_adpcm_device, "nes_jf17_pcm", "NES Cart Jaleco JF-17 + ADPCM (Moero!! Pro Tennis) PCB")
DEFINE_DEVICE_TYPE(NES_JF19,       nes_jf19_device,       "nes_jf19",     "NES Cart Jaleco JF-19 (Moero!! Pro Soccer) PCB")
DEFINE_DEVICE_TYPE(NES_JF19_ADPCM, nes_jf19_adpcm_device, "nes_jf19_pcm", "NES Cart Jaleco JF-19 + ADPCM (Moero!! Pro Yakyuu 88) PCB")
DEFINE_DEVICE_TYPE(NES_SS88006,    nes_ss88006_device,    "nes_ss88006",  "NES Cart Jaleco SS88006 PCB")
DEFINE_DEVICE_TYPE(NES_JF23,       nes_jf23_device,       "nes_jf23",     "NES Cart Jaleco JF-23 (Shin Moero Pro Yakyuu) PCB")
DEFINE_DEVICE_TYPE(NES_JF24,       nes_jf24_device,       "nes_jf24",     "NES Cart Jaleco JF-24 (Terao no Dosukoi Oozumou) PCB")
DEFINE_DEVICE_TYPE(NES_JF29,       nes_jf29_device,       "nes_jf29",     "NES Cart Jaleco JF-29 (Moe Pro! '90) PCB")
DEFINE_DEVICE_TYPE(NES_JF33,       nes_jf33_device,       "nes_jf33",     "NES Cart Jaleco JF-33 (Moe Pro! Saikyou-hen) PCB")


nes_jf11_device::nes_jf11_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_JF11, tag, owner, clock)
{
}

nes_jf13_device::nes_jf13_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_JF13, tag, owner, clock)
	, m_samples(*this, "samples")
{
}

nes_jf16_device::nes_jf16_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_JF16, tag, owner, clock)
{
}

nes_jf17_device::nes_jf17_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock, bool prg_flip)
	: nes_nrom_device(mconfig, type, tag, owner, clock)
	, m_samples(*this, "samples")
	, m_latch(0)
	, m_prg_flip(prg_flip)
{
}

nes_jf17_device::nes_jf17_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_jf17_device(mconfig, NES_JF17, tag, owner, clock, false)
{
}

nes_jf17_adpcm_device::nes_jf17_adpcm_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_jf17_device(mconfig, NES_JF17_ADPCM, tag, owner, clock, false)
{
}

nes_jf19_device::nes_jf19_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_jf17_device(mconfig, NES_JF19, tag, owner, clock, true)
{
}

nes_jf19_adpcm_device::nes_jf19_adpcm_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_jf17_device(mconfig, NES_JF19_ADPCM, tag, owner, clock, true)
{
}

nes_ss88006_device::nes_ss88006_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, type, tag, owner, clock)
	, m_samples(*this, "samples")
	, m_irq_count(0)
	, m_irq_count_latch(0)
	, m_irq_mode(0)
	, m_irq_enable(0)
	, m_irq_delay(0)
	, irq_timer(nullptr)
	, m_maincpu6502(nullptr)
	, m_wram_protect(0)
{
}

nes_ss88006_device::nes_ss88006_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_ss88006_device(mconfig, NES_SS88006, tag, owner, clock)
{
}

nes_jf23_device::nes_jf23_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_ss88006_device(mconfig, NES_JF23, tag, owner, clock)
{
}

nes_jf24_device::nes_jf24_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_ss88006_device(mconfig, NES_JF24, tag, owner, clock)
{
}

nes_jf29_device::nes_jf29_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_ss88006_device(mconfig, NES_JF29, tag, owner, clock)
{
}

nes_jf33_device::nes_jf33_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_ss88006_device(mconfig, NES_JF33, tag, owner, clock)
{
}



void nes_jf16_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);
	set_nt_mirroring(PPU_MIRROR_LOW);
}

void nes_jf17_device::device_start()
{
	common_start();
	save_item(NAME(m_latch));
}

void nes_jf17_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_flip ? 0 : m_prg_chunks - 1);
	chr8(0, m_chr_source);
	m_latch = 0;
}

void nes_ss88006_device::device_start()
{
	common_start();
	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");
	irq_timer = timer_alloc(FUNC(nes_ss88006_device::irq_timer_tick), this);
	irq_timer->adjust(attotime::zero, 0, clocks_to_attotime(1));

	save_item(NAME(m_mmc_prg_bank));
	save_item(NAME(m_mmc_vrom_bank));
	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_count_latch));
	save_item(NAME(m_irq_mode));
	save_item(NAME(m_irq_delay));
	save_item(NAME(m_wram_protect));
}

void nes_ss88006_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, m_chr_source);

	std::fill(std::begin(m_mmc_prg_bank), std::end(m_mmc_prg_bank), 0x00);
	std::fill(std::begin(m_mmc_vrom_bank), std::end(m_mmc_vrom_bank), 0x00);
	m_irq_enable = 0;
	m_irq_mode = 0;
	m_irq_count = 0;
	m_irq_count_latch = 0;
	m_irq_delay = 0;
	m_wram_protect = 0;

	m_maincpu6502->cancel_delayed_mapper_irq();
	set_irq_line(CLEAR_LINE);
}


/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

Jaleco JF-11 and JF-14 boards

Games: Bio Senshi Dan, Mississippi Satsujin Jiken,
Yousai Club

Writes throughout $6000-$7FFF select a 32 KiB
PRG-ROM bank with data bits 4-5 and an 8 KiB
CHR-ROM bank with data bits 0-3.

The cartridge does not contain PRG-RAM, so reads
from $6000-$7FFF return CPU open bus.

iNES: mapper 140

In MAME: Supported.

-------------------------------------------------*/

void nes_jf11_device::write_m(offs_t offset, u8 data) {
	LOG("jf11 write_m, offset: %04x, data: %02x\n", offset, data);

	prg32(BIT(data, 4, 2));
	chr8(BIT(data, 0, 4), CHRROM);
}

/*-------------------------------------------------

Jaleco JF-13 board

Game: Moero!! Pro Yakyuu

Writes to $6000-$6FFF select a 32 KiB PRG-ROM
bank with data bits 4-5 and an 8 KiB CHR-ROM
bank with data bits 6, 1 and 0.

The board contains a µPD7756C speech synthesizer.
MAME approximates its output using external samples.
The chip's reset timing and electrical behavior are
not emulated.

Writes to $7000-$7FFF with bits 4-5 equal to $20
start or restart the sample selected by bits 0-3.

iNES: mapper 86

In MAME: Partially supported.

-------------------------------------------------*/

void nes_jf13_device::write_m(offs_t offset, u8 data) {
	LOG("jf13 write_m, offset: %04x, data: %02x\n", offset, data);

	if (offset < 0x1000) {
		prg32(BIT(data, 4, 2));
		chr8(bitswap<3>(data, 6, 1, 0), CHRROM);
	}
	else if (m_samples && (data & 0x30) == 0x20) {
		const u8 sample = data & 0x0f;

		m_samples->start(0, sample);
	}
}

/*-------------------------------------------------

Jaleco JF-16 board

Game: Uchuusen - Cosmo Carrier

Writes throughout $8000-$FFFF select a 16 KiB
PRG-ROM bank with data bits 0-2 and an 8 KiB
CHR-ROM bank with data bits 4-7.

Data bit 3 selects lower or upper one-screen
nametable mirroring. The board is subject to
AND-type bus conflicts.

Mapper 78 is also used by Irem's Holy Diver board,
which has incompatible horizontal/vertical
mirroring control.

NES 2.0 submapper 1 identifies the Jaleco JF-16.
NES 2.0 submapper 3 identifies Irem Holy Diver.

iNES: mapper 78
NES 2.0: mapper 78, submapper 1

In MAME: Supported.

-------------------------------------------------*/

void nes_jf16_device::write_h(offs_t offset, u8 data) {
	LOG("jf16 write_h, offset: %04x, data: %02x\n", offset, data);

	data = account_bus_conflict(offset, data);

	prg16_89ab(BIT(data, 0, 3));
	chr8(BIT(data, 4, 4), CHRROM);
	set_nt_mirroring(BIT(data, 3) ? PPU_MIRROR_HIGH : PPU_MIRROR_LOW);
}


/*-------------------------------------------------

Jaleco JF-17 and JF-19 boards

Games: Moero!! Juudou Warriors, Moero!! Pro Tennis,
Pinball Quest (Japan), Moero!! Pro Soccer,
Moero!! Pro Yakyuu '88

Data bit 7 clocks the PRG-ROM bank register on a
0-to-1 transition. Data bit 6 similarly clocks the
CHR-ROM bank register.

JF-17 switches the 16 KiB PRG-ROM bank mapped at
$8000-$BFFF. JF-19 switches the bank mapped at
$C000-$FFFF.

Some board variants contain a µPD7756C speech
processor. MAME approximates its output using
external samples; the sound chip and its precise
reset and start timing are not emulated.

Both boards are subject to AND-type bus conflicts.

iNES: mapper 72 for JF-17
iNES: mapper 92 for JF-19

In MAME: Supported with sample-based speech.

-------------------------------------------------*/

void nes_jf17_device::write_h(offs_t offset, u8 data) {
	LOG("jf17 write_h, offset: %04x, data: %02x\n", offset, data);

	data = account_bus_conflict(offset, data);

	if (BIT(data, 7) && !BIT(m_latch, 7)) {
		if (m_prg_flip) {
			prg16_cdef(BIT(data, 0, 4));
		}
		else {
			prg16_89ab(BIT(data, 0, 3));
		}
	}

	if (BIT(data, 6) && !BIT(m_latch, 6)) {
		chr8(BIT(data, 0, 4), CHRROM);
	}

	m_latch = data;

	if (m_samples && (data & 0x30) == 0x20) {
		const u8 sample = offset & 0x1f;

		m_samples->start(0, sample);
	}
}

/*-------------------------------------------------

Jaleco SS88006 ASIC

Boards include JF-23, JF-24, JF-27, JF-29,
JF-30, JF-33, JF-38, JF-40 and JF-41.

Games include The Lord of King, Magic John,
Moe Pro! '90, Ninja Jajamaru, Pizza Pop!
and Plasma Ball.

The ASIC provides three switchable 8 KiB PRG-ROM
banks, eight switchable 1 KiB CHR-ROM banks,
four mirroring modes and protected PRG-RAM.

Its IRQ counter can operate as a 16-bit, 12-bit,
8-bit or 4-bit decrementing CPU M2-cycle counter.
An IRQ is requested whenever the selected portion
of the counter underflows.

Some boards contain a µPD7756C speech processor.
MAME approximates its output using external samples;
the speech chip and internal mask ROM are not
emulated here.

iNES: mapper 18

In MAME: Supported with sample-based speech.

-------------------------------------------------*/

TIMER_CALLBACK_MEMBER(nes_ss88006_device::irq_timer_tick) {
	if (m_irq_delay) {
		m_irq_delay--;

		if (!m_irq_delay) {
			m_maincpu6502->queue_delayed_mapper_irq(2);
		}
	}

	if (m_irq_enable) {
		u16 mask;

		switch (BIT(m_irq_mode, 1, 3)) {
			case 0:
				mask = 0xffff;
				break;

			case 1:
				mask = 0x0fff;
				break;

			case 2:
			case 3:
				mask = 0x00ff;
				break;

			default:
				mask = 0x000f;
				break;
		}

		m_irq_count = (m_irq_count & ~mask) | ((m_irq_count - 1) & mask);

		if ((m_irq_count & mask) == mask) {
			m_irq_delay = 2;
		}
	}
}

u8 nes_ss88006_device::read_m(offs_t offset) {
	LOG("ss88006 read_m, offset: %04x\n", offset);

	if (BIT(m_wram_protect, 0)) {
		return device_nes_cart_interface::read_m(offset);
	}

	return get_open_bus();
}

void nes_ss88006_device::write_m(offs_t offset, u8 data) {
	LOG("ss88006 write_m, offset: %04x, data: %02x\n", offset, data);

	if ((m_wram_protect & 0x03) == 0x03) {
		device_nes_cart_interface::write_m(offset, data);
	}
}

void nes_ss88006_device::write_h(offs_t offset, u8 data) {
	LOG("ss88006 write_h, offset: %04x, data: %02x\n", offset, data);

	int bank;
	int shift;

	switch (offset & 0x7003) {
		case 0x0000:
		case 0x0001:
		case 0x0002:
		case 0x0003:
		case 0x1000:
		case 0x1001:
			bank = bitswap<2>(offset, 12, 1);
			shift = BIT(offset, 0) << 2;

			m_mmc_prg_bank[bank] &= ~(0x0f << shift);
			m_mmc_prg_bank[bank] |= (data & 0x0f) << shift;

			prg8_x(bank, m_mmc_prg_bank[bank]);
			break;

		case 0x1002:
			m_wram_protect = data & 0x03;
			break;

		case 0x2000:
		case 0x2001:
		case 0x2002:
		case 0x2003:
		case 0x3000:
		case 0x3001:
		case 0x3002:
		case 0x3003:
		case 0x4000:
		case 0x4001:
		case 0x4002:
		case 0x4003:
		case 0x5000:
		case 0x5001:
		case 0x5002:
		case 0x5003:
			bank = 2 * (BIT(offset, 12, 3) - 2) + BIT(offset, 1);
			shift = BIT(offset, 0) << 2;

			m_mmc_vrom_bank[bank] &= ~(0x0f << shift);
			m_mmc_vrom_bank[bank] |= (data & 0x0f) << shift;

			chr1_x(bank, m_mmc_vrom_bank[bank], CHRROM);
			break;

		case 0x6000:
		case 0x6001:
		case 0x6002:
		case 0x6003:
			shift = 4 * (offset & 0x03);

			m_irq_count_latch &= ~(0x000f << shift);
			m_irq_count_latch |= (data & 0x0f) << shift;
			break;

		case 0x7000:
			m_irq_count = m_irq_count_latch;
			m_irq_delay = 0;

			m_maincpu6502->cancel_delayed_mapper_irq();
			set_irq_line(CLEAR_LINE);
			break;

		case 0x7001:
			m_irq_enable = BIT(data, 0);
			m_irq_mode = data & 0x0e;
			m_irq_delay = 0;

			m_maincpu6502->cancel_delayed_mapper_irq();
			set_irq_line(CLEAR_LINE);
			break;

		case 0x7002:
			switch (data & 0x03) {
				case 0:
					set_nt_mirroring(PPU_MIRROR_HORZ);
					break;

				case 1:
					set_nt_mirroring(PPU_MIRROR_VERT);
					break;

				case 2:
					set_nt_mirroring(PPU_MIRROR_LOW);
					break;

				case 3:
					set_nt_mirroring(PPU_MIRROR_HIGH);
					break;
			}
			break;

		case 0x7003:
			if (m_samples && (data & 0x03) == 0x02) {
				const u8 sample = BIT(data, 2, 5);

				m_samples->start(0, sample);
			}
			break;

		default:
			LOGMASKED(LOG_UNHANDLED, "Jaleco SS88006 uncaught write, addr: %04x, value: %02x\n", offset + 0x8000, data);
			break;
	}
}

/***************************************************************************

Boards using external samples because the internal
uPD7755C/uPD7756C mask ROMs are currently undumped.

JF-13 - Moero!! Pro Yakyuu
         iNES mapper 86

JF-17 - Moero!! Pro Tennis
         iNES mapper 72 with ADPCM

JF-19 - Moero!! Pro Yakyuu '88 - Kettei Ban
         iNES mapper 92 with ADPCM

JF-23 - Shin Moero!! Pro Yakyuu
         iNES mapper 18 with ADPCM

JF-24 - Terao no Dosukoi Oozumou
         iNES mapper 18 with ADPCM

JF-29 - Moe Pro! '90
         iNES mapper 18 with ADPCM

JF-33 - Moe Pro!: Saikyou-hen
         iNES mapper 18 with ADPCM

Each cartridge contains one speech processor and
can play only one phrase at a time.

***************************************************************************/

static const char *const jf13_sample_names[] =
{
	"*moepro",
	"00",   // strike
	"01",   // ball
	"02",   // time
	"03",   // out
	"04",   // safe
	"05",   // foul
	"06",   // (catcher obtains the ball)
	"07",   // batter out
	"08",   // play ball
	"09",   // ball four
	"10",   // home run
	"11",   // new pitcher
	"12",   // ouch (pitcher hits batter)
	"13",   // aho (idiot)
	"14",   // (bat hits the ball)
	"15",   // (crowd)
	nullptr
};

static const char *const jf17_sample_names[] =
{
	"*mptennis",
	"00",   // NOT EXISTING?
	"01",   // NOT EXISTING?
	"02",   // "love" (points)
	"03",   // 15 (points)
	"04",   // 30 (points)
	"05",   // 40 (points)
	"06",   // advantage
	"07",   // (advantage) server
	"08",   // (advantage) receiver
	"09",   // all (equal points)
	"10",   // deuce
	"11",   // game
	"12",   // (racket hits ball)
	"13",   // (crowd?)
	"14",   // fault
	"15",   // net
	"16",   // out
	"17",   // ??
	"18",   // (ball hits player)
	"19",   // NOT EXISTING?
	nullptr
};

static const char *const jf19_sample_names[] =
{
	"*moepro88",
	"00",   // out
	"01",   // safe
	"02",   // foul
	"03",   // fair
	"04",   // strike
	"05",   // ball
	"06",   // time
	"07",   // batter out
	"08",   // ball four
	"09",   // home run
	"10",   // play ball
	"11",   // new pitcher
	"12",   // pinch-hit
	"13",   // (hit by pitch)
	"14",   // (bat hits the ball)
	"15",   // (bunt)
	"16",   // NOT EXISTING?
	"17",   // (catcher obtains the ball)
	"18",   // (pitcher obtains the ball)
	"19",   // (crowd)
	nullptr
};

static const char *const jf23_sample_names[] =
{
	"*smoepro",
	"00",   // out
	"01",   // safe
	"02",   // foul
	"03",   // NOT EXISTING?
	"04",   // strike
	"05",   // ball
	"06",   // time
	"07",   // batter out
	"08",   // ball four
	"09",   // home run
	"10",   // play ball
	"11",   // new pitcher
	"12",   // pinch-hit
	"13",   // (hit by pitch)
	"14",   // (bat hits the ball)
	"15",   // (bunt)
	"16",   // (catcher obtains the ball)
	"17",   // fair
	"18",   // (catcher obtains the ball, alt)
	"19",   // (crowd)
	nullptr
};

static const char *const jf24_sample_names[] =
{
	"*terao",
	"00",   // (tree beating sound)
	"01",   // Hakkyoyoi
	"02",   // Nokotta
	"03",   // Matta Nashi
	"04",   // Nokotta Nokotta
	"05",   // Matta Arimasen
	nullptr
};

static const char *const jf29_sample_names[] =
{
	"*moepro90",
	"00",   // out
	"01",   // safe
	"02",   // foul
	"03",   // Not existing?
	"04",   // strike
	"05",   // ball
	"06",   // time
	"07",   // batter out
	"08",   // ball four
	"09",   // home run
	"10",   // play ball
	"11",   // new pitcher
	"12",   // pinch-hit
	"13",   // (hit by pitch)
	"14",   // (bat hits the ball)
	"15",   // (bunt)
	"16",   // (catcher obtains the ball, alt)
	"17",   // (catcher obtains the ball)
	"18",   // (catcher obtains the ball, alt 2)
	"19",   // (crowd)
	nullptr
};

static const char *const jf33_sample_names[] =
{
	"*mpsaikyo",
	"00",   // out
	"01",   // safe
	"02",   // foul
	"03",   // ???
	"04",   // strike
	"05",   // ball
	"06",   // time
	"07",   // batter out
	"08",   // ball four
	"09",   // home run
	"10",   // play ball
	"11",   // new pitcher
	"12",   // pinch-hit
	"13",   // (hit by pitch)
	"14",   // (bat hits the ball)
	"15",   // (bunt)
	"16",   // (catcher obtains the ball)
	"17",   // (catcher obtains the ball, alt)
	"18",   // ??
	"19",   // (crowd)
	nullptr
};


//-------------------------------------------------
//  device_add_mconfig
//-------------------------------------------------

void nes_jf13_device::device_add_mconfig(machine_config &config) {
	SPEAKER(config, "addon").front_center();

	SAMPLES(config, m_samples);
	m_samples->set_channels(1);
	m_samples->set_samples_names(jf13_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "addon", 0.50);
}

void nes_jf17_adpcm_device::device_add_mconfig(machine_config &config) {
	SPEAKER(config, "addon").front_center();

	SAMPLES(config, m_samples);
	m_samples->set_channels(1);
	m_samples->set_samples_names(jf17_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "addon", 0.50);
}

void nes_jf19_adpcm_device::device_add_mconfig(machine_config &config) {
	SPEAKER(config, "addon").front_center();

	SAMPLES(config, m_samples);
	m_samples->set_channels(1);
	m_samples->set_samples_names(jf19_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "addon", 0.50);
}

void nes_jf23_device::device_add_mconfig(machine_config &config) {
	SPEAKER(config, "addon").front_center();

	SAMPLES(config, m_samples);
	m_samples->set_channels(1);
	m_samples->set_samples_names(jf23_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "addon", 0.50);
}

void nes_jf24_device::device_add_mconfig(machine_config &config) {
	SPEAKER(config, "addon").front_center();

	SAMPLES(config, m_samples);
	m_samples->set_channels(1);
	m_samples->set_samples_names(jf24_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "addon", 0.50);
}

void nes_jf29_device::device_add_mconfig(machine_config &config) {
	SPEAKER(config, "addon").front_center();

	SAMPLES(config, m_samples);
	m_samples->set_channels(1);
	m_samples->set_samples_names(jf29_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "addon", 0.50);
}

void nes_jf33_device::device_add_mconfig(machine_config &config) {
	SPEAKER(config, "addon").front_center();

	SAMPLES(config, m_samples);
	m_samples->set_channels(1);
	m_samples->set_samples_names(jf33_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "addon", 0.50);
}