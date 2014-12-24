/**
 * \file
 *
 * \brief SAM C21 Clock Driver
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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

#ifndef CLOCK_CONFIG_CHECK_H
#  define CLOCK_CONFIG_CHECK_H

#if !defined(CONF_CLOCK_FLASH_WAIT_STATES)
#  error CONF_CLOCK_FLASH_WAIT_STATES not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_CPU_DIVIDER)
#  error CONF_CLOCK_CPU_DIVIDER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_OSC48M_FREQ_DIV)
#  error CONF_CLOCK_OSC48M_FREQ_DIV not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_OSC48M_ON_DEMAND)
#  error CONF_CLOCK_OSC48M_ON_DEMAND not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_OSC48M_RUN_IN_STANDBY)
#  error CONF_CLOCK_OSC48M_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC_ENABLE)
#  error CONF_CLOCK_XOSC_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC_EXTERNAL_CRYSTAL)
#  error CONF_CLOCK_XOSC_EXTERNAL_CRYSTAL not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC_EXTERNAL_FREQUENCY)
#  error CONF_CLOCK_XOSC_EXTERNAL_FREQUENCY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC_STARTUP_TIME)
#  error CONF_CLOCK_XOSC_STARTUP_TIME not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC_AUTO_GAIN_CONTROL)
#  error CONF_CLOCK_XOSC_AUTO_GAIN_CONTROL not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC_ON_DEMAND)
#  error CONF_CLOCK_XOSC_ON_DEMAND not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC_RUN_IN_STANDBY)
#  error CONF_CLOCK_XOSC_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC_FAILURE_DETECTOT_PRE)
#  error CONF_CLOCK_XOSC_FAILURE_DETECTOT_PRE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC_FAILURE_DETECTOT_EVENT_OUTPUT_EANBLE)
#  error CONF_CLOCK_XOSC_FAILURE_DETECTOT_EVENT_OUTPUT_EANBLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC_FAILURE_DETECTOT_EANBLE)
#  error CONF_CLOCK_XOSC_FAILURE_DETECTOT_EANBLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC_FAILURE_SWITCH_BACK_EANBLE)
#  error CONF_CLOCK_XOSC_FAILURE_SWITCH_BACK_EANBLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC32K_ENABLE)
#  error CONF_CLOCK_XOSC32K_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC32K_EXTERNAL_CRYSTAL)
#  error CONF_CLOCK_XOSC32K_EXTERNAL_CRYSTAL not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC32K_STARTUP_TIME)
#  error CONF_CLOCK_XOSC32K_STARTUP_TIME not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC32K_ENABLE_1KHZ_OUPUT)
#  error CONF_CLOCK_XOSC32K_ENABLE_1KHZ_OUPUT not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC32K_ENABLE_32KHZ_OUTPUT)
#  error CONF_CLOCK_XOSC32K_ENABLE_32KHZ_OUTPUT not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC32K_ON_DEMAND)
#  error CONF_CLOCK_XOSC32K_ON_DEMAND not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC32K_RUN_IN_STANDBY)
#  error CONF_CLOCK_XOSC32K_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC32K_FAILURE_DETECTOT_PRE)
#  error CONF_CLOCK_XOSC32K_FAILURE_DETECTOT_PRE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC32K_FAILURE_DETECTOT_EVENT_OUTPUT_EANBLE)
#  error CONF_CLOCK_XOSC32K_FAILURE_DETECTOT_EVENT_OUTPUT_EANBLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC32K_FAILURE_DETECTOT_EANBLE)
#  error CONF_CLOCK_XOSC32K_FAILURE_DETECTOT_EANBLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_XOSC32K_FAILURE_SWITCH_BACK_EANBLE)
#  error CONF_CLOCK_XOSC32K_FAILURE_SWITCH_BACK_EANBLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_OSC32K_ENABLE)
#  error CONF_CLOCK_OSC32K_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_OSC32K_STARTUP_TIME)
#  error CONF_CLOCK_OSC32K_STARTUP_TIME not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_OSC32K_ENABLE_1KHZ_OUTPUT)
#  error CONF_CLOCK_OSC32K_ENABLE_1KHZ_OUTPUT not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_OSC32K_ENABLE_32KHZ_OUTPUT)
#  error CONF_CLOCK_OSC32K_ENABLE_32KHZ_OUTPUT not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_OSC32K_ON_DEMAND)
#  error CONF_CLOCK_OSC32K_ON_DEMAND not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_OSC32K_RUN_IN_STANDBY)
#  error CONF_CLOCK_OSC32K_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_OSCULP32K_ENABLE_1KHZ_OUTPUT)
#  error CONF_CLOCK_OSCULP32K_ENABLE_1KHZ_OUTPUT not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_OSCULP32K_ENABLE_32KHZ_OUTPUT)
#  error CONF_CLOCK_OSCULP32K_ENABLE_32KHZ_OUTPUT not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_ENABLE)
#  error CONF_CLOCK_DPLL_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_ON_DEMAND)
#  error CONF_CLOCK_DPLL_ON_DEMAND not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_RUN_IN_STANDBY)
#  error CONF_CLOCK_DPLL_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_LOCK_BYPASS)
#  error CONF_CLOCK_DPLL_LOCK_BYPASS not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_WAKE_UP_FAST)
#  error CONF_CLOCK_DPLL_WAKE_UP_FAST not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_LOW_POWER_ENABLE)
#  error CONF_CLOCK_DPLL_LOW_POWER_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_LOCK_TIME)
#  error CONF_CLOCK_DPLL_LOCK_TIME not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_REFERENCE_CLOCK)
#  error CONF_CLOCK_DPLL_REFERENCE_CLOCK not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_FILTER)
#  error CONF_CLOCK_DPLL_FILTER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_REFERENCE_FREQUENCY)
#  error CONF_CLOCK_DPLL_REFERENCE_FREQUENCY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_REFEREMCE_DIVIDER)
#  error CONF_CLOCK_DPLL_REFEREMCE_DIVIDER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_OUTPUT_FREQUENCY)
#  error CONF_CLOCK_DPLL_OUTPUT_FREQUENCY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_PRESCALER)
#  error CONF_CLOCK_DPLL_PRESCALER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_REFERENCE_GCLK_GENERATOR)
#  error CONF_CLOCK_DPLL_REFERENCE_GCLK_GENERATOR not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_DPLL_REFERENCE_GCLK_32K_GENERATOR)
#  error CONF_CLOCK_DPLL_REFERENCE_GCLK_32K_GENERATOR not defined in conf_clock.h
#endif


#if !defined(CONF_CLOCK_CONFIGURE_GCLK)
#  error CONF_CLOCK_CONFIGURE_GCLK not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_0_ENABLE)
#  error CONF_CLOCK_GCLK_0_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_0_RUN_IN_STANDBY)
#  error CONF_CLOCK_GCLK_0_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_0_CLOCK_SOURCE)
#  error CONF_CLOCK_GCLK_0_CLOCK_SOURCE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_0_PRESCALER)
#  error CONF_CLOCK_GCLK_0_PRESCALER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_0_OUTPUT_ENABLE)
#  error CONF_CLOCK_GCLK_0_OUTPUT_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_1_ENABLE)
#  error CONF_CLOCK_GCLK_1_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_1_RUN_IN_STANDBY)
#  error CONF_CLOCK_GCLK_1_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_1_CLOCK_SOURCE)
#  error CONF_CLOCK_GCLK_1_CLOCK_SOURCE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_1_PRESCALER)
#  error CONF_CLOCK_GCLK_1_PRESCALER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_1_OUTPUT_ENABLE)
#  error CONF_CLOCK_GCLK_1_OUTPUT_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_2_ENABLE)
#  error CONF_CLOCK_GCLK_2_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_2_RUN_IN_STANDBY)
#  error CONF_CLOCK_GCLK_2_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_2_CLOCK_SOURCE)
#  error CONF_CLOCK_GCLK_2_CLOCK_SOURCE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_2_PRESCALER)
#  error CONF_CLOCK_GCLK_2_PRESCALER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_2_OUTPUT_ENABLE)
#  error CONF_CLOCK_GCLK_2_OUTPUT_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_3_ENABLE)
#  error CONF_CLOCK_GCLK_3_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_3_RUN_IN_STANDBY)
#  error CONF_CLOCK_GCLK_3_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_3_CLOCK_SOURCE)
#  error CONF_CLOCK_GCLK_3_CLOCK_SOURCE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_3_PRESCALER)
#  error CONF_CLOCK_GCLK_3_PRESCALER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_3_OUTPUT_ENABLE)
#  error CONF_CLOCK_GCLK_3_OUTPUT_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_4_ENABLE)
#  error CONF_CLOCK_GCLK_4_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_4_RUN_IN_STANDBY)
#  error CONF_CLOCK_GCLK_4_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_4_CLOCK_SOURCE)
#  error CONF_CLOCK_GCLK_4_CLOCK_SOURCE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_4_PRESCALER)
#  error CONF_CLOCK_GCLK_4_PRESCALER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_4_OUTPUT_ENABLE)
#  error CONF_CLOCK_GCLK_4_OUTPUT_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_5_ENABLE)
#  error CONF_CLOCK_GCLK_5_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_5_RUN_IN_STANDBY)
#  error CONF_CLOCK_GCLK_5_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_5_CLOCK_SOURCE)
#  error CONF_CLOCK_GCLK_5_CLOCK_SOURCE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_5_PRESCALER)
#  error CONF_CLOCK_GCLK_5_PRESCALER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_5_OUTPUT_ENABLE)
#  error CONF_CLOCK_GCLK_5_OUTPUT_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_6_ENABLE)
#  error CONF_CLOCK_GCLK_6_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_6_RUN_IN_STANDBY)
#  error CONF_CLOCK_GCLK_6_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_6_CLOCK_SOURCE)
#  error CONF_CLOCK_GCLK_6_CLOCK_SOURCE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_6_PRESCALER)
#  error CONF_CLOCK_GCLK_6_PRESCALER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_6_OUTPUT_ENABLE)
#  error CONF_CLOCK_GCLK_6_OUTPUT_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_7_ENABLE)
#  error CONF_CLOCK_GCLK_7_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_7_RUN_IN_STANDBY)
#  error CONF_CLOCK_GCLK_7_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_7_CLOCK_SOURCE)
#  error CONF_CLOCK_GCLK_7_CLOCK_SOURCE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_7_PRESCALER)
#  error CONF_CLOCK_GCLK_7_PRESCALER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_7_OUTPUT_ENABLE)
#  error CONF_CLOCK_GCLK_7_OUTPUT_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_8_ENABLE)
#  error CONF_CLOCK_GCLK_8_ENABLE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_8_RUN_IN_STANDBY)
#  error CONF_CLOCK_GCLK_8_RUN_IN_STANDBY not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_8_CLOCK_SOURCE)
#  error CONF_CLOCK_GCLK_8_CLOCK_SOURCE not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_8_PRESCALER)
#  error CONF_CLOCK_GCLK_8_PRESCALER not defined in conf_clock.h
#endif

#if !defined(CONF_CLOCK_GCLK_8_OUTPUT_ENABLE)
#  error CONF_CLOCK_GCLK_8_OUTPUT_ENABLE not defined in conf_clock.h
#endif

#endif /* CLOCK_CONFIG_CHECK_H */
