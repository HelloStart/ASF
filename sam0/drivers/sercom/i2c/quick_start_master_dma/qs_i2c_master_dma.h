/**
 * \file
 *
 * \brief SAM D2x SERCOM I2C Master Driver with DMA Quick Start Guide
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

/**
 * \page asfdoc_sam0_sercom_i2c_master_dma_use_case Quick Start Guide for 
SERCOM I2C Master with DMA
 *
 * In this use case, the I<SUP>2</SUP>C will used and set up as follows:
 *  - Master mode
 *  - 100kHz operation speed
 *  - Not operational in standby
 *  - 10000 packet timeout value
 *  - 65535 unknown bus state timeout value
 *
 *
 * \section asfdoc_sam0_sercom_i2c_master_dma_use_case_prereq Prerequisites
 * The device must be connected to an I<SUP>2</SUP>C slave.
 *
 * \section asfdoc_sam0_sercom_i2c_master_dma_use_setup Setup
 *
 * \subsection asfdoc_sam0_sercom_i2c_master_dma_use_setup_code Code
 * The following must be added to the user application:
 *
 * - A sample buffer to send, number of entries to send and address of slave:
 * \snippet qs_i2c_master_dma_use.c packet_data
 *
 * Number of times to try to send packet if it fails:
 * \snippet qs_i2c_master_dma_use.c timeout
 *
 * - Globally accessible module structure:
 * \snippet qs_i2c_master_dma_use.c dev_i2c_inst
 *
 * - Function for setting up the module:
 * \snippet qs_i2c_master_dma_use.c initialize_i2c
 *
 * - Globally accessible DMA module structure:
 * \snippet qs_i2c_slave_dma.c dma_resource
 *
 * - Globally transfer done flag:
 * \snippet qs_i2c_slave_dma.c transfer_done_flag
 *
 * - Globally accessible DMA transfer descriptor:
 * \snippet qs_i2c_slave_dma.c transfer_descriptor
 *
 * - Function for transfer done callback:
 * \snippet qs_i2c_slave_dma.c _transfer_done
 *
 * - Function for setting up the DMA resource:
 * \snippet qs_i2c_slave_dma.c config_dma_resource
 *
 * - Function for setting up the DMA transfer descriptor:
 * \snippet qs_i2c_slave_dma.c setup_dma_transfer_descriptor
 * - Add to user application \c main():
 * \snippet qs_i2c_master_dma_use.c init
 *
 * \subsection asfdoc_sam0_sercom_i2c_master_dma_use_setup_workflow Workflow
 * -# Configure and enable module:
 *  \snippet qs_i2c_master_dma_use.c initialize_i2c
 *   -# Create and initialize configuration structure.
 *    \snippet qs_i2c_master_dma_use.c init_conf
 *   -# Change settings in the configuration.
 *    \snippet qs_i2c_master_dma_use.c conf_change
 *   -# Initialize the module with the set configurations.
 *    \snippet qs_i2c_master_dma_use.c init_module
 *   -# Enable the module.
 *    \snippet qs_i2c_master_dma_use.c enable_module
 * -# Create a DMA resource configuration structure, which can be filled out to
 *    adjust the configuration of a single DMA transfer.
 *  \snippet qs_dma_basic.c dma_setup_1
 *
 * -# Initialize the DMA resource configuration struct with the module's
 *    default values.
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 *  \snippet qs_dma_basic.c dma_setup_2
 *
 * -# Set extra configurations for the DMA resource. It is using peripheral trigger,
 * SERCOM Tx trigger and trigger causes a transaction transfer in this example.
 *  \snippet qs_dma_basic.c dma_setup_3
 *
 * -# Allocate a DMA resource with the configurations.
 *  \snippet qs_dma_basic.c dma_setup_4
 *
 * -# Create a DMA transfer descriptor configuration structure, which can be
 * filled out to adjust the configuration of a single DMA transfer.
 *  \snippet qs_dma_basic.c dma_setup_5
 *
 * -# Initialize the DMA transfer descriptor configuration struct with the module's
 *    default values.
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 *  \snippet qs_dma_basic.c dma_setup_6
 *
 * -# Set the specific parameters for a DMA transfer with transfer size, source
 *    address, destination address.
 *  \snippet qs_dma_basic.c dma_setup_7
 *
 * -# Create the DMA transfer descriptor.
 *  \snippet qs_dma_basic.c dma_setup_8
 *
 * \section asfdoc_sam0_sercom_i2c_master_dma_use_implemenation 
Implementation
 * \subsection asfdoc_sam0_sercom_i2c_master_dma_use_implemenation_code Code
 * Add to user application \c main():
 * \snippet qs_i2c_master_dma_use.c main
 *
 * \subsection asfdoc_sam0_sercom_i2c_master_dma_use_implemenation_workflow 
Workflow
 * -# Start the DMA transfer job.
 *  \snippet qs_i2c_master_dma_use.c start_transfer_job
 *
 * -# Set the auto address length and enable flag.
 *  \snippet qs_i2c_master_dma_use.c set_i2c_addr
 * 
 * -# Waiting for transfer complete
 *  \snippet qs_i2c_master_dma_use.c waiting_for_complete
 */
