/**
 * \file
 *
 * \brief Unit tests for I2S driver.
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

#include <asf.h>
#include <conf_test.h>
#include <string.h>

/**
 * \mainpage
 *
 * \section intro Introduction
 * This is the unit test application for the I2SC driver.
 * It contains one test case for the I2SC module:
 * - Test audio data by set loop back mode.
 *
 * \note The delay between line 212 to 214 which is before enabling the I2SC
 * receiver is depend on the clock timing. So in different boards and MCK,
 * the delay need to be fine tune to fit the timing.
 *
 * \section files Main Files
 * - \ref unit_tests.c
 * - \ref conf_test.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 * - \ref conf_uart_serial.h
 * - \ref conf_sleepmgr.h
 *
 * \section device_info Device Info
 * SAM devices with I2SC module can be used.
 * This example has been tested with the following setup:
 * - samg53_stk600_samg53
 *
 * \section compinfo Compilation info
 * This software was written for the GNU GCC and IAR for ARM. Other compilers
 * may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit <a href="http://www.atmel.com/">Atmel</a>.\n
 * Support and FAQ: http://support.atmel.no/
 */

#define SOUND_SAMPLES 0x100
int16_t output_samples_left[SOUND_SAMPLES] = {
	0x7F, 0x7F, 0x7D, 0x7E, 0x7D, 0x7E, 0x7D, 0x7E,
	0x7D, 0x7D, 0x7D, 0x7F, 0x7E, 0x7D, 0x7E, 0x7D,
	0x7D, 0x7D, 0x7C, 0x7A, 0x7B, 0x7C, 0x7A, 0x7A,
	0x7C, 0x7B, 0x7E, 0x7F, 0x7F, 0x7F, 0x80, 0x80,
	0x81, 0x82, 0x82, 0x83, 0x83, 0x83, 0x84, 0x84,
	0x86, 0x83, 0x81, 0x81, 0x83, 0x83, 0x83, 0x84,
	0x82, 0x84, 0x84, 0x83, 0x85, 0x85, 0x82, 0x83,
	0x82, 0x82, 0x82, 0x82, 0x7F, 0x80, 0x81, 0x7E,
	0x7E, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7E,
	0x7C, 0x7E, 0x7E, 0x7F, 0x7F, 0x7D, 0x7D, 0x7C,
	0x7D, 0x7D, 0x7D, 0x7B, 0x7C, 0x7B, 0x7C, 0x7D,
	0x7E, 0x7E, 0x7F, 0x7E, 0x7D, 0x7F, 0x7E, 0x7D,
	0x7D, 0x7B, 0x7D, 0x7D, 0x7E, 0x7D, 0x7D, 0x7E,
	0x7D, 0x7D, 0x7D, 0x7E, 0x7E, 0x7C, 0x7E, 0x7E,
	0x7F, 0x7F, 0x7E, 0x7E, 0x7F, 0x7F, 0x80, 0x81,
	0x7F, 0x80, 0x81, 0x80, 0x81, 0x81, 0x81, 0x81,
	0x82, 0x81, 0x82, 0x82, 0x81, 0x80, 0x7F, 0x80,
	0x7F, 0x7F, 0x7E, 0x80, 0x81, 0x82, 0x83, 0x82,
	0x83, 0x84, 0x81, 0x82, 0x82, 0x81, 0x82, 0x81,
	0x80, 0x80, 0x82, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x7F, 0x80, 0x7F, 0x80, 0x81, 0x80,
	0x80, 0x7F, 0x7F, 0x80, 0x81, 0x80, 0x80, 0x7E,
	0x7E, 0x80, 0x7F, 0x7F, 0x80, 0x80, 0x7F, 0x7F,
	0x80, 0x80, 0x81, 0x7E, 0x7F, 0x80, 0x7E, 0x7E,
	0x7E, 0x7F, 0x7E, 0x7E, 0x7E, 0x7C, 0x7D, 0x7C,
	0x81, 0x7D, 0x7C, 0x7C, 0x7B, 0x7D, 0x7C, 0x7D,
	0x7D, 0x7D, 0x7B, 0x7D, 0x80, 0x80, 0x82, 0x80,
	0x7F, 0x80, 0x83, 0x82, 0x80, 0x82, 0x84, 0x86,
	0x86, 0x84, 0x84, 0x86, 0x87, 0x84, 0x85, 0x85,
	0x85, 0x85, 0x86, 0x85, 0x85, 0x84, 0x83, 0x80,
	0x81, 0x82, 0x83, 0x7F, 0x7E, 0x7F, 0x7F, 0x80,
	0x7E, 0x7E, 0x7E, 0x7C, 0x7C, 0x7D, 0x7D, 0x7C
};
int16_t output_samples_right[SOUND_SAMPLES] = {
	0x5A, 0x7F, 0x7D, 0x7E, 0x7D, 0x7E, 0x7D, 0x7E,
	0x7D, 0x7D, 0x7D, 0x7F, 0x7E, 0x7D, 0x7E, 0x7D,
	0x7D, 0x7D, 0x7C, 0x7A, 0x7B, 0x7C, 0x7A, 0x7A,
	0x7C, 0x7B, 0x7E, 0x7F, 0x7F, 0x7F, 0x80, 0x80,
	0x81, 0x82, 0x82, 0x83, 0x83, 0x83, 0x84, 0x84,
	0x86, 0x83, 0x81, 0x81, 0x83, 0x83, 0x83, 0x84,
	0x82, 0x84, 0x84, 0x83, 0x85, 0x85, 0x82, 0x83,
	0x82, 0x82, 0x82, 0x82, 0x7F, 0x80, 0x81, 0x7E,
	0x7E, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7E,
	0x7C, 0x7E, 0x7E, 0x7F, 0x7F, 0x7D, 0x7D, 0x7C,
	0x7D, 0x7D, 0x7D, 0x7B, 0x7C, 0x7B, 0x7C, 0x7D,
	0x7E, 0x7E, 0x7F, 0x7E, 0x7D, 0x7F, 0x7E, 0x7D,
	0x7D, 0x7B, 0x7D, 0x7D, 0x7E, 0x7D, 0x7D, 0x7E,
	0x7D, 0x7D, 0x7D, 0x7E, 0x7E, 0x7C, 0x7E, 0x7E,
	0x7F, 0x7F, 0x7E, 0x7E, 0x7F, 0x7F, 0x80, 0x81,
	0x7F, 0x80, 0x81, 0x80, 0x81, 0x81, 0x81, 0x81,
	0x82, 0x81, 0x82, 0x82, 0x81, 0x80, 0x7F, 0x80,
	0x7F, 0x7F, 0x7E, 0x80, 0x81, 0x82, 0x83, 0x82,
	0x83, 0x84, 0x81, 0x82, 0x82, 0x81, 0x82, 0x81,
	0x80, 0x80, 0x82, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x7F, 0x80, 0x7F, 0x80, 0x81, 0x80,
	0x80, 0x7F, 0x7F, 0x80, 0x81, 0x80, 0x80, 0x7E,
	0x7E, 0x80, 0x7F, 0x7F, 0x80, 0x80, 0x7F, 0x7F,
	0x80, 0x80, 0x81, 0x7E, 0x7F, 0x80, 0x7E, 0x7E,
	0x7E, 0x7F, 0x7E, 0x7E, 0x7E, 0x7C, 0x7D, 0x7C,
	0x81, 0x7D, 0x7C, 0x7C, 0x7B, 0x7D, 0x7C, 0x7D,
	0x7D, 0x7D, 0x7B, 0x7D, 0x80, 0x80, 0x82, 0x80,
	0x7F, 0x80, 0x83, 0x82, 0x80, 0x82, 0x84, 0x86,
	0x86, 0x84, 0x84, 0x86, 0x87, 0x84, 0x85, 0x85,
	0x85, 0x85, 0x86, 0x85, 0x85, 0x84, 0x83, 0x80,
	0x81, 0x82, 0x83, 0x7F, 0x7E, 0x7F, 0x7F, 0x80,
	0x7E, 0x7E, 0x7E, 0x7C, 0x7C, 0x7D, 0x7D, 0xA5
};
int16_t input_samples_left[SOUND_SAMPLES];
int16_t input_samples_right[SOUND_SAMPLES];

/* Flag indicate */
volatile uint32_t flag = true;

/**
 * \brief Test audio data transfer and receive.
 *
 * \param test Current test case.
 */
static void run_i2s_test(const struct test_case *test)
{
	uint32_t i;
	struct i2s_config config;
	struct i2s_dev_inst dev_inst;
	Pdc *p_i2sc_pdc;
	Pdc *p_i2sc_pdc2;
	pdc_packet_t pdc_i2sc_packet_tx, pdc_i2sc_packet_rx;
	pdc_packet_t pdc2_i2sc_packet_tx, pdc2_i2sc_packet_rx;

	/* Set the configuration */
	i2s_get_config_defaults(&config);
	config.data_format = I2S_DATE_16BIT;
	config.fs_ratio = I2S_FS_RATE_256;
	config.loopback = true;
	i2s_init(&dev_inst, I2SC0, &config);

	/* Enable the I2SC module. */
	i2s_enable(&dev_inst);

	/* Get pointer to I2SC PDC register base */
	p_i2sc_pdc = i2s_get_pdc_base(&dev_inst);
	p_i2sc_pdc2 = (Pdc *)((uint32_t)p_i2sc_pdc + 0x100U);
	/* Initialize PDC data packet for transfer */
	pdc_i2sc_packet_tx.ul_addr = (uint32_t) output_samples_left;
	pdc_i2sc_packet_tx.ul_size = SOUND_SAMPLES;
	pdc_i2sc_packet_rx.ul_addr = (uint32_t) input_samples_left;
	pdc_i2sc_packet_rx.ul_size = SOUND_SAMPLES;
	pdc2_i2sc_packet_tx.ul_addr = (uint32_t) output_samples_right;
	pdc2_i2sc_packet_tx.ul_size = SOUND_SAMPLES;
	pdc2_i2sc_packet_rx.ul_addr = (uint32_t) input_samples_right;
	pdc2_i2sc_packet_rx.ul_size = SOUND_SAMPLES;
	/* Configure PDC for data transfer */
	pdc_tx_init(p_i2sc_pdc, &pdc_i2sc_packet_tx, NULL);
	pdc_rx_init(p_i2sc_pdc, &pdc_i2sc_packet_rx, NULL);
	pdc_tx_init(p_i2sc_pdc2, &pdc2_i2sc_packet_tx, NULL);
	pdc_rx_init(p_i2sc_pdc2, &pdc2_i2sc_packet_rx, NULL);
	/* Enable PDC transfers */
	pdc_enable_transfer(p_i2sc_pdc, PERIPH_PTCR_TXTEN | PERIPH_PTCR_RXTEN);
	pdc_enable_transfer(p_i2sc_pdc2, PERIPH_PTCR_TXTEN | PERIPH_PTCR_RXTEN);

	/* Enable the functions */
	i2s_enable_transmission(&dev_inst);
	i2s_enable_clocks(&dev_inst);

	/**
	 * Since the transfer and receive timing is not under control, we
	 * need adjust here the enable sequence and add some delay
	 * functions if it's needed.
	 */
	for (i = 0; i < 0x4; i++) {
		input_samples_left[i] = i;
	}

	i2s_enable_reception(&dev_inst);

	/* Wait transfer complete */
	while (!(i2s_get_status(&dev_inst) & I2SC_SR_RXBUFF)) {
	}

	/**
	 * Wait a moment to let the PDC finish. The status bit is cleared
	 * before all transfer finish.
	 */
	delay_us(10);

	/* Disable the PDC module. */
	pdc_disable_transfer(p_i2sc_pdc, PERIPH_PTCR_RXTDIS| PERIPH_PTCR_TXTDIS);
	pdc_disable_transfer(p_i2sc_pdc2, PERIPH_PTCR_RXTDIS| PERIPH_PTCR_TXTDIS);

	/* Disable the I2SC module. */
	i2s_disable(&dev_inst);

	/* Compare the data. */
	for (i = 0; i < SOUND_SAMPLES; i++) {
		if ((input_samples_left[i] != output_samples_left[i]) ||
			(input_samples_right[i] != output_samples_right[i])) {
			flag = false;
		}
	}

	test_assert_true(test, flag == true, "Audio data did not match!");
}

/**
 * \brief Run I2SC driver unit tests.
 */
int main(void)
{
	sysclk_init();
	board_init();

	const usart_serial_options_t usart_serial_options = {
		.baudrate = CONF_TEST_BAUDRATE,
		.paritytype = CONF_TEST_PARITY,
	};

	stdio_serial_init(CONF_TEST_USART, &usart_serial_options);

	/* Define all the test cases. */
	DEFINE_TEST_CASE(i2s_test, NULL, run_i2s_test, NULL,
			"SAM I2S transfer test");

	/* Put test case addresses in an array. */
	DEFINE_TEST_ARRAY(i2s_tests) = {
		&i2s_test,
	};

	/* Define the test suite. */
	DEFINE_TEST_SUITE(i2s_suite, i2s_tests, "SAM I2S driver test suite");

	/* Run all tests in the test suite. */
	test_suite_run(&i2s_suite);

	while (1) {
		/* Busy-wait forever. */
	}
}
