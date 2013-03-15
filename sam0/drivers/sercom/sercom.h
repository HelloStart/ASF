/**
 * \file
 *
 * \brief SAMD20 Serial Peripheral Interface Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

#ifndef SERCOM_H_INCLUDED
#define SERCOM_H_INCLUDED

#include <compiler.h>
#include <system.h>
#include <clock.h>
#include "sercom_interrupt.h"
#include "sercom_pinout.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (SERCOM0_GCLK_ID_SLOW == SERCOM1_GCLK_ID_SLOW && \
     SERCOM0_GCLK_ID_SLOW == SERCOM2_GCLK_ID_SLOW && \
     SERCOM0_GCLK_ID_SLOW == SERCOM3_GCLK_ID_SLOW)
#  define SERCOM_GCLK_ID SERCOM0_GCLK_ID_SLOW
#else
#  error "SERCOM modules must share the same slow GCLK channel ID."
#endif

enum status_code sercom_set_gclk_generator(
		enum gclk_generator generator_source,
		bool run_in_standby,
		bool force_change);

enum status_code _sercom_get_sync_baud_val(uint32_t baudrate,
		uint32_t external_clock, uint16_t *baudval);

enum status_code _sercom_get_async_baud_val(uint32_t baudrate,
		uint32_t peripheral_clock, uint16_t *baudval);

uint32_t _sercom_get_default_pad(Sercom *sercom_module, uint8_t pad);

#ifdef __cplusplus
}
#endif

#endif //__SERCOM_H_INCLUDED
