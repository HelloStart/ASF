/**
 * \file
 *
 * \brief SAMD20 GPIO PINMUX Driver Quick Start
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

void config_port_pins(void);

//! [setup]
void config_port_pins(void)
{
//! [setup_1]
	struct system_pinmux_conf pin_conf;
//! [setup_1]
//! [setup_2]
	system_pinmux_get_config_defaults(&pin_conf);
//! [setup_2]

//! [setup_3]
	pin_conf.mux_position = SYSTEM_PINMUX_GPIO;
	pin_conf.direction    = SYSTEM_PINMUX_PIN_DIR_INPUT;
	pin_conf.input_pull   = SYSTEM_PINMUX_PIN_PULL_UP;
//! [setup_3]
//! [setup_4]
	system_pinmux_pin_set_config(10, &pin_conf);
//! [setup_4]

//! [setup_5]
	system_pinmux_pin_set_input_sample_mode(10,
			SYSTEM_PINMUX_PIN_SAMPLING_ONDEMAND);
//! [setup_5]
}
//! [setup]

int main(void)
{
	//! [setup_init]
	config_port_pins();
	//! [setup_init]

	//! [main]
	while (true) {
		// Do nothing
	}
	//! [main]
}
