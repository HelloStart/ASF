/**
 * \file
 *
 * \brief Instance description for TC5
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

#ifndef _SAML21_TC5_INSTANCE_
#define _SAML21_TC5_INSTANCE_

/* ========== Register definition for TC5 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TC5_CTRLA              (0x42002400U) /**< \brief (TC5) Control A */
#define REG_TC5_CTRLBCLR           (0x42002404U) /**< \brief (TC5) Control B Clear */
#define REG_TC5_CTRLBSET           (0x42002405U) /**< \brief (TC5) Control B Set */
#define REG_TC5_EVCTRL             (0x42002406U) /**< \brief (TC5) Event Control */
#define REG_TC5_INTENCLR           (0x42002408U) /**< \brief (TC5) Interrupt Enable Clear */
#define REG_TC5_INTENSET           (0x42002409U) /**< \brief (TC5) Interrupt Enable Set */
#define REG_TC5_INTFLAG            (0x4200240AU) /**< \brief (TC5) Interrupt Flag Status and Clear */
#define REG_TC5_STATUS             (0x4200240BU) /**< \brief (TC5) Status */
#define REG_TC5_WAVE               (0x4200240CU) /**< \brief (TC5) Waveform Generation Control */
#define REG_TC5_DRVCTRL            (0x4200240DU) /**< \brief (TC5) Control C */
#define REG_TC5_DBGCTRL            (0x4200240FU) /**< \brief (TC5) Debug Control */
#define REG_TC5_SYNCBUSY           (0x42002410U) /**< \brief (TC5) Synchronization Status */
#define REG_TC5_COUNT16_COUNT      (0x42002414U) /**< \brief (TC5) COUNT16 Count */
#define REG_TC5_COUNT16_CC0        (0x4200241CU) /**< \brief (TC5) COUNT16 Compare and Capture 0 */
#define REG_TC5_COUNT16_CC1        (0x4200241EU) /**< \brief (TC5) COUNT16 Compare and Capture 1 */
#define REG_TC5_COUNT32_COUNT      (0x42002414U) /**< \brief (TC5) COUNT32 Count */
#define REG_TC5_COUNT32_PER        (0x42002418U) /**< \brief (TC5) COUNT32 Period */
#define REG_TC5_COUNT32_CC0        (0x4200241CU) /**< \brief (TC5) COUNT32 Compare and Capture 0 */
#define REG_TC5_COUNT32_CC1        (0x42002420U) /**< \brief (TC5) COUNT32 Compare and Capture 1 */
#define REG_TC5_COUNT8_COUNT       (0x42002414U) /**< \brief (TC5) COUNT8 Count */
#define REG_TC5_COUNT8_PER         (0x4200241BU) /**< \brief (TC5) COUNT8 Period */
#define REG_TC5_COUNT8_CC0         (0x4200241CU) /**< \brief (TC5) COUNT8 Compare and Capture 0 */
#define REG_TC5_COUNT8_CC1         (0x4200241DU) /**< \brief (TC5) COUNT8 Compare and Capture 1 */
#else
#define REG_TC5_CTRLA              (*(RwReg  *)0x42002400U) /**< \brief (TC5) Control A */
#define REG_TC5_CTRLBCLR           (*(RwReg8 *)0x42002404U) /**< \brief (TC5) Control B Clear */
#define REG_TC5_CTRLBSET           (*(RwReg8 *)0x42002405U) /**< \brief (TC5) Control B Set */
#define REG_TC5_EVCTRL             (*(RwReg16*)0x42002406U) /**< \brief (TC5) Event Control */
#define REG_TC5_INTENCLR           (*(RwReg8 *)0x42002408U) /**< \brief (TC5) Interrupt Enable Clear */
#define REG_TC5_INTENSET           (*(RwReg8 *)0x42002409U) /**< \brief (TC5) Interrupt Enable Set */
#define REG_TC5_INTFLAG            (*(RwReg8 *)0x4200240AU) /**< \brief (TC5) Interrupt Flag Status and Clear */
#define REG_TC5_STATUS             (*(RwReg8 *)0x4200240BU) /**< \brief (TC5) Status */
#define REG_TC5_WAVE               (*(RwReg8 *)0x4200240CU) /**< \brief (TC5) Waveform Generation Control */
#define REG_TC5_DRVCTRL            (*(RwReg8 *)0x4200240DU) /**< \brief (TC5) Control C */
#define REG_TC5_DBGCTRL            (*(RwReg8 *)0x4200240FU) /**< \brief (TC5) Debug Control */
#define REG_TC5_SYNCBUSY           (*(RoReg  *)0x42002410U) /**< \brief (TC5) Synchronization Status */
#define REG_TC5_COUNT16_COUNT      (*(RwReg16*)0x42002414U) /**< \brief (TC5) COUNT16 Count */
#define REG_TC5_COUNT16_CC0        (*(RwReg16*)0x4200241CU) /**< \brief (TC5) COUNT16 Compare and Capture 0 */
#define REG_TC5_COUNT16_CC1        (*(RwReg16*)0x4200241EU) /**< \brief (TC5) COUNT16 Compare and Capture 1 */
#define REG_TC5_COUNT32_COUNT      (*(RwReg  *)0x42002414U) /**< \brief (TC5) COUNT32 Count */
#define REG_TC5_COUNT32_PER        (*(RwReg  *)0x42002418U) /**< \brief (TC5) COUNT32 Period */
#define REG_TC5_COUNT32_CC0        (*(RwReg  *)0x4200241CU) /**< \brief (TC5) COUNT32 Compare and Capture 0 */
#define REG_TC5_COUNT32_CC1        (*(RwReg  *)0x42002420U) /**< \brief (TC5) COUNT32 Compare and Capture 1 */
#define REG_TC5_COUNT8_COUNT       (*(RwReg8 *)0x42002414U) /**< \brief (TC5) COUNT8 Count */
#define REG_TC5_COUNT8_PER         (*(RwReg8 *)0x4200241BU) /**< \brief (TC5) COUNT8 Period */
#define REG_TC5_COUNT8_CC0         (*(RwReg8 *)0x4200241CU) /**< \brief (TC5) COUNT8 Compare and Capture 0 */
#define REG_TC5_COUNT8_CC1         (*(RwReg8 *)0x4200241DU) /**< \brief (TC5) COUNT8 Compare and Capture 1 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for TC5 peripheral ========== */
#define TC5_CC_NUM                  2
#define TC5_DMAC_ID_MC_0            31
#define TC5_DMAC_ID_MC_1            32
#define TC5_DMAC_ID_MC_LSB          31
#define TC5_DMAC_ID_MC_MSB          32
#define TC5_DMAC_ID_MC_SIZE         2
#define TC5_DMAC_ID_OVF             30
#define TC5_EXT                     0
#define TC5_GCLK_ID                 29
#define TC5_MASTER                  0
#define TC5_OW_NUM                  2

#endif /* _SAML21_TC5_INSTANCE_ */
