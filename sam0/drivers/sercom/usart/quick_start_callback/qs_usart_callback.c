/**
 * \file
 *
 * \brief SAM D20 System related functionality
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
#include <stdio.h>


struct usart_module usart_edbg;

volatile uint8_t rx_buffer[5];

volatile uint8_t buff[20];

static void read_callback(const struct usart_module *const mod)
{
	usart_write_buffer_job(&usart_edbg, (uint8_t *)rx_buffer, sizeof(rx_buffer)-1);

}

static void write_callback(const struct usart_module *const mod)
{
	static uint8_t cnt;
	uint8_t len = sprintf((char *)&buff[0], "CNT:%u\n\r", cnt);

	if(cnt++ < 10) {
		usart_write_buffer_job(&usart_edbg,(uint8_t *)buff, len);
	}
}

static void configure_callbacks(void)
{
	usart_register_callback(&usart_edbg, write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_edbg, read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&usart_edbg, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_edbg, USART_CALLBACK_BUFFER_RECEIVED);
}

int main(void)
{
	struct usart_config config_struct;
	volatile uint32_t delay;

	uint8_t string[] = "hello there world!\n\r";


	/* Initialize system clocks */
	system_init();

	/* Get configuration defaults for the USART
	 * 9600 8N1
	 */
	usart_get_config_defaults(&config_struct);
	config_struct.mux_settings = USART_RX_1_TX_0_XCK_1;

	config_struct.pinout_pad3 = EDBG_CDC_RX_PINMUX;
	config_struct.pinout_pad2 = EDBG_CDC_TX_PINMUX;

	/* Apply configuration */
	while (usart_init(&usart_edbg, EDBG_CDC_MODULE,
			&config_struct) != STATUS_OK) {
	}

	configure_callbacks();

	/* Enable USARTs */
	usart_enable(&usart_edbg);

	/* Enable transmitter */
	usart_enable_transceiver(&usart_edbg, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&usart_edbg, USART_TRANSCEIVER_RX);

	for(delay=0; delay < 100000; delay++);

	usart_write_buffer_job(&usart_edbg,string, sizeof(string));

	/* Echo back characters received */
	while (1) {
		usart_read_buffer_job(&usart_edbg, (uint8_t *)rx_buffer, sizeof(rx_buffer));
	}
}
