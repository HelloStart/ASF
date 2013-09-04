/**
 * \file
 *
 * \brief Management of C42364A LCD Glass component.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "compiler.h"
#include "board.h"
#include "c42364a_segmap.h"
#include "c42364a.h"
#include "conf_c42364a_lcdca.h"
#include "lcdca.h"

status_code_t c42364a_init(void)
{
	struct lcdca_config lcdca_cfg;

	lcdca_clk_init();
	lcdca_cfg.port_mask = C42364A_PORT_MASK;
	lcdca_cfg.x_bias = CONF_C42364A_X_BIAS;
	lcdca_cfg.lp_wave = true;
	lcdca_cfg.duty_type = C42364A_LCD_DUTY;
	lcdca_cfg.lcd_pres = CONF_C42364A_PRES;
	lcdca_cfg.lcd_clkdiv = CONF_C42364A_CLKDIV;
	lcdca_cfg.fc0 = CONF_C42364A_FC0;
	lcdca_cfg.fc1 = CONF_C42364A_FC1;
	lcdca_cfg.fc2 = CONF_C42364A_FC2;
	lcdca_cfg.contrast = CONF_C42364A_CONTRAST;
	lcdca_set_config(&lcdca_cfg);
	lcdca_enable();
	lcdca_enable_timer(LCDCA_TIMER_FC0);
	lcdca_enable_timer(LCDCA_TIMER_FC1);
	lcdca_enable_timer(LCDCA_TIMER_FC2);

	return STATUS_OK;
}

void c42364a_write_alphanum_packet(const uint8_t *data)
{
	lcdca_write_packet(LCDCA_TDG_14SEG4COM, C42364A_FIRST_14SEG_4C, data,
			C42364A_WIDTH_14SEG_4C, LCDCA_CMCFG_DREV_LEFT);
}

void c42364a_write_num_packet(const uint8_t *data)
{
	lcdca_write_packet(LCDCA_TDG_7SEG4COM, C42364A_FIRST_7SEG_4C, data,
			C42364A_WIDTH_7SEG_4C, LCDCA_CMCFG_DREV_LEFT);
}

void c42364a_show_all(void)
{
	lcdca_blink_disable();
	lcdca_circular_shift_disable();
	lcdca_automated_char_stop();
	lcdca_set_display_memory();
}

void c42364a_clear_all(void)
{
	lcdca_blink_disable();
	lcdca_circular_shift_disable();
	lcdca_automated_char_stop();
	lcdca_clear_display_memory();
}

void c42364a_show_text(const uint8_t *data)
{
	lcdca_write_packet(LCDCA_TDG_14SEG4COM, C42364A_FIRST_14SEG_4C, data,
			C42364A_WIDTH_14SEG_4C, LCDCA_CMCFG_DREV_LEFT);
}

void c42364a_clear_text(void)
{
	uint8_t clear_data[7] = {0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F};

	lcdca_write_packet(LCDCA_TDG_14SEG4COM, C42364A_FIRST_14SEG_4C,
			clear_data, C42364A_WIDTH_14SEG_4C, LCDCA_CMCFG_DREV_LEFT);
}

void c42364a_show_icon(uint8_t icon_com, uint8_t icon_seg)
{
	if (icon_com < 2) {
		lcdca_clear_blink_pixel(icon_com, icon_seg);
	}

	lcdca_set_pixel(icon_com, icon_seg);
}

void c42364a_clear_icon(uint8_t icon_com, uint8_t icon_seg)
{
	lcdca_clear_pixel(icon_com, icon_seg);
}

void c42364a_blink_icon_start(uint8_t icon_com, uint8_t icon_seg)
{
	struct lcdca_blink_config blink_cfg;

	blink_cfg.lcd_blink_timer = CONF_C42364A_BLINK_TIMER;
	blink_cfg.lcd_blink_mode = LCDCA_BLINK_SELECTED;
	lcdca_blink_set_config(&blink_cfg);

	/* Be sure to blink only pixel that can do that by hardware. */
	if (icon_seg < 2) {
		lcdca_set_blink_pixel(icon_com, icon_seg);
	}

	lcdca_blink_enable();
	lcdca_set_pixel(icon_com, icon_seg);
}

void c42364a_blink_icon_stop(uint8_t icon_com, uint8_t icon_seg)
{
	lcdca_clear_blink_pixel(icon_com, icon_seg);
}

void c42364a_blink_screen(void)
{
	struct lcdca_blink_config blink_cfg;

	lcdca_clear_blink_all_pixel();

	blink_cfg.lcd_blink_timer = CONF_C42364A_BLINK_TIMER;
	blink_cfg.lcd_blink_mode = LCDCA_BLINK_FULL;
	lcdca_blink_set_config(&blink_cfg);

	lcdca_blink_enable();
}

void c42364a_blink_disable(void)
{
	lcdca_blink_disable();
}

void c42364a_set_contrast(int8_t contrast)
{
	lcdca_set_contrast(contrast);
}

void c42364a_circular_animation_start(uint8_t csr_dir,
		uint8_t size, uint8_t data)
{
	struct lcdca_circular_shift_config cs_cfg;

	cs_cfg.lcd_csr_timer = CONF_C42364A_CIRCULAR_ANIMATION_TIMER;
	cs_cfg.lcd_csr_dir = csr_dir;
	cs_cfg.size = size;
	cs_cfg.data = data;
	lcdca_circular_shift_set_config(&cs_cfg);

	lcdca_circular_shift_enable();
}

void c42364a_circular_animation_stop(void)
{
	lcdca_circular_shift_disable();
}

void c42364a_show_battery(enum c42364a_battery_value val)
{
	lcdca_clear_pixel(C42364A_ICON_BAT_LEVEL_1);
	lcdca_clear_pixel(C42364A_ICON_BAT_LEVEL_2);
	lcdca_clear_pixel(C42364A_ICON_BAT_LEVEL_3);
	lcdca_set_pixel(C42364A_ICON_BAT);
	if (val > 2) {
		lcdca_set_pixel(C42364A_ICON_BAT_LEVEL_3);
	}
	if (val > 1) {
		lcdca_set_pixel(C42364A_ICON_BAT_LEVEL_2);
	}
	if (val > 0) {
		lcdca_set_pixel(C42364A_ICON_BAT_LEVEL_1);
	}
}

void c42364a_show_numeric_dec(int32_t value)
{
	uint8_t lcd_num[5];

	Assert(value > -20000);
	Assert(value < 20000);

	if(value < 0) {
		lcdca_set_pixel(C42364A_ICON_MINUS);
	} else {
		lcdca_clear_pixel(C42364A_ICON_MINUS);
	}

	value = Abs(value);

	if(value > 9999) {
		value -= 10000;
		lcdca_set_pixel(C42364A_ICON_MINUS_SEG1);
		lcdca_set_pixel(C42364A_ICON_MINUS_SEG2);
	} else {
		lcdca_clear_pixel(C42364A_ICON_MINUS_SEG1);
		lcdca_clear_pixel(C42364A_ICON_MINUS_SEG2);
	}

	sprintf((char*)lcd_num, "%4d", (int)value);

	c42364a_write_num_packet((uint8_t const*)&lcd_num);
}

void c42364a_clear_numeric_dec(void)
{
	uint8_t clear_data[4] = {0x0F, 0x0F, 0x0F, 0x0F};

	lcdca_clear_pixel(C42364A_ICON_MINUS);
	lcdca_clear_pixel(C42364A_ICON_MINUS_SEG1);
	lcdca_clear_pixel(C42364A_ICON_MINUS_SEG2);
	lcdca_write_packet(LCDCA_TDG_7SEG4COM, C42364A_FIRST_7SEG_4C,
			clear_data, C42364A_WIDTH_7SEG_4C, LCDCA_CMCFG_DREV_LEFT);
}

void c42364a_text_scrolling_start(const uint8_t *data, uint32_t length)
{
	/* Settings of automated display */
	struct lcdca_automated_char_config automated_char_config;

	automated_char_config.automated_mode = LCDCA_AUTOMATED_MODE_SCROLLING;
	automated_char_config.automated_timer =
			CONF_C42364A_TEXT_SCROLLING_TIMER;
	automated_char_config.lcd_tdg = LCDCA_TDG_14SEG4COM;
	automated_char_config.stseg = C42364A_FIRST_14SEG_4C;
	automated_char_config.dign = C42364A_WIDTH_14SEG_4C;
	/* STEPS = string length - DIGN + 1 */
	automated_char_config.steps = length - C42364A_WIDTH_14SEG_4C + 1;
	automated_char_config.dir_reverse = LCDCA_AUTOMATED_DIR_REVERSE;
	lcdca_automated_char_set_config(&automated_char_config);
	lcdca_automated_char_start(data, length);
}

void c42364a_text_scrolling_stop(void)
{
	lcdca_automated_char_stop();
}
