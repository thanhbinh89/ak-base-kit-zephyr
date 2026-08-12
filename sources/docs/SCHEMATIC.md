# AK Embedded Base Kit v2.3 — Schematic Reference

Machine-readable transcription of
`board/schematic-ak-embedded-base-kit-version-3.pdf` (Altium, 9 pages, Rev A,
title block "AK MCU KIT 2.3 / AK MCU KIT 2I3_B.PrjPcb").

**Read this instead of re-converting the PDF.** Everything below was extracted
from the vector text layer with coordinates and cross-checked against rendered
crops of each sheet. Where the PDF is genuinely ambiguous it says so.

- PCB: 60 × 60 mm, 1.6 mm thick
- MCU: **STM32L151CBT6A** — LQFP48, Cortex-M3, 128 KiB flash, 32 KiB SRAM, 4 KiB EEPROM
- Zephyr SoC id: `stm32l151xba` (`SOC_STM32L151XBA`, `st/l1/stm32l151Xb-a.dtsi`)
- Zephyr pinctrl: `st/l1/stm32l151c(6-8-b)txa-pinctrl.dtsi`

Sheet index: 1 cover · 2 block diagram · 3 power · 4 RS485 · 5 LCD ·
6 connectors · 7 MCU · 8 revision history · 9 PCB assembly drawing.

---

## 1. Complete MCU pin map (U1, sheet 7)

Nets named `PAxx`/`PBxx`/`PCxx` are pins brought out to a header with no
dedicated on-board function.

| Pin | Port | Net | Function on this board |
|----:|------|-----|------------------------|
| 1 | VLCD | +3V3 | tied to +3V3 |
| 2 | PC13 | `USER_BTN2_N` | User button **SW2** (active low) |
| 3 | PC14 | `PC14` | free — J13 header (OSC32_IN, **no LSE crystal fitted**) |
| 4 | PC15 | `PC15` | free — J13 header (OSC32_OUT, **no LSE crystal fitted**) |
| 5 | PH0 | `OSC_IN` | X1 **8 MHz** crystal (C1/C2 12 pF) |
| 6 | PH1 | `OSC_OUT` | X1 8 MHz crystal |
| 7 | NRST | `NRST` | S4 reset button, C15 100 nF, R36 100R → J6 pin 10 |
| 8 | VSSA | GND | |
| 9 | VDDA | `VDDA` | +3V3 via R8 0Ω |
| 10 | PA0 | `PA0` | free — J13 header (ADC12_IN0 / WKUP1) |
| 11 | PA1 | `RS485_DIR` | SP3485 DE+nRE (high = transmit) |
| 12 | PA2 | `RS485_TX` | USART2_TX → SP3485 DI |
| 13 | PA3 | `RS485_RX` | USART2_RX ← SP3485 RO |
| 14 | PA4 | `PA4` | SPI1 chip select for expansion header J6 pin 4 |
| 15 | PA5 | `SPI1_SCK` | SPI1_SCK via **R2 22R** series → NOR flash + J6 |
| 16 | PA6 | `SPI1_MISO` | SPI1_MISO |
| 17 | PA7 | `SPI1_MOSI` | SPI1_MOSI |
| 18 | PB0 | `BUZZER_PWM` | Buzzer drive (TIM3_CH3) |
| 19 | PB1 | `PB1` | free — J6 pin 8 + J13 (TIM3_CH4) |
| 20 | PB2 | `PB2` | BOOT1, **R1 10k pull-down**, otherwise unused |
| 21 | PB10 | `USART3_TX` | USART3_TX → Grove J5 / Qwiic J4 |
| 22 | PB11 | `USART3_RX` | USART3_RX → Grove J5 / Qwiic J4 |
| 23 | VSS_1 | GND | |
| 24 | VDD_1 | +3V3 | |
| 25 | PB12 | `LCD_SDA` | OLED I²C **SDA** (bit-bang), R5 10k pull-up |
| 26 | PB13 | `LCD_SCL` | OLED I²C **SCL** (bit-bang), R6 10k pull-up |
| 27 | PB14 | `FLASH_CS_N` | W25Q80 CS# (R12 10k pull-up) |
| 28 | PB15 | `PB15` | free — J13 header |
| 29 | PA8 | `PA8` | free — J6 pin 3 + J13 (MCO) |
| 30 | PA9 | `USART1_TX` | **Console TX** → SB1 → CH340E |
| 31 | PA10 | `USART1_RX` | **Console RX** ← SB2 ← CH340E |
| 32 | PA11 | `PA11` | free — J13 header (USB_DM, USB not used) |
| 33 | PA12 | `PA12` | free — J13 header (USB_DP, USB not used) |
| 34 | PA13 | `SWDIO` | SWD — J14 pin 4, J15 pin 4 |
| 35 | VSS_2 | GND | |
| 36 | VDD_2 | +3V3 | |
| 37 | PA14 | `SWCLK` | SWD — J14 pin 3, J15 pin 3 |
| 38 | PA15 | `PA15` | **OLED reset** via R41 0Ω (R40 10k pull-up, C22 100 nF) |
| 39 | PB3 | `USER_BTN1_N` | User button **SW1** (active low), JTDO |
| 40 | PB4 | `USER_BTN3_N` | User button **SW3** (active low), NJTRST |
| 41 | PB5 | `PB5` | free — J13 header |
| 42 | PB6 | `I2C1_SCL` | I²C1 SCL, R3 10k pull-up |
| 43 | PB7 | `I2C1_SDA` | I²C1 SDA, R4 10k pull-up |
| 44 | BOOT0 | `BOOT0` | R7 10k pull-down |
| 45 | PB8 | `LED_DBG_N` | Blue debug LED D3 (**active low**, sinks through R10 1k) |
| 46 | PB9 | `PB9` | free — J13 header |
| 47 | VSS_3 | GND | |
| 48 | VDD_3 | +3V3 | |

### Clocking consequences

- **HSE = 8 MHz crystal (X1)** on PH0/PH1.
- **No 32.768 kHz crystal on the MCU.** PC14/PC15 are routed to the J13 header
  as plain GPIO. The 32.768 kHz crystal X2 on the board belongs to the *external*
  PCF85063A RTC, not to the STM32. So the STM32 LSE is unavailable — use LSI
  (~37 kHz) for the internal RTC/IWDG.
- Max SYSCLK for STM32L1 is 32 MHz. PLL from HSE: 8 MHz × 8 = 64 MHz VCO
  (≤ 96 MHz in range 1), ÷ 2 = **32 MHz**.

---

## 2. Peripherals

### 2.1 Console — USART1 → CH340E → USB Type-C (sheets 3, 6)

`USB-C J1 → D4 (DSK34) → F1 (BSMD0603L-150 polyfuse) → +5V → U4 RT9193-33GB LDO → +3V3`

U6 **CH340E** USB-to-TTL runs off 3V3 and carries the console:

| CH340E | net | goes to |
|--------|-----|---------|
| TXD (8) | `CH340_TXD` | SB2 → `USART1_RX` (PA10) |
| RXD (9) | `CH340_RXD` | SB1 → `USART1_TX` (PA9) |

`SB1`/`SB2` are **3-pad** solder jumpers that route USART1 to *either* the
CH340E *or* the debug headers. Pin 2 is the common (the MCU side):

| Bridge | pin 1 | pin 2 (common) | pin 3 |
|--------|-------|----------------|-------|
| SB1 | `CH340_RXD` | `USART1_TX` (PA9) | `USART1_TX_DBG` → J12 pin 3 / J14 pin 5 |
| SB2 | `CH340_TXD` | `USART1_RX` (PA10) | `USART1_RX_DBG` → J12 pin 4 / J14 pin 6 |

Schematic note on both: *"closed pin1 and pin2 for default"* — i.e. the default
strap is **pin1–pin2**, console over USB-C at **115200 8N1**. Restrap to
pin2–pin3 to take the console out on J12/J14 instead.

PA9/PA10 are **not** present on the J13 pin header, so USART1 cannot be probed
externally without changing these jumpers.

D5 TPUSBLC6-2P6 provides USB ESD protection. `SB3` sits in the VBUS path.

> The CH340E enumerates from VBUS and its own D+/D- lines alone — it does not
> depend on the MCU. If no `1a86:7523` device appears on the host, the fault is
> in the USB path (charge-only cable, VM passthrough, charger instead of PC),
> not in the firmware. A charge-only cable still powers the board, so the OLED,
> buzzer and LEDs will all work normally.

### 2.2 RS-485 — USART2 (sheet 4)

U7 **SP3485EN-L/TR**, +3V3:

| SP3485 | net |
|--------|-----|
| 1 RO | `RS485_RX` → PA3 |
| 2 nRE | `RS485_DIR` (PA1) |
| 3 DE | `RS485_DIR` (PA1) |
| 4 DI | `RS485_TX` ← PA2 |
| 6 A | R25 10R → `RS485_A` |
| 7 B | R24 10R → `RS485_B` |

DE and nRE are **tied together** on PA1: drive it **high to transmit**, low to
receive. R23 120R is the bus termination; D6 SM712 is the TVS.

TX/RX activity LEDs: D8 blue (`LED_TX`, Q3 MMBT3906) and D7 red (`LED_RX`,
Q2 MMBT3906), both PNP driven from the data lines — no MCU pin involved.

Bus brought out on Grove J11, Qwiic J10, and the 2×5 header J9.

### 2.3 I²C1 — RTC + Grove/Qwiic (sheets 6, 7)

PB6 (SCL) / PB7 (SDA), pull-ups R3/R4 10k to +3V3.

**U3 PCF85063ATT/AJ RTC, address 0x51**, X2 32.768 kHz crystal.
`INT#` (pin 3) and `CLKOUT` (pin 7) are **not connected** — no interrupt line to
the MCU, so alarms must be polled.

Battery backup: **BT1 CR1220** → D1 BAT54C dual Schottky → `RTC_VBAT`
(so the RTC runs from whichever of +3V3 / battery is higher).

Also exposed on Grove J8 and Qwiic J7.

### 2.4 SPI1 — NOR flash + expansion (sheets 6, 7)

PA5 SCK (through R2 22R) / PA6 MISO / PA7 MOSI.

**U2 W25Q80DVSNIG TR — 8 Mbit (1 MiB) SPI NOR**, JEDEC ID `EF 40 14`:

| W25Q80 | net |
|--------|-----|
| 1 CS# | `FLASH_CS_N` (PB14), R12 10k pull-up |
| 2 DO/IO1 | `SPI1_MISO` |
| 3 WP#/IO2 | **not connected** |
| 5 DI/IO0 | `SPI1_MOSI` |
| 6 CLK | `SPI1_SCK` |
| 7 RESET#/IO3 | R13 10k pull-up |
| 8 VCC | +3V3 |

Single-lane SPI only (IO2/IO3 unusable ⇒ no quad mode).

**J6 expansion header** — PM200-2-05-S-4.3, 2×5, 2.0 mm.
Silkscreened "SPI Port for extension board – Optional W5500 / nRF24L01+":

| pin | signal | pin | signal |
|----:|--------|----:|--------|
| 1 | GND | 2 | +3V3 |
| 3 | `PA8` | 4 | `PA4` (chip select) |
| 5 | `SPI1_SCK` | 6 | `SPI1_MOSI` |
| 7 | `SPI1_MISO` | 8 | `PB1` |
| 9 | GND | 10 | `NRST` (via R36 100R) |

### 2.5 OLED — SSD1309, bit-banged I²C (sheet 5)

**1.54" 128×64 mono OLED, WEO012864A module, SSD1309 controller**, on FPC
24-pin connector J2 (FPC 0.5-24P HYH2.0).

Wiring mode is fixed by the schematic note *"BS1 to VCC, BS2 to GND : I2C mode"*
plus *"D/C tied to GND (I2C mode)"* → **I²C, address 0x3C**.

| SSD1309 | J2 pin | net |
|---------|-------:|-----|
| BS1 | 6 | `LCD_BS1` → SB10 (I²C mode) |
| BS2 | 7 | `LCD_BS2` → SB11 |
| CS# | 8 | `LCD_CS` → header J3 pin 7 |
| RES# | 9 | `LCD_RES` ← **PA15** via R41 0Ω |
| D/C# | 10 | `LCD_DC` → SB12, tied GND for I²C |
| D0 | 13 | `LCD_SCL` ← **PB13** (I²C clock) |
| D1 | 14 | `LCD_SDA` ← **PB12** (I²C data in) |
| D2 | 15 | tied to D1 through **R34 0Ω** (I²C data out) |
| IREF | 21 | R35 910k → external Iref (do *not* enable internal Iref) |
| VCOMH | 22 | C32 4.7 µF |
| VCC | 23 | `+12V5_LCD` panel supply |

> **PB12/PB13 have no I²C alternate function on the STM32L151.** This bus must
> be bit-banged (Zephyr `gpio-i2c`). R5/R6 (10k to +3V3, sheet 7) are the bus
> pull-ups.

**Panel high-voltage supply:** U8 TPS61040DBVR boost, L2 10 µH, D9 DSK34,
feedback R32 121k / R33 13k3 → **+12.5 V** (`+12V5_LCD`) from +5V.

`J3` is a 1×7 2.54 mm header repeating the OLED pinout
(7 `LCD_CS`, 6 `LCD_DC`, 5 `LCD_RES`, 4 `LCD_SDA`, 3 `LCD_SCL`, 2/1 power).

### 2.6 LEDs, buzzer, buttons (sheet 7)

| Part | Net | Behaviour |
|------|-----|-----------|
| D2 red "LED Power" | — | +3V3 → R9 1k → D2 → GND, **always on**, no MCU control |
| D3 blue "LED Debug" | `LED_DBG_N` | +3V3 → R10 1k → D3 → **PB8**, **active LOW** |
| L1 MLT-8530 buzzer | `BUZZER_PWM` | +5V → buzzer → Q1 MMBT3904 collector; base from **PB0** via R11 1k (R14 10k pull-down) — **active HIGH**, PB0 = TIM3_CH3 |

Buttons S1–S4, all TS-1102S-B-G-B, each with a 10k pull-up to +3V3, a 100R
series resistor and a 100 nF debounce cap — **all active LOW**:

| Switch | Net | Pin |
|--------|-----|-----|
| S1 | `USER_BTN1_N` | PB3 (R15/R18/C12) |
| S2 | `USER_BTN2_N` | PC13 (R16/R19/C13) |
| S3 | `USER_BTN3_N` | PB4 (R17/R20/C14) |
| S4 | `NRST` | MCU reset (C15) |

> PB3/PB4/PA15 are JTDO/NJTRST/JTDI at reset. Only PA13/PA14 are needed for
> SWD, so all three are free to use as GPIO.

### 2.7 Debug and breakout headers (sheet 6)

- **J14** — 1×7, 2.54 mm debug header: 3 `SWCLK`, 4 `SWDIO`,
  5 `USART1_TX_DBG`, 6 `USART1_RX_DBG` (+ power/GND).
- **J15** — SH1.0 4-pin "SWD Program" (Qwiic-style): 3 `SWCLK`, 4 `SWDIO`.
- **J12** — SH1.0 4-pin "UART Debug": 2 `USART1_VCC`, 3 `USART1_TX_DBG`,
  4 `USART1_RX_DBG`.
- **J13** — 2×6, 2.54 mm "MCU Pinout": `PB15`, `PB9`, `PB5`, `PC15`, `PC14`,
  `PA0`, `PA11`, `PA12`, `PA15`, +3V3, GND.

### 2.8 Grove / Qwiic connectors (sheet 6)

Grove = HY-4AW_No3D 2.0 mm (Seeed). Qwiic = SM04B-SRSS-TB(LF)(SN) 1.0 mm
(SparkFun). Each pair shares one bus; pin 1 = GND, pin 2 = VCC, pins 3/4 = data.

| Bus | Qwiic | Grove | VCC select | Pin 3 | Pin 4 |
|-----|-------|-------|------------|-------|-------|
| UART3 | J4 | J5 | `SB4` (default 3V3) | `USART3_TX` | `USART3_RX` |
| I²C | J7 | J8 | `SB5` (default 3V3) | `I2C1_SDA` | `I2C1_SCL` |
| RS-485 | J10 | J11 | `SB6` (**off** by default) | `RS485_B` | `RS485_A` |

`SB4`/`SB5`/`SB6` are 3-pad solder jumpers choosing +5V or +3V3 for the
connector VCC; both UART3 and I²C ship strapped to **+3V3**.

---

## 3. Power tree (sheet 3)

```
USB Type-C J1 ──VBUS──> D4 DSK34 ──> F1 polyfuse (1.5A) ──> +5V
                                                             │
                                    ┌────────────────────────┼──────────────┐
                                    │                        │              │
                              U4 RT9193-33GB           U8 TPS61040     L1 buzzer
                              5V → 3V3 LDO             5V → 12.5V
                                    │                        │
                                  +3V3                  +12V5_LCD
                        (MCU, RS485, CH340E,           (OLED panel VCC)
                         RTC, NOR flash, OLED logic)
```

- CC1/CC2 pulled down by R21/R22 5.1k (USB-C sink, 5 V / up to 1 A).
- Decoupling: C3 1 µF + C4/C5/C6 100 nF on VDD1–3, C7 100 nF + C8 1 µF on VDDA.
- 3.2 mm non-plated mounting holes MH1/MH2 for M3 screws.

---

## 4. Reference designator index

| Ref | Part | Function |
|-----|------|----------|
| U1 | STM32L151CBT6A | MCU |
| U2 | W25Q80DVSNIG TR | 8 Mbit SPI NOR flash |
| U3 | PCF85063ATT/AJ | I²C RTC, 0x51 |
| U4 | RT9193-33GB | 5 V → 3.3 V LDO |
| U6 | CH340E | USB ↔ TTL console bridge |
| U7 | SP3485EN-L/TR | RS-485 transceiver |
| U8 | TPS61040DBVR | 5 V → 12.5 V boost for OLED |
| X1 | 8 MHz crystal | STM32 HSE |
| X2 | 32.768 kHz crystal | PCF85063A RTC (not the STM32) |
| BT1 | CR1220 | RTC backup battery |
| D1 | BAT54C | RTC battery/3V3 OR-ing |
| D2 / D3 | LED red / blue | Power (always on) / Debug (PB8) |
| D5 | TPUSBLC6-2P6 | USB ESD protection |
| D6 | SM712 | RS-485 TVS |
| D7 / D8 | LED red / blue | RS-485 RX / TX activity |
| L1 | MLT-8530 | Buzzer |
| Q1 | MMBT3904 | Buzzer NPN driver |
| Q2 / Q3 | MMBT3906 | RS-485 LED PNP drivers |
| S1–S3 | TS-1102S-B-G-B | User buttons |
| S4 | TS-1102S-B-G-B | MCU reset |
| J1 | USB Type-C 16P | Power + console |
| J2 | FPC 0.5-24P | OLED panel |
| J3 | 1×7 2.54 mm | OLED pinout breakout |
| J4 / J5 | Qwiic / Grove | UART3 |
| J6 | PM200-2-05-S-4.3 | SPI expansion (2×5) |
| J7 / J8 | Qwiic / Grove | I²C |
| J9 | PM200-2-05-S-4.3 | RS-485 (2×5) |
| J10 / J11 | Qwiic / Grove | RS-485 |
| J12 | SH1.0 4-pin | UART debug |
| J13 | 2×6 2.54 mm | MCU pinout breakout |
| J14 | 1×7 2.54 mm | Debug header (SWD + UART) |
| J15 | SH1.0 4-pin | SWD program |

### Solder jumpers

| Ref | Selects | Default |
|-----|---------|---------|
| SB1 / SB2 | USART1 → CH340E (pin1-2) or J12/J14 (pin2-3) | **pin1–pin2** (console on USB-C) |
| SB3 | USB VBUS path | on |
| SB4 | UART3 connector VCC | +3V3 |
| SB5 | I²C connector VCC | +3V3 |
| SB6 | RS-485 connector VCC | **off** |
| SB7 | UART console routing | off |
| SB8 / SB9 | SWD routing | off |
| SB10 / SB11 / SB12 | OLED BS1 / BS2 / D-C | I²C mode |

---

## 5. Notes and gotchas for firmware

1. **No LSE.** Do not enable `clk_lse`; the STM32 RTC must run off LSI, or use
   the external PCF85063A over I²C1.
2. **OLED needs a bit-banged I²C bus** (PB12/PB13 have no I²C AF). Its reset is
   on PA15, active low.
3. **RS-485 direction is manual** — STM32L1 USARTs have no hardware DE. Drive
   PA1 high before transmitting and low afterwards (Zephyr's
   `zephyr,modbus-serial` `de-gpios` handles this for Modbus).
4. **The debug LED is active low**; the red power LED is not software
   controllable.
5. **Buzzer is active high** through an NPN, on TIM3_CH3 (PB0). MLT-8530
   resonates near 2.7 kHz.
6. **SPI NOR is single-lane only** — IO2/IO3 are pulled up, not routed.
7. **The RTC has no interrupt line**; poll it.
8. **PB2 is BOOT1** with a 10k pull-down — leave it alone.
9. **USB (PA11/PA12) is not wired to the USB-C connector.** The connector goes
   to the CH340E; the STM32's USB peripheral is unusable on this board.
10. Buttons are active low with external pull-ups; internal pull-ups are
    harmless but redundant.
