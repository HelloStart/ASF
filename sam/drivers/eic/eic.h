/**
 * \file
 *
 * \brief EIC driver for SAM
 *
 * Copyright (c) 2012-2014 Atmel Corporation. All rights reserved.
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

#ifndef EIC_H_INCLUDED
#define EIC_H_INCLUDED

/**
 * \defgroup asfdoc_sam_drivers_eic_group  SAM4 External Interrupt Controller (EIC)
 *
 * This document describes the SAM4 driver providing access to the
 * advanced functions of the on-chip EIC controller interrupt and event support.
 *
* The outline of this documentation is as follows:
 *  - \ref asfdoc_sam_drivers_eic_prerequisites
 *  - \ref asfdoc_sam_drivers_eic_module_overview
 *  - \ref asfdoc_sam_drivers_eic_special_considerations
 *  - \ref asfdoc_sam_drivers_eic_extra_info
 *  - \ref asfdoc_sam_drivers_eic_examples
 *  - \ref asfdoc_sam_drivers_eic_api_overview
 *
 *
 * \section asfdoc_sam_drivers_eic_module_overview Module Overview
 * The External Interrupt Controller (EIC) allows pins to be configured as external interrupts. Each
 * external interrupt has its own interrupt request and can be individually masked. Each external
 * interrupt can generate an interrupt on rising or falling edge, or high or low level. Every interrupt
 * input has a configurable filter to remove spikes from the interrupt source. Every interrupt pin can
 * also be configured to be asynchronous, in order to wake-up the part from sleep modes where the
 * CLK_SYNC clock has been disabled.
 *
 * An additional Non-Maskable Interrupt (NMI) pin is also supported. This has the same properties as the
 * other external interrupts, but is connected to the NMI request of the CPU, enabling it to interrupt any
 * other interrupt mode.
*
*
 * \section asfdoc_sam_drivers_eic_special_considerations Special Considerations
 *
 * - The external interrupt pins (EXTINTn and NMI) may be multiplexed with I/O Controller lines. The
 * programmer must first program the I/O Controller to assign the desired EIC pins to their
 * peripheral function. If I/O lines of the EIC are not used by the application, they can be used for other
 * purposes by the I/O Controller.
 * It is only required to enable the EIC inputs actually in use. For example, if an application only
 * requires two external interrupts, then only two I/O lines will be assigned to EIC inputs.
 *
 * - All interrupts are available in all sleep modes as long as the EIC module is powered. However, in
 * sleep modes where CLK_SYNC is stopped, the interrupt must be configured to asynchronous
 * mode.
 *
 * - The clock for the EIC bus interface (CLK_EIC) is generated by the Power Manager. This clock is
 * enabled at reset, and can be disabled in the Power Manager.
 * The filter and synchronous edge/level detector runs on a clock which is stopped in any of the
 * sleep modes where the system RC oscillator (RCSYS) is not running. This clock is referred to as
 * CLK_SYNC.
 *
 * - The external interrupt request lines are connected to the NVIC. Using the external interrupts
 * requires the NVIC to be programmed first.
 * Using the Non-Maskable Interrupt does not require the NVIC to be programmed.
 *
 * - When an external debugger forces the CPU into debug mode, the EIC continues normal
 * operation. If the EIC is configured in a way that requires it to be periodically serviced by the CPU
 * through interrupts or similar, improper operation or data loss may result during debugging.
 *
*
 * \section asfdoc_sam_drivers_eic_prerequisites Prerequisites
 *
 * - None
 *
 *
 * \section asfdoc_sam_drivers_eic_extra_info Extra Information
 *
 * For extra information, see \ref asfdoc_sam_drivers_eic_extra. This includes:
 *  - \ref asfdoc_sam_drivers_eic_extra_acronyms
 *  - \ref asfdoc_sam_drivers_eic_extra_dependencies
 *  - \ref asfdoc_sam_drivers_eic_extra_errata
 *  - \ref asfdoc_sam_drivers_eic_extra_history
 *
 * \section asfdoc_sam_drivers_eic_examples Examples
 *
 * For a list of examples related to this driver, see
 * - \ref sam_eic_quickstart
 *
 *
 * \section asfdoc_sam_drivers_eic_api_overview API Overview
 * @{
 */

#include "compiler.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** Number of available EIC lines, device dependent. */
#if SAM4L
#define EIC_NUMBER_OF_LINES                 9
#else
#error  'This device does not support EIC driver'
#endif

/** \name External Interrupt lines */
/* @{ */
/** Non-Maskable Interrupt */
#define EXT_NMI     0
/** External Interrupt 1. */
#define EXT_INT1    1
/** External Interrupt 2. */
#define EXT_INT2    2
/** External Interrupt 3. */
#define EXT_INT3    3
/** External Interrupt 4. */
#define EXT_INT4    4
/** External Interrupt 5. */
#define EXT_INT5    5
/** External Interrupt 6. */
#define EXT_INT6    6
/** External Interrupt 7. */
#define EXT_INT7    7
/** External Interrupt 8. */
#define EXT_INT8    8
/* @} */

/** \name Mode Trigger Options */
/* @{ */
/** The interrupt is edge triggered. */
#define EIC_MODE_EDGE_TRIGGERED   0
/** The interrupt is level triggered. */
#define EIC_MODE_LEVEL_TRIGGERED  1
/* @} */

/** \name Edge level Options */
/* @{ */
/** The interrupt triggers on falling edge. */
#define EIC_EDGE_FALLING_EDGE     0
/** The interrupt triggers on rising edge. */
#define EIC_EDGE_RISING_EDGE      1
/* @} */

/** \name Level Options */
/* @{ */
/** The interrupt triggers on low level. */
#define EIC_LEVEL_LOW_LEVEL       0
/** The interrupt triggers on high level. */
#define EIC_LEVEL_HIGH_LEVEL      1
/* @} */

/** \name Filter Options */
/* @{ */
/** The interrupt is filtered. */
#define EIC_FILTER_ENABLED        1
/** The interrupt is not filtered. */
#define EIC_FILTER_DISABLED       0
/* @} */

/** \name Synch Mode Options */
/* @{ */
/** The interrupt is synchronized to CLK_SYNC. */
#define EIC_SYNCH_MODE            0
/** The interrupt is asynchronous. */
#define EIC_ASYNCH_MODE           1
/* @} */

/** Configuration parameters of the EIC module. */
struct eic_line_config {
	/** Mode: \ref EIC_MODE_EDGE_TRIGGERED or \ref EIC_MODE_LEVEL_TRIGGERED */
	uint8_t eic_mode;
	/** Edge: \ref EIC_EDGE_FALLING_EDGE or \ref EIC_EDGE_RISING_EDGE */
	uint8_t eic_edge;
	/** Level: \ref EIC_LEVEL_LOW_LEVEL or \ref EIC_LEVEL_HIGH_LEVEL */
	uint8_t eic_level;
	/** Filter: \ref EIC_FILTER_DISABLED or \ref EIC_FILTER_ENABLED */
	uint8_t eic_filter;
	/** Async: \ref EIC_ASYNCH_MODE or \ref EIC_SYNCH_MODE */
	uint8_t eic_async;
};

/**
 * \brief Callback definition
 */
typedef void (*eic_callback_t)(void);

void eic_disable(Eic *eic);
void eic_enable(Eic *eic);

void eic_line_set_config(Eic *eic, uint8_t line_number,
	struct eic_line_config *eic_line_conf);

void eic_line_set_callback(Eic *eic, uint8_t line_number,
	eic_callback_t callback, uint8_t irq_line, uint8_t irq_level);

/**
 * \brief Enable the external interrupt on specified line.
 *
 * \param [in]  eic Base address of the EIC module
 * \param [in]  line_number The line number to enable
 */
static inline void eic_line_enable(Eic *eic, uint8_t line_number)
{
	eic->EIC_EN = 1 << line_number;
}

/**
 * \brief Disable the external interrupt on specified line.
 *
 * \param [in]  eic Base address of the EIC module
 * \param [in]  line_number Line number to disable
 */
static inline void eic_line_disable(Eic *eic, uint8_t line_number)
{
	eic->EIC_DIS = 1 << line_number;
}

/**
 * \brief Tells whether an EIC line is enabled.
 *
 * \param [in]  eic Base address of the EIC module
 * \param [in]  line_number Line number to test
 *
 * \retval true EIC line is enabled
 * \retval false EIC line is not enabled
 */
static inline bool eic_line_is_enabled(Eic *eic, uint8_t line_number)
{
	return (eic->EIC_CTRL & (1 << line_number)) != 0;
}

/**
 * \brief Enables the propagation from the EIC to the interrupt
 * controller of the external interrupt on a specified line.
 *
 * \param [in]  eic Base address of the EIC module
 * \param [in]  line_number Line number of interrupt to enable
 */
static inline void eic_line_enable_interrupt(Eic *eic,
		uint8_t line_number)
{
	eic->EIC_IER = 1 << line_number;
}

/**
 * \brief Disables the propagation from the EIC to the interrupt
 * controller of the external interrupt on a specified line.
 *
 * \param [in]  eic Base address of the EIC (i.e. EIC)
 * \param [in]  line_number Line number of interrupt to disable
 */
static inline void eic_line_disable_interrupt(Eic *eic,
		uint8_t line_number)
{
	eic->EIC_IDR = 1 << line_number;
	eic->EIC_IMR;
}

/**
 * \brief Tells whether an EIC interrupt line is enabled.
 *
 * \param [in]  eic Base address of the EIC module
 * \param [in]  line_number Line number to test
 *
 * \retval true EIC interrupt line is enabled
 * \retval false EIC interrupt line is not enabled
 */
static inline bool eic_line_interrupt_is_enabled(Eic *eic,
		uint8_t line_number)
{
	return (eic->EIC_IMR & (1 << line_number)) != 0;
}

/**
 * \brief Clear the interrupt flag of specified pin.
 *          Call this function once you have handled the interrupt.
 *
 * \param [in]  eic Base address of the EIC (i.e. EIC)
 * \param [in]  line_number Line number to clear
 */
static inline void eic_line_clear_interrupt(Eic *eic,
		uint8_t line_number)
{
	eic->EIC_ICR = 1 << line_number;
	eic->EIC_ISR;
}

/**
 * \brief Tells whether an EIC interrupt line is pending.
 *
 * \param [in]  eic Base address of the EIC module
 * \param [in]  line_number Line number to test
 *
 * \retval true EIC interrupt line is pending
 * \retval false EIC interrupt line is not pending
 */
static inline bool eic_line_interrupt_is_pending(Eic *eic,
		uint8_t line_number)
{
	return (eic->EIC_ISR & (1 << line_number)) != 0;
}

/** @} */

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond


/**
 * \page asfdoc_sam_drivers_eic_extra Extra Information for EIC
 *
 * \section asfdoc_sam_drivers_eic_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *  <tr>
 *      <th>Acronym</th>
 *      <th>Definition</th>
 *  </tr>
 *  <tr>
 *		<td>EIC</td>
 *		<td>External Interrupt Controller</td>
 *  </tr>
 *  <tr>
 *		<td>GPIO</td>
 *		<td>General-Purpose Input/Output</td>
 *  </tr>
 *  <tr>
 *      <td>NMI</td>
 *      <td>Non-Maskable Interrupt</td>
 * </tr>
 *  <tr>
 *      <td>NVIC</td>
 *      <td>Nested Vectored Interrupt Controller</td>
 * </tr>
 *  <tr>
 *      <td>QSG</td>
 *      <td>Quick Start Guide</td>
 * </tr>
 * </table>
 *
 *
 * \section asfdoc_sam_drivers_eic_extra_dependencies Dependencies
 *
 *  - \ref asfdoc_sam_sysclock "Clocks"
 *  - \ref asfdoc_sam_io_lines "I/O Lines"
 *  - \ref asfdoc_sam_interrupts "Interrupts"
 *  - \ref asfdoc_sam_debug_operation "Debug Operation"
 *
 * \section asfdoc_sam_drivers_eic_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam_drivers_eic_extra_history Module History
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
 *		<td>Initial document release</td>
 *	</tr>
 * </table>


 * \page sam_eic_quickstart Quickstart guide for SAM EIC driver
 *
 * This is the quickstart guide for the \ref sam_drivers_eic_group "SAM EIC driver",
 * with step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section eic_basic_use_case Basic Use Case
 * In this basic use case, the EIC module and single line are configured for:
 * - Falling edge trigger and async mode
 * - Interrupt-based handling
 * - GPIO_PUSH_BUTTON_EIC_IRQ as input
 *
 * \subsection sam_eic_quickstart_prereq Prerequisites
 * \ref sysclk_group "System Clock Management (Sysclock)."
 *
 * \section eic_basic_use_case_setup Setup Steps
 * \subsection eic_basic_use_case_setup_code Example Code
 * Add to application C-file:
 * \snippet example/eic_example.c eic_example_code
 *
 * \subsection eic_basic_use_case_setup_flow Workflow
 * Define the interrupt callback function in the application:
 * \snippet example/eic_example.c set_eic_callback
 * Enable EIC module:
 * \snippet example/eic_example.c enable_eic_module
 *
 * \note Including enable module clock and lock sleep mode.
 *
 * Configure EIC line with specified mode:
 * \snippet example/eic_example.c configure_eic_mode
 * Set the EIC callback function and enable EIC interrupt.
 * \snippet example/eic_example.c set_eic_callback_1
 * Enable EIC line:
 * \snippet example/eic_example.c enable_eic_line
 *
 * \page asfdoc_sam_drivers_aes_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>42278A</td>
 *		<td>03/2014</td>
 *		<td>Initial document release</td>
 *	</tr>
 * </table>
 */
#endif // EIC_H_INCLUDED
