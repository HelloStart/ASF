/**
 * \file
 *
 * \brief SAM D2x DMA System Driver Quick Start
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

//! [setup]
// [transfer_length]
#define DATA_LENGTH (1024)
// [transfer_length]

// [source_memory]
static uint8_t source_memory[DATA_LENGTH];
// [source_memory]

// [destination_memory]
static uint8_t destination_memory[DATA_LENGTH];
// [destination_memory]

// [transfer_done]
static volatile bool transfer_is_done = false;
// [transfer_done]

// [_transfer_done]
static void transfer_done( const struct dma_resource* const resource )
{
	transfer_is_done = true;
}
// [_transfer_done]

// [config_dma_resource]
static void configure_dma_resource(struct dma_resource *resource)
{
//! [setup_1]
	struct dma_transfer_config config;
//! [setup_1]

//! [setup_2]
	dma_get_config_defaults(&config);
//! [setup_2]

//! [setup_3]
	dma_allocate(resource, &config);
//! [setup_3]
}
// [config_dma_resource]

// [setup_dma_transfer_descriptor]
static void setup_transfer_descriptor( struct dma_transfer_descriptor *descriptor )
{
	//! [setup_dma_descriptor]
	descriptor->block_transfer_control = DMAC_BTCTRL_VALID |DMAC_BTCTRL_BEATSIZE_BYTE
			 |DMAC_BTCTRL_DSTINC | DMAC_BTCTRL_SRCINC;
	descriptor->block_transfer_count = sizeof(source_memory);
	descriptor->source_address = (uint32_t)source_memory + sizeof(source_memory);
	descriptor->destination_address = (uint32_t)destination_memory + sizeof(source_memory);
	descriptor->next_descriptor_address = 0;
	//! [setup_dma_descriptor]
}
// [setup_dma_transfer_descriptor]

//! [setup]

int main(void)
{
	struct dma_resource example_resource;
	struct dma_transfer_descriptor example_descriptor;

	system_init();

	//! [setup_dma_resource]
	configure_dma_resource(&example_resource);
	//! [setup_dma_resource]

	//! [setup_transfer_descriptor]
	setup_transfer_descriptor(&example_descriptor);
	//! [setup_transfer_descriptor]

	//! [setup_callback_register]
	dma_register_callback(&example_resource, transfer_done,
			DMA_CALLBACK_TRANSFER_DONE);
	//! [setup_callback_register]

	//! [setup_enable_callback]
	dma_enable_callback(&example_resource, DMA_CALLBACK_TRANSFER_DONE);
	//! [setup_enable_callback]

	//! [setup_source_memory_content]
	for (uint32_t i = 0; i < DATA_LENGTH; i++) {
		source_memory[i] = i;
	}
	//! [setup_source_memory_content]
	
	//! [main]
	//! [main_1]
	dma_transfer_job(&example_resource, &example_descriptor);
	//! [main_1]
	
	//! [main_2]
	while (!transfer_is_done) {
		/* Wait for transfer done */
	}
	/* ! [main_2] */

	while (true) {
		/* Nothing to do */
	}

	//! [main]
}
