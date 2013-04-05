/**
 * \file
 *
 * \brief SAM D20 Brown Out Detector Driver
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
#ifndef BOD_H_INCLUDED
#define BOD_H_INCLUDED

#include <compiler.h>

/**
 * \defgroup asfdoc_samd20_bod_group SAM D29 Brown Out Detector Driver (BOD)
 *
 * This driver for SAM D29 devices provides an interface for the configuration
 * and management of the device's Brown Out Detector (BOD) modules, to detect
 * and respond to under-voltage events and take an appropriate action.
 *
 * The following peripherals are used by this module:
 *
 * - SYSCTRL (System Control)
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_samd20_bod_prerequisites
 *  - \ref asfdoc_samd20_bod_module_overview
 *  - \ref asfdoc_samd20_bod_special_considerations
 *  - \ref asfdoc_samd20_bod_extra_info
 *  - \ref asfdoc_samd20_bod_examples
 *  - \ref asfdoc_samd20_bod_api_overview
 *
 *
 * \section asfdoc_samd20_bod_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_samd20_bod_module_overview Module Overview
 *
 * The SAM D29 devices contain a number of Brown Out Detector (BOD) modules. Each
 * BOD monitors the supply voltage for any dips that go below the set threshold
 * for the module. In case of a BOD detection the BOD will either reset the
 * system or raise a hardware interrupt so that a safe power-down sequence can
 * be attempted.
 *
 *
 * \section asfdoc_samd20_bod_special_considerations Special Considerations
 *
 * The time between a BOD interrupt being raised and a failure of the processor
 * to continue executing (in the case of a core power failure) is system
 * specific; care must be taken that all critical BOD detection events can
 * complete within the amount of time available.
 *
 *
 * \section asfdoc_samd20_bod_extra_info Extra Information for BOD
 *
 * For extra information see \ref asfdoc_samd20_bod_extra. This includes:
 *  - \ref asfdoc_samd20_bod_extra_acronyms
 *  - \ref asfdoc_samd20_bod_extra_dependencies
 *  - \ref asfdoc_samd20_bod_extra_errata
 *  - \ref asfdoc_samd20_bod_extra_history
 *
 *
 * \section asfdoc_samd20_bod_examples Examples
 *
 * The following Quick Start guides and application examples are available for this driver:
 * - \ref asfdoc_samd20_bod_basic_use_case
 *
 *
 * \section asfdoc_samd20_bod_api_overview API Overview
 * @{
 */

/**
 * \brief Brown Out Detector hardware instance IDs.
 *
 * List of possible BOD controllers within the device.
 */
enum bod {
	/** BOD12 Internal core voltage. */
	BOD_BOD12,
	/** BOD33 External I/O voltage, */
	BOD_BOD33,
};

/**
 * \breif Brown Out Detector detection actions.
 *
 * List of possible BOD actions when a BOD module detects a brown-out condition.
 */
enum bod_action {
	/** A BOD detect will reset the device. */
	BOD_ACTION_RESET     = SYSCTRL_BOD33_ACTION(1),
	/** A BOD detect will fire an interrupt. */
	BOD_ACTION_INTERRUPT = SYSCTRL_BOD33_ACTION(2),
};

/**
 * \breif Brown Out Detector sampling modes.
 *
 * List of possible BOD module voltage sampling modes.
 */
enum bod_mode {
	/** BOD will sample the supply line continuously. */
	BOD_MODE_CONTINIOUS  = 0,
	/** BOD will use the BOD sampling clock (1kHz) to sample the supply line. */
	BOD_MODE_SAMPLED     = SYSCTRL_BOD33_MODE,
};

/** Configuration structure for a BOD module. */
struct bod_config {
	/** Sampling configuration mode for the BOD. */
	enum bod_mode mode;
	/** Action to perform when a low power detection is made. */
	enum bod_action action;
	/** BOD level to trigger at. */
	uint8_t level; /* TODO: document this more */
	/** If \c true, enables detection hysteresis. */
	bool hysteresis;
};


/**
 * \name Configuration and Initialization
 * @{
 */

/**
 * \brief Get default BOD configuration
 *
 * The default BOD configuration is:
 * - Continuous mode
 * - Reset on BOD detect
 * - Hysteresis enabled
 * - BOD level 0x12
 *
 * \param[out] conf BOD configuration struct to set to default settings
 */
static inline void bod_get_config_defaults(
		struct bod_config *const conf)
{
	/* Sanity check arguments */
	Assert(conf);

	conf->mode       = BOD_MODE_CONTINIOUS;
	conf->action     = BOD_ACTION_RESET;
	conf->level      = 0x12; /* TODO: Need to revisit this level */
	conf->hysteresis = true;
}

enum status_code bod_set_config(
		const enum bod bod,
		struct bod_config *const conf);

/**
 * \brief Enables a configured BOD module
 *
 * Enables the specified BOD module that has been previously configured.
 *
 * \param[in] bod  BOD module to enable
 *
 * \return Error code indicating the status of the enable operation.
 *
 * \retval STATUS_OK               If the BOD was successfully enabled
 * \retval STATUS_ERR_INVALID_ARG  An invalid BOD was supplied
 */
static inline enum status_code bod_enable(
		const enum bod bod)
{
	switch (bod) {
		case BOD_BOD33:
			SYSCTRL->BOD33.reg |= SYSCTRL_BOD33_ENABLE;
			break;

		case BOD_BOD12:
			SYSCTRL->BOD12.reg |= SYSCTRL_BOD12_ENABLE;
			break;

		default:
			return STATUS_ERR_INVALID_ARG;
	}

	return STATUS_OK;
}

/**
 * \brief Disables an enabled BOD module
 *
 * Disables the specified BOD module that was previously enabled.
 *
 * \param[in] bod  BOD module to disable
 *
 * \return Error code indicating the status of the disable operation.
 *
 * \retval STATUS_OK               If the BOD was successfully disabled
 * \retval STATUS_ERR_INVALID_ARG  An invalid BOD was supplied
 */
static inline enum status_code bod_disable(
		const enum bod bod)
{
	switch (bod) {
		case BOD_BOD33:
			SYSCTRL->BOD33.reg &= ~SYSCTRL_BOD33_ENABLE;
			break;

		case BOD_BOD12:
			SYSCTRL->BOD12.reg &= ~SYSCTRL_BOD12_ENABLE;
			break;

		default:
			return STATUS_ERR_INVALID_ARG;
	}

	return STATUS_OK;
}

/** @} */

/**
 * @}
 */


/**
 * \page asfdoc_samd20_bod_extra Extra Information for BOD Driver
 *
 * \section asfdoc_samd20_bod_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *  <tr>
 *      <th>Acronym</th>
 *      <th>Definition</th>
 *  </tr>
 *  <tr>
 *      <td>BOD</td>
 *      <td>Brownout detector</td>
 *  </tr>
 * </table>
 *
 *
 * \section asfdoc_samd20_bod_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - None
 *
 *
 * \section asfdoc_samd20_bod_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_samd20_bod_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 */

/**
 * \page asfdoc_samd20_bod_exqsg Examples for BOD Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_samd20_bod_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_samd20_bod_basic_use_case
 */

#endif /* BOD_H_INCLUDED */
