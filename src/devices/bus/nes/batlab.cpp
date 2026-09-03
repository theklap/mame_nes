// license:BSD-3-Clause
// copyright-holders:kmg
/***************************************************************************

NES/Famicom cartridge emulation for Batlab Electronics and related PCBs

Here we emulate the following homebrew PCBs

- Batlab BATMAP-000 [mapper 399]
- Batlab BATMAP-SRR-X [mapper 413]

***************************************************************************/

#include "emu.h"
#include "batlab.h"

#include "cpu/m6502/m6502.h"
#include "video/ppu2c0x.h"

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

DEFINE_DEVICE_TYPE(NES_BATMAP_000,  nes_batmap_000_device,  "nes_batmap_000",  "NES Cart Batlab BATMAP-000 PCB")
DEFINE_DEVICE_TYPE(NES_BATMAP_SRRX, nes_batmap_srrx_device, "nes_batmap_srrx", "NES Cart Batlab BATMAP-SRR-X PCB")

nes_batmap_000_device::nes_batmap_000_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_txrom_device(mconfig, NES_BATMAP_000, tag, owner, clock)
{
}

nes_batmap_srrx_device::nes_batmap_srrx_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_nrom_device(mconfig, NES_BATMAP_SRRX, tag, owner, clock)
	, m_reg(0)
	, m_dpcm_addr(0)
	, m_dpcm_ctrl(0)
	, m_irq_count(0)
	, m_irq_count_latch(0)
	, m_irq_enable(0)
	, m_irq_reload(false)
	, delay_irq(0)
	, m_last_a12_low_cycle(0)
	, m_prev_ppu_addr(0)
	, m_a12_low_seen(false)
	, m_maincpu6502(nullptr)
{
}

void nes_batmap_000_device::pcb_reset()
{
	mmc3_common_initialize(0x0f, 0xff, 0);

	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);
	chr8(0, CHRRAM);
}

void nes_batmap_srrx_device::device_start()
{
	common_start();

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_reg));
	save_item(NAME(m_dpcm_addr));
	save_item(NAME(m_dpcm_ctrl));

	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_count_latch));
	save_item(NAME(m_irq_reload));
	save_item(NAME(delay_irq));

	save_item(NAME(m_last_a12_low_cycle));
	save_item(NAME(m_prev_ppu_addr));
	save_item(NAME(m_a12_low_seen));
}

void nes_batmap_srrx_device::pcb_reset()
{
	prg8_89(0);
	prg8_ab(0);

	// CPU $C000-$CFFF is occupied by EXROM reads. The upper
	// half of this 8 KiB PRG mapping remains visible at
	// CPU $D000-$DFFF as fixed 4 KiB PRG bank 7.
	prg8_cd(3);

	// CPU $E000-$FFFF contains fixed 8 KiB PRG bank 4.
	prg8_ef(4);

	chr4_0(0, CHRROM);
	chr4_4(0x3d, CHRROM);

	m_reg = 0;
	m_dpcm_addr = 0;
	m_dpcm_ctrl = 0;

	m_irq_enable = 0;
	m_irq_count = 0;
	m_irq_count_latch = 0;
	m_irq_reload = false;
	delay_irq = 0;

	m_last_a12_low_cycle = 0;
	m_prev_ppu_addr = 0;
	m_a12_low_seen = false;

	set_irq_line(CLEAR_LINE);
	m_maincpu6502->cancel_delayed_mapper_irq();

	machine().root_device().subdevice<ppu2c0x_device>("ppu")->set_mapper(413);
}

/*-------------------------------------------------*
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

Batlab BATMAP-000 board

Games: Star Versus, Anamanaguchi cartridges

These boards use a Xilinx XC9572XL CPLD. They provide
MMC3-compatible mirroring and qualified PPU A12 IRQ
logic, but use simpler PRG-ROM and CHR-RAM banking.

CPU $8000-$9FFF: fixed first 8 KiB PRG-ROM bank
CPU $A000-$BFFF: switchable 8 KiB PRG-ROM bank
CPU $C000-$DFFF: switchable 8 KiB PRG-ROM bank
CPU $E000-$FFFF: fixed final 8 KiB PRG-ROM bank

PPU $0000-$0FFF: switchable 4 KiB CHR-RAM bank
PPU $1000-$1FFF: switchable 4 KiB CHR-RAM bank

The board has 1 MiB of PRG ROM, 32 KiB of CHR RAM,
and no WRAM at CPU $6000-$7FFF.

NES 2.0: mapper 399

In MAME: Supported.

-------------------------------------------------*/

void nes_batmap_000_device::write_h(offs_t offset, u8 data)
{
	LOG("batmap_000 write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x6001)
	{
		case 0x0000:
			chr4_x(BIT(data, 7) << 2, data & 0x07, CHRRAM);
			break;

		case 0x0001:
			prg8_x(BIT(data, 7) + 1, data & 0x7f);
			break;

		default:
			txrom_write(offset, data);
			break;
	}
}

/*-------------------------------------------------

Batlab BATMAP-SRR-X board

Game: Super Russian Roulette

The cartridge contains:

- 256 KiB PRG ROM
- 256 KiB CHR ROM
- 8 MiB SPI flash EXROM containing DPCM speech
- Xilinx XC9572XL CPLD

The CPLD performs serial SPI access to the EXROM and
presents each fetched byte in parallel at CPU
$4800-$4FFF and $C000-$CFFF.

CPU read map:

$4100-$47FF: CPU open bus
$4800-$4FFF: EXROM data
$5000-$5FFF: fixed 4 KiB PRG-ROM bank 1
$6000-$7FFF: switchable 8 KiB PRG-ROM bank register 0
$8000-$9FFF: switchable 8 KiB PRG-ROM bank register 1
$A000-$BFFF: switchable 8 KiB PRG-ROM bank register 2
$C000-$CFFF: EXROM data
$D000-$DFFF: fixed 4 KiB PRG-ROM bank 7
$E000-$FFFF: fixed 8 KiB PRG-ROM bank 4

PPU map:

$0000-$0FFF: switchable 4 KiB CHR-ROM bank register 3
$1000-$1FFF: fixed 4 KiB CHR-ROM bank $3D

The 23-bit EXROM address is shifted serially through
writes to CPU $C000-$CFFF using data bit 7. Bit 1 of
writes to CPU $D000-$DFFF controls automatic address
incrementing after EXROM reads.

The IRQ counter is clocked by qualified PPU A12 rising
edges. A12 must remain low for more than nine PPU cycles
before a rising edge clocks the counter.

NES 2.0: mapper 413

In MAME: Supported.

-------------------------------------------------*/

void nes_batmap_srrx_device::ppu_bus_address(uint16_t ppu_address, uint64_t ppu_cycle, int ppu_tick, bool odd_frame)
{
	ppu_address &= 0x3fff;

	const bool previous_a12 = BIT(m_prev_ppu_addr, 12);
	const bool current_a12 = BIT(ppu_address, 12);

	if (!current_a12)
	{
		if (previous_a12)
		{
			m_last_a12_low_cycle = ppu_cycle;
			m_a12_low_seen = true;
		}
		else if (!m_a12_low_seen)
		{
			m_last_a12_low_cycle = ppu_cycle;
			m_a12_low_seen = true;
		}
	}

	if (!previous_a12 && current_a12)
	{
		const uint64_t low_time = ppu_cycle - m_last_a12_low_cycle;

		if (m_a12_low_seen && low_time > 9)
			irq_clock();

		m_a12_low_seen = false;
	}

	m_prev_ppu_addr = ppu_address;
}

void nes_batmap_srrx_device::irq_clock()
{
	if (m_irq_reload || !m_irq_count)
	{
		m_irq_count = m_irq_count_latch;
		m_irq_reload = false;
	}
	else
	{
		m_irq_count--;
	}

	if (m_irq_enable && !m_irq_count)
		delay_irq = 2;
}

void nes_batmap_srrx_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick, uint16_t ppu_address)
{
	if (delay_irq > 0)
	{
		--delay_irq;

		if (delay_irq == 0)
			m_maincpu6502->queue_delayed_mapper_irq(2);
	}
}

u8 nes_batmap_srrx_device::read_dpcm()
{
	m_dpcm_addr &= 0x7fffff;

	const u8 data = m_misc_rom[m_dpcm_addr];

	if (BIT(m_dpcm_ctrl, 1))
		m_dpcm_addr = (m_dpcm_addr + 1) & 0x7fffff;

	return data;
}

u8 nes_batmap_srrx_device::read_l(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "batmap_srrx read_l, offset: %04x\n", offset);

	offset += 0x100;

	switch (offset & 0x1800)
	{
		case 0x0000:
			return get_open_bus();

		case 0x0800:
			return read_dpcm();

		default:
			return m_prg[offset];
	}
}

u8 nes_batmap_srrx_device::read_m(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "batmap_srrx read_m, offset: %04x\n", offset);

	return m_prg[(m_reg * 0x2000 + offset) & (m_prg_size - 1)];
}

u8 nes_batmap_srrx_device::read_h(offs_t offset)
{
	LOGMASKED(LOG_HIFREQ, "batmap_srrx read_h, offset: %04x\n", offset);

	if ((offset & 0x7000) == 0x4000)
		return read_dpcm();

	return hi_access_rom(offset);
}

void nes_batmap_srrx_device::write_h(offs_t offset, u8 data)
{
	LOG("batmap_srrx write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x7000)
	{
		case 0x0000: // $8000-$8FFF: set IRQ reload value
			m_irq_count_latch = data;
			break;

		case 0x1000: // $9000-$9FFF: request reload on next qualified A12 rising edge
			m_irq_reload = true;
			break;

		case 0x2000: // $A000-$AFFF: disable IRQ, acknowledge it, and clear counter
			m_irq_enable = 0;
			m_irq_count = 0;
			m_irq_reload = false;
			delay_irq = 0;

			set_irq_line(CLEAR_LINE);
			m_maincpu6502->cancel_delayed_mapper_irq();
			break;

		case 0x3000: // $B000-$BFFF: enable IRQ
			m_irq_enable = 1;
			break;

		case 0x4000: // $C000-$CFFF: serially load the 23-bit EXROM address
			m_dpcm_addr = ((m_dpcm_addr << 1) | BIT(data, 7)) & 0x7fffff;
			break;

		case 0x5000: // $D000-$DFFF: control EXROM automatic incrementing
			m_dpcm_ctrl = data;
			break;

		case 0x6000:
		case 0x7000:
			switch (data >> 6)
			{
				case 0:
					m_reg = data & 0x3f;
					break;

				case 1:
					prg8_89(data & 0x3f);
					break;

				case 2:
					prg8_ab(data & 0x3f);
					break;

				case 3:
					chr4_0(data & 0x3f, CHRROM);
					break;
			}
			break;
	}
}