/**
 * \file
 *
 * \brief API driver for ILI9488 TFT display component.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

/**
 * \defgroup ili9488_display_group Display - ILI9488 Controller
 *
 * Low-level driver for the ILI9488 LCD controller. This driver provides access to the main
 * features of the ILI9488 controller.
 *
 * \{
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "ili9488.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "pio.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
 extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/* Pixel cache used to speed up communication */
#define LCD_DATA_CACHE_SIZE ILI9488_LCD_WIDTH
static ili9488_color_t g_ul_pixel_cache[LCD_DATA_CACHE_SIZE];

/* Global variable describing the font size used by the driver */
const struct ili9488_font gfont = {10, 14};
/**
 * Character set table for font 10x14
 * Coding format:
 * Char height is 14 bits, which is coded using 2 bytes per column (2 unused bits).
 * Char width is 10 bits.
 */
const uint8_t p_uc_charset10x14[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCC,
	0xFF, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0C, 0xC0, 0x0C, 0xC0, 0xFF, 0xFC, 0xFF, 0xFC, 0x0C, 0xC0,
	0x0C, 0xC0, 0xFF, 0xFC, 0xFF, 0xFC, 0x0C, 0xC0, 0x0C, 0xC0,
	0x0C, 0x60, 0x1E, 0x70, 0x3F, 0x30, 0x33, 0x30, 0xFF, 0xFC,
	0xFF, 0xFC, 0x33, 0x30, 0x33, 0xF0, 0x39, 0xE0, 0x18, 0xC0,
	0x60, 0x00, 0xF0, 0x0C, 0xF0, 0x3C, 0x60, 0xF0, 0x03, 0xC0,
	0x0F, 0x00, 0x3C, 0x18, 0xF0, 0x3C, 0xC0, 0x3C, 0x00, 0x18,
	0x3C, 0xF0, 0x7F, 0xF8, 0xC3, 0x1C, 0xC7, 0x8C, 0xCF, 0xCC,
	0xDC, 0xEC, 0x78, 0x78, 0x30, 0x30, 0x00, 0xFC, 0x00, 0xCC,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0xEC, 0x00,
	0xF8, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x3F, 0xF0, 0x78, 0x78,
	0x60, 0x18, 0xC0, 0x0C, 0xC0, 0x0C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0x60, 0x18,
	0x78, 0x78, 0x3F, 0xF0, 0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00,
	0x0C, 0x60, 0x0E, 0xE0, 0x07, 0xC0, 0x03, 0x80, 0x3F, 0xF8,
	0x3F, 0xF8, 0x03, 0x80, 0x07, 0xC0, 0x0E, 0xE0, 0x0C, 0x60,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x3F, 0xF0,
	0x3F, 0xF0, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x00, 0x44, 0x00, 0xEC, 0x00, 0xF8, 0x00, 0x70, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x00, 0x18, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0C, 0x00, 0x3C, 0x00, 0xF0, 0x03, 0xC0,
	0x0F, 0x00, 0x3C, 0x00, 0xF0, 0x00, 0xC0, 0x00, 0x00, 0x00,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0xFC, 0xC1, 0xCC, 0xC3, 0x8C,
	0xC7, 0x0C, 0xCE, 0x0C, 0xFC, 0x1C, 0x7F, 0xF8, 0x3F, 0xF0,
	0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x70, 0x0C, 0xFF, 0xFC,
	0xFF, 0xFC, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
	0x30, 0x0C, 0x70, 0x1C, 0xE0, 0x3C, 0xC0, 0x7C, 0xC0, 0xEC,
	0xC1, 0xCC, 0xC3, 0x8C, 0xE7, 0x0C, 0x7E, 0x0C, 0x3C, 0x0C,
	0x30, 0x30, 0x70, 0x38, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE3, 0x1C, 0x7F, 0xF8, 0x3C, 0xF0,
	0x03, 0xC0, 0x07, 0xC0, 0x0E, 0xC0, 0x1C, 0xC0, 0x38, 0xC0,
	0x70, 0xC0, 0xFF, 0xFC, 0xFF, 0xFC, 0x00, 0xC0, 0x00, 0xC0,
	0xFC, 0x30, 0xFC, 0x38, 0xCC, 0x1C, 0xCC, 0x0C, 0xCC, 0x0C,
	0xCC, 0x0C, 0xCC, 0x0C, 0xCE, 0x1C, 0xC7, 0xF8, 0xC3, 0xF0,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE3, 0x1C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE3, 0x9C, 0x71, 0xF8, 0x30, 0xF0,
	0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC3, 0xFC,
	0xC7, 0xFC, 0xCE, 0x00, 0xDC, 0x00, 0xF8, 0x00, 0xF0, 0x00,
	0x3C, 0xF0, 0x7F, 0xF8, 0xE7, 0x9C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE7, 0x9C, 0x7F, 0xF8, 0x3C, 0xF0,
	0x3C, 0x00, 0x7E, 0x00, 0xE7, 0x0C, 0xC3, 0x0C, 0xC3, 0x1C,
	0xC3, 0x38, 0xC3, 0x70, 0xE7, 0xE0, 0x7F, 0xC0, 0x3F, 0x80,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x60, 0x3C, 0xF0,
	0x3C, 0xF0, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x44, 0x3C, 0xEC,
	0x3C, 0xF8, 0x18, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x03, 0x00, 0x07, 0x80, 0x0F, 0xC0, 0x1C, 0xE0,
	0x38, 0x70, 0x70, 0x38, 0xE0, 0x1C, 0xC0, 0x0C, 0x00, 0x00,
	0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0,
	0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0,
	0x00, 0x00, 0xC0, 0x0C, 0xE0, 0x1C, 0x70, 0x38, 0x38, 0x70,
	0x1C, 0xE0, 0x0F, 0xC0, 0x07, 0x80, 0x03, 0x00, 0x00, 0x00,
	0x30, 0x00, 0x70, 0x00, 0xE0, 0x00, 0xC0, 0x00, 0xC1, 0xEC,
	0xC3, 0xEC, 0xC3, 0x00, 0xE6, 0x00, 0x7E, 0x00, 0x3C, 0x00,
	0x30, 0xF0, 0x71, 0xF8, 0xE3, 0x9C, 0xC3, 0x0C, 0xC3, 0xFC,
	0xC3, 0xFC, 0xC0, 0x0C, 0xE0, 0x1C, 0x7F, 0xF8, 0x3F, 0xF0,
	0x3F, 0xFC, 0x7F, 0xFC, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
	0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0xC0, 0x7F, 0xFC, 0x3F, 0xFC,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE7, 0x9C, 0x7F, 0xF8, 0x3C, 0xF0,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC0, 0x0C, 0xC0, 0x0C, 0xE0, 0x1C, 0x70, 0x38, 0x30, 0x30,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC0, 0x0C, 0xC0, 0x0C, 0xE0, 0x1C, 0x7F, 0xF8, 0x3F, 0xF0,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00,
	0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC0, 0x00, 0xC0, 0x00,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xE3, 0x1C, 0x73, 0xF8, 0x33, 0xF0,
	0xFF, 0xFC, 0xFF, 0xFC, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0xFF, 0xFC, 0xFF, 0xFC,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0xFF, 0xFC,
	0xFF, 0xFC, 0xC0, 0x0C, 0xC0, 0x0C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x30, 0x00, 0x38, 0xC0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC0, 0x1C, 0xFF, 0xF8, 0xFF, 0xF0, 0xC0, 0x00, 0xC0, 0x00,
	0xFF, 0xFC, 0xFF, 0xFC, 0x07, 0x80, 0x07, 0x80, 0x0F, 0xC0,
	0x1C, 0xE0, 0x38, 0x70, 0x70, 0x38, 0xE0, 0x1C, 0xC0, 0x0C,
	0xFF, 0xFC, 0xFF, 0xFC, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
	0xFF, 0xFC, 0xFF, 0xFC, 0x70, 0x00, 0x38, 0x00, 0x1F, 0x00,
	0x1F, 0x00, 0x38, 0x00, 0x70, 0x00, 0xFF, 0xFC, 0xFF, 0xFC,
	0xFF, 0xFC, 0xFF, 0xFC, 0x1C, 0x00, 0x0E, 0x00, 0x07, 0x00,
	0x03, 0x80, 0x01, 0xC0, 0x00, 0xE0, 0xFF, 0xFC, 0xFF, 0xFC,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xC0, 0x0C, 0xC0, 0x0C, 0xE0, 0x1C, 0x7F, 0xF8, 0x3F, 0xF0,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00,
	0xC3, 0x00, 0xC3, 0x00, 0xE7, 0x00, 0x7E, 0x00, 0x3C, 0x00,
	0x3F, 0xF0, 0x7F, 0xF8, 0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0xCC,
	0xC0, 0xEC, 0xC0, 0x7C, 0xE0, 0x38, 0x7F, 0xFC, 0x3F, 0xEC,
	0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0x00, 0xC3, 0x80, 0xC3, 0x80,
	0xC3, 0xC0, 0xC3, 0xC0, 0xE7, 0x70, 0x7E, 0x3C, 0x3C, 0x1C,
	0x3C, 0x18, 0x7E, 0x1C, 0xE7, 0x0C, 0xC3, 0x0C, 0xC3, 0x0C,
	0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x9C, 0xE1, 0xF8, 0x60, 0xF0,
	0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xFF, 0xFC,
	0xFF, 0xFC, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
	0xFF, 0xF0, 0xFF, 0xF8, 0x00, 0x1C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x1C, 0xFF, 0xF8, 0xFF, 0xF0,
	0xFF, 0xC0, 0xFF, 0xE0, 0x00, 0x70, 0x00, 0x38, 0x00, 0x1C,
	0x00, 0x1C, 0x00, 0x38, 0x00, 0x70, 0xFF, 0xE0, 0xFF, 0xC0,
	0xFF, 0xF0, 0xFF, 0xF8, 0x00, 0x1C, 0x00, 0x3C, 0x00, 0xF8,
	0x00, 0xF8, 0x00, 0x3C, 0x00, 0x1C, 0xFF, 0xF8, 0xFF, 0xF0,
	0xF0, 0x3C, 0xF8, 0x7C, 0x1C, 0xE0, 0x0F, 0xC0, 0x07, 0x80,
	0x07, 0x80, 0x0F, 0xC0, 0x1C, 0xE0, 0xF8, 0x7C, 0xF0, 0x3C,
	0xFC, 0x00, 0xFE, 0x00, 0x07, 0x00, 0x03, 0x80, 0x01, 0xFC,
	0x01, 0xFC, 0x03, 0x80, 0x07, 0x00, 0xFE, 0x00, 0xFC, 0x00,
	0xC0, 0x3C, 0xC0, 0x7C, 0xC0, 0xEC, 0xC1, 0xCC, 0xC3, 0x8C,
	0xC7, 0x0C, 0xCE, 0x0C, 0xDC, 0x0C, 0xF8, 0x0C, 0xF0, 0x0C,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0xFF, 0xFC, 0xC0, 0x0C,
	0xC0, 0x0C, 0xC0, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x30, 0x00, 0x30, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x30, 0x00, 0x30,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C,
	0xFF, 0xFC, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0C, 0x00, 0x1C, 0x00, 0x38, 0x00, 0x70, 0x00, 0xE0, 0x00,
	0xE0, 0x00, 0x70, 0x00, 0x38, 0x00, 0x1C, 0x00, 0x0C, 0x00,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xE0, 0x00, 0x70, 0x00,
	0x38, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x30, 0x06, 0x78, 0x0E, 0xFC, 0x0C, 0xCC, 0x0C, 0xCC,
	0x0C, 0xCC, 0x0C, 0xCC, 0x0E, 0xCC, 0x07, 0xFC, 0x03, 0xF8,
	0xFF, 0xFC, 0xFF, 0xFC, 0x03, 0x0C, 0x03, 0x0C, 0x03, 0x0C,
	0x03, 0x0C, 0x03, 0x0C, 0x03, 0x9C, 0x01, 0xF8, 0x00, 0xF0,
	0x03, 0xF0, 0x07, 0xF8, 0x0E, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C,
	0x0C, 0x0C, 0x0C, 0x0C, 0x0E, 0x1C, 0x07, 0x38, 0x03, 0x30,
	0x00, 0xF0, 0x01, 0xF8, 0x03, 0x9C, 0x03, 0x0C, 0x03, 0x0C,
	0x03, 0x0C, 0x03, 0x0C, 0x03, 0x0C, 0xFF, 0xFC, 0xFF, 0xFC,
	0x03, 0xF0, 0x07, 0xF8, 0x0E, 0xDC, 0x0C, 0xCC, 0x0C, 0xCC,
	0x0C, 0xCC, 0x0C, 0xCC, 0x0E, 0xDC, 0x07, 0xD8, 0x03, 0x90,
	0x00, 0x00, 0x03, 0x00, 0x3F, 0xFC, 0x7F, 0xFC, 0xE3, 0x00,
	0xE3, 0x00, 0x70, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x18, 0x07, 0x9C, 0x0F, 0xCC, 0x0C, 0xCC, 0x0C, 0xCC,
	0x0C, 0xCC, 0x0C, 0xCC, 0x0C, 0xDC, 0x0F, 0xF8, 0x07, 0xF0,
	0xFF, 0xFC, 0xFF, 0xFC, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x03, 0x80, 0x01, 0xFC, 0x00, 0xFC, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0xFC,
	0x1B, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x30, 0x00, 0x38, 0x00, 0x1C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x1C, 0xCF, 0xF8, 0xCF, 0xF0, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0xFC, 0xFF, 0xFC, 0x00, 0xE0, 0x01, 0xE0,
	0x03, 0xF0, 0x07, 0x38, 0x0E, 0x1C, 0x0C, 0x0C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0xFF, 0xFC,
	0xFF, 0xFC, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
	0x0F, 0xFC, 0x0F, 0xFC, 0x0E, 0x00, 0x07, 0x00, 0x03, 0xC0,
	0x03, 0xC0, 0x07, 0x00, 0x0E, 0x00, 0x0F, 0xFC, 0x0F, 0xFC,
	0x0F, 0xFC, 0x0F, 0xFC, 0x03, 0x00, 0x07, 0x00, 0x0E, 0x00,
	0x0C, 0x00, 0x0C, 0x00, 0x0E, 0x00, 0x07, 0xFC, 0x03, 0xFC,
	0x03, 0xF0, 0x07, 0xF8, 0x0E, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C,
	0x0C, 0x0C, 0x0C, 0x0C, 0x0E, 0x1C, 0x07, 0xF8, 0x03, 0xF0,
	0x0F, 0xFC, 0x0F, 0xFC, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0,
	0x0C, 0xC0, 0x0C, 0xC0, 0x0F, 0xC0, 0x07, 0x80, 0x03, 0x00,
	0x03, 0x00, 0x07, 0x80, 0x0F, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0,
	0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0F, 0xFC, 0x0F, 0xFC,
	0x0F, 0xFC, 0x0F, 0xFC, 0x03, 0x80, 0x07, 0x00, 0x0E, 0x00,
	0x0C, 0x00, 0x0C, 0x00, 0x0E, 0x00, 0x07, 0x00, 0x03, 0x00,
	0x03, 0x18, 0x07, 0x9C, 0x0F, 0xCC, 0x0C, 0xCC, 0x0C, 0xCC,
	0x0C, 0xCC, 0x0C, 0xCC, 0x0C, 0xFC, 0x0E, 0x78, 0x06, 0x30,
	0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0xFF, 0xF0, 0xFF, 0xF8,
	0x0C, 0x1C, 0x0C, 0x1C, 0x0C, 0x38, 0x0C, 0x30, 0x00, 0x00,
	0x0F, 0xF0, 0x0F, 0xF8, 0x00, 0x1C, 0x00, 0x0C, 0x00, 0x0C,
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x1C, 0x0F, 0xF8, 0x0F, 0xF0,
	0x0F, 0xC0, 0x0F, 0xE0, 0x00, 0x70, 0x00, 0x38, 0x00, 0x1C,
	0x00, 0x1C, 0x00, 0x38, 0x00, 0x70, 0x0F, 0xE0, 0x0F, 0xC0,
	0x0F, 0xF0, 0x0F, 0xF8, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0xF8,
	0x00, 0xF8, 0x00, 0x1C, 0x00, 0x1C, 0x0F, 0xF8, 0x0F, 0xF0,
	0x0C, 0x0C, 0x0E, 0x1C, 0x07, 0x38, 0x03, 0xF0, 0x01, 0xE0,
	0x01, 0xE0, 0x03, 0xF0, 0x07, 0x38, 0x0E, 0x1C, 0x0C, 0x0C,
	0x0C, 0x00, 0x0E, 0x00, 0x07, 0x0C, 0x03, 0x9C, 0x01, 0xF8,
	0x01, 0xF0, 0x03, 0x80, 0x07, 0x00, 0x0E, 0x00, 0x0C, 0x00,
	0x0C, 0x0C, 0x0C, 0x1C, 0x0C, 0x3C, 0x0C, 0x7C, 0x0C, 0xEC,
	0x0D, 0xCC, 0x0F, 0x8C, 0x0F, 0x0C, 0x0E, 0x0C, 0x0C, 0x0C,
	0x00, 0x00, 0x03, 0x00, 0x07, 0x80, 0x3F, 0xF0, 0x7C, 0xF8,
	0xE0, 0x1C, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0x00, 0x00,
	0x03, 0x0C, 0x03, 0x0C, 0x3F, 0xFC, 0x7F, 0xFC, 0xE3, 0x0C,
	0xC3, 0x0C, 0xC0, 0x0C, 0xE0, 0x0C, 0x70, 0x0C, 0x30, 0x0C,
	0x00, 0x00, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0x0C, 0xE0, 0x1C,
	0x7C, 0xF8, 0x3F, 0xF0, 0x07, 0x80, 0x03, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
	0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
	0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC,
	0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC
};

/**
 * \brief Read 32 bit data.
 */
static uint32_t ili9488_lcd_get_16(void)
{
	uint32_t readbuf[5];
	uint32_t *ptr, i;
	uint32_t shift_cnt = 2;
	uint32_t chipid = 0;
	uint32_t retval = 0;

	pio_set(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_MULTI_RD(readbuf, 5);
	ptr = &readbuf[1];
	for(i = 1; i < 4; i++) {
		chipid |= (*ptr &0xFF)<< (shift_cnt << 3);
		ptr++;
		shift_cnt--;
	}
	retval = chipid;
	return retval;
}

/**
 * \brief Prepare to write GRAM data.
 */
static void ili9488_write_ram_prepare(void)
{
	volatile uint32_t i;

	pio_clear(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_IR(0);
	LCD_IR(ILI9488_CMD_MEMORY_WRITE); /* Write Data to GRAM (R2Ch) */
	for(i = 0; i < 0xF; i++);
}

/**
 * \brief Write data to LCD GRAM.
 *
 * \param ul_color 16-bits RGB color.
 */
static void ili9488_write_ram(ili9488_color_t ul_color)
{
	pio_set(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_WD((uint16_t)ul_color);
}

/**
 * \brief Write multiple data in buffer to LCD controller.
 *
 * \param p_ul_buf data buffer.
 * \param ul_size size in pixels.
 */
static void ili9488_write_ram_buffer(const ili9488_color_t *p_ul_buf, uint32_t ul_size)
{
	uint32_t ul_addr;

	for (ul_addr = 0; ul_addr < (ul_size - ul_size % 8); ul_addr += 8) {
		ili9488_write_ram(p_ul_buf[ul_addr]);
		ili9488_write_ram(p_ul_buf[ul_addr + 1]);
		ili9488_write_ram(p_ul_buf[ul_addr + 2]);
		ili9488_write_ram(p_ul_buf[ul_addr + 3]);
		ili9488_write_ram(p_ul_buf[ul_addr + 4]);
		ili9488_write_ram(p_ul_buf[ul_addr + 5]);
		ili9488_write_ram(p_ul_buf[ul_addr + 6]);
		ili9488_write_ram(p_ul_buf[ul_addr + 7]);
	}
	for (; ul_addr < ul_size; ul_addr++) {
		ili9488_write_ram(p_ul_buf[ul_addr]);
	}
}

/**
 * \brief Write data to LCD Register.
 *
 * \param uc_reg register address.
 * \param us_data data to be written.
 * \param size the number of parameters.
 */
 void ili9488_write_register(uint8_t uc_reg, uint16_t *us_data, uint32_t size)
{
	volatile uint32_t i;

	/* CDS pin is set low level when writing command*/
	pio_clear(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_IR(0);
	LCD_IR(uc_reg);
	for(i = 0; i<0xF; i++);

	if(size == 0) {
		return;
	}

	/* CDS pin is set high level when writing parameters or image data*/
	pio_set(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_MULTI_WD(us_data, size);


}

/**
 * \brief Prepare to read GRAM data.
 */
static void ili9488_read_ram_prepare(void)
{
	volatile uint32_t i;

	pio_clear(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_IR(0);
	LCD_IR(ILI9488_CMD_MEMORY_READ); /* Write Data to GRAM (R2Eh) */
	for(i = 0; i<0xF; i++);

}
/**
 * \brief Read data to LCD GRAM.
 *
 * \note Because pixel data LCD GRAM is 18-bits, so convertion to RGB 24-bits
 * will cause low color bit lose.
 *
 * \return color 24-bits RGB color.
 */
static uint32_t ili9488_read_ram(void)
{
	uint32_t value[2];
	uint32_t color = 0;
	uint8_t  tmp[2];

	pio_clear(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_IR(0);
	LCD_IR(ILI9488_CMD_MEMORY_READ);

	pio_set(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_MULTI_RD(value, 2);

	/* The first data is dummy*/
	tmp[0] = (uint8_t)((value[1] >> 8) & 0xFFu);
	tmp[1] = (uint8_t)(value[1] & 0xFFu);

	/* Convert RGB565 to RGB888 */
	/* For BGR format */
	color = ((tmp[0] & 0xF8)) |  /* R */
			((tmp[0] & 0x07) << 13) | ((tmp[1] & 0xE0) << 5) |  /* G */
			((tmp[1] & 0x1F) << 19);  /* B */
	return color;
}

/**
 * \brief Read data from LCD Register.
 *
 * \param uc_reg register address.
 *
 * \return register value.
 */
static uint32_t ili9488_read_register(uint8_t uc_reg)
{
	volatile uint32_t i;

	pio_clear(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_IR(0);
	LCD_IR(uc_reg);
	for(i = 0; i<0xF; i++);

	return ili9488_lcd_get_16();
}

/**
 * \brief Delay function.
 */
void ili9488_delay(uint32_t ul_ms)
{
	 uint32_t i;

	for(i = 0; i < ul_ms; i++) {
		for(i = 0; i < 100000; i++) {
		}
	}
}

/**
 * \brief Check box coordinates.
 *
 * \param p_ul_x1 X coordinate of upper-left corner on LCD.
 * \param p_ul_y1 Y coordinate of upper-left corner on LCD.
 * \param p_ul_x2 X coordinate of lower-right corner on LCD.
 * \param p_ul_y2 Y coordinate of lower-right corner on LCD.
 */
static void ili9488_check_box_coordinates(uint32_t *p_ul_x1, uint32_t *p_ul_y1,
		uint32_t *p_ul_x2, uint32_t *p_ul_y2)
{
	uint32_t dw;

	if (*p_ul_x1 >= ILI9488_LCD_WIDTH) {
		*p_ul_x1 = ILI9488_LCD_WIDTH - 1;
	}

	if (*p_ul_x2 >= ILI9488_LCD_WIDTH) {
		*p_ul_x2 = ILI9488_LCD_WIDTH - 1;
	}

	if (*p_ul_y1 >= ILI9488_LCD_HEIGHT) {
		*p_ul_y1 = ILI9488_LCD_HEIGHT - 1;
	}

	if (*p_ul_y2 >= ILI9488_LCD_HEIGHT) {
		*p_ul_y2 = ILI9488_LCD_HEIGHT - 1;
	}

	if (*p_ul_x1 > *p_ul_x2) {
		dw = *p_ul_x1;
		*p_ul_x1 = *p_ul_x2;
		*p_ul_x2 = dw;
	}

	if (*p_ul_y1 > *p_ul_y2) {
		dw = *p_ul_y1;
		*p_ul_y1 = *p_ul_y2;
		*p_ul_y2 = dw;
	}
}
/**
 * \brief ILI9488 configure landscape.

 * \Param dwRGB RGB mode.
 * \Param LandscaprMode Landscape Mode.
 */
void ili9488_setdisplaylandscape( uint8_t dwRGB, uint8_t LandscapeMode )
{
	uint16_t value;
	if(LandscapeMode) {
		if(dwRGB) {
			value = 0xE8;
		} else {
			value = 0xE0;
		}
	} else {
		if(dwRGB) {
			value = 0x48;
		} else {
			value = 0x40;
		}
	}
	ili9488_write_register(ILI9488_CMD_MEMORY_ACCESS_CONTROL, &value, 1);
}

/**
 * \brief ILI9488 configure window.

 * \Param dwX X start position.
 * \Param dwX Y start position.
 * \Param dwWidth  Width of window.
 * \Param dwHeight Height of window.
 */
void ili9488_set_window(uint16_t dwX, uint16_t dwY, uint16_t dwWidth, uint16_t dwHeight )
{
	uint16_t ColStart, ColEnd, RowStart, RowEnd;
	uint32_t cnt = 0;
	uint16_t buf[4];

	cnt = sizeof(buf)/sizeof(uint16_t);

	ColStart  =  dwX ;
	ColEnd    =  dwWidth + dwX;

	RowStart = dwY ;
	RowEnd   = dwHeight + dwY;

	buf[0] = get_8b_to_16b(ColStart);
	buf[1] = get_0b_to_8b(ColStart);
	buf[2] = get_8b_to_16b(ColEnd);
	buf[3] = get_0b_to_8b(ColEnd);
	ili9488_write_register(ILI9488_CMD_COLUMN_ADDRESS_SET, (uint16_t*)buf, cnt);
	ili9488_write_register(ILI9488_CMD_NOP, 0, 0);

	/* Set Horizontal Address End Position */
	buf[0] = get_8b_to_16b(RowStart);
	buf[1] = get_0b_to_8b(RowStart);
	buf[2] = get_8b_to_16b(RowEnd);
	buf[3] = get_0b_to_8b(RowEnd);
	ili9488_write_register(ILI9488_CMD_PAGE_ADDRESS_SET, (uint16_t*)buf, cnt);
	ili9488_write_register(ILI9488_CMD_NOP, 0, 0);
}

/**
 * \brief Initialize the ILI9488 lcd driver.
 *
 * \note Make sure below works have been done before calling ili9488_init()\n
 * 1. ILI9488 related Pins have been initialized correctly. \n
 * 2. SMC has been configured correctly for access ILI9488 (8-bit system interface for now). \n
 *
 * \param p_opt pointer to ILI9488 option structure.
 *
 * \return 0 if initialization succeeds, otherwise fails.
 */
uint32_t ili9488_init(struct ili9488_opt_t *p_opt)
{
	uint16_t param;
	uint32_t chipid;

	ili9488_write_register(ILI9488_CMD_SOFTWARE_RESET, 0x0000, 0);
	ili9488_delay(200);

	ili9488_write_register(ILI9488_CMD_SLEEP_OUT, 0x0000, 0);
	ili9488_delay(200);

	/** read chipid */
	chipid = ili9488_read_register(ILI9488_CMD_READ_ID4);
	if (chipid != ILI9488_DEVICE_CODE) {
		return 1;
	}

	/** make it tRGB and reverse the column order */
	param = 0x48;
	ili9488_write_register(ILI9488_CMD_MEMORY_ACCESS_CONTROL, &param, 1);
	ili9488_delay(100);

	param = 0x04;
	ili9488_write_register(ILI9488_CMD_CABC_CONTROL_9, &param, 1);
	ili9488_delay(100);
	/** Set ILI9488 Pixel Format in SMC mode.*/
	param = 0x05;
	ili9488_write_register(ILI9488_CMD_COLMOD_PIXEL_FORMAT_SET, &param, 1);
	ili9488_delay(100);
	ili9488_write_register(ILI9488_CMD_NORMAL_DISP_MODE_ON, 0, 0);
	ili9488_delay(100);

	ili9488_display_on();
	ili9488_delay(100);

	ili9488_setdisplaylandscape(1, 0);
	ili9488_delay(100);

	ili9488_set_window(0, 0,p_opt->ul_width,p_opt->ul_height);
	ili9488_set_foreground_color(p_opt->foreground_color);
	ili9488_set_cursor_position(0, 0);

	return 0;
}

/**
 * \brief Turn on the LCD.
 */
void ili9488_display_on(void)
{
	ili9488_write_register(ILI9488_CMD_DISPLAY_ON, 0, 0);
}

/**
 * \brief Turn off the LCD.
 */
void ili9488_display_off(void)
{
	ili9488_write_register(ILI9488_CMD_DISPLAY_OFF, 0x00, 0);
}

/**
 * \brief Set foreground color.
 *
 * \param ul_color foreground color.
 */
void ili9488_set_foreground_color(ili9488_color_t ul_color)
{
	uint32_t i;

	/* Fill the cache with selected color */
	for (i = 0; i < LCD_DATA_CACHE_SIZE; ++i) {
		g_ul_pixel_cache[i] = ul_color;
	}

}

/**
 * \brief Fill the LCD buffer with the specified color.
 *
 * \param ul_color fill color.
 */
void ili9488_fill(ili9488_color_t ul_color)
{
	uint32_t dw;

	ili9488_set_cursor_position(0, 0);
	ili9488_write_ram_prepare();

	for (dw = ILI9488_LCD_WIDTH * ILI9488_LCD_HEIGHT; dw > 0; dw--) {
		ili9488_write_ram(ul_color);
	}
}

/**
 * \brief Set cursor of LCD screen.
 *
 * \param x X coordinate of upper-left corner on LCD.
 * \param y Y coordinate of upper-left corner on LCD.
 */
void ili9488_set_cursor_position(uint16_t x, uint16_t y)
{
	/* Set Horizontal Address Start Position */
	uint32_t cnt = 0;

	uint16_t buf[4];
	cnt = sizeof(buf)/sizeof(uint16_t);

	buf[0] = get_8b_to_16b(x);
	buf[1] = get_0b_to_8b(x);
	x+=1;
	buf[2] = get_8b_to_16b(x);
	buf[3] = get_0b_to_8b(x);
	ili9488_write_register(ILI9488_CMD_COLUMN_ADDRESS_SET, (uint16_t*)buf, cnt);
	ili9488_write_register(ILI9488_CMD_NOP, 0, 0);


	/* Set Horizontal Address End Position */
	buf[0] = get_8b_to_16b(y);
	buf[1] = get_0b_to_8b(y);
	y+=1;
	buf[2] = get_8b_to_16b(y);
	buf[3] = get_0b_to_8b(y);
	ili9488_write_register(ILI9488_CMD_PAGE_ADDRESS_SET, (uint16_t*)buf, cnt);
	ili9488_write_register(ILI9488_CMD_NOP, 0, 0);
}

/**
 * \brief Scroll up/down for the number of specified lines.
 *
 * \param ul_tfa  Top Fixed Area in number of lines from the top of the frame memory.
 * \param ul_lines number of lines to scroll.
 */
void ili9488_scroll(uint32_t ul_tfa, uint32_t ul_lines, uint32_t ul_bfa)
{
	uint32_t cnt = 0;

	uint16_t buf[6];
	cnt = sizeof(buf)/sizeof(uint16_t);

	buf[0] = get_8b_to_16b(ul_tfa);
	buf[1] = get_0b_to_8b(ul_tfa);

	buf[2] = get_8b_to_16b(ul_lines);
	buf[3] = get_0b_to_8b(ul_lines);

	buf[4] = get_8b_to_16b(ul_bfa);
	buf[5] = get_0b_to_8b(ul_bfa);
	ili9488_write_register(ILI9488_CMD_VERT_SCROLL_DEFINITION, buf, cnt);
}

/**
 * \brief Enable the scrolling feature.
 *
 * \param ul_vsp Vertical Scrolling Start Address
 */
void ili9488_set_scroll_address(uint32_t ul_vsp)
{
	uint32_t cnt = 0;

	uint16_t buf[2];
	cnt = sizeof(buf)/sizeof(uint16_t);

	buf[0] = get_8b_to_16b(ul_vsp);
	buf[1] = get_0b_to_8b(ul_vsp);
	ili9488_write_register(ILI9488_CMD_VERT_SCROLL_DEFINITION, buf, cnt);
}

/**
 * \brief Draw a pixel on LCD.
 *
 * \param ul_x X coordinate of pixel.
 * \param ul_y Y coordinate of pixel.
 *
 * \return 0 if succeeds, otherwise fails.
 */
uint32_t ili9488_draw_pixel(uint32_t ul_x, uint32_t ul_y)
{
	if ((ul_x >= ILI9488_LCD_WIDTH) || (ul_y >= ILI9488_LCD_HEIGHT)) {
		return 1;
	}

	/* Set cursor */
	ili9488_set_cursor_position(ul_x, ul_y);

	/* Prepare to write in GRAM */
	ili9488_write_ram_prepare();
	ili9488_write_ram(*g_ul_pixel_cache);
	return 0;
}

/**
 * \brief Get a pixel from LCD.
 *
 * \param ul_x X coordinate of pixel.
 * \param ul_y Y coordinate of pixel.
 *
 * \return the pixel color.
 */
ili9488_color_t ili9488_get_pixel(uint32_t ul_x, uint32_t ul_y)
{
	Assert(ul_x <= ILI9488_LCD_WIDTH);
	Assert(ul_y <= ILI9488_LCD_HEIGHT);

	/* Set cursor */
	ili9488_set_cursor_position(ul_x, ul_y);

	/* Prepare to write in GRAM */
	ili9488_read_ram_prepare();
	return ili9488_read_ram();
}

/**
 * \brief Draw a line on LCD, which is not horizontal or vertical.
 *
 * \param ul_x1 X coordinate of line start.
 * \param ul_y1 Y coordinate of line start.
 * \param ul_x2 X coordinate of line end.
 * \param ul_y2 Y coordinate of line endl.
 */
static void ili9488_draw_line_bresenham(uint32_t ul_x1, uint32_t ul_y1,
		uint32_t ul_x2, uint32_t ul_y2)
{
	int dx, dy;
	int i;
	int xinc, yinc, cumul;
	int x, y;

	x = ul_x1;
	y = ul_y1;
	dx = ul_x2 - ul_x1;
	dy = ul_y2 - ul_y1;
	xinc = (dx > 0) ? 1 : -1;
	yinc = (dy > 0) ? 1 : -1;
	dx = abs(ul_x2 - ul_x1);
	dy = abs(ul_y2 - ul_y1);

	ili9488_draw_pixel(x, y);

	if (dx > dy) {
		cumul = dx >> 1;

		for (i = 1; i <= dx; i++) {
			x += xinc;
			cumul += dy;

			if (cumul >= dx) {
				cumul -= dx;
				y += yinc;
			}

			ili9488_draw_pixel(x, y);
		}
	} else {
		cumul = dy >> 1;

		for (i = 1; i <= dy; i++) {
			y += yinc;
			cumul += dx;

			if (cumul >= dy) {
				cumul -= dy;
				x += xinc;
			}

			ili9488_draw_pixel(x, y);
		}
	}
}

/**
 * \brief Draw a line on LCD.
 *
 * \param ul_x1 X coordinate of line start.
 * \param ul_y1 Y coordinate of line start.
 * \param ul_x2 X coordinate of line end.
 * \param ul_y2 Y coordinate of line end.
 */
void ili9488_draw_line(uint32_t ul_x1, uint32_t ul_y1,
		uint32_t ul_x2, uint32_t ul_y2)
{
	if ((ul_y1 == ul_y2) || (ul_x1 == ul_x2)) {
		ili9488_draw_filled_rectangle(ul_x1, ul_y1, ul_x2, ul_y2);
	} else {
		ili9488_draw_line_bresenham(ul_x1, ul_y1, ul_x2, ul_y2);
	}
}

/**
 * \brief Draw a rectangle on LCD.
 *
 * \param ul_x1 X coordinate of upper-left corner on LCD.
 * \param ul_y1 Y coordinate of upper-left corner on LCD.
 * \param ul_x2 X coordinate of lower-right corner on LCD.
 * \param ul_y2 Y coordinate of lower-right corner on LCD.
 */
void ili9488_draw_rectangle(uint32_t ul_x1, uint32_t ul_y1,
		uint32_t ul_x2, uint32_t ul_y2)
{
	ili9488_check_box_coordinates(&ul_x1, &ul_y1, &ul_x2, &ul_y2);

	ili9488_draw_filled_rectangle(ul_x1, ul_y1, ul_x2, ul_y1);
	ili9488_draw_filled_rectangle(ul_x1, ul_y2, ul_x2, ul_y2);

	ili9488_draw_filled_rectangle(ul_x1, ul_y1, ul_x1, ul_y2);
	ili9488_draw_filled_rectangle(ul_x2, ul_y1, ul_x2, ul_y2);
}

/**
 * \brief Draw a filled rectangle on LCD.
 *
 * \param ul_x1 X coordinate of upper-left corner on LCD.
 * \param ul_y1 Y coordinate of upper-left corner on LCD.
 * \param ul_x2 X coordinate of lower-right corner on LCD.
 * \param ul_y2 Y coordinate of lower-right corner on LCD.
 */
void ili9488_draw_filled_rectangle(uint32_t ul_x1, uint32_t ul_y1,
		uint32_t ul_x2, uint32_t ul_y2)
{
	uint32_t size, blocks;

	/* Swap coordinates if necessary */
	ili9488_check_box_coordinates(&ul_x1, &ul_y1, &ul_x2, &ul_y2);

	/* Determine the refresh window area */
	ili9488_set_window(ul_x1, ul_y1, (ul_x2 - ul_x1) + 1, (ul_y2 - ul_y1) + 1);

	/* Prepare to write in GRAM */
	ili9488_write_ram_prepare();

	size = (ul_x2 - ul_x1 + 1) * (ul_y2 - ul_y1 + 1);
	/* Send pixels blocks => one SPI IT / block */
	blocks = size / LCD_DATA_CACHE_SIZE;
	while (blocks--) {
		ili9488_write_ram_buffer(g_ul_pixel_cache, LCD_DATA_CACHE_SIZE);
	}

	/* Send remaining pixels */
	ili9488_write_ram_buffer(g_ul_pixel_cache, size % LCD_DATA_CACHE_SIZE);

	/* Reset the refresh window area */
	ili9488_set_window(0, 0, ILI9488_LCD_WIDTH, ILI9488_LCD_HEIGHT);

}

/**
 * \brief Draw a circle on LCD.
 *
 * \param ul_x X coordinate of circle center.
 * \param ul_y Y coordinate of circle center.
 * \param ul_r circle radius.
 *
 * \return 0 if succeeds, otherwise fails.
 */
uint32_t ili9488_draw_circle(uint32_t ul_x, uint32_t ul_y, uint32_t ul_r)
{
	int32_t   d;    /* Decision Variable */
	uint32_t  curX; /* Current X Value */
	uint32_t  curY; /* Current Y Value */

	if (ul_r == 0) {
		return 1;
	}

	d = 3 - (ul_r << 1);
	curX = 0;
	curY = ul_r;

	while (curX <= curY) {
		ili9488_draw_pixel(ul_x + curX, ul_y + curY);
		ili9488_draw_pixel(ul_x + curX, ul_y - curY);
		ili9488_draw_pixel(ul_x - curX, ul_y + curY);
		ili9488_draw_pixel(ul_x - curX, ul_y - curY);
		ili9488_draw_pixel(ul_x + curY, ul_y + curX);
		ili9488_draw_pixel(ul_x + curY, ul_y - curX);
		ili9488_draw_pixel(ul_x - curY, ul_y + curX);
		ili9488_draw_pixel(ul_x - curY, ul_y - curX);

		if (d < 0) {
			d += (curX << 2) + 6;
		} else {
			d += ((curX - curY) << 2) + 10;
			curY--;
		}
		curX++;
	}

	return 0;
}

/**
 * \brief Draw a filled circle on LCD.
 *
 * \param ul_x X coordinate of circle center.
 * \param ul_y Y coordinate of circle center.
 * \param ul_r circle radius.
 *
 * \return 0 if succeeds, otherwise fails.
 */
uint32_t ili9488_draw_filled_circle(uint32_t ul_x, uint32_t ul_y, uint32_t ul_r)
{
	signed int d;       /* Decision Variable */
	uint32_t dwCurX;    /* Current X Value */
	uint32_t dwCurY;    /* Current Y Value */
	uint32_t dwXmin, dwYmin;

	if (ul_r == 0) {
		return 1;
	}

	d = 3 - (ul_r << 1);
	dwCurX = 0;
	dwCurY = ul_r;

	while (dwCurX <= dwCurY) {
		dwXmin = (dwCurX > ul_x) ? 0 : ul_x - dwCurX;
		dwYmin = (dwCurY > ul_y) ? 0 : ul_y - dwCurY;
		ili9488_draw_filled_rectangle(dwXmin, dwYmin, ul_x + dwCurX, dwYmin);
		ili9488_draw_filled_rectangle(dwXmin, ul_y + dwCurY, ul_x + dwCurX, ul_y + dwCurY);
		dwXmin = (dwCurY > ul_x) ? 0 : ul_x - dwCurY;
		dwYmin = (dwCurX > ul_y) ? 0 : ul_y - dwCurX;
		ili9488_draw_filled_rectangle(dwXmin, dwYmin, ul_x + dwCurY, dwYmin);
		ili9488_draw_filled_rectangle(dwXmin, ul_y + dwCurX, ul_x + dwCurY, ul_y + dwCurX);

		if (d < 0) {
			d += (dwCurX << 2) + 6;
		} else {
			d += ((dwCurX - dwCurY) << 2) + 10;
			dwCurY--;
		}

		dwCurX++;
	}

	return 0;
}

/**
 * \brief Prepare LCD to draw.
 *
 * \param ul_x Horizontal address start position
 * \param ul_y Vertical address start position
 * \param ul_width The width of the window.
 * \param ul_height The height of the window.
 */
void ili9488_draw_prepare(uint32_t ul_x, uint32_t ul_y, uint32_t ul_width,
		uint32_t ul_height)
{
	/* Determine the refresh window area */
	ili9488_set_window(ul_x, ul_y, ul_width, ul_height);

	/* Set cursor */
	ili9488_set_cursor_position(ul_x, ul_y);

	/* Prepare to write in GRAM */
	ili9488_write_ram_prepare();
}

/**
 * \brief Draw an ASCII character on LCD.
 *
 * \param ul_x X coordinate of character upper-left corner.
 * \param ul_y Y coordinate of character upper-left corner.
 * \param uc_c character to print.
 */
static void ili9488_draw_char(uint32_t ul_x, uint32_t ul_y, uint8_t uc_c)
{
	uint32_t row, col;
	uint32_t offset, offset0, offset1;

	/* Compute offset according of the specified ASCII character */
	/* Note: the first 32 characters of the ASCII table are not handled */
	offset = ((uint32_t)uc_c - 0x20) * 20;

	for (col = 0; col < 10; col++) {
		/* Compute the first and second byte offset of a column */
		offset0 = offset + col * 2;
		offset1 = offset0 + 1;

		/* Draw pixel on screen depending on the corresponding bit value from the charset */
		for (row = 0; row < 8; row++) {
			if ((p_uc_charset10x14[offset0] >> (7 - row)) & 0x1) {
				ili9488_draw_pixel(ul_x + col, ul_y + row);
			}
		}

		for (row = 0; row < 6; row++) {
			if ((p_uc_charset10x14[offset1] >> (7 - row)) & 0x1) {
				ili9488_draw_pixel(ul_x + col, ul_y + row + 8);
			}
		}
	}
}

/**
 * \brief Draw a string on LCD.
 *
 * \param ul_x X coordinate of string top-left corner.
 * \param ul_y Y coordinate of string top-left corner.
 * \param p_str String to display.
 */
void ili9488_draw_string(uint32_t ul_x, uint32_t ul_y, const uint8_t *p_str)
{
	uint32_t xorg = ul_x;

	while (*p_str != 0) {
		/* If newline, jump to the next line (font height + 2) */
		if (*p_str == '\n') {
			ul_y += gfont.height + 2;
			ul_x = xorg;
		} else {
			/* Draw the character and place cursor right after (font width + 2) */
			ili9488_draw_char(ul_x, ul_y, *p_str);
			ul_x += gfont.width + 2;
		}
		p_str++;
	}
}

/**
 * \brief Draw a pixmap on LCD.
 *
 * \param ul_x X coordinate of upper-left corner on LCD.
 * \param ul_y Y coordinate of upper-left corner on LCD.
 * \param ul_width width of the picture.
 * \param ul_height height of the picture.
 * \param p_ul_pixmap pixmap of the image.
 */
void ili9488_draw_pixmap(uint32_t ul_x, uint32_t ul_y, uint32_t ul_width,
		uint32_t ul_height, const ili9488_color_t *p_ul_pixmap)
{
	uint32_t size;
	uint32_t dwX1, dwY1, dwX2, dwY2;
	dwX1 = ul_x;
	dwY1 = ul_y;
	dwX2 = ul_x + ul_width;
	dwY2 = ul_y + ul_height;

	/* Swap coordinates if necessary */
	ili9488_check_box_coordinates(&dwX1, &dwY1, &dwX2, &dwY2);

	/* Determine the refresh window area */
	ili9488_set_window(dwX1, dwY1, (dwX2 - dwX1 + 1), (dwY2 - dwY1 + 1));

	/* Prepare to write in GRAM */
	ili9488_write_ram_prepare();

	size = (dwX2 - dwX1) * (dwY2 - dwY1);

	ili9488_write_ram_buffer(p_ul_pixmap, size);

	/* Reset the refresh window area */
	ili9488_set_window(0, 0, ILI9488_LCD_WIDTH, ILI9488_LCD_HEIGHT);
}

/**
 * \brief Read a single color from the graphical memory
 *
 * Use this function to read a color from the graphical memory of the
 * controller.
 *
 * Limits have to be set prior to calling this function, e.g.:
 * \code
	ili9488_set_top_left_limit(0, 0);
	ili9488_set_bottom_right_limit(320, 240);
	...
\endcode
 *
 * \retval ili9488_color_t The read color pixel
 */
ili9488_color_t ili9488_read_gram(void)
{
	uint32_t value[2];
	ili9488_color_t color;

	pio_clear(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_IR(0);
	LCD_IR(ILI9488_CMD_MEMORY_READ);

	pio_set(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_MULTI_RD(value, 2);

	/* The first data is dummy*/
	color = value[1];
	return color;
}

/**
 * \brief Sets the orientation of the display data
 *
 * Configures the display for a given orientation, including mirroring and/or
 * screen rotation.
 *
 * \param flags Orientation flags to use, see \ref ILI9488_FLIP_X, \ref ILI9488_FLIP_Y.
 */
void ili9488_set_orientation(uint8_t flags)
{
	uint32_t cnt = 0;
	uint16_t buf[3];
	cnt = sizeof(buf)/sizeof(uint16_t);

	buf[0] = 0x2;
	buf[1] = 0x2;
	buf[2] = 0x3B;

	buf[1] |= (flags & ILI9488_FLIP_X ? (buf[1] | (1 << 6)) : 0);
	buf[1] |= (flags & ILI9488_FLIP_Y ? (buf[1] | (1 << 5)) : 0);

	ili9488_write_register(ILI9488_CMD_DISPLAY_FUNCTION_CONTROL, buf, cnt);
}

/**
 * \brief Set display brightness
 *
 * Configures the display for a given brightness value
 *
 * \param us_value brightness value need to written.
 */
void ili9488_write_brightness(uint16_t us_value)
{
	ili9488_write_register(ILI9488_CMD_WRITE_DISPLAY_BRIGHTNESS, &us_value, 1);
}

/**
 * \brief read current display brightness
 *
 * \return brightness display brightness value.
 */
uint32_t ili9488_read_brightness(void)
{
	uint32_t value[2];
	uint32_t brightness;

	pio_clear(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_IR(0);
	LCD_IR(ILI9488_CMD_READ_DISPLAY_BRIGHTNESS);
	/* The first data is dummy*/

	pio_set(PIN_EBI_CDS_PIO, PIN_EBI_CDS_MASK);
	LCD_MULTI_RD(value, 2);

	brightness = value[1];
	return brightness;
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \}
 */
