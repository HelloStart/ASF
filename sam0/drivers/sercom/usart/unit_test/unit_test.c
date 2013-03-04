/**
 * \file
 *
 * \brief SAMD20 USART Unit test
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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
#include <stdio_serial.h>
#include <string.h>

/* USART for STDIO */
#define CONF_RX_USART      EDBG_CDC_MODULE
/* RX USART to test */
#define RX_USART           EXT1_UART_MODULE
/* TX USART to test */
#define TX_USART           EXT2_UART_MODULE

/* Test string to send */
#define TEST_STRING        "Hello world!"
/* Length of test string */
#define TEST_STRING_LEN    13

/* Speed to test USART at */
#define TEST_USART_SPEED   115200

struct usart_module usart_rx_module;
struct usart_config usart_rx_config;
struct usart_module usart_tx_module;
struct usart_config usart_tx_config;

volatile bool transfer_complete;


/**
 * \internal
 * \brief USART interrupt callback function
 *
 * Called by USART driver when receiving is complete.
 *
 * * \param module USART module causing the interrupt (not used)
 */
static void usart_callback(const struct usart_module *const module)
{
	transfer_complete = true;
}


/**
 * \internal
 * \brief Test single 8-bit character send and receive.
 *
 * This test sends on 9-bit character and checks it's received properly
 * on the other end.
 *
 * \param test Current test case.
 */
static void run_transfer_single_8bit_char_test(const struct test_case *test)
{
	volatile uint16_t tx_char = 0x53;
	volatile uint16_t rx_char = 0;

	/* Write and read the data */
	usart_write_wait(&usart_tx_module, tx_char);
	usart_read_wait(&usart_rx_module, (uint16_t*)&rx_char);

	/* Make sure we received what we sent */
	test_assert_true(test, tx_char==rx_char,
			"Failed receiving sent byte. TX=%d, RX=%d", tx_char, rx_char);

}

/**
 * \internal
 * \brief Test single 9-bit character send and receive.
 *
 * This test sends one 9-bit character and checks it's received properly
 * on the other end.
 *
 * \param test Current test case.
 */
static void run_transfer_single_9bit_char_test(const struct test_case *test)
{
	uint16_t tx_char = 0x166;
	uint16_t rx_char;

	/* Re-configure RX USART to operate with 9 bit character size */
	usart_disable(&usart_rx_module);
	usart_rx_config.char_size = USART_CHAR_SIZE_9BIT;
	usart_init(&usart_rx_module, RX_USART,	&usart_rx_config);
	usart_enable(&usart_rx_module);
	usart_enable_transceiver(&usart_rx_module, USART_TRANSCEIVER_RX);

	/* Re-configure TX USART to operate with 9 bit character size */
	usart_disable(&usart_tx_module);
	usart_tx_config.char_size = USART_CHAR_SIZE_9BIT;
	usart_init(&usart_tx_module, TX_USART,	&usart_tx_config);
	usart_enable(&usart_tx_module);
	usart_enable_transceiver(&usart_tx_module, USART_TRANSCEIVER_TX);

	/* Write and read the data */
	usart_write_wait(&usart_tx_module, tx_char);
	usart_read_wait(&usart_rx_module, &rx_char);

	/* Make sure we received what we sent */
	test_assert_true(test, tx_char==rx_char,
			"Failed receiving sent byte. TX=%d, RX=%d", tx_char, rx_char);

	/* Put RX USART back in 8 bit mode */
	usart_disable(&usart_rx_module);
	usart_rx_config.char_size = USART_CHAR_SIZE_8BIT;
	usart_init(&usart_rx_module, RX_USART,	&usart_rx_config);
	usart_enable(&usart_rx_module);
	usart_enable_transceiver(&usart_rx_module, USART_TRANSCEIVER_RX);

	/* Put TX USART back in 8 bit mode */
	usart_disable(&usart_tx_module);
	usart_tx_config.char_size = USART_CHAR_SIZE_8BIT;
	usart_init(&usart_tx_module, TX_USART,	&usart_tx_config);
	usart_enable(&usart_tx_module);
	usart_enable_transceiver(&usart_tx_module, USART_TRANSCEIVER_TX);
}


/**
 * \internal
 * \brief Test sending (blocking) and receiving (interrupt) a string.
 *
 * This test sends one string from one USART to another.
 * It's sent using a blocking write, while it's received using interrupts.
 *
 * \param test Current test case.
 */
static void run_buffer_write_blocking_read_interrupt_test
		(const struct test_case *test)
{

	uint8_t tx_string[TEST_STRING_LEN] = TEST_STRING;
	volatile uint8_t rx_string[TEST_STRING_LEN] = "";
	int16_t result;

	/* We will read back the buffer using interrupt */
	usart_register_callback(&usart_rx_module, usart_callback,
			USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&usart_rx_module, USART_CALLBACK_BUFFER_RECEIVED);

	/* Start receiving */
	transfer_complete = false;
	usart_read_buffer_job(&usart_rx_module, (uint8_t*)rx_string,
			TEST_STRING_LEN);

	/* Send the string */
	usart_write_buffer_wait(&usart_tx_module, tx_string,
			TEST_STRING_LEN );

	/* Wait for receive to complete */
	while (!transfer_complete);

	usart_disable_callback(&usart_rx_module, USART_CALLBACK_BUFFER_RECEIVED);
	usart_unregister_callback(&usart_rx_module, USART_CALLBACK_BUFFER_RECEIVED);

	/* Compare strings */
	result = strcmp((char*)tx_string, (char*)rx_string);

	/* Make sure we 0-terminate the received string in case it's
		gibberish and we have to print it */
	rx_string[TEST_STRING_LEN-1] = 0;

	test_assert_false(test, result,
			"Failed receiving string. TX='%s', RX='%s'", tx_string, rx_string);
}


/**
 * \internal
 * \brief Test sending and receiving a string using interrupts.
 *
 * This test sends one string from one USART to another.
 * Both the send and receive is done using interrupts.
 *
 * \param test Current test case.
 */
static void run_buffer_read_write_interrupt_test(const struct test_case *test)
{

	volatile uint8_t tx_string[TEST_STRING_LEN] = TEST_STRING;
	volatile uint8_t rx_string[TEST_STRING_LEN] = "";
	int16_t result;

	usart_register_callback(&usart_rx_module, usart_callback,
			USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&usart_rx_module, USART_CALLBACK_BUFFER_RECEIVED);

	/* start send */
	transfer_complete = false;

	usart_write_buffer_job(&usart_tx_module, (uint8_t*)tx_string,
			TEST_STRING_LEN);
	usart_read_buffer_job(&usart_rx_module, (uint8_t*)rx_string,
			TEST_STRING_LEN);

	/* Wait for receive to complete */
	while (!transfer_complete);

	/* Compare strings */
	result = strcmp((char*)tx_string, (char*)rx_string);

	/* Make sure we 0-terminate the received string in case it's
		gibberish and we have to print it */
	rx_string[TEST_STRING_LEN-1] = 0;

	test_assert_false(test, result,
			"Failed receiving string. TX='%s', RX='%s'", tx_string, rx_string);

}

/**
 * \brief Initialize USARTs for unit tests
 *
 * Initializes the three USARTs used by the unit test. One for
 * outputting the results (using embedded debugger) and two for
 * the actual unit tests (one for RX and one for TX).
 *
 * The RX USART used is the one connected to EXT1 on the SAMD20 Xplained Pro.
 * The TX USART used is the one connected to EXT2 on the SAMD20 Xplained Pro.
 * RX is on pin 13 while TX is on pin 14. So for this unit test pin 13 on EXT1
 * should be connected to pin 14 on EXT2.
 *
 */
static void test_system_init(void)
{
	struct usart_config usart_conf;
	struct usart_module unit_test_output;

	/* Configure USART for unit test output */
	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_settings = USART_RX_3_TX_2_XCK_3;
	usart_conf.pinout_pad3 = EDBG_CDC_RX_PINMUX;
	usart_conf.pinout_pad2 = EDBG_CDC_TX_PINMUX;
	usart_conf.baudrate = 38400;

	stdio_serial_init(&unit_test_output, CONF_RX_USART, &usart_conf);
	usart_enable(&unit_test_output);
	/* Enable transceivers */
	usart_enable_transceiver(&unit_test_output, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&unit_test_output, USART_TRANSCEIVER_RX);

	/* Configure RX USART */
	usart_get_config_defaults(&usart_rx_config);
	usart_rx_config.mux_settings = USART_RX_1_TX_0_XCK_1;
	usart_rx_config.pinout_pad1 = PINMUX_PA11B_SERCOM0_PAD1;
	usart_rx_config.baudrate = TEST_USART_SPEED;
	/* Apply configuration */
	usart_init(&usart_rx_module, RX_USART, &usart_rx_config);
	/* Enable USART */
	usart_enable(&usart_rx_module);

	/* Configure TX USART */
	usart_get_config_defaults(&usart_tx_config);
	usart_tx_config.mux_settings = USART_RX_1_TX_0_XCK_1;
	usart_tx_config.pinout_pad0 = PINMUX_PA24B_SERCOM3_PAD0;
	usart_tx_config.baudrate = TEST_USART_SPEED;
	/* Apply configuration */
	usart_init(&usart_tx_module, TX_USART, &usart_tx_config);
	/* Enable USART */
	usart_enable(&usart_tx_module);

	/* Enable transceivers */
	usart_enable_transceiver(&usart_tx_module, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&usart_rx_module, USART_TRANSCEIVER_RX);
}


/**
 * \brief Run USART unit tests
 *
 * Initializes the system and serial output, then sets up the
 * USART unit test suite and runs it.
 */
int main(void)
{
	system_init();
	test_system_init();

	/* Define Test Cases */
	DEFINE_TEST_CASE(transfer_single_8bit_char_test, NULL,
			run_transfer_single_8bit_char_test, NULL,
			"Transfer single 8-bit character");

	DEFINE_TEST_CASE(transfer_single_9bit_char_test, NULL,
			run_transfer_single_9bit_char_test, NULL,
			"Transfer single 9-bit character");

	DEFINE_TEST_CASE(transfer_buffer_test, NULL,
			run_buffer_write_blocking_read_interrupt_test, NULL,
			"Buffer write blocking/read interrupt");

	DEFINE_TEST_CASE(receive_buffer_test, NULL,
			run_buffer_read_write_interrupt_test, NULL,
			"Buffer interrupt read and write");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(usart_tests) = {
			&transfer_single_8bit_char_test,
			&transfer_single_9bit_char_test,
			&transfer_buffer_test,
			&receive_buffer_test,
			};

	/* Define the test suite */
	DEFINE_TEST_SUITE(usart_suite, usart_tests,
			"SAM0 USART driver test suite");

	/* Run all tests in the suite*/
	test_suite_run(&usart_suite);

	while(1) {
		/* Intentionally left empty */
	}

}
