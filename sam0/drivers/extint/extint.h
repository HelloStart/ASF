/**
 * \file
 *
 * \brief SAM External Interrupt Driver
 *
 * Copyright (C) 2012-2015 Atmel Corporation. All rights reserved.
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
#ifndef EXTINT_H_INCLUDED
#define EXTINT_H_INCLUDED

/**
 * \defgroup asfdoc_sam0_extint_group SAM External Interrupt (EXTINT) Driver
 *
 * This driver for Atmel&reg; | SMART ARM&reg;-based microcontrollers provides
 * an interface for the configuration and management of external interrupts
 * generated by the physical device pins, including edge detection.
 * The following driver API modes are covered by this
 * manual:
 *
 *  - Polled APIs
 * \if EXTINT_CALLBACK_MODE
 *  - Callback APIs
 * \endif
 *
 * The following peripheral is used by this module:
 *  - EIC (External Interrupt Controller)
 *
 * The following devices can use this module:
 *  - Atmel | SMART SAM D20/D21
 *  - Atmel | SMART SAM R21
 *  - Atmel | SMART SAM D09/D10/D11
 *  - Atmel | SMART SAM L21/L22
 *  - Atmel | SMART SAM DA1
 *  - Atmel | SMART SAM C20/C21
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_sam0_extint_prerequisites
 *  - \ref asfdoc_sam0_extint_module_overview
 *  - \ref asfdoc_sam0_extint_special_considerations
 *  - \ref asfdoc_sam0_extint_extra_info
 *  - \ref asfdoc_sam0_extint_examples
 *  - \ref asfdoc_sam0_extint_api_overview
 *
 *
 * \section asfdoc_sam0_extint_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_sam0_extint_module_overview Module Overview
 *
 * The External Interrupt (EXTINT) module provides a method of asynchronously
 * detecting rising edge, falling edge, or specific level detection on individual
 * I/O pins of a device. This detection can then be used to trigger a software
 * interrupt or event, or polled for later use if required. External interrupts
 * can also optionally be used to automatically wake up the device from sleep
 * mode, allowing the device to conserve power while still being able to react
 * to an external stimulus in a timely manner.
 *
 * \subsection asfdoc_sam0_extint_logical_channels Logical Channels
 * The External Interrupt module contains a number of logical channels, each of
 * which is capable of being individually configured for a given pin routing,
 * detection mode, and filtering/wake up characteristics.
 *
 * Each individual logical external interrupt channel may be routed to a single
 * physical device I/O pin in order to detect a particular edge or level of the
 * incoming signal.
 *
 * \subsection asfdoc_sam0_extint_module_overview_nmi_chanel NMI Channels
 *
 * One or more Non Maskable Interrupt (NMI) channels are provided within each
 * physical External Interrupt Controller module, allowing a single physical pin
 * of the device to fire a single NMI interrupt in response to a particular
 * edge or level stimulus. An NMI cannot, as the name suggests, be disabled in
 * firmware and will take precedence over any in-progress interrupt sources.
 *
 * NMIs can be used to implement critical device features such as forced
 * software reset or other functionality where the action should be executed in
 * preference to all other running code with a minimum amount of latency.
 *
 * \subsection asfdoc_sam0_extint_module_overview_filtering Input Filtering and Detection
 *
 * To reduce the possibility of noise or other transient signals causing
 * unwanted device wake-ups, interrupts, and/or events via an external interrupt
 * channel. A hardware signal filter can be enabled on individual channels. This
 * filter provides a Majority-of-Three voter filter on the incoming signal, so
 * that the input state is considered to be the majority vote of three
 * subsequent samples of the pin input buffer. The possible sampled input and
 * resulting filtered output when the filter is enabled is shown in
 * \ref asfdoc_sam0_extint_filter_table "the table below".
 *
 * \anchor asfdoc_sam0_extint_filter_table
 * <table>
 *  <caption>Sampled Input and Resulting Filtered Output</caption>
 *  <tr>
 *      <th>Input Sample 1</th>
 *      <th>Input Sample 2</th>
 *      <th>Input Sample 3</th>
 *      <th>Filtered Output</th>
 *  </tr>
 *  <tr>
 *      <td>0</td> <td>0</td> <td>0</td> <td>0</td>
 *  </tr>
 *  <tr>
 *      <td>0</td> <td>0</td> <td>1</td> <td>0</td>
 *  </tr>
 *  <tr>
 *      <td>0</td> <td>1</td> <td>0</td> <td>0</td>
 *  </tr>
 *  <tr>
 *      <td>0</td> <td>1</td> <td>1</td> <td>1</td>
 *  </tr>
 *  <tr>
 *      <td>1</td> <td>0</td> <td>0</td> <td>0</td>
 *  </tr>
 *  <tr>
 *      <td>1</td> <td>0</td> <td>1</td> <td>1</td>
 *  </tr>
 *  <tr>
 *      <td>1</td> <td>1</td> <td>0</td> <td>1</td>
 *  </tr>
 *  <tr>
 *      <td>1</td> <td>1</td> <td>1</td> <td>1</td>
 *  </tr>
 * </table>
 *
 * \subsection asfdoc_sam0_extint_module_overview_events Events and Interrupts
 *
 * Channel detection states may be polled inside the application for synchronous
 * detection, or events and interrupts may be used for asynchronous behavior.
 * Each channel can be configured to give an asynchronous hardware event (which
 * may in turn trigger actions in other hardware modules) or an asynchronous
 * software interrupt.
 *
 * \note The connection of events between modules requires the use of the
 *       \ref asfdoc_sam0_events_group "SAM Event System Driver (EVENTS)"
 *       to route output event of one module to the input event of another.
 *       For more information on event routing, refer to the event driver
 *       documentation.
 *
 * \subsection asfdoc_sam0_extint_module_overview_physical Physical Connection
 *
 * \ref asfdoc_sam0_extint_int_connections "The diagram below" shows how this
 * module is interconnected within the device.
 *
 * \anchor asfdoc_sam0_extint_int_connections
 * \dot
 * digraph overview {
 *   node [label="Port Pad" shape=square] pad;
 *
 *   subgraph driver {
 *     node [label="Peripheral MUX" shape=trapezium] pinmux;
 *     node [label="EIC Module" shape=ellipse] eic;
 *     node [label="Other Peripheral Modules" shape=ellipse style=filled fillcolor=lightgray] peripherals;
 *   }
 *
 *   pinmux -> eic;
 *   pad    -> pinmux;
 *   pinmux -> peripherals;
 * }
 * \enddot
 *
 * \section asfdoc_sam0_extint_special_considerations Special Considerations
 *
 * Not all devices support disabling of the NMI channel(s) detection mode - see
 * your device datasheet.
 *
 *
 * \section asfdoc_sam0_extint_extra_info Extra Information
 *
 * For extra information, see \ref asfdoc_sam0_extint_extra. This includes:
 *  - \ref asfdoc_sam0_extint_extra_acronyms
 *  - \ref asfdoc_sam0_extint_extra_dependencies
 *  - \ref asfdoc_sam0_extint_extra_errata
 *  - \ref asfdoc_sam0_extint_extra_history
 *
 *
 * \section asfdoc_sam0_extint_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_sam0_extint_exqsg.
 *
 *
 * \section asfdoc_sam0_extint_api_overview API Overview
 * @{
 */

#include <compiler.h>
#include <pinmux.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief External interrupt edge detection configuration enum.
 *
 * Enum for the possible signal edge detection modes of the External
 * Interrupt Controller module.
 */
enum extint_detect {
	/** No edge detection. Not allowed as a NMI detection mode on some
	 *  devices. */
	EXTINT_DETECT_NONE    = 0,
	/** Detect rising signal edges */
	EXTINT_DETECT_RISING  = 1,
	/** Detect falling signal edges */
	EXTINT_DETECT_FALLING = 2,
	/** Detect both signal edges */
	EXTINT_DETECT_BOTH    = 3,
	/** Detect high signal levels */
	EXTINT_DETECT_HIGH    = 4,
	/** Detect low signal levels */
	EXTINT_DETECT_LOW     = 5,
};

/**
 * \brief External interrupt internal pull configuration enum.
 *
 * Enum for the possible pin internal pull configurations.
 *
 * \note Disabling the internal pull resistor is not recommended if the driver
 *       is used in interrupt (callback) mode, due the possibility of floating
 *       inputs generating continuous interrupts.
 */
enum extint_pull {
	/** Internal pull-up resistor is enabled on the pin */
	EXTINT_PULL_UP        = SYSTEM_PINMUX_PIN_PULL_UP,
	/** Internal pull-down resistor is enabled on the pin */
	EXTINT_PULL_DOWN      = SYSTEM_PINMUX_PIN_PULL_DOWN,
	/** Internal pull resistor is disconnected from the pin */
	EXTINT_PULL_NONE      = SYSTEM_PINMUX_PIN_PULL_NONE,
};

/** The EIC is clocked by GCLK_EIC. */
#define EXTINT_CLK_GCLK   0
/** The EIC is clocked by CLK_ULP32K. */
#define EXTINT_CLK_ULP32K 1

/**
 * \brief External Interrupt Controller channel configuration structure.
 *
 *  Configuration structure for the edge detection mode of an external
 *  interrupt channel.
 */
struct extint_chan_conf {
	/** GPIO pin the NMI should be connected to */
	uint32_t gpio_pin;
	/** MUX position the GPIO pin should be configured to */
	uint32_t gpio_pin_mux;
	/** Internal pull to enable on the input pin */
	enum extint_pull gpio_pin_pull;
#if (SAML21) || (SAML22) || (SAMC20) || (SAMC21)
	/** Enable asynchronous edge detection. */
	bool enable_async_edge_detection;
#else
	/** Wake up the device if the channel interrupt fires during sleep mode */
	bool wake_if_sleeping;
#endif
	/** Filter the raw input signal to prevent noise from triggering an
	 *  interrupt accidentally, using a three sample majority filter */
	bool filter_input_signal;
	/** Edge detection mode to use */
	enum extint_detect detection_criteria;
};

/**
 * \brief External Interrupt event enable/disable structure.
 *
 * Event flags for the \ref extint_enable_events() and
 * \ref extint_disable_events().
 */
struct extint_events {
	/** If \c true, an event will be generated when an external interrupt
	 *  channel detection state changes */
	bool generate_event_on_detect[32 * EIC_INST_NUM];
};

/**
 * \brief External Interrupt Controller NMI configuration structure.
 *
 *  Configuration structure for the edge detection mode of an external
 *  interrupt NMI channel.
 */
struct extint_nmi_conf {
	/** GPIO pin the NMI should be connected to */
	uint32_t gpio_pin;
	/** MUX position the GPIO pin should be configured to */
	uint32_t gpio_pin_mux;
	/** Internal pull to enable on the input pin */
	enum extint_pull gpio_pin_pull;
	/** Filter the raw input signal to prevent noise from triggering an
	 *  interrupt accidentally, using a three sample majority filter */
	bool filter_input_signal;
	/** Edge detection mode to use. Not all devices support all possible
	 *  detection modes for NMIs.
	 */
	enum extint_detect detection_criteria;
#if (SAML21) || (SAML22) || (SAMC20) || (SAMC21)
	/** Enable asynchronous edge detection. */
	bool enable_async_edge_detection;
#endif
};

#if EXTINT_CALLBACK_MODE == true
/** Type definition for an EXTINT module callback function */
typedef void (*extint_callback_t)(void);

#ifndef EIC_NUMBER_OF_INTERRUPTS
#  define EIC_NUMBER_OF_INTERRUPTS 16
#endif
#endif

#if !defined(__DOXYGEN__)
/** \internal
 *  Internal EXTINT module device instance structure definition.
 */
struct _extint_module
{
#  if EXTINT_CALLBACK_MODE == true
	/** Asynchronous channel callback table, for user-registered handlers */
	extint_callback_t callbacks[EIC_NUMBER_OF_INTERRUPTS];
#  else
	/** Dummy value to ensure the struct has at least one member */
	uint8_t _dummy;
#  endif
};

/**
 * \brief Retrieves the base EIC module address from a given channel number.
 *
 * Retrieves the base address of a EIC hardware module associated with the
 * given external interrupt channel.
 *
 * \param[in] channel  External interrupt channel index to convert
 *
 * \return Base address of the associated EIC module.
 */
static inline Eic * _extint_get_eic_from_channel(
		const uint8_t channel)
{
	uint8_t eic_index = (channel / 32);

	if (eic_index < EIC_INST_NUM) {
		/* Array of available EICs */
		Eic *const eics[EIC_INST_NUM] = EIC_INSTS;

		return eics[eic_index];
	} else {
		Assert(false);
		return NULL;
	}
}

/**
 * \brief Retrieves the base EIC module address from a given NMI channel number.
 *
 * Retrieves the base address of a EIC hardware module associated with the
 * given non-maskable external interrupt channel.
 *
 * \param[in] nmi_channel  Non-Maskable interrupt channel index to convert
 *
 * \return Base address of the associated EIC module.
 */
static inline Eic * _extint_get_eic_from_nmi(
		const uint8_t nmi_channel)
{
	uint8_t eic_index = nmi_channel;

	if (eic_index < EIC_INST_NUM) {
		/* Array of available EICs */
		Eic *const eics[EIC_INST_NUM] = EIC_INSTS;

		return eics[eic_index];
	} else {
		Assert(false);
		return NULL;
	}
}
#endif

/** \name Event Management
 * @{
 */

void extint_enable_events(
		struct extint_events *const events);

void extint_disable_events(
		struct extint_events *const events);

/** @} */

/** \name Configuration and Initialization (Channel)
 * @{
 */

void extint_chan_get_config_defaults(
		struct extint_chan_conf *const config);

void extint_chan_set_config(
		const uint8_t channel,
		const struct extint_chan_conf *const config);

/** @} */

/** \name Configuration and Initialization (NMI)
 * @{
 */

/**
 * \brief Initializes an External Interrupt NMI channel configuration structure to defaults.
 *
 * Initializes a given External Interrupt NMI channel configuration structure
 * to a set of known default values. This function should be called on all new
 * instances of these configuration structures before being modified by the
 * user application.
 *
 * The default configuration is as follows:
 * \li Input filtering disabled
 * \li Detect falling edges of a signal
 * \li Asynchronous edge detection is disabled
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void extint_nmi_get_config_defaults(
		struct extint_nmi_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->gpio_pin            = 0;
	config->gpio_pin_mux        = 0;
	config->gpio_pin_pull       = EXTINT_PULL_UP;
	config->filter_input_signal = false;
	config->detection_criteria  = EXTINT_DETECT_FALLING;
#if (SAML21) || (SAML22) || (SAMC20) || (SAMC21)
	 config->enable_async_edge_detection = false;
#endif

}

enum status_code extint_nmi_set_config(
		const uint8_t nmi_channel,
		const struct extint_nmi_conf *const config);

/** @} */

/** \name Detection testing and clearing (channel)
 * @{
 */

/**
 * \brief Retrieves the edge detection state of a configured channel.
 *
 *  Reads the current state of a configured channel, and determines
 *  if the detection criteria of the channel has been met.
 *
 *  \param[in] channel  External Interrupt channel index to check
 *
 *  \return Status of the requested channel's edge detection state.
 *  \retval true   If the channel's edge/level detection criteria was met
 *  \retval false  If the channel has not detected its configured criteria
 */
static inline bool extint_chan_is_detected(
		const uint8_t channel)
{
	Eic *const eic_module = _extint_get_eic_from_channel(channel);
	uint32_t eic_mask   = (1UL << (channel % 32));

	return (eic_module->INTFLAG.reg & eic_mask);
}

/**
 * \brief Clears the edge detection state of a configured channel.
 *
 *  Clears the current state of a configured channel, readying it for
 *  the next level or edge detection.
 *
 *  \param[in] channel  External Interrupt channel index to check
 */
static inline void extint_chan_clear_detected(
		const uint8_t channel)
{
	Eic *const eic_module = _extint_get_eic_from_channel(channel);
	uint32_t eic_mask   = (1UL << (channel % 32));

	eic_module->INTFLAG.reg = eic_mask;
}

/** @} */

/** \name Detection Testing and Clearing (NMI)
 * @{
 */

/**
 * \brief Retrieves the edge detection state of a configured NMI channel.
 *
 *  Reads the current state of a configured NMI channel, and determines
 *  if the detection criteria of the NMI channel has been met.
 *
 *  \param[in] nmi_channel  External Interrupt NMI channel index to check
 *
 *  \return Status of the requested NMI channel's edge detection state.
 *  \retval true   If the NMI channel's edge/level detection criteria was met
 *  \retval false  If the NMI channel has not detected its configured criteria
 */
static inline bool extint_nmi_is_detected(
		const uint8_t nmi_channel)
{
	Eic *const eic_module = _extint_get_eic_from_nmi(nmi_channel);

	return (eic_module->NMIFLAG.reg & EIC_NMIFLAG_NMI);
}

/**
 * \brief Clears the edge detection state of a configured NMI channel.
 *
 *  Clears the current state of a configured NMI channel, readying it for
 *  the next level or edge detection.
 *
 *  \param[in] nmi_channel  External Interrupt NMI channel index to check
 */
static inline void extint_nmi_clear_detected(
		const uint8_t nmi_channel)
{
	Eic *const eic_module = _extint_get_eic_from_nmi(nmi_channel);

	eic_module->NMIFLAG.reg = EIC_NMIFLAG_NMI;
}

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

#if EXTINT_CALLBACK_MODE == true
#  include "extint_callback.h"
#endif

/**
 * \page asfdoc_sam0_extint_extra Extra Information for EXTINT Driver
 *
 * \section asfdoc_sam0_extint_extra_acronyms Acronyms
 * The table below presents the acronyms used in this module:
 *
 * <table>
 *  <tr>
 *      <th>Acronym</th>
 *      <th>Description</th>
 *  </tr>
 *  <tr>
 *      <td>EIC</td>
 *      <td>External Interrupt Controller</td>
 *  </tr>
 *  <tr>
 *      <td>MUX</td>
 *      <td>Multiplexer</td>
 *  </tr>
 *  <tr>
 *      <td>NMI</td>
 *      <td>Non-Maskable Interrupt</td>
 *  </tr>
 * </table>
 *
 *
 * \section asfdoc_sam0_extint_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - \ref asfdoc_sam0_system_pinmux_group "System Pin Multiplexer Driver"
 *
 *
 * \section asfdoc_sam0_extint_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam0_extint_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
 *
 * <table>
 *  <tr>
 *      <th>Changelog</th>
 *  </tr>
 *  <tr>
 *      <td>
 *      \li Driver updated to follow driver type convention
 *      \li Removed \c %extint_reset(), \c %extint_disable() and
 *          \c extint_enable() functions. Added internal function
 *          \c %_system_extint_init().
 *      \li Added configuration EXTINT_CLOCK_SOURCE in conf_extint.h
 *      \li Removed configuration EXTINT_CALLBACKS_MAX in conf_extint.h, and
 *          added channel parameter in the register functions
 *         \c %extint_register_callback() and \c %extint_unregister_callback()
 *      </td>
 *  </tr>
 *  <tr>
 *      <td>Updated interrupt handler to clear interrupt flag before calling
 *          callback function</td>
 *  </tr>
 *  <tr>
 *      <td>Updated initialization function to also enable the digital interface
 *          clock to the module if it is disabled</td>
 *  </tr>
 *  <tr>
 *      <td>Initial Release</td>
 *  </tr>
 * </table>
 */

/**
 * \page asfdoc_sam0_extint_exqsg Examples for EXTINT Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_sam0_extint_group.
 * QSGs are simple examples with step-by-step instructions to configure and
 * use this driver in a selection of use cases. Note that a QSG can be compiled
 * as a standalone application or be added to the user application.
 *
 *  - \subpage asfdoc_sam0_extint_basic_use_case
 * \if EXTINT_CALLBACK_MODE
 *  - \subpage asfdoc_sam0_extint_callback_use_case
 * \endif
 *
 * \page asfdoc_sam0_extint_document_revision_history Document Revision History
 *
 * <table>
 *  <tr>
 *      <th>Doc. Rev.</td>
 *      <th>Date</td>
 *      <th>Comments</td>
 *  </tr>
 *  <tr>
 *      <td>42112E</td>
 *      <td>12/2015</td>
 *      <td>Added support for SAM L21/L22, SAM C21, and SAM DA1</td>
 *  </tr>
 *  <tr>
 *      <td>42112D</td>
 *      <td>12/2014</td>
 *      <td>Added support for SAM R21 and SAM D10/D11</td>
 *  </tr>
 *  <tr>
 *      <td>42112C</td>
 *      <td>01/2014</td>
 *      <td>Added support for SAM D21</td>
 *  </tr>
 *  <tr>
 *      <td>42112B</td>
 *      <td>06/2013</td>
 *      <td>Added additional documentation on the event system. Corrected
 *          documentation typos.</td>
 *  </tr>
 *  <tr>
 *      <td>42112A</td>
 *      <td>06/2013</td>
 *      <td>Initial release</td>
 *  </tr>
 * </table>
 */

#endif
