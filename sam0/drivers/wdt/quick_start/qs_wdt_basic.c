/**
 * \file
 *
 * \brief SAM D20 Watchdog Driver Quick Start
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

void configure_wdt(void);

//! [setup]
void configure_wdt(void)
{
	/* Create a new configuration structure for the Watchdog settings and fill
	 * with the default module settings. */
	//! [setup_1]
	struct wdt_conf wdt_conf;
	//! [setup_1]
	//! [setup_2]
	wdt_get_config_defaults(&wdt_conf);
	//! [setup_2]

	// TODO: Move into driver once scheme to select clock is determined
	struct system_gclk_chan_config gclk_chan_conf;
	system_gclk_chan_get_config_defaults(&gclk_chan_conf);
	gclk_chan_conf.source_generator = GCLK_GENERATOR_4;
	gclk_chan_conf.run_in_standby   = false;
	system_gclk_chan_set_config(WDT_GCLK_ID, &gclk_chan_conf);
	system_gclk_chan_enable(WDT_GCLK_ID);

	/* Set the Watchdog configuration settings */
	//! [setup_3]
	wdt_conf.always_on      = true;
	wdt_conf.timeout_period = WDT_PERIOD_4096CLK;
	//! [setup_3]

	/* Initialize and enable the Watchdog with the user settings */
	//! [setup_4]
	wdt_init(&wdt_conf);
	//! [setup_4]
	//! [setup_5]
	wdt_enable();
	//! [setup_5]
}
//! [setup]

int main(void)
{
	system_init();

	//! [setup_init]
	configure_wdt();
	//! [setup_init]

	//! [main]
	//! [main_1]
	enum system_reset_cause reset_cause = system_get_reset_cause();
	//! [main_1]

	//! [main_2]
	if (reset_cause == SYSTEM_RESET_CAUSE_WDT) {
		port_pin_set_output_level(LED_0_PIN, false);
	}
	else {
		port_pin_set_output_level(LED_0_PIN, true);
	}
	//! [main_2]

	//! [main_3]
	while (true) {
	//! [main_3]
		//! [main_4]
		if (port_pin_get_input_level(BUTTON_0_PIN) == false) {
		//! [main_4]
		//! [main_5]
			port_pin_set_output_level(LED_0_PIN, true);

			wdt_reset_count();
		//! [main_5]
		}
	}
	//! [main]
}
