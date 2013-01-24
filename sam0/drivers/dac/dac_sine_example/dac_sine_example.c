/**
 * \file
 *
 * \brief SAMD20 DAC Sine Function Example
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
 *
 * \license
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

uint8_t sine_wave[256] = {
		0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
		0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
		0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
		0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
		0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
		0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
		0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
		0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
		0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
		0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
		0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
		0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
		0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
		0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
		0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
		0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
		0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
		0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
		0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
		0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
		0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
		0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
		0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
		0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
		0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
		0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
		0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
		0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
		0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
		0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
};

static void configure_pins(void)
{
	struct system_pinmux_conf pin_conf;
	system_pinmux_get_config_defaults(&pin_conf);

	/* Set up the DAC VOUT pin */
	pin_conf.mux_position = 7; /* mux H */
	pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
	pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;
	system_pinmux_pin_set_config(0, &pin_conf); /* PA0 mux H */

	/* Set up the Xplained PRO LED pin to output status info */
	pin_conf.mux_position = SYSTEM_PINMUX_GPIO;
	pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
	system_pinmux_pin_set_config(40, &pin_conf);
}

static void error(uint8_t number)
{
	switch (number) {
	case 1:
		port_pin_set_output_level(40, true);
		break;
	case 2:
		port_pin_set_output_level(40, false);
		break;
	default:
		break;
	}


	while (1) {
		/* */
	}
}

int main(void)
{
	struct dac_dev_inst dev_inst;
	struct dac_conf config;
	struct dac_ch_conf ch_config;

	configure_pins();

	/* Initialize all the system clocks, pm, gclk... */
	system_init();

	//system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBA, 0xffffffff);
	//system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBB, 0xffffffff);

	if (system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, (1 << 18)) != STATUS_OK) {
		error(0);
	}

	system_vref_enable(SYSTEM_VREF_BANDGAP);

	dac_get_config_defaults(&config);
	dac_init(&dev_inst, DAC, &config);

	dac_enable(&dev_inst);

	/*port_pin_set_output_level(40, false);

	dac_ctrla = DAC->CTRLA;
	dac_ctrlb = DAC->CTRLB;

	if (DAC->CTRLB.reg != 0x01) {
		error(2);
	}

	if (DAC->CTRLA.reg != 0x02) {
		error(1);
	}

	DAC->CTRLB.reg = 0x01;
	DAC->CTRLB.reg = 0x01;
	if (DAC->STATUS.reg & (1 << 7)) {
		error(3);
	}
	 */

	dac_ch_get_config_defaults(&ch_config);

	ch_config.enable_start_on_event = false;

	dac_ch_set_config(&dev_inst, DAC_CHANNEL_0, &ch_config);

	dac_ch_enable(&dev_inst, DAC_CHANNEL_0);

	dac_write(&dev_inst, DAC_CHANNEL_0, 0x3FF, false);

	while (1) {
		for (uint8_t i = 0; i < 0xFF; i++) {
			dac_write(&dev_inst, DAC_CHANNEL_0, (sine_wave[i] << 2), false);
		}

	}

}
