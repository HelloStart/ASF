/**
 * \file
 *
 * \brief SAM0+ Analog Comparator Driver Quick Start
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

/**
 * \page ac_basic_use_case Quick Start Guide for the Analog Comparator module - Basic Use Case
 *
 * In this use case, the Analog Comparator module is configured for:
 *  \li Comparator peripheral in manually triggered (i.e. "Single Shot" mode)
 *  \li One comparator channel connected to input MUX pin 0 and compared to a
 *      scaled VCC/2 voltage
 *  \li Comparators 0 and 1 enabled during device sleep
 *
 * This use case sets up the Analog Comparator to compare an input voltage fed
 * into a GPIO pin of the device against a scaled voltage of the
 * microcontroller's VCC power rail. The comparisons are made on-demand in
 * single-shot mode, and the result stored into a local variable which can then
 * be used in a user application to alter the system behavior based on the
 * comparison result.
 *
 * \section ac_basic_use_case_setup Setup
 *
 * \subsection ac_basic_use_case_setup_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection ac_basic_use_case_setup_code Code
 * Copy-paste the following setup code to your user application:
 * \snippet qs_ac_basic.c setup
 *
 * Add to user application initialization (typically the start of \c main()):
 * \snippet qs_ac_basic.c setup_init
 *
 * \subsection ac_basic_use_case_setup_flow Workflow
 * -# Create an AC device instance struct, which will be associated with an
 *    Analog Comparator peripheral hardware instance.
 *    \note Device instance structures should \b never go out of scope when in
 *          use.
 *
 *  - \snippet qs_ac_basic.c setup_1
 * -# Define a macro to select the comparator channel that will be sampled, for
 *    convenience.
 *  - \snippet qs_ac_basic.c setup_2
 * -# Create a new function \c configure_ac(), which will be used to configure
 *    the overall Analog Comparator peripheral.
 *  - \snippet qs_ac_basic.c setup_3
 * -# Create an Analog Comparator peripheral configuration structure that will
 *    be filled out to set the module configuration.
 *  - \snippet qs_ac_basic.c setup_4
 * -# Fill the Analog Comparator peripheral configuration structure with the
 *    default module configuration values.
 *  - \snippet qs_ac_basic.c setup_5
 * -# Alter the sleep mode configuration value so that the comparator pairs
 *    remain enabled even during sleep mode.
 *  - \snippet qs_ac_basic.c setup_6
 * -# Initialize the Analog Comparator peripheral and associate it with the
 *    software instance structure that was defined previously.
 *  - \snippet qs_ac_basic.c setup_7
 * -# Enable the now initialized Analog Comparator peripheral.
 *  - \snippet qs_ac_basic.c setup_8
 *
 * -# Create a new function \c configure_ac_channel(), which will be used to
 *    configure the overall Analog Comparator peripheral.
 *  - \snippet qs_ac_basic.c setup_9
 * -# Create an Analog Comparator channel configuration structure that will
 *    be filled out to set the channel configuration.
 *  - \snippet qs_ac_basic.c setup_10
 * -# Fill the Analog Comparator channel configuration structure with the
 *    default channel configuration values.
 *  - \snippet qs_ac_basic.c setup_11
 * -# Alter the channel configuration parameters to set the channel to one-shot
 *    mode, with the correct negative and positive MUX selections and the
 *    desired voltage scaler.
 *    \note The voltage scalar formula is documented
 *          \ref ac_ch_conf.vcc_scale_factor "here".
 *
 *  - \snippet qs_ac_basic.c setup_12
 * -# Initialize the Analog Comparator channel and configure it with the desired
 *    settings.
 *  - \snippet qs_ac_basic.c setup_13
 * -# Enable the now initialized Analog Comparator channel.
 *  - \snippet qs_ac_basic.c setup_14
 *
 * \section ac_basic_use_case Use Case
 *
 * \subsection ac_basic_use_case_code Code
 * Copy-paste the following code to your user application:
 * \snippet qs_ac_basic.c main
 *
 * \subsection ac_basic_use_case_flow Workflow
 * -# Trigger the first comparison on the comparator channel.
 *  - \snippet qs_ac_basic.c main_1
 * -# Create a local variable to maintain the current comparator state. Since no
 *    comparison has taken place, it is initialized to \ref AC_CH_STATE_UNKNOWN.
 *  - \snippet qs_ac_basic.c main_2
 * -# Make the application loop infinitely, while performing triggered
 *    comparisons.
 *  - \snippet qs_ac_basic.c main_3
 * -# Check if the comparator is ready for the last triggered comparison result
 *    to be read.
 *  - \snippet qs_ac_basic.c main_4
 * -# Read the comparator output state into the local variable for application
 *    use.
 *  - \snippet qs_ac_basic.c main_5
 * -# Trigger the next conversion on the Analog Comparator channel.
 *  - \snippet qs_ac_basic.c main_6
 */
