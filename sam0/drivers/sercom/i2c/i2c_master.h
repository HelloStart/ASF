/**
 * \file
 *
 * \brief SAM0+ RTC Driver for calendar mode
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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

#ifndef I2C_MASTER_H_INCLUDED
#define I2C_MASTER_H_INCLUDED

#include "i2c_common.h"
#include "../sercom.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup sam0_i2c_master_group SAM0+ I2C Master
 *
 * This is that!
 *
 * \section i2c_master_intro Introduction
 * This is an intro!
 *
 * \section i2c_master_api_overview API Overview
 * @{
 */

/**
 * \brief Callback types.
 * The available callback types for the I2C master module.
 */
enum i2c_master_callback_type {
	I2C_MASTER_CALLBACK_WRITE_DONE = 0,
	I2C_MASTER_CALLBACK_READ_DONE  = 1,
	I2C_MASTER_CALLBACK_ERROR      = 2,
};

enum i2c_master_interrupt_flag {
	I2C_MASTER_INTERRUPT_WRITE = I2C_INTERRUPT_WIF_bp,
	I2C_MASTER_INTERRUPT_READ  = I2C_INTERRUPT_RIF_bp,
};

/** Enum for the possible I2C master mode SDA internal hold times. */
enum i2c_master_sda_hold_time {
	I2C_MASTER_SDA_HOLD_DISABLED, /**< Internal SDA hold time disabled. */
	I2C_MASTER_SDA_HOLD_50ns_100ns, /**< Internal SDA hold time 50ns-100ns. */
	I2C_MASTER_SDA_HOLD_300ns_600ns, /**< Internal SDA hold time 300ns-600ns. */
	I2C_MASTER_SDA_HOLD_400ns_800ns, /**< Internal SDA hold time 400ns-800ns. */
};

enum i2c_master_inactive_timeout {
	I2C_MASTER_INACTIVE_TIMEOUT_DISABELED,
	I2C_MASTER_INACTIVE_TIMEOUT_50US_60US,
	I2C_MASTER_INACTIVE_TIMEOUT_100US_110US,
	I2C_MASTER_INACTIVE_TIMEOUT_200US_210US,
};


/**
 * \brief Configuration structure for the I2C device.
 *
 * This is the configuration structure for the I2C Master device. It is used
 * as an argument for \ref i2c_master_init to provide the desired
 * configurations for the module. The structure should be initiated using the
 * \ref i2c_master_get_config_defaults .
 */
struct i2c_master_conf {
	enum i2c_master_baud_rate baud_rate;
	bool active_in_sleep;
	bool hold_line_timeout;
	enum i2c_master_inactive_timeout inactive_timeout;
	enum i2c_master_sda_hold_time sda_hold_time;
	/** If true, external will be used that supports tri-stating. 4 wires
	  * will be used. */
	bool external_reciever;
};

/**
 * \name Configuration and Initialization
 * @{
 */

typedef (*i2c_master_callback_t)(
		const struct i2c_master_dev_inst *const dev_inst);

#if !defined(__DOXYGEN__)
/**
 * \internal Wait for hardware module to sync.
 * \param[in]  dev_inst Pointer to device instance structure.
 */

static void _i2c_wait_for_sync(const struct i2c_master_dev_inst dev_inst)
{
	/* Sanity check. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	while(dev_inst->hw_dev->I2C_MASTER.STATUS & I2C_SYNC_BUSY_bm) {
		/* Wait for I2C module to sync. */
	}
}
#endif

/**
 * \brief Get the I2C master default configurations.
 *
 * Use to initialize the configuration structure to known default values. This
 * function should be called at the start of any I2C initiation.
 *
 * The default configuration is as follows:
 * - Blabla!
 *
 * \param[out] config Pointer to configuration structure to be initiated.
 */
static inline void i2c_master_get_config_defaults(
		struct i2c_master_conf *const config)
{
	/*Sanity check argument. */
	Assert(config);


}

/**
 * \brief Initializes the requested I2C Hardware module.
 *
 * Initializes the SERCOM I2C Master device requested and sets the provided
 * device instance struct. This will also reset the hardware module, so all
 * current settings will be lost. Run this function before any further use of
 * the driver.
 *
 * \param[out] dev_inst Pointer to device instance struct.
 * \param[in]  module   Pointer to the hardware instance.
 * \param[in]  config   Pointer to the configuration struct.
 * \return                 [description]
 */
enum status_code i2c_master_init(struct i2c_master_dev_inst *const dev_inst,
		SERCOM_t *const module,
		const struct i2c_master_conf *const config);

/**
 * \brief Enable the I2C module.
 *
 * This will enable the requested I2C module.
 *
 * \param[in]  dev_inst Pointer to the device instance struct.
 * \return          [description]
 */
enum status_code i2c_master_enable(
		const struct i2c_master_dev_inst *const dev_inst);

/**
 * \brief Disable the I2C module.
 *
 * This will disable the I2C module specified in the provided device instance
 * structure.
 *
 * \param[in]  dev_inst Pointer to the device instance struct.
 * \return          [description]
 */
enum status_code i2c_master_disable(
		const struct i2c_master_dev_inst *const dev_inst);

/** @} */

/**
* \name Interrupts
* @{
*/

/**
 * \brief Check interrupt flag.
 *
 * Check status of the requested interrupt flag.
 *
 * \param[in]  dev_inst       Pointer to device instance struct.
 * \param[in]  interrupt_flag Value of interrupt flag to check.
 *
 * \return Status of the requested interrupt flag.
 */
static inline bool i2c_master_is_interrupt(
		const struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_interrupt_flag interrupt_flag)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	return (dev_inst->hw_dev->I2C_MASTER.INTFLAG & (1 << interrupt_flag));
}

/**
 * \brief Clear interrupt flag.
 *
 * Clear the specified interrupt flag.
 *
 * \param[out] dev_inst       Pointer to device instance struct.
 * \param[in]  interrupt_flag Value of interrupt flag to clear.
 */
static inline void i2c_master_clear_interrupt(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_interrupt_flag interrupt_flag)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	dev_inst->hw_dev->I2C_MASTER.INTFLAG = (1 << interrupt_flag);
}

/**
 * \brief Register callback for the specified callback type.
 *
 * When called, the given callback function will be associated with the
 * specified callback type.
 *
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]  callback      Pointer to the function desired for the specified
 *                       callback.
 * \param[in]  callback_type Specifies the callback type to register.
 * \return          [description]
 */
enum status_code i2c_master_register_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type callback_type);

/**
 * \brief Unregister callback for the specified callback type.
 *
 * When called, the currently registered callback for the given callback type
 * will be removed.
 *
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]      callback_type Specifies the callback type to unregister.
 * \return          [description]
 */
enum status_code i2c_master_unregister_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type callback_type);

/**
 * \brief Enable callback.
 * Enables the callback specified the callback_value.
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]      callback_type Callback type to enable.
 * \return               [description]
 */
enum status_code i2c_master_enable_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type callback_type);

/**
 * \brief Disable callback.
 * Disables the callback specified by the callback_type.
 * \param[in,out]  dev_inst      Pointer to the device instance struct.
 * \param[in]      callback_type Callback type to disable.
 * \return               [description]
 */
enum status_code i2c_master_disable_callback(
		struct i2c_master_dev_inst *const dev_inst,
		enum i2c_master_callback_type callback_type);

/** @} */

/**
* \name Read and Write, Blocking
* @{
*/

/**
 * \brief Read data packet from slave.
 *
 * Reads a data packet from the specified slave address on the I2C bus.
 *
 * \param[in,out]     dev_inst  Pointer to device instance struct.
 * \param[in,out]     packet    Pointer to I2C packet to transfer.
 * \return          [description]
 */
enum status_code i2c_master_read_packet(
		const struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet);

/**
 * \brief Write data packet to slave.
 *
 * Writes a data packet to the specified slave address on the I2C bus.
 *
 * \param[in,out]     dev_inst  Pointer to device instance struct.
 * \param[in,out] packet    Pointer to I2C packet to transfer.
 * \return          [description]
 */
enum status_code i2c_master_write_packet(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet);


/** @} */

/**
* \name Read and Write, Non-Blocking
* @{
*/

/**
 * \brief Read data packet from slave asynchronous.
 *
 * Reads a data packet from the specified slave address on the I2C bus. This
 * is the non-blocking equivalent of \ref i2c_master_read .
 *
 * \param[in,out]     dev_inst  Pointer to device instance struct.
 * \param[in,out] packet    Pointer to I2C packet to transfer.
 * \return          [description]
 */
enum status_code i2c_master_read_packet_async(
		const struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet);

/**
 * \brief Write data packet to slave asynchronous.
 *
 * Writes a data packet to the specified slave address on the I2C bus. This
 * is the non-blocking equivalent of \ref i2c_master_write .
 *
 * \param[in,out]     dev_inst  Pointer to device instance struct.
 * \param[in,out] packet    Pointer to I2C packet to transfer.
 * \return          [description]
 */
enum status_code i2c_master_write_packet_async(
		struct i2c_master_dev_inst *const dev_inst,
		i2c_packet_t *const packet);

/**
 * \brief Cancel the currently running operation.
 * This will terminate the running write or read transfer.
 *
 * \param  dev_inst Pointer to device instance struct.
 * \return          [description]
 */
enum status_code i2c_master_cancel_operation(
		struct i2c_master_dev_inst *const dev_inst);

/**
 * \brief Check if a started transfer is done.
 * This will return the current status of the read/write transfer.
 *
 * \param  dev_inst Pointer to the device instance struct.
 * \return          [description]
 */
enum status_code i2c_master_is_transfer_done(
		struct i2c_master_dev_inst *const dev_inst);

/** @} */
/** @} */


#ifdef __cplusplus
}
#endif

#endif /* I2C_MASTER_H_INCLUDED */