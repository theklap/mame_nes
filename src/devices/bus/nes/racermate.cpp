// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for RacerMate PCBs


 Here we emulate the UNL-RACERMATE PCB [mapper 168]

 TODO:
 - save VRAM
 - emulate the bike controller?
 - emulate battery-backed CHR RAM protection/open bus behavior

 ***********************************************************************************************************/


#include "emu.h"
#include "racermate.h"
#include "cpu/m6502/m6502.h"


#ifdef NES_PCB_DEBUG
#define VERBOSE (LOG_GENERAL)
#else
#define VERBOSE (0)
#endif
#include "logmacro.h"


//-------------------------------------------------
//  constructor
//-------------------------------------------------

DEFINE_DEVICE_TYPE(NES_RACERMATE, nes_racermate_device, "nes_racermate", "NES Cart Racermate PCB")


nes_racermate_device::nes_racermate_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, NES_RACERMATE, tag, owner, clock)
	, m_latch(0)
	, m_irq_count(0)
	, m_irq_ppu_divider(0)
	, m_irq_delay(0)
	, m_irq_enabled(false)
	, m_maincpu6502(nullptr)
{
}


void nes_racermate_device::device_start()
{
	common_start();

	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");

	save_item(NAME(m_latch));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_ppu_divider));
	save_item(NAME(m_irq_delay));
	save_item(NAME(m_irq_enabled));
}


void nes_racermate_device::pcb_reset()
{
	prg16_89ab(0);
	prg16_cdef(m_prg_chunks - 1);

	chr4_0(0, CHRRAM);
	chr4_4(0, CHRRAM);

	set_nt_mirroring(PPU_MIRROR_VERT);

	m_latch = 0;
	m_irq_count = 0;
	m_irq_ppu_divider = 0;
	m_irq_delay = 0;
	m_irq_enabled = false;

	set_irq_line(CLEAR_LINE);

	if (m_maincpu6502)
		m_maincpu6502->cancel_delayed_mapper_irq();
}



/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 Board UNL-RACERMATE

 Mapper 168 bank select, CPU $8000-$BFFF:

   7  bit  0
   ---- ----
   PPxx CCCC
   ||   ||||
   ||   ++++- Select 4K CHR RAM bank at PPU $1000-$1FFF
   ++-------- Select 16K PRG ROM bank at CPU $8000-$BFFF

 CPU $C000-$FFFF:

   Delivered boards use D2 as the IRQ/protection control bit.

   D2 = 1:
      IRQ acknowledged
      IRQ counter frozen at 0

   D2 = 0:
      IRQ counter counts up every CPU M2 cycle

 The IRQ line is asserted while the counter's 1024s bit is set.
 Since this MAME tree calls ppu_to_mapper() every PPU tick, we divide
 PPU ticks by 3 to approximate CPU M2 cycles.

 -------------------------------------------------*/

void nes_racermate_device::update_banks()
{
	const uint8_t prg_bank = (m_latch >> 6) & 0x03;
	const uint8_t chr_bank = m_latch & 0x0f;

	prg16_89ab(prg_bank);
	prg16_cdef(m_prg_chunks - 1);

	chr4_0(0, CHRRAM);
	chr4_4(chr_bank, CHRRAM);
}


void nes_racermate_device::write_h(offs_t offset, uint8_t data)
{
	LOG("racermate write_h, offset: %04x, data: %02x\n", offset, data);

	// MAME write_h offset is CPU address - $8000:
	//
	//   offset $0000-$3FFF = CPU $8000-$BFFF
	//   offset $4000-$7FFF = CPU $C000-$FFFF

	if (offset < 0x4000)
	{
		m_latch = data;
		update_banks();
	}
	else
	{
		// Delivered boards use D2:
		//
		//   D2 = 1: acknowledge IRQ and freeze counter at 0
		//   D2 = 0: allow counter to run
		if (BIT(data, 2))
		{
			m_irq_enabled = false;
			m_irq_count = 0;
			m_irq_ppu_divider = 0;
			m_irq_delay = 0;

			if (m_maincpu6502)
				m_maincpu6502->cancel_delayed_mapper_irq();

			set_irq_line(CLEAR_LINE);
		}
		else
		{
			m_irq_enabled = true;
		}
	}
}


void nes_racermate_device::clock_irq_counter()
{
	if (!m_irq_enabled)
		return;

	const bool old_irq_bit = BIT(m_irq_count, 10);

	m_irq_count = (m_irq_count + 1) & 0x07ff;

	const bool new_irq_bit = BIT(m_irq_count, 10);

	// IRQ becomes active when the 1024s bit rises.
	// Queue it through the CPU path with the same 2-PPU-dot delay style used
	// by your MMC3 implementation.
	if (!old_irq_bit && new_irq_bit)
		m_irq_delay = 2;

	// When the 1024s bit naturally clears after wrap, clear the IRQ line.
	if (old_irq_bit && !new_irq_bit)
	{
		m_irq_delay = 0;

		if (m_maincpu6502)
			m_maincpu6502->cancel_delayed_mapper_irq();

		set_irq_line(CLEAR_LINE);
	}
}


void nes_racermate_device::ppu_to_mapper(int scanline, unsigned dot)
{
	if (m_irq_delay > 0)
	{
		--m_irq_delay;

		if (m_irq_delay == 0 && m_irq_enabled && BIT(m_irq_count, 10))
			m_maincpu6502->queue_delayed_mapper_irq(2);
	}

	if (++m_irq_ppu_divider >= 3)
	{
		m_irq_ppu_divider = 0;
		clock_irq_counter();
	}
}