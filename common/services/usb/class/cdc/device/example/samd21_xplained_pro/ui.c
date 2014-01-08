/**
 * \file
 *
 * \brief User Interface
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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

#include <asf.h>
#include "ui.h"

#define  LED_On()          port_pin_set_output_level(LED_0_PIN, 0)
#define  LED_Off()         port_pin_set_output_level(LED_0_PIN, 1)

static uint32_t blink_interval;


void ui_init(void)
{
	blink_interval = 1000;
	/* Initialize LEDs */
	LED_Off();
}

void ui_powerdown(void)
{
	LED_Off();
}


void ui_wakeup(void)
{
	LED_On();
}


void ui_com_open(uint8_t port)
{
	UNUSED(port);
}


void ui_com_close(uint8_t port)
{
	UNUSED(port);
}


void ui_com_rx_start(void)
{
	blink_interval = 500;
}


void ui_com_rx_stop(void)
{
	blink_interval = 1000;
}


void ui_com_tx_start(void)
{
	blink_interval = 500;
}


void ui_com_tx_stop(void)
{
	blink_interval = 1000;
}


void ui_com_error(void)
{
	blink_interval = 100;
}


void ui_com_overflow(void)
{
	;
}

void ui_process(uint16_t framenumber)
{
	if ((framenumber % blink_interval) == 0) {
		LED_On();
	}
	if ((framenumber % blink_interval) == (blink_interval / 2)) {
		LED_Off();
	}
}

/**
 * \defgroup UI User Interface
 *
 * Human interface on SAMD21-XPlain:
 * - Led 0 blinks when USB is connected and active
 * - When communication starts, the blink frequency will be 2HZ. otherwise, it is 1HZ.
 *
 */
