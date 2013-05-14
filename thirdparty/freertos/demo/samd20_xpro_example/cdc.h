/**
 * \file
 *
 * \brief Embedded Debugger (EDBG) Virtual COM Port driver for FreeRTOS demo.
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

#ifndef CDC_H
#define CDC_H

#include <board.h>
#include <usart_interrupt.h>

/**
 * \defgroup edbg_cdc_rx_group Embedded Debugger (EDBG) Virtual COM Port driver
 *
 * Simple driver for receiving from the EDBG Virtual COM Port via SERCOM USART.
 *
 * This makes use of the \ref asfdoc_samd20_sercom_usart_group, but configures a
 * custom interrupt handler for the SERCOM instead of using the driver's own
 * handler.
 *
 * Only the receiver and its interrupt are enabled, so the custom interrupt
 * handler can, e.g., be a stripped down version of the SERCOM USART callback
 * driver's interrupt handler (\ref _usart_interrupt_handler()).
 *
 * The purpose of this driver is to avoid the overhead of the SERCOM driver's
 * callback functionality, allowing the received data to be handled directly for
 * increased throughput and reduced overhead.
 *
 * @{
 */

/**
 * \brief Initialize USART for reception from EDBG Virtual COM Port
 *
 * This function initializes the specified SERCOM USART driver instance for use
 * with the Embedded Debugger (EDBG) Virtual COM Port.
 *
 * A custom handler function for the receive interrupt must must be supplied.
 * See the SERCOM USART callback driver's handler for an implementation
 * template: \ref _usart_interrupt_handler().
 *
 * \param usart Address of USART instance to use.
 * \param cdc_rx_handler Address of receive interrupt handler.
 */
static inline void cdc_rx_init(struct usart_module *const usart,
		sercom_handler_t cdc_rx_handler)
{
	struct usart_config config_struct;
	uint8_t instance_index;

	usart_get_config_defaults(&config_struct);

	config_struct.mux_settings = USART_RX_3_TX_2_XCK_3;
	config_struct.pinout_pad3 = EDBG_CDC_RX_PINMUX;
	config_struct.pinout_pad2 = EDBG_CDC_TX_PINMUX;

	while (usart_init(usart, EDBG_CDC_MODULE, &config_struct) != STATUS_OK) {
		// Intentionally left empty
	}

	// Inject our own interrupt handler
	instance_index = _sercom_get_sercom_inst_index(EDBG_CDC_MODULE);
	_sercom_set_handler(instance_index, cdc_rx_handler);

	// Enable the UART receiver
	usart_enable(usart);
	usart_enable_transceiver(usart, USART_TRANSCEIVER_RX);

	// ..and the RX Complete interrupt
	((SercomUsart *)EDBG_CDC_MODULE)->INTENSET.reg = SERCOM_USART_INTFLAG_RXCIF;
}

/** @} */

#endif // CDC_H
