// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************


 NES/Famicom cartridge emulation for Nintendo MMC-3 & MMC-6 PCBs


 Here we emulate the following PCBs

 * Nintendo TxROM, aka MMC-3 [mapper 4]
 * Nintendo HKROM, aka MMC-6 [mapper 4]
 * Nintendo TxSROM [mapper 118]
 * Nintendo TQROM [mapper 119]
 * Nintendo NES-QJ [mapper 47]
 * Nintendo PAL-ZZ [mapper 37]


 Known issues on specific mappers:

 * 004 Mendel Palace has never worked properly
 * 004 Ninja Gaiden 2 has flashing bg graphics in the second level

 ***********************************************************************************************************/


#include "emu.h"
#include "mmc3.h"
#include "cpu/m6502/m6502.h"
#include "video/ppu2c0x.h"      // this has to be included so that IRQ functions can access ppu2c0x_device::BOTTOM_VISIBLE_SCANLINE

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

DEFINE_DEVICE_TYPE(NES_TXROM,  nes_txrom_device,  "nes_txrom",  "NES Cart TxROM (MMC-3) PCB")
DEFINE_DEVICE_TYPE(NES_HKROM,  nes_hkrom_device,  "nes_hkrom",  "NES Cart HKROM (MMC-6) PCB")
DEFINE_DEVICE_TYPE(NES_TXSROM, nes_txsrom_device, "nes_txsrom", "NES Cart TxSROM PCB")
DEFINE_DEVICE_TYPE(NES_TQROM,  nes_tqrom_device,  "nes_tqrom",  "NES Cart TQROM PCB")
DEFINE_DEVICE_TYPE(NES_QJ_PCB, nes_qj_device,     "nes_qj",     "NES Cart NES-QJ PCB")
DEFINE_DEVICE_TYPE(NES_ZZ_PCB, nes_zz_device,     "nes_zz",     "NES Cart PAL-ZZ PCB")


nes_txrom_device::nes_txrom_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock)
	: nes_nrom_device(mconfig, type, tag, owner, clock),
	  m_prg_base(0),
	  m_prg_mask(0),
	  m_chr_base(0),
	  m_chr_mask(0),
	  m_latch(0),
	  m_mmc_mirror(0),
	  m_wram_protect(0),
	  m_irq_count(0),
	  m_irq_count_latch(0),
	  m_irq_enable(0),
	  m_irq_reload(false),
	  rev_b_behavior(true),
	  delay_irq(0),
	  m_last_a12_low_cpu(0),
	  m_prev_ppu_addr(0),
	  m_maincpu6502(nullptr),
	  m_scanline(0),
	  m_dot(0),
	  m_mmc3_clocks_since_c001(0xff),
	  m_mmc3_seen_c001_recent(false),
	  m_a12_low_seen(false),
	  m_c001_pathology_pending(false)
{
	std::fill(std::begin(m_mmc_prg_bank), std::end(m_mmc_prg_bank), 0);
	std::fill(std::begin(m_mmc_vrom_bank), std::end(m_mmc_vrom_bank), 0);
}

nes_txrom_device::nes_txrom_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_txrom_device(mconfig, NES_TXROM, tag, owner, clock)
{
}

nes_hkrom_device::nes_hkrom_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: nes_txrom_device(mconfig, NES_HKROM, tag, owner, clock), m_wram_enable(0), m_mmc6_reg(0)
{
}

nes_txsrom_device::nes_txsrom_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock)
	: nes_txrom_device(mconfig, type, tag, owner, clock)
{
}

nes_txsrom_device::nes_txsrom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_txsrom_device(mconfig, NES_TXSROM, tag, owner, clock)
{
}

nes_tqrom_device::nes_tqrom_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock)
	: nes_txrom_device(mconfig, type, tag, owner, clock)
{
}

nes_tqrom_device::nes_tqrom_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_tqrom_device(mconfig, NES_TQROM, tag, owner, clock)
{
}

nes_qj_device::nes_qj_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_txrom_device(mconfig, NES_QJ_PCB, tag, owner, clock)
{
}

nes_zz_device::nes_zz_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: nes_txrom_device(mconfig, NES_ZZ_PCB, tag, owner, clock)
{
}

void nes_txrom_device::mmc3_start()
{
	common_start();
	m_maincpu6502 = machine().root_device().subdevice<m6502_device>("maincpu");
	
	save_item(NAME(m_mmc_prg_bank));
	save_item(NAME(m_mmc_vrom_bank));
	save_item(NAME(m_latch));
	save_item(NAME(m_mmc_mirror));
	save_item(NAME(m_wram_protect));
	save_item(NAME(m_prg_base));
	save_item(NAME(m_prg_mask));
	save_item(NAME(m_chr_base));
	save_item(NAME(m_chr_mask));
	save_item(NAME(m_irq_enable));
	save_item(NAME(m_irq_count));
	save_item(NAME(m_irq_count_latch));

	save_item(NAME(rev_b_behavior));
	save_item(NAME(delay_irq));
	save_item(NAME(m_irq_reload));
	save_item(NAME(m_last_a12_low_cpu));
	save_item(NAME(m_prev_ppu_addr));
	save_item(NAME(m_mmc3_clocks_since_c001));
	save_item(NAME(m_mmc3_seen_c001_recent));
}

void nes_txrom_device::mmc3_common_initialize( int prg_mask, int chr_mask, int nec_irq_behavior )
{
	m_mmc_prg_bank[0] = m_mmc_prg_bank[2] = 0xffe; // m_mmc_prg_bank[2] & m_mmc_prg_bank[3] remain always the same in most MMC3 variants
	m_mmc_prg_bank[1] = m_mmc_prg_bank[3] = 0xfff; // but some pirate clone mappers change them after writing certain registers

	// Point CHR banks to the first 8K of memory. This is needed for a few unlicensed carts that use VRAM and don't init banks properly.
	// This includes at least some Waixing games, EverQuest and Sanguozhi, and some multicarts, New Star 6 in 1 and Famicom Yarou Vol 1.
	m_mmc_vrom_bank[0] = 0;
	m_mmc_vrom_bank[1] = 2;
	m_mmc_vrom_bank[2] = 4;
	m_mmc_vrom_bank[3] = 5;
	m_mmc_vrom_bank[4] = 6;
	m_mmc_vrom_bank[5] = 7;
	m_mmc_vrom_bank[6] = 0;  // extension reg used by clone boards
	m_mmc_vrom_bank[7] = 0;  // extension reg used by clone boards
	
	m_latch = 0;
	m_mmc_mirror = 0;
	m_wram_protect = 0x80;

	m_prg_base = m_chr_base = 0;
	m_prg_mask = prg_mask;
	m_chr_mask = chr_mask;

	m_irq_enable = 0;
	m_irq_count = m_irq_count_latch = 0;

	set_prg(m_prg_base, m_prg_mask);
	set_chr(m_chr_source, m_chr_base, m_chr_mask);

	delay_irq = 0;
	m_irq_reload = false;
	m_last_a12_low_cpu = 0;
	m_prev_ppu_addr = 0;
	m_mmc3_clocks_since_c001 = 0xff;
	m_mmc3_seen_c001_recent = false;
	
	m_a12_low_seen = false;
	m_c001_pathology_pending = false;
	
	// 0 = Sharp/new behavior, nonzero = NEC/old behavior.
	rev_b_behavior = !nec_irq_behavior;
	
	machine().root_device().subdevice<ppu2c0x_device>("ppu")->set_mapper(4);
}


void nes_txrom_device::pcb_reset()
{
	// 4-screen mirroring is taken care in pcb_start, if needed...
	mmc3_common_initialize(0xff, 0xff, 0);
}


void nes_hkrom_device::device_start()
{
	mmc3_start();
	save_item(NAME(m_mmc6_reg));
	save_item(NAME(m_wram_enable));

	save_item(NAME(m_mmc6_ram));

	m_mapper_sram_size = 0x400;
	m_mapper_sram = m_mmc6_ram;
}

void nes_hkrom_device::pcb_reset()
{
	mmc3_common_initialize(0xff, 0xff, 1);
	m_mmc6_reg = 0xf0;
	m_wram_enable = 0;
}


void nes_qj_device::pcb_reset()
{
	mmc3_common_initialize(0x0f, 0x7f, 0);
}


void nes_zz_device::pcb_reset()
{
	mmc3_common_initialize(0x07, 0x7f, 0);
}


/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

/*-------------------------------------------------

 TxROM (MMC3 based) board emulation

 iNES: mapper 4

 -------------------------------------------------*/

void nes_txrom_device::observe_ppu_a12(uint16_t ppu_addr, uint64_t cpu_cycles)
{
	ppu_addr &= 0x3FFF;

	const bool prev_a12 = BIT(m_prev_ppu_addr, 12);
	const bool a12 = BIT(ppu_addr, 12);

	if (!a12)
	{
		if (prev_a12)
		{
			m_last_a12_low_cpu = cpu_cycles;
			m_a12_low_seen = true;
		}
		else if (!m_a12_low_seen)
		{
			m_last_a12_low_cpu = cpu_cycles;
			m_a12_low_seen = true;
		}
	}

	if (!prev_a12 && a12)
	{
		const uint64_t low_time = cpu_cycles - m_last_a12_low_cpu;

		if (m_a12_low_seen && low_time >= 4)
		{
			mmc3_irq_clock();
		}

		m_a12_low_seen = false;
	}

	m_prev_ppu_addr = ppu_addr;
}

void nes_txrom_device::mmc3_irq_clock()
{
	if (m_mmc3_seen_c001_recent && m_mmc3_clocks_since_c001 < 0xff)
		++m_mmc3_clocks_since_c001;

	if (m_c001_pathology_pending)
	{
		logerror("[MMC3 PATHOLOGY APPLY] cpu=%lld sl=%d dot=%u "
				 "before count=%02X latch=%02X reload=%d enable=%d rev_b=%d\n",
			(long long)m_maincpu6502->total_cycles(),
			m_scanline,
			m_dot,
			m_irq_count,
			m_irq_count_latch,
			m_irq_reload ? 1 : 0,
			m_irq_enable ? 1 : 0,
			rev_b_behavior ? 1 : 0);

		if (rev_b_behavior)
		{
			// MMC3B/Sharp/new behavior described by mmc3_irq_tests:
			// counter is ORed with $80, and this clock neither decrements nor reloads.
			m_irq_count |= 0x80;
		}
		else
		{
			// MMC3A/NEC/old behavior described as frozen:
			// this clock neither decrements nor reloads.
			// Leave m_irq_count unchanged.
		}

		m_irq_reload = false;
		m_c001_pathology_pending = false;

		logerror("[MMC3 PATHOLOGY RESULT] cpu=%lld sl=%d dot=%u "
				 "after count=%02X latch=%02X reload=%d enable=%d\n",
			(long long)m_maincpu6502->total_cycles(),
			m_scanline,
			m_dot,
			m_irq_count,
			m_irq_count_latch,
			m_irq_reload ? 1 : 0,
			m_irq_enable ? 1 : 0);

		return;
	}
	
    const uint8_t old_count = m_irq_count;
    const bool had_reload_request = m_irq_reload;

    if (rev_b_behavior)
    {
		// Sharp / "new" behavior:
		// if counter == 0 or reload requested -> reload latch
		// else decrement
		// then IRQ if counter == 0 and enabled
        if (m_irq_count == 0 || m_irq_reload)
        {
            m_irq_count = m_irq_count_latch;
            m_irq_reload = false;
        }
        else
        {
            --m_irq_count;
        }

        if (m_irq_enable && m_irq_count == 0) {
			//logerror("ppu_tick_in_cpu_cycle: %d, m_scanline:%d, m_dot: %d \n", m_ppu_tick, m_scanline, m_dot);
			if(m_ppu_tick == 1) {
				delay_irq = 3;
			}
			if(m_ppu_tick == 2) {
				delay_irq = 2;
			}
			if(m_ppu_tick == 3) {
				delay_irq = 1;
			}
			//m_maincpu6502->queue_delayed_mapper_irq(2);
		}
    }
    else
    {
		// Alternate / old behavior:
		// same reload/decrement structure, but IRQ on 1->0 transition
		// plus IRQ when reloading to 0 due to an explicit reload request ($C001 clear/reload),
		// but NOT when reloading to 0 only because the counter had already naturally reached 0
        if (m_irq_count == 0 || m_irq_reload)
        {
            m_irq_count = m_irq_count_latch;
            m_irq_reload = false;
        }
        else
        {
            --m_irq_count;
        }

        if (m_irq_enable)
        {
            const bool dec_1_to_0 =
                (!had_reload_request && old_count == 1 && m_irq_count == 0);

            const bool reload_request_to_0 =
                (had_reload_request && m_irq_count == 0);

            if (dec_1_to_0 || reload_request_to_0) {
				//logerror("ppu_tick_in_cpu_cycle: %d, m_scanline:%d, m_dot: %d \n", m_ppu_tick, m_scanline, m_dot);
                if(m_ppu_tick == 1) {
					delay_irq = 3;
				}
				if(m_ppu_tick == 2) {
					delay_irq = 2;
				}
				if(m_ppu_tick == 3) {
					delay_irq = 1;
				}
				//m_maincpu6502->queue_delayed_mapper_irq(2);
			}
        }
    }
}

void nes_txrom_device::ppu_to_mapper(int scanline, unsigned dot, int ppu_tick)
{
	m_ppu_tick = ppu_tick;
	m_scanline = scanline;
	m_dot = dot;
	if (delay_irq > 0)
	{
		--delay_irq;

		if (delay_irq == 0)
			m_maincpu6502->queue_delayed_mapper_irq(2);
	}
}

// base MMC3 simply calls prg8_x
void nes_txrom_device::prg_cb( int start, int bank )
{
	prg8_x(start, bank);
}

// base MMC3 simply calls chr1_x
void nes_txrom_device::chr_cb( int start, int bank, int source )
{
	chr1_x(start, bank, source);
}


void nes_txrom_device::set_prg( int prg_base, int prg_mask )
{
	uint8_t prg_flip = (m_latch & 0x40) ? 2 : 0;

	prg_cb(0, prg_base | (m_mmc_prg_bank[0 ^ prg_flip] & prg_mask));
	prg_cb(1, prg_base | (m_mmc_prg_bank[1] & prg_mask));
	prg_cb(2, prg_base | (m_mmc_prg_bank[2 ^ prg_flip] & prg_mask));
	prg_cb(3, prg_base | (m_mmc_prg_bank[3] & prg_mask));
}

void nes_txrom_device::set_chr( uint8_t chr, int chr_base, int chr_mask )
{
	
	uint8_t chr_page = (m_latch & 0x80) >> 5;

	chr_cb(chr_page ^ 0, chr_base | ((m_mmc_vrom_bank[0] & ~0x01) & chr_mask), chr);
	chr_cb(chr_page ^ 1, chr_base | ((m_mmc_vrom_bank[0] |  0x01) & chr_mask), chr);
	chr_cb(chr_page ^ 2, chr_base | ((m_mmc_vrom_bank[1] & ~0x01) & chr_mask), chr);
	chr_cb(chr_page ^ 3, chr_base | ((m_mmc_vrom_bank[1] |  0x01) & chr_mask), chr);
	chr_cb(chr_page ^ 4, chr_base | (m_mmc_vrom_bank[2] & chr_mask), chr);
	chr_cb(chr_page ^ 5, chr_base | (m_mmc_vrom_bank[3] & chr_mask), chr);
	chr_cb(chr_page ^ 6, chr_base | (m_mmc_vrom_bank[4] & chr_mask), chr);
	chr_cb(chr_page ^ 7, chr_base | (m_mmc_vrom_bank[5] & chr_mask), chr);
	
}

void nes_txrom_device::txrom_write(offs_t offset, uint8_t data)
{
	uint8_t mmc_helper, cmd;

	LOG("txrom_write, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x6001)
	{
		case 0x0000:	//Bank select ($8000-$9FFE, even)
			mmc_helper = m_latch ^ data;
			m_latch = data;

			// Has PRG Mode changed?
			if (mmc_helper & 0x40)
				set_prg(m_prg_base, m_prg_mask);

			// Has CHR Mode changed?
			if (mmc_helper & 0x80)
				set_chr(m_chr_source, m_chr_base, m_chr_mask);
			break;

		case 0x0001:	//Bank data ($8001-$9FFF, odd)
			cmd = m_latch & 0x07;
			switch (cmd)
			{
				case 0: case 1: // these do not need to be separated: we take care of them in set_chr!
				case 2: case 3: case 4: case 5:
					m_mmc_vrom_bank[cmd] = data;
					set_chr(m_chr_source, m_chr_base, m_chr_mask);
					break;
				case 6:
				case 7:
					m_mmc_prg_bank[cmd - 6] = data;
					set_prg(m_prg_base, m_prg_mask);
					break;
			}
			break;

		case 0x2000:	//Mirroring ($A000-$BFFE, even)
			m_mmc_mirror = data;
			if (m_mirroring != PPU_MIRROR_4SCREEN)
				set_nt_mirroring(BIT(data, 0) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT);
			break;

		case 0x2001:	//PRG RAM protect ($A001-$BFFF, odd)
			m_wram_protect = data;
			break;

		case 0x4000:	//IRQ latch ($C000-$DFFE, even)
			m_irq_count_latch = data;
			break;

		case 0x4001:    // IRQ reload ($C001-$DFFF, odd)
		{
			if (m_mmc3_seen_c001_recent && m_mmc3_clocks_since_c001 == 1)
			{
				/*m_c001_pathology_pending = true;

				logerror("[MMC3 PATHOLOGY ARMED] cpu=%lld sl=%d dot=%u "
						 "count=%02X latch=%02X reload=%d enable=%d rev_b=%d\n",
					(long long)m_maincpu6502->total_cycles(),
					m_scanline,
					m_dot,
					m_irq_count,
					m_irq_count_latch,
					m_irq_reload ? 1 : 0,
					m_irq_enable ? 1 : 0,
					rev_b_behavior ? 1 : 0);
				*/
			}

			m_mmc3_seen_c001_recent = true;
			m_mmc3_clocks_since_c001 = 0;

			m_irq_count = 0;
			m_irq_reload = true;
			break;
		}

		case 0x6000:	//IRQ disable ($E000-$FFFE, even)
			m_irq_enable = 0;
			set_irq_line(CLEAR_LINE);
			if(delay_irq > 0)
				m_maincpu6502->cancel_delayed_mapper_irq();
			delay_irq = 0;
			break;

		case 0x6001:	// IRQ enable ($E001-$FFFF, odd)
			m_irq_enable = 1;
			break;

		default:
			LOGMASKED(LOG_UNHANDLED, "txrom_write uncaught: %04x value: %02x\n", offset + 0x8000, data);
			break;
	}
}

void nes_txrom_device::write_m(offs_t offset, uint8_t data)
{
	LOG("txrom write_m, offset: %04x, data: %02x\n", offset, data);

	if (BIT(m_wram_protect, 7) && !BIT(m_wram_protect, 6))
	{
		if (!m_battery.empty())
			m_battery[offset & (m_battery.size() - 1)] = data;
		else if (!m_prgram.empty())
			m_prgram[offset & (m_prgram.size() - 1)] = data;
	}
}

uint8_t nes_txrom_device::read_m(offs_t offset)
{
	LOG("txrom read_m, offset: %04x\n", offset);

	if (BIT(m_wram_protect, 7))
	{
		if (!m_battery.empty())
			return m_battery[offset & (m_battery.size() - 1)];
		if (!m_prgram.empty())
			return m_prgram[offset & (m_prgram.size() - 1)];
	}

	return get_open_bus();
}


/*-------------------------------------------------

 HKROM (MMC6 based) board emulation

 iNES: mapper 4

 -------------------------------------------------*/

void nes_hkrom_device::write_m(offs_t offset, uint8_t data)
{
	uint8_t write_hi, write_lo;
	LOG("hkrom write_m, offset: %04x, data: %02x\n", offset, data);

	if (offset < 0x1000)
		return;

	// banks can be written only if both read & write is enabled!
	write_hi = ((m_mmc6_reg & 0xc0) == 0xc0);
	write_lo = ((m_mmc6_reg & 0x30) == 0x30);

	if (BIT(offset, 9) && write_hi) // access to upper half of 1k
		m_mmc6_ram[offset & 0x3ff] = data;

	if (!BIT(offset, 9) && write_lo)    // access to lower half of 1k
		m_mmc6_ram[offset & 0x3ff] = data;
}

uint8_t nes_hkrom_device::read_m(offs_t offset)
{
	LOG("hkrom read_m, offset: %04x\n", offset);
	
	if (offset < 0x1000)
		return get_open_bus();

	if (!(m_mmc6_reg & 0xa0))
		return get_open_bus();

	if (BIT(offset, 9) && BIT(m_mmc6_reg, 7))   // access to upper half of 1k when upper read is enabled
		return m_mmc6_ram[offset & 0x3ff];

	if (!BIT(offset, 9) && BIT(m_mmc6_reg, 5))  // access to lower half of 1k when lower read is enabled
		return m_mmc6_ram[offset & 0x3ff];

	// If only one bank is enabled for reading, the other reads back as zero
	return 0x00;
}

void nes_hkrom_device::write_h(offs_t offset, uint8_t data)
{
	uint8_t mmc6_helper;
	LOG("hkrom write_h, offset: %04x, data: %02x\n", offset, data);
	
	switch (offset & 0x6001)
	{
		case 0x0000:
			mmc6_helper = m_latch ^ data;
			m_latch = data;

			if (!m_wram_enable && BIT(data, 5))  // if WRAM is disabled and has to be enabled, write
				m_wram_enable = BIT(data, 5);    // (once WRAM has been enabled, it cannot be disabled without resetting the game)

			// Has PRG Mode changed?
			if (BIT(mmc6_helper, 6))
				set_prg(m_prg_base, m_prg_mask);

			// Has CHR Mode changed?
			if (BIT(mmc6_helper, 7))
				set_chr(m_chr_source, m_chr_base, m_chr_mask);
			break;

		case 0x2001:
			if (m_wram_enable)
				m_mmc6_reg = data;
			break;

		case 0x4001:
			txrom_write(offset, data);
			break;

		default:
			txrom_write(offset, data);
			break;
	}
}

/*-------------------------------------------------

 TxSROM (MMC3 based) board emulation

 Games: Armadillo, Play Action Football, Pro Hockey, RPG
 Jinsei Game, Y's 3

 iNES: mapper 118

 In MAME: Supported. It also uses mmc3_irq.

 -------------------------------------------------*/

void nes_txsrom_device::set_chr(u8 chr, int chr_base, int chr_mask)
{
	nes_txrom_device::set_chr(chr, chr_base, chr_mask);

	// do nametables
	static constexpr u8 bank[8] = { 0, 0, 1, 1, 2, 3, 4, 5 };
	int start = (m_latch & 0x80) >> 5;

	for (int i = 0; i < 4; i++)
		set_nt_page(i, CIRAM, BIT(m_mmc_vrom_bank[bank[start + i]], 7), 1);
}

void nes_txsrom_device::write_h(offs_t offset, u8 data)
{
	LOG("txsrom write_h, offset: %04x, data: %02x\n", offset, data);

	switch (offset & 0x6001)
	{
		case 0x2000:
			break;
		default:
			txrom_write(offset, data);
			break;
	}
}

/*-------------------------------------------------

 TQROM (MMC3 based) board emulation

 Games: Pin Bot, High Speed

 iNES: mapper 119

 In MAME: Supported. It also uses mmc3_irq.

 -------------------------------------------------*/

void nes_tqrom_device::chr_cb(int start, int bank, int source)
{
	if (BIT(bank, 6))
		chr1_x(start, bank & 0x07, CHRRAM);
	else
		chr1_x(start, bank & 0x3f, CHRROM);
}

/*-------------------------------------------------

 NES-QJ board (MMC3 variant for US 2-in-1 Nintendo cart
 Super Spike V'Ball + Nintendo World Cup)

 iNES: mapper 47

 -------------------------------------------------*/

void nes_qj_device::write_m(offs_t offset, u8 data)
{
	LOG("qj write_m, offset: %04x, data: %02x\n", offset, data);

	if (BIT(m_wram_protect, 7) && !BIT(m_wram_protect, 6))
	{
		m_prg_base = BIT(data, 0) << 4;
		m_chr_base = BIT(data, 0) << 7;
		set_prg(m_prg_base, m_prg_mask);
		set_chr(m_chr_source, m_chr_base, m_chr_mask);
	}
}

/*-------------------------------------------------

 PAL-ZZ board (MMC3 variant for European 3-in-1 Nintendo cart
 Super Mario Bros. + Tetris + Nintendo World Cup)

 iNES: mapper 37

 TODO: this board apparently only resets to the menu
 screen on systems with a CIC (the outer PRG lines
 are somehow tied to the CIC's reset line).

 -------------------------------------------------*/

void nes_zz_device::write_m(offs_t offset, u8 data)
{
	LOG("zz write_m, offset: %04x, data: %02x\n", offset, data);

	if (BIT(m_wram_protect, 7) && !BIT(m_wram_protect, 6))
	{
		m_prg_base = ((data & 0x04) | (data & (data << 1) & 0x02)) << 2;
		m_prg_mask = (m_prg_mask == 0x10) ? 0x0f : 0x07;
		m_chr_base = BIT(data, 2) << 7;
		set_prg(m_prg_base, m_prg_mask);
		set_chr(m_chr_source, m_chr_base, m_chr_mask);
	}
}
