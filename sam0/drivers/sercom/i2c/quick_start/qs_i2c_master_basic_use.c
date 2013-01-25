/**
 * \file
 *
 * \brief SAMD20 Serial Peripheral Interface Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
 *
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

//! [packet_data]
#define DATA_LENGTH 10

static uint8_t buffer[DATA_LENGTH] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
};

#define SLAVE_ADDRESS 0x01
//! [packet_data]

/* Number of time to try and send packet if failed. */
#define TIMEOUT 1000

/* Init device instance. */
//! [dev_inst]
struct i2c_master_dev_inst dev_inst;
//! [dev_inst]

//! [initialize_i2c]
static void configure_i2c(void)
{
	/* Initialize config structure and device instance. */
	//! [init_conf]
	struct i2c_master_conf conf;
	i2c_master_get_config_defaults(&conf);
	//! [init_conf]

	/* Change buffer timeout to something longer. */
	//! [conf_change]
	conf.buffer_timeout = 10000;
	//! [conf_change]

	/* Initialize and enable device with config. */
	//! [init_module]
	i2c_master_init(&dev_inst, SERCOM0, &conf);
	//! [init_module]

	//! [enable_module]
	i2c_master_enable(&dev_inst);
	//! [enable_module]
}
//! [initialize_i2c]

int main(void)
{

	//! [run_initialize_i2c]
	/* Init system. */
	//system_init();
	/* Configure device and enable. */
	configure_i2c();
	//! [run_initialize_i2c]

	/* Timeout counter. */
	uint16_t timeout = 0;

	/* Init i2c packet. */
	//! [packet]
	i2c_packet_t packet = {
		.address     = SLAVE_ADDRESS,
		.data_length = DATA_LENGTH,
		.data        = buffer,
	};
	//! [packet]


	/* Write buffer to slave until success. */
	//! [write_packet]
	while(i2c_master_write_packet(&dev_inst, &packet) != STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ >= TIMEOUT) {
			break;
		}
	}
	//! [write_packet]

	while (1) {
		/* Inf loop. */
	}
}
