/**
 * \file
 *
 * \brief Unit tests for MegaRF IOPORT service
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

/**
 * \mainpage
 *
 * \section intro Introduction
 * This is the unit test for the IOPORT service. The IO Pin is
 * tested by setting high and low and read back using another
 * IO Pin by shorting the pins.
 *
 * \section files Main Files
 * - \ref unit_tests.c
 * - \ref conf_test.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 * - \ref conf_usart_serial.h
 *
 * \section device_info Device Info
 * This example has been tested with the following setup(s):
 * - atmega128rfa1_stk600 and atmega256rfr2_xplained_pro
 *
 * \section description Description of the unit tests
 * See the documentation for the individual unit test functions
 * \ref unit_tests.c "here" for detailed descriptions of the tests.
 *
 * \section dependencies Dependencies
 * This example depends directly on the following modules:
 * - \ref test_suite_group
 * - \ref megarf_usart_group
 * - \ref megarf_ioport_group
 *
 * \section compinfo Compilation info
 * This software was written for the GNU GCC and IAR for AVR. Other compilers
 * may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit <a href="http://www.atmel.com/">Atmel</a>.\n
 */

/**
 * \brief Test IOPORT pin level is getting changed.
 *
 * This function set the direction of CONF_OUT_PIN to output mode with pull-up enabled
 * and read the status of pin using CONF_IN_PIN which is configured in input mode.
 * The pin CONF_OUT_PIN and CONF_IN_PIN are shorted using a jumper.
 *
 * \param test Current test case.
 */
static void run_ioport_pin_test(const struct test_case *test)
{
	static volatile pin_mask_t pin_val;

	/* Set output direction on the given IO Pin */
	ioport_set_pin_dir(CONF_OUT_PIN, IOPORT_DIR_OUTPUT);

	/* Set direction and pull-up on the given IO Pin */
	ioport_set_pin_dir(CONF_IN_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(CONF_IN_PIN, IOPORT_MODE_PULLUP);

	/* Set IO pin as high */
	ioport_set_pin_level(CONF_OUT_PIN, IOPORT_PIN_LEVEL_HIGH);
	delay_ms(10);
	pin_val = ioport_get_pin_level(CONF_IN_PIN);
	test_assert_true(test, pin_val == 1,
			"IOPORT Set pin level high test failed");

	/* Set IO pin as low */
	ioport_set_pin_level(CONF_OUT_PIN, IOPORT_PIN_LEVEL_LOW);
	delay_ms(10);
	pin_val = ioport_get_pin_level(CONF_IN_PIN);
	test_assert_true(test, pin_val == 0,
			"IOPORT Set pin level low test failed");

	/* Toggle IO pin */
	ioport_toggle_pin_level(CONF_OUT_PIN);
	delay_ms(10);
	pin_val = ioport_get_pin_level(CONF_IN_PIN);
	test_assert_true(test, pin_val == 1,
			"IOPORT Set pin level toggle test failed");
}

/**
 * \brief Test IOPORT port level is getting changed.
 *
 * This function set the direction of CONF_PORT_OUT_PIN_MASK to output mode with pull-up enabled
 * and read the status of pin using CONF_PORT_IN_PIN_MASK which is configured in input mode.
 * The pin CONF_PORT_OUT_PIN_MASK and CONF_PORT_IN_PIN_MASK are shorted using a jumper.
 *
 * \param test Current test case.
 */
static void run_ioport_port_test(const struct test_case *test)
{
	static volatile pin_mask_t port_val;

	/* Set direction and pull-up on the given IOPORT */
	ioport_set_port_dir(CONF_PORT, CONF_PORT_IN_PIN_MASK, IOPORT_DIR_INPUT);
	ioport_set_port_mode(CONF_PORT, CONF_PORT_IN_PIN_MASK, IOPORT_MODE_PULLUP);

	/* Set output direction on the given IOPORT */
	ioport_set_port_dir(CONF_PORT, CONF_PORT_OUT_PIN_MASK, IOPORT_DIR_OUTPUT);

	/* Set  IOPORT as high */
	ioport_set_port_level(CONF_PORT, CONF_PORT_OUT_PIN_MASK, IOPORT_PIN_LEVEL_HIGH);
	delay_ms(10);
	port_val = ioport_get_port_level(CONF_PORT, CONF_PORT_IN_PIN_MASK);
	test_assert_true(test, port_val == CONF_PORT_IN_PIN_MASK, "IOPORT Set port level high test failed.");
	
	/* Set  IOPORT as low */
	ioport_set_port_level(CONF_PORT, CONF_PORT_OUT_PIN_MASK, IOPORT_PIN_LEVEL_LOW);
	delay_ms(10);
	port_val = ioport_get_port_level(CONF_PORT, CONF_PORT_IN_PIN_MASK);
	test_assert_true(test, port_val == 0, "IOPORT Set port level lowtest failed.");

	/* Toggle  IOPORT */
	ioport_toggle_port_level(CONF_PORT, CONF_PORT_OUT_PIN_MASK);
	delay_ms(10);
	port_val = ioport_get_port_level(CONF_PORT, CONF_PORT_IN_PIN_MASK);
	test_assert_true(test, port_val == CONF_PORT_IN_PIN_MASK, "IOPORT Set port level toggle test failed.");
}

/**
 * \brief Run IOPORT unit tests
 *
 * Initializes the clock system, board and serial output, then sets up the
 * IOPORT Service unit test suite and runs it.
 */
int main(void)
{
	const usart_serial_options_t usart_serial_options = {
		.baudrate   = CONF_TEST_BAUDRATE,
		.charlength = CONF_TEST_CHARLENGTH,
		.paritytype = CONF_TEST_PARITY,
		.stopbits   = CONF_TEST_STOPBITS,
	};

	board_init();
	sysclk_init();
	stdio_serial_init(CONF_TEST_USART, &usart_serial_options);

	DEFINE_TEST_CASE(ioport_port_test, NULL, run_ioport_port_test, NULL,
			"Test setting of various IOPORT functions");
	DEFINE_TEST_CASE(ioport_pin_test, NULL, run_ioport_pin_test, NULL,
			"Test IOPORT pin level is getting changed");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(ioport_test) = {
		&ioport_port_test,
		&ioport_pin_test,
	};

	/* Define the test suite */
	DEFINE_TEST_SUITE(ioport_suite, ioport_test,
			"MEGARF IOPORT test suite");

	/* Run all tests in the suite */
	test_suite_run(&ioport_suite);

	while (1) {
		/* Intentionally left empty. */
	}
}
