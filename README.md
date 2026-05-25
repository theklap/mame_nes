[README.md](https://github.com/user-attachments/files/28231572/README.md)
# MAME NES / Famicom Accuracy Improvements

This branch contains a major NES/Famicom accuracy update for MAME.

The main **M6502/NES CPU timing path**, **APU**, and **PPU** have been heavily reworked toward cycle-accurate behavior. The goal of this work is to improve hardware accuracy while keeping the implementation compatible with MAME's existing NES/Famicom architecture.

## Current Status

The current implementation passes **100% of the Accuracy Coin NES test suite**.

```text
Accuracy Coin: 100% passing
```

## Major Areas Reworked

### M6502 / NES CPU Timing

The NES CPU execution path has been updated to better support cycle-level behavior required by the rest of the system.

This includes improved interaction with:

- **APU timing**
- **DMA timing**
- **DMC DMA stalls**
- **Controller reads**
- **Mapper IRQ timing**
- **PPU-visible timing behavior**

### APU

The NES APU has been reworked for more accurate timing and DMA behavior.

Areas improved include:

- **Frame timing**
- **Channel timing**
- **DMC timing**
- **DMC DMA scheduling**
- **DMC DMA abort behavior**
- **DMC DMA interaction with OAM DMA**
- **Controller read conflicts during DMC DMA**

### PPU

The PPU has been substantially reworked around dot/cycle-level behavior.

Areas improved include:

- **Rendering enable/disable timing**
- **PPUMASK behavior**
- **Background and sprite pipeline timing**
- **Stale background shift registers**
- **Stale sprite shift registers**
- **Sprite zero hit behavior**
- **Sprite overflow behavior**
- **OAM evaluation and `$2004` behavior**
- **`$2006` / `$2007` timing**
- **Scroll reload and increment timing**
- **PPU bus behavior visible to mappers**

## Mapper Improvements

Several mapper implementations have also been improved as part of this work.

Areas of focus include:

- **MMC1**
- **MMC3 / MMC6**
- **MMC5**
- **Konami VRC mappers**
- **MMC3-based clone boards and multicarts**

These changes improve banking behavior, IRQ timing, PPU address observation, CHR/PRG handling, and board-specific edge cases.

## Testing

Testing has focused on both dedicated test ROMs and commercial games.

Primary test coverage includes:

- **Accuracy Coin**
- **PPU timing tests**
- **Sprite zero tests**
- **Stale shifter tests**
- **OAM / `$2004` behavior tests**
- **DMA and controller timing tests**
- **Mapper IRQ and banking tests**
- **Commercial game visual checks**

## Notes

This work is focused on hardware-shaped behavior rather than game-specific hacks. In several places, the emulator now separates internal PPU state from final pixel output so that stale shifter behavior, sprite-zero hits, forced blanking, and visible rendering can behave closer to hardware.

## Acknowledgements

This work was guided by NESdev documentation, hardware behavior research, test ROMs, and commercial game testing.
