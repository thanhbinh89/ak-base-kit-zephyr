/*
 * Copyright (c) 2026 AK Embedded Base Kit Zephyr port
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Board bring-up firmware for the AK Embedded Base Kit v2.3 (STM32L151CBT6A).
 *
 * What it does:
 *   - blinks the blue debug LED (PB8)
 *   - prints a hardware inventory over the USB-C console (USART1, 115200 8N1)
 *   - reads the JEDEC ID of the on-board W25Q80 NOR flash
 *   - reads the date/time from the PCF85063A RTC
 *   - draws a banner on the 1.54" SSD1309 OLED
 *   - beeps the buzzer and toggles the LED when a user button is pressed
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/input/input.h>
#include <zephyr/display/cfb.h>
#include <zephyr/logging/log.h>
#include <zephyr/version.h>

LOG_MODULE_REGISTER(ak_base_kit, LOG_LEVEL_INF);

#define BLINK_PERIOD_MS 500U
#define BEEP_FREQ_HZ    2700U /* MLT-8530 resonant frequency */
#define BEEP_MS         60U

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct pwm_dt_spec buzzer = PWM_DT_SPEC_GET(DT_ALIAS(buzzer));
static const struct device *const nor = DEVICE_DT_GET(DT_ALIAS(spi_flash0));
static const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));
static const struct device *const display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

static void beep(void)
{
	if (!pwm_is_ready_dt(&buzzer)) {
		return;
	}

	(void)pwm_set_dt(&buzzer, PWM_HZ(BEEP_FREQ_HZ), PWM_HZ(BEEP_FREQ_HZ) / 2U);
	k_msleep(BEEP_MS);
	(void)pwm_set_dt(&buzzer, PWM_HZ(BEEP_FREQ_HZ), 0);
}

static void button_handler(struct input_event *evt, void *user_data)
{
	ARG_UNUSED(user_data);

	if (evt->type != INPUT_EV_KEY || evt->value == 0) {
		return;
	}

	LOG_INF("button 0x%02x pressed", evt->code);
	beep();
}
INPUT_CALLBACK_DEFINE(NULL, button_handler, NULL);

static void report_nor_flash(void)
{
	uint8_t jedec_id[3];
	int rc;

	if (!device_is_ready(nor)) {
		LOG_WRN("NOR flash not ready");
		return;
	}

	rc = flash_read_jedec_id(nor, jedec_id);
	if (rc != 0) {
		LOG_WRN("NOR flash JEDEC read failed (%d)", rc);
		return;
	}

	LOG_INF("NOR flash W25Q80: JEDEC ID %02x %02x %02x", jedec_id[0], jedec_id[1],
		jedec_id[2]);
}

static void report_rtc(void)
{
	struct rtc_time now;
	int rc;

	if (!device_is_ready(rtc)) {
		LOG_WRN("RTC not ready");
		return;
	}

	rc = rtc_get_time(rtc, &now);
	if (rc == -ENODATA) {
		LOG_WRN("RTC time not set yet");
		return;
	}
	if (rc != 0) {
		LOG_WRN("RTC read failed (%d)", rc);
		return;
	}

	LOG_INF("RTC PCF85063A: %04d-%02d-%02d %02d:%02d:%02d", now.tm_year + 1900,
		now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);
}

static void show_banner(void)
{
	int rc;

	if (!device_is_ready(display)) {
		LOG_WRN("OLED not ready");
		return;
	}

	rc = display_blanking_off(display);
	if (rc != 0) {
		LOG_WRN("display_blanking_off failed (%d)", rc);
		return;
	}

	rc = cfb_framebuffer_init(display);
	if (rc != 0) {
		LOG_WRN("cfb init failed (%d)", rc);
		return;
	}

	cfb_framebuffer_clear(display, false);
	cfb_print(display, "AK Base Kit", 0, 0);
	cfb_print(display, "STM32L151", 0, 16);
	cfb_print(display, "Zephyr " KERNEL_VERSION_STRING, 0, 32);
	cfb_framebuffer_finalize(display);
}

int main(void)
{
	int rc;

	LOG_INF("AK Embedded Base Kit v2.3 -- %s", CONFIG_BOARD_TARGET);
	LOG_INF("SYSCLK %u Hz", sys_clock_hw_cycles_per_sec());

	if (!gpio_is_ready_dt(&led)) {
		LOG_ERR("debug LED not ready");
		return -ENODEV;
	}

	rc = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (rc != 0) {
		LOG_ERR("LED configure failed (%d)", rc);
		return rc;
	}

	report_nor_flash();
	report_rtc();
	show_banner();
	beep();

	LOG_INF("bring-up done, blinking");

	while (true) {
		gpio_pin_toggle_dt(&led);
		k_msleep(BLINK_PERIOD_MS);
	}

	return 0;
}
