/**
 * \file
 *
 * \brief SAM TIMER Driver Quick Start for SAMB11
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

static void delay(uint32_t cycles)
{
	volatile uint32_t i = 0;
	
	for (i=0; i < cycles; i++) {
		__NOP();
	}
}

//! [setup]
static void configure_gpio_pins(void)
{
//! [setup_1]
	struct gpio_config config_gpio_pin;
//! [setup_1]
//! [setup_2]
	gpio_get_config_defaults(&config_gpio_pin);
//! [setup_2]
//! [setup_3]
	config_gpio_pin.direction = GPIO_PIN_DIR_OUTPUT;
//! [setup_3]
//! [setup_4]
	gpio_pin_set_config(LED_0_PIN, &config_gpio_pin);
//! [setup_4]
}
//! [setup]

int main(void)
{
//! [setup_init]

	//system_init();
//! [gpio_init]	
	configure_gpio_pins();
//! [gpio_init]
//! [timer_init]
	aon_sleep_timer_init(AON_SLEEP_TIMER_WAKEUP_EN,
			AON_SLEEP_TIMER_RELOAD_MODE, CONF_AON_SLEEP_RELOAD_COUNTER);
//! [timer_init]

//! [setup_init]
	
//! [main_imp]
//! [main_loop]
	while (true) {
//! [main_loop]
//! [main_1]
		gpio_pin_set_output_level(LED_0_PIN, true);
//! [main_1]
		delay(1000000);
//! [main_2]
		while (!aon_sleep_timer_sleep_timer_active());
//! [main_2]
//! [main_3]
		aon_sleep_timer_sleep_request();
//! [main_3]
//! [main_4]
		gpio_pin_set_output_level(LED_0_PIN, false);
//! [main_4]
//! [main_5]
		asm volatile ("wfi");
		asm volatile ("nop");
//! [main_5]
	}
//! [main_imp]
}
