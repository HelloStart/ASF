/**
 * \file
 *
 * \brief Unit tests for GMAC driver.
 *
 * Copyright (c) 2013-2015 Atmel Corporation. All rights reserved.
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
#include "board.h"
#include <sysclk.h>
#include <string.h>
#include <unit_test/suite.h>
#include <stdio_serial.h>
#include "conf_test.h"
#include "conf_board.h"
#include "mini_ip.h"
#include "conf_eth.h"

/**
 * \mainpage
 *
 * \section intro Introduction
 * This is the unit test application for the GMAC driver.
 * It consists of test cases for the following functionalities:
 * - Write an arp frame through the GMAC
 * - Read the GMAC buffer and check if the frame sent is received
 *
 * \section files Main Files
 * - \ref unit_tests.c
 * - \ref conf_test.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 * - \ref conf_usart_serial.h
 *
 * \section device_info Device Info
 * All SAM devices can be used.
 * This example has been tested with the following setup:
 * - sam4e16e_sam4e_ek
 * - samv71q21_samv71_xplained_ultra
 * - same70q21_same70_xplained
 *
 * \section compinfo Compilation info
 * This software was written for the GNU GCC and IAR for ARM. Other compilers
 * may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit <a href="http://www.atmel.com/">Atmel</a>.\n
 * Support and FAQ: http://www.atmel.com/design-support/
 */

//! \name Unit test configuration
//@{
/**
 * \def CONF_TEST_GMAC
 * \brief Test the functions provided by the GMAC driver.
 */
//@}

/** The GMAC driver instance */
static gmac_device_t gs_gmac_dev;

/** Buffer for ethernet packets */
static uint8_t gs_uc_eth_buffer[GMAC_FRAME_LENTGH_MAX];

static uint8_t gs_arp_frame[] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	ETHERNET_CONF_ETHADDR0, ETHERNET_CONF_ETHADDR1, ETHERNET_CONF_ETHADDR2,
	ETHERNET_CONF_ETHADDR3, ETHERNET_CONF_ETHADDR4,
	ETHERNET_CONF_ETHADDR5, 0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01,
	ETHERNET_CONF_ETHADDR0, ETHERNET_CONF_ETHADDR1, ETHERNET_CONF_ETHADDR2,
	ETHERNET_CONF_ETHADDR3, ETHERNET_CONF_ETHADDR4,
	ETHERNET_CONF_ETHADDR5,
	ETHERNET_CONF_IPADDR0, ETHERNET_CONF_IPADDR1, ETHERNET_CONF_IPADDR2,
	ETHERNET_CONF_IPADDR3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	ETHERNET_CONF_GATEWAY_ADDR0, ETHERNET_CONF_GATEWAY_ADDR1,
	ETHERNET_CONF_GATEWAY_ADDR2, ETHERNET_CONF_GATEWAY_ADDR3
};

/** The MAC address used for the test */
static uint8_t gs_uc_mac_address[] =
		{ ETHERNET_CONF_ETHADDR0, ETHERNET_CONF_ETHADDR1, ETHERNET_CONF_ETHADDR2,
			ETHERNET_CONF_ETHADDR3, ETHERNET_CONF_ETHADDR4, ETHERNET_CONF_ETHADDR5
};

#define GMAC_UINT_TEST_MAX_RETRY_TIME    50

/**
 * \brief GMAC interrupt handler.
 */
void GMAC_Handler(void)
{
#if (SAM4E)
	gmac_handler(&gs_gmac_dev);
#else
	gmac_handler(&gs_gmac_dev, GMAC_QUE_0);
#endif
}

/**
 * \brief GMAC link test function.
 *
 * \param test Current test case.
 */
static void run_gmac_link_test(const struct test_case *test)
{
	gmac_options_t gmac_option;
	uint8_t uc_rc = 1;

	/* Enable GMAC clock */
	pmc_enable_periph_clk(ID_GMAC);

	/* Fill in GMAC options */
	gmac_option.uc_copy_all_frame = 1;
	gmac_option.uc_no_boardcast = 1;

	memcpy(gmac_option.uc_mac_addr, gs_uc_mac_address, sizeof(gs_uc_mac_address));

	gs_gmac_dev.p_hw = GMAC;

	/* Init GMAC driver structure */
	gmac_dev_init(GMAC, &gs_gmac_dev, &gmac_option);

	/* Enable Interrupt */
	NVIC_EnableIRQ(GMAC_IRQn);

	/* Init MAC PHY driver */
	if (ethernet_phy_init(GMAC, BOARD_GMAC_PHY_ADDR, sysclk_get_cpu_hz())
			!= GMAC_OK) {
		uc_rc = 0;
	}

	/* Auto Negotiate, work in RMII mode */
	if (ethernet_phy_auto_negotiate(GMAC, BOARD_GMAC_PHY_ADDR) != GMAC_OK) {
		uc_rc = 0;

	}

	/* Establish ethernet link */
	while (ethernet_phy_set_link(GMAC, BOARD_GMAC_PHY_ADDR, 1) != GMAC_OK) {
		uc_rc = 0;
	}

	test_assert_true(test, uc_rc == 1, "Test GMAC: GMAC link test error!");
}

/**
 * \brief Test GMAC read/write interfaces.
 *
 * \param test Current test case.
 */
static void run_gmac_read_write_test(const struct test_case *test)
{
	uint32_t ul_frm_size;
	uint8_t uc_rc = 0;
	uint32_t ul_retry_time = 0;

	/* Write the arp frame first */
#if (SAM4E)
	gmac_dev_write(&gs_gmac_dev, gs_arp_frame, sizeof(gs_arp_frame), NULL);
#else
	gmac_dev_write(&gs_gmac_dev, GMAC_QUE_0, gs_arp_frame, sizeof(gs_arp_frame), NULL);
#endif

	while ( ul_retry_time < GMAC_UINT_TEST_MAX_RETRY_TIME ) {
		/* Read the frame in the gmac buffer */
#if (SAM4E)
		if (GMAC_OK != gmac_dev_read(&gs_gmac_dev, (uint8_t *) gs_uc_eth_buffer,
						sizeof(gs_uc_eth_buffer), &ul_frm_size)) {
#else
		if (GMAC_OK != gmac_dev_read(&gs_gmac_dev, GMAC_QUE_0, (uint8_t *) gs_uc_eth_buffer,
						sizeof(gs_uc_eth_buffer), &ul_frm_size)) {
#endif
			continue;
		}

		/* Is arp frame sent? */
		if (strncmp((const char *)gs_uc_eth_buffer, (const char *)gs_uc_mac_address, sizeof(gs_uc_mac_address))) {
			uc_rc = 1;
			break;
		}

		ul_retry_time++;
	}

	test_assert_true(test, uc_rc == 1, "Test GMAC: gmac write error!");
}

/**
 * \brief Run GMAC driver unit tests.
 */
int main(void)
{
	const usart_serial_options_t usart_serial_options = {
		.baudrate = CONF_TEST_BAUDRATE,
#ifdef CONF_TEST_CHAR_LENGTH
        .charlength = CONF_TEST_CHAR_LENGTH,
#endif
        .paritytype = CONF_TEST_PARITY,
#ifdef CONF_TEST_STOP_BITS
        .stopbits = CONF_TEST_STOP_BITS,
#endif
	};

	/* Initialize the system clock and board */
	sysclk_init();
	board_init();

	/* Enable the debug uart */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_TEST_USART, &usart_serial_options);

	/* Define all the test cases */
	DEFINE_TEST_CASE(gmac_link_test, NULL, run_gmac_link_test, NULL,
			"GMAC link test");

	/* Define all the test cases */
	DEFINE_TEST_CASE(gmac_rw_test, NULL, run_gmac_read_write_test, NULL,
			"GMAC read/write test");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(gmac_tests) = {
	&gmac_link_test,
	&gmac_rw_test};

	/* Define the test suite */
	DEFINE_TEST_SUITE(gmac_suite, gmac_tests, "SAM GMAC driver test suite");

	/* Run all tests in the test suite */
	test_suite_run(&gmac_suite);

	while (1) {
		/* Busy-wait forever */
	}
}
