/**
 * \file
 *
 * \brief AFEC enhanced resolution mode example for SAM.
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

/**
 * \mainpage AFEC Enhanced Resolution Example
 *
 * \section Purpose
 *
 * The example demonstrates how to use the enhanced resolution mode
 * inside the microcontroller to sample analog voltage.
 *
 * \section Requirements
 *
 * This example can be used on sam4e-ek boards.
 *
 * \section Description
 *
 * The adc_temp_sensor is aimed to demonstrate the temperature sensor feature
 * inside the device. To use this feature, the temperature sensor should be
 * turned on by setting TSON bit in ADC_ACR. The channel 15 is connected to the
 * sensor by default. With PDC support, the Interrupt Handler of ADC is designed
 * to handle RXBUFF interrupt.
 *
 * \section Usage
 *
 * -# Build the program and download it into the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/6421B.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a>
 *    application note or the
 *    <a href="http://www.iar.com/website1/1.0.1.0/78/1/">
 *    IAR EWARM User and reference guides</a>,
 *    depending on the solutions that users choose.
 * -# On the computer, open and configure a terminal application
 *    (e.g., HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# In the terminal window, the
 *    following text should appear (values depend on the board and the chip used):
 *    \code
 *     -- AFEC Enhanced Resolution xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *    \endcode
 * -# The application will output current voltage of potentiometer on the terminal.
 *
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "asf.h"
#include "conf_board.h"

/** Reference voltage for AFEC,in mv. */
#define VOLT_REF        (3300)

/** The maximal digital value */
#define MAX_DIGITAL     (4095)

/** AFEC channel for potentiometer */
#define AFEC_CHANNEL_POTENTIOMETER  AFEC_CHANNEL_5

#define STRING_EOL    "\r"
#define STRING_HEADER "-- AFEC Temperature Sensor Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

#define MENU_HEADER "\n\r-- press a key to change the resolution--\n\r" \
		"-- 0: 12 bit resolution--\n\r" \
		"-- 1: 13 bit resolution--\n\r" \
		"-- 2: 14 bit resolution--\n\r" \
		"-- 3: 15 bit resolution--\n\r" \
		"-- 4: 16 bit resolution--\n\r" \
		"-- q: Quit Configuration.--\n\r"

/** AFEC sample data */
struct {
	uint16_t us_value;
	bool is_done;
} g_afec_sample_data;

/**Channel list for sequence*/
enum afec_channel_num ch_list[4] = {
	AFEC_CHANNEL_POTENTIOMETER,
	AFEC_CHANNEL_POTENTIOMETER,
	AFEC_CHANNEL_POTENTIOMETER,
	AFEC_CHANNEL_POTENTIOMETER
};

/**
 * \brief Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};
	
	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Display main menu.
 */
static void display_menu(void)
{
	puts(MENU_HEADER);
}

/**
 * \brief Set AFEC resolution mode.
 */
static void set_afec_resolution(void)
{
	uint8_t uc_key;
	uint8_t uc_done = 0;

	display_menu();
	
	while (!uc_done) {
		while (uart_read(CONF_UART, &uc_key));

		switch (uc_key) {
		case '0':
			afec_set_resolution(AFEC0, AFEC_12_BITS);
			puts(" Set Resolution to 12 bit \n\r");
			break;
		case '1':
			afec_set_resolution(AFEC0, AFEC_13_BITS);
			puts(" Set Resolution to 13 bit \n\r");
			break;
		case '2':
			afec_set_resolution(AFEC0, AFEC_14_BITS);
			puts(" Set Resolution to 14 bit \n\r");
			break;
		case '3':
			afec_set_resolution(AFEC0, AFEC_15_BITS);
			puts(" Set Resolution to 15 bit \n\r");
			break;
		case '4':
			afec_set_resolution(AFEC0, AFEC_16_BITS);
			puts(" Set Resolution to 16 bit \n\r");
			break;
		case 'q':
			uc_done = 1;
			break;
		default:
			break;	
		}
	}
}

/**
 * \brief AFEC interrupt callback function.
 */
static void afec_data_ready(void)
{
	if ((afec_get_interrupt_status(AFEC0) & AFE_ISR_DRDY) == AFE_ISR_DRDY) {
		g_afec_sample_data.us_value = afec_get_latest_value(AFEC0);
		g_afec_sample_data.is_done = true;
	}
}

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint8_t uc_key;
	
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();

	configure_console();
	
	/* Output example information. */
	puts(STRING_HEADER);

	g_afec_sample_data.us_value = 0;
	g_afec_sample_data.is_done = false;
	
	afec_enable(AFEC0);
	
	struct afec_config afec_cfg;
	
	afec_get_config_defaults(&afec_cfg);
	
	afec_init(AFEC0, &afec_cfg);

	afec_set_trigger(AFEC0, AFEC_TRIG_SW);

	/* Enable channel for temp sensor. */
	afec_channel_enable(AFEC0, AFEC_CHANNEL_POTENTIOMETER);

	afec_set_callback(AFEC0, AFEC_INTERRUPT_DATA_READY, afec_data_ready, 1);

	while (1) {
		afec_start_software_conversion(AFEC0);
		delay_ms(1000);
		
		/* Check if the user enters a key. */
		if (!uart_read(CONF_UART, &uc_key)) {
			afec_disable_interrupt(AFEC0, AFEC_INTERRUPT_ALL);	/* Disable all afec interrupt. */
			set_afec_resolution();
			afec_enable_interrupt(AFEC0, AFEC_INTERRUPT_DATA_READY);
		}

		/* Check if AFEC sample is done. */
		if (g_afec_sample_data.is_done == true) {
			printf("Potentiometer Voltage: %04d mv.    ",
					(int)(	g_afec_sample_data.us_value * VOLT_REF /	MAX_DIGITAL));
			puts("\r");
			g_afec_sample_data.is_done = false;
		}
	}
}
