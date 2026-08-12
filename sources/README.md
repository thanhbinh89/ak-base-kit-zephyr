# AK Embedded Base Kit — Zephyr firmware

Firmware project for the **AK Embedded Base Kit v2.3** (STM32L151CBT6A),
built on Zephyr RTOS.

The board is not in Zephyr's tree, so this repository carries its own board
port. The project root doubles as a Zephyr module that contributes the
`boards/` and `dts/` roots — that is how `-b ak_base_kit` resolves.

## Layout

```
sources/
├── app/                       application
│   ├── CMakeLists.txt
│   ├── prj.conf
│   └── src/main.c             board bring-up firmware
├── boards/ak/ak_base_kit/     out-of-tree board port
│   ├── ak_base_kit.dts        <- the hardware description
│   ├── ak_base_kit_defconfig
│   ├── board.yml / board.cmake / Kconfig.ak_base_kit
│   ├── support/openocd.cfg
│   └── doc/index.rst
├── docs/SCHEMATIC.md          full transcription of the board schematic
├── dts/bindings/              out-of-tree DT bindings (vendor prefix)
├── zephyr/module.yml          declares board_root + dts_root
└── tools/                     env.sh, build.sh
```

`docs/SCHEMATIC.md` is the authoritative hardware reference — read it instead
of re-converting `../board/schematic-ak-embedded-base-kit-version-3.pdf`.

## Prerequisites

- Zephyr workspace at `~/zephyrproject` (Zephyr 4.4.99, west 1.5.0)
- Zephyr SDK at `~/zephyr-sdk-1.0.1`

Override either with `ZEPHYRPROJECT` / `ZEPHYR_SDK_INSTALL_DIR`.

## Build

```bash
tools/build.sh              # incremental
tools/build.sh -p always    # pristine
```

or manually:

```bash
source tools/env.sh
west build -b ak_base_kit app
```

Current footprint: **~55 KiB flash (42 %)**, **~11 KiB RAM (33 %)**.

## Flash and debug

SWD is on **J14** (pin 3 SWCLK, pin 4 SWDIO) and on the **J15** SH1.0
connector. Runners configured: `stm32cubeprogrammer`, `openocd`, `jlink`,
`pyocd`, `stlink_gdbserver`.

```bash
west flash -d build                     # default runner
west flash -d build -r openocd
west debug -d build
```

## Console

USART1 (PA9/PA10) goes through the on-board CH340E to the USB-C connector, so
the console is a plain USB serial port at **115200 8N1**:

```bash
minicom -D /dev/ttyUSB0 -b 115200
```

### No console output?

The CH340E is a self-contained USB device: it enumerates from VBUS and its own
D+/D- lines, **independently of the MCU**. So check in this order:

1. **Does the bridge appear on the host at all?**
   ```bash
   lsusb | grep 1a86        # expect: 1a86:7523 QinHeng CH340
   ls /dev/ttyUSB*
   ```
   If nothing shows up, the MCU and its firmware are not the problem — the
   break is on the USB side. Common causes:
   - **Charge-only USB-C cable.** It still delivers VBUS, so the board powers up
     and the OLED/buzzer/LED all work, but there are no data lines. Swap it for
     a known-good data cable.
   - **Running in a VM.** VMware/VirtualBox attach USB devices one at a time;
     passing the ST-LINK through does not pass the CH340 through. Attach it
     explicitly (VMware: *VM → Removable Devices → QinHeng CH340*).
   - USB-C plugged into a charger rather than the PC.

2. **Only if the bridge does enumerate but the port stays silent**, check the
   `SB1`/`SB2` solder bridges near the CH340E. They are 3-pad jumpers that route
   USART1 either to the bridge or to the debug headers:

   | Bridge | pin 1 | pin 2 (common) | pin 3 |
   |--------|-------|----------------|-------|
   | SB1 | `CH340_RXD` | `USART1_TX` (PA9) | `USART1_TX_DBG` → J12/J14 |
   | SB2 | `CH340_TXD` | `USART1_RX` (PA10) | `USART1_RX_DBG` → J12/J14 |

   Default is **pin1–pin2** (console on USB-C). If yours is strapped pin2–pin3,
   the console comes out on J12 (SH1.0) / J14 instead.

3. **Hardware-independent fallback:** move the console to USART3, which is
   exposed on the Grove (J5) and Qwiic (J4) connectors, and use any USB-TTL
   adapter:
   ```bash
   tools/build.sh -p always -- -DEXTRA_DTC_OVERLAY_FILE=overlays/console-usart3.overlay
   ```
   Wiring is documented at the top of `app/overlays/console-usart3.overlay`.

Note that PA9/PA10 are **not** brought out on the J13 pin header, so USART1
cannot be probed externally without moving SB1/SB2.

## What the sample application does

`app/src/main.c` exercises the board end to end:

- blinks the blue debug LED (PB8, active low)
- reads the W25Q80 JEDEC ID over SPI1
- reads the date/time from the PCF85063A RTC over I²C1
- draws a banner on the SSD1309 OLED via the bit-banged I²C bus
- beeps the buzzer and logs a message on each user-button press

## Board quick reference

| Function | Pin(s) | Notes |
|----------|--------|-------|
| Console UART1 | PA9 / PA10 | via CH340E → USB-C |
| RS-485 UART2 | PA2 / PA3, DIR PA1 | SP3485, DE+nRE tied, high = TX |
| UART3 | PB10 / PB11 | Grove J5 / Qwiic J4 |
| I²C1 | PB6 / PB7 | RTC 0x51 + Grove J8 / Qwiic J7 |
| OLED I²C (bit-bang) | PB13 SCL / PB12 SDA | SSD1309 @ 0x3C, reset PA15 |
| SPI1 | PA5 / PA6 / PA7 | NOR flash CS PB14, expansion CS PA4 |
| Debug LED | PB8 | **active low** |
| Buttons SW1/SW2/SW3 | PB3 / PC13 / PB4 | **active low** |
| Buzzer | PB0 | TIM3_CH3, active high, ~2.7 kHz |

Clock: 8 MHz HSE → PLL ×8 ÷2 → **32 MHz** SYSCLK. There is no LSE crystal on
the MCU; the internal RTC runs from LSI.

## Adding features

Board-wide hardware changes belong in
`boards/ak/ak_base_kit/ak_base_kit.dts`. Application-only overrides belong in
`app/boards/ak_base_kit.overlay`. The `modbus0` node under `usart2` is already
wired to the RS-485 direction pin and only needs `status = "okay"`.

Note that **SPI2 and I2C2 are not usable** on this board: their pins are taken
by other functions (PB12/PB13 by the OLED, PB14 by the flash chip select,
PB10/PB11 by UART3). Free pins broken out on J13 are PA0, PA11, PA12, PB5, PB9,
PB15, PC14 and PC15.

If the application writes to `storage_partition`, also set
`CONFIG_USE_DT_CODE_PARTITION=y` so the linker keeps the image inside the
120 KiB `slot0_partition`.
