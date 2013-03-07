/**
 * \file
 *
 * \brief API driver for re200b pir sensor component.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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
 * \defgroup re200b_pir_sensor_group
 *
 * Low-level driver for the re200b pir sensor. This driver provides access to
 * the main features of the  re200b pir sensor.
 *
 * \{
 */

#include "re200b.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/** indicate if there's any comparison event triggered */
volatile uint32_t g_ul_compare_event_flag = false;

/** indicate type of comparison event(CMP_GREATER or CMP_LESS) */
static volatile E_CMP_RES g_compare_result = CMP_EQUAL;

/**
 * \brief re200b_motion_detect_handler which allow to know comparison result
 * (CMP_GREATER or CMP_LESS)
 */
void re200b_motion_detect_handler( void )
{
	uint32_t ul_status;

	ul_status = acc_get_interrupt_status(ACC);
	/* Compare Output Interrupt */
	if ((ul_status & ACC_IER_CE) == ACC_IER_CE) {
		g_ul_compare_event_flag = true;

		if (acc_get_comparison_result(ACC)) {
			g_compare_result = CMP_GREATER;
		} else {
			g_compare_result = CMP_LESS;
		}
	}
}

/**
 * \brief Initialize re200b sensor in order to detect motion
 */
void re200b_motion_detect_init(void)
{
	pmc_enable_periph_clk( ID_ACC );

	/* Initialize ACC */
	acc_init( ACC, ACC_MR_SELPLUS_AD4, ACC_MR_SELMINUS_AD0,
			ACC_MR_EDGETYP_ANY, ACC_MR_INV_DIS);

	/* select comparison pair */
	acc_set_input(ACC, ACC_MR_SELMINUS_AD0, ACC_MR_SELPLUS_AD4);

	/* clear status */
	acc_get_interrupt_status(ACC);

	/* reset event flags */
	g_compare_result = CMP_EQUAL;
	g_ul_compare_event_flag = false;
}

/**
 * \brief Reset variable used in re200b sensor functions
 */
void re200b_motion_detect_reset(void)
{
	g_ul_compare_event_flag = false;
	g_compare_result = CMP_EQUAL;
}

/**
 * \brief Enable re200b pir sensor
 */
void re200b_motion_detect_enable(void)
{
	acc_enable(ACC);
	/* Enable compasion interrupt */
	acc_enable_interrupt(ACC);
	/* Enable ACC interrupt */
	NVIC_EnableIRQ(ACC_IRQn);
}

/**
 * \brief Disable re200b pir sensor
 */
void re200b_motion_detect_disable(void)
{
	NVIC_DisableIRQ(ACC_IRQn);
	acc_disable_interrupt(ACC);
	acc_disable(ACC);
}

/**
 * \brief return comparison result
 */
E_CMP_RES re200b_motion_detect_get_compare_result(void)
{
	return g_compare_result;
}

/**
 * \brief Allow to know if a motion was detected
 *
 * \return 1 if motion is detected; 0 otherwise
 */
uint32_t re200b_motion_detection(void)
{
	return (g_ul_compare_event_flag && (g_compare_result == CMP_GREATER));
}

#ifdef __cplusplus
}
#endif

/**
 * \}
 */
