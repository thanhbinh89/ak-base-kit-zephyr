.. _ak_base_kit:

AK Embedded Base Kit v2.3
#########################

Overview
********

The AK Embedded Base Kit v2.3 is a 60 x 60 mm STM32L151CBT6A development board
with an on-board USB-C console bridge, a 1.54" OLED, RS-485, SPI NOR flash, a
battery-backed RTC and Grove/Qwiic expansion.

Hardware
********

- STM32L151CBT6A (Cortex-M3, 32 MHz, 128 KiB flash, 32 KiB SRAM, 4 KiB EEPROM)
- 8 MHz HSE crystal; **no 32.768 kHz crystal** on the MCU
- 1.54" 128x64 OLED, SSD1309, I2C address 0x3C on a bit-banged bus (PB12/PB13)
- W25Q80DVSNIG 8 Mbit SPI NOR flash on SPI1 (CS PB14)
- PCF85063A RTC at 0x51 on I2C1, backed by a CR1220 cell
- SP3485 RS-485 transceiver on USART2, direction pin PA1
- CH340E USB-to-UART console bridge on USART1, over USB Type-C
- 3 user buttons, 1 reset button, blue debug LED, magnetic buzzer
- Grove (2.0 mm) and Qwiic (1.0 mm) connectors for UART3, I2C and RS-485
- 2x5 SPI expansion header (W5500 / nRF24L01+)

A full net-by-net description of the board lives in ``docs/SCHEMATIC.md`` at
the root of this project.

Supported Features
******************

.. zephyr:board-supported-hw::

Default configuration
=====================

+-----------------+---------------------------+--------------------------------+
| Interface       | Pins                      | Notes                          |
+=================+===========================+================================+
| ``usart1``      | PA9 / PA10                | Console + shell, 115200 8N1    |
+-----------------+---------------------------+--------------------------------+
| ``usart2``      | PA2 / PA3                 | RS-485, DE/nRE on PA1          |
+-----------------+---------------------------+--------------------------------+
| ``usart3``      | PB10 / PB11               | Grove / Qwiic UART             |
+-----------------+---------------------------+--------------------------------+
| ``i2c1``        | PB6 / PB7                 | RTC + Grove / Qwiic I2C        |
+-----------------+---------------------------+--------------------------------+
| ``i2c_oled``    | PB13 / PB12               | Bit-banged bus for the OLED    |
+-----------------+---------------------------+--------------------------------+
| ``spi1``        | PA5 / PA6 / PA7           | NOR flash (PB14), J6 (PA4)     |
+-----------------+---------------------------+--------------------------------+
| ``pwm3``        | PB0 (TIM3_CH3)            | Buzzer                         |
+-----------------+---------------------------+--------------------------------+

Aliases: ``led0`` (PB8, active low), ``sw0``/``sw1``/``sw2`` (PB3/PC13/PB4,
active low), ``buzzer``, ``rtc``, ``spi-flash0``, ``eeprom-0``, ``watchdog0``.

Programming and Debugging
*************************

Connect an ST-LINK (or any SWD probe) to the J14 header (pin 3 SWCLK,
pin 4 SWDIO) or the J15 SH1.0 connector.

.. code-block:: console

   west build -b ak_base_kit app
   west flash

The console appears as a CH340 serial port over USB-C:

.. code-block:: console

   west build -t run   # or: minicom -D /dev/ttyUSB0 -b 115200

References
**********

- `STM32L151CB datasheet <https://www.st.com/resource/en/datasheet/stm32l151cb.pdf>`_
- `RM0038 STM32L1 reference manual <https://www.st.com/resource/en/reference_manual/rm0038-stm32l100xx-stm32l151xx-stm32l152xx-and-stm32l162xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf>`_
