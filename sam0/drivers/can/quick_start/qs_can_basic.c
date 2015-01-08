/**
 * \file
 *
 * \brief SAM CAN basic Quick Start
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
/**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <string.h>
#include <conf_can.h>

//! [module_var]

//! [module_inst]
static struct usart_module cdc_instance;
static struct can_module can_instance;
//! [module_inst]


//! [can_filter_setting]
#define CAN_RX_STANDARD_FILTER_INDEX    0
#define CAN_RX_STANDARD_FILTER_ID_0     0x5A
#define CAN_RX_STANDARD_FILTER_ID_1     0x69
//! [can_filter_setting]

//! [can_transfer_message_setting]
#define CAN_TX_BUFFER_INDEX    0
#define CAN_DATA_LEN 8
static uint8_t tx_message_0[CAN_DATA_LEN]={0x00,0x01,0x02,0x03,0x04
		,0x05,0x06,0x07};
static uint8_t tx_message_1[CAN_DATA_LEN]={0x08,0x09,0x0A,0x0B,0x0C
		,0x0D,0x0E,0x0F};
//! [can_transfer_message_setting]

//! [can_receive_message_setting]
static volatile uint32_t receive_flag = 0;
static struct can_rx_element *rx_element;
//! [can_receive_message_setting]

//! [module_var]

//! [setup]

//! [cdc_setup]
static void configure_usart_cdc(void)
{

	struct usart_config config_cdc;
	usart_get_config_defaults(&config_cdc);
	config_cdc.baudrate	 = 38400;
	config_cdc.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_cdc.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_cdc.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_cdc.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_cdc.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	stdio_serial_init(&cdc_instance, EDBG_CDC_MODULE, &config_cdc);
	usart_enable(&cdc_instance);
}
//! [cdc_setup]

//! [can_init_setup]
static void configure_can(void)
{
	/* Set up the CAN TX/RX pins */
	struct system_pinmux_config pin_config;
	system_pinmux_get_config_defaults(&pin_config);
	pin_config.mux_position = CAN_TX_MUX_SETTING;
	system_pinmux_pin_set_config(CAN_TX_PIN, &pin_config);
	pin_config.mux_position = CAN_RX_MUX_SETTING;
	system_pinmux_pin_set_config(CAN_RX_PIN, &pin_config);


	struct can_config config_can;
	can_get_config_defaults(&config_can);

	can_module_init(&can_instance, CAN_MODULE, &config_can);

	/* Reject all nonmatching frames and remote frames. */
	can_set_global_filter(&can_instance, CAN_NONMATCHING_FRAMES_REJECT,
			CAN_NONMATCHING_FRAMES_REJECT, true, true);

	can_switch_mode(&can_instance, CAN_MODE_NORMAL_OPERATION);
}
//! [can_init_setup]

//! [can_receive_filter_setup]
static void can_set_filter(uint32_t filter_value)
{
	struct can_sd_message_filter_element sd_filter;

	can_get_sd_message_filter_element_default(&sd_filter);
	sd_filter.S0.bit.SFID1 = filter_value;

	can_set_rx_standand_filter(&can_instance, &sd_filter,
			CAN_RX_STANDARD_FILTER_INDEX);
	can_set_interrupt(&can_instance, CAN_RX_FIFO_0_NEW_MESSAGE);
}
//! [can_receive_filter_setup]

//! [can_transfer_message_setup]
static void can_send_message(uint32_t id_value, uint8_t *data)
{
	uint32_t i;
	struct can_tx_element tx_element;

	can_get_tx_buffer_element_defaults(&tx_element);
	tx_element.T0.reg |= CAN_TX_ELEMENT_T0_ID(id_value << 18);
	for (i = 0; i < 8; i++) {
		tx_element.data[i] = *data;
		data++;
	}

	can_set_tx_buffer_element(&can_instance, &tx_element,
			CAN_TX_BUFFER_INDEX);
	can_tx_add_request(&can_instance, CAN_TX_BUFFER_INDEX);
}
//! [can_transfer_message_setup]

//! [can_interrupt_handler]
void CAN0_Handler(void)
{
	uint32_t status;
	status = can_read_interrupt_status(&can_instance);

	if (status & CAN_RX_FIFO_0_NEW_MESSAGE) {
		can_clear_interrupt_status(&can_instance, CAN_RX_FIFO_0_NEW_MESSAGE);
		receive_flag = 1;
		can_get_rx_fifo_0_element(&can_instance, rx_element,
				CAN_RX_STANDARD_FILTER_INDEX);
		can_rx_fifo_acknowledge(&can_instance, 0,
				CAN_RX_STANDARD_FILTER_INDEX);
		printf("The received buffer is: %d, %d, %d, %d, %d, %d, %d, %d.\n\r",
				rx_element->data[0], rx_element->data[1], rx_element->data[2],
				rx_element->data[3], rx_element->data[4], rx_element->data[5],
				rx_element->data[6], rx_element->data[7]);
	}
}
//! [can_interrupt_handler]

//! [user_menu]
static void display_menu(void)
{
	printf("Menu :\r\n"
			"  -- Select the action:\r\n"
			"  0: Set filter ID 0: 0x5A. \r\n"
			"  1: Set filter ID 0: 0x69. \r\n"
			"  2: Send message with ID 0: 0x5A and 8 byte data 0 to 7. \r\n"
			"  3: Send message with ID 0: 0x69 and 8 byte data 8 to 15. \r\n"
			"  h: Display menu \r\n\r\n");
}
//! [user_menu]

//! [setup]

int main(void)
{
	uint8_t key;

//! [setup_init]
	system_init();
	configure_usart_cdc();
//! [setup_init]

//! [main_setup]

//! [configure_can]
	configure_can();
//! [configure_cab]

//! [display_user_menu]
	display_menu();
//! [display_user_menu]

//! [main_loop]
	while(1) {
		scanf("%c", (char *)&key);

		switch (key) {
		case 'h':
			display_menu();
			break;

		case '0':
			can_set_filter(CAN_RX_STANDARD_FILTER_ID_0);
			break;

		case '1':
			can_set_filter(CAN_RX_STANDARD_FILTER_ID_1);
			break;

		case '2':
			can_send_message(CAN_RX_STANDARD_FILTER_ID_0, tx_message_0);
			break;

		case '3':
			can_send_message(CAN_RX_STANDARD_FILTER_ID_1, tx_message_1);
			break;

		default:
			break;
		}
	}
//! [main_loop]

//! [main_setup]
}
