/**
 * \file
 *
 * \brief registers definition for ili9325 TFT display component.
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef ILI9325_REGS_H_INCLUDED
#define ILI9325_REGS_H_INCLUDED

/** @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/** @endcond */

/** ili9325 LCD Registers */

/** Driver Code Read */
#define ILI9325_DEVICE_CODE_REG (0x00u)

/** ILI9325_START_OSC_CTRL : (Offset: 0x00) Start Oscillator Control*/
#define ILI9325_START_OSC_CTRL    (0x00u)
#define ILI9325_START_OSC_CTRL_EN (0x1u << 0)

/** ILI9325_DRIVER_OUTPUT_CTRL1 : (Offset: 0x01) Driver Output Control 1*/
#define ILI9325_DRIVER_OUTPUT_CTRL1    (0x01u)
#define ILI9325_DRIVER_OUTPUT_CTRL1_SS (0x1u << 8)
#define ILI9325_DRIVER_OUTPUT_CTRL1_SM (0x1u << 10)

/** ILI9325_LCD_DRIVING_CTRL : (Offset: 0x02) LCD Driving Control */
#define ILI9325_LCD_DRIVING_CTRL       (0x02u)
#define ILI9325_LCD_DRIVING_CTRL_EOR   (0x1u << 8)
#define ILI9325_LCD_DRIVING_CTRL_BC0   (0x1u << 9)
#define ILI9325_LCD_DRIVING_CTRL_BIT10 (0x1u << 10)

/** ILI9325_ENTRY_MODE : (Offset: 0x03) Entry Mode*/
#define ILI9325_ENTRY_MODE           (0x03u)
#define ILI9325_ENTRY_MODE_AM        (0x1u << 3)
#define ILI9325_ENTRY_MODE_ID_POS     4
#define ILI9325_ENTRY_MODE_ID_MSK    (0x3u << ILI9325_ENTRY_MODE_ID_POS)
#define ILI9325_ENTRY_MODE_ID(value) ((ILI9325_ENTRY_MODE_ID_MSK & \
	((value) << ILI9325_ENTRY_MODE_ID_POS)))
#define ILI9325_ENTRY_MODE_ORG       (0x1u << 7)
#define ILI9325_ENTRY_MODE_HWM       (0x1u << 9)
#define ILI9325_ENTRY_MODE_BGR       (0x1u << 12)
#define ILI9325_ENTRY_MODE_DFM       (0x1u << 14)
#define ILI9325_ENTRY_MODE_TRI       (0x1u << 15)

/** ILI9325_RESIZE_CTRL : (Offset: 0x04) Resize Control */
#define ILI9325_RESIZE_CTRL            (0x04u)
#define ILI9325_RESIZE_CTRL_RSZ_POS     0
#define ILI9325_RESIZE_CTRL_RSZ_MSK    (0x3u << ILI9325_RESIZE_CTRL_RSZ_POS)
#define ILI9325_RESIZE_CTRL_RSZ(value) ((ILI9325_RESIZE_CTRL_RSZ_MSK & \
	((value) << ILI9325_RESIZE_CTRL_RSZ_POS)))
#define ILI9325_RESIZE_CTRL_RCH_POS     4
#define ILI9325_RESIZE_CTRL_RCH_MSK    (0x3u << ILI9325_RESIZE_CTRL_RCH_POS)
#define ILI9325_RESIZE_CTRL_RCH(value) ((ILI9325_RESIZE_CTRL_RCH_MSK & \
	((value) << ILI9325_RESIZE_CTRL_RCH_POS)))
#define ILI9325_RESIZE_CTRL_RCV_POS     8
#define ILI9325_RESIZE_CTRL_RCV_MSK    (0x3u << ILI9325_RESIZE_CTRL_RCV_POS)
#define ILI9325_RESIZE_CTRL_RCV(value) ((ILI9325_RESIZE_CTRL_RCV_MSK & \
	((value) << ILI9325_RESIZE_CTRL_RCV_POS)))

/** ILI9325_DISP_CTRL1 : (Offset: 0x07) Display Control 1 */
#define ILI9325_DISP_CTRL1             (0x07u)
#define ILI9325_DISP_CTRL1_D_POS        0
#define ILI9325_DISP_CTRL1_D_MSK       (0x3u << ILI9325_DISP_CTRL1_D_POS)
#define ILI9325_DISP_CTRL1_D(value)    ((ILI9325_DISP_CTRL1_D_MSK & \
	((value) << ILI9325_DISP_CTRL1_D_POS)))
#define ILI9325_DISP_CTRL1_CL          (0x1u << 3)
#define ILI9325_DISP_CTRL1_DTE         (0x1u << 4)
#define ILI9325_DISP_CTRL1_GON         (0x1u << 5)
#define ILI9325_DISP_CTRL1_BASEE       (0x1u << 8)
#define ILI9325_DISP_CTRL1_PTDE_POS     12
#define ILI9325_DISP_CTRL1_PTDE_MSK    (0x3u << ILI9325_DISP_CTRL1_PTDE_POS)
#define ILI9325_DISP_CTRL1_PTDE(value) ((ILI9325_DISP_CTRL1_PTDE_MSK & \
	((value) << ILI9325_DISP_CTRL1_PTDE_POS)))

/** ILI9325_DISP_CTRL2 : (Offset: 0x08) Display Control 2 */
#define ILI9325_DISP_CTRL2           (0x08u)
#define ILI9325_DISP_CTRL2_BP_POS     0
#define ILI9325_DISP_CTRL2_BP_MSK    (0xfu << ILI9325_DISP_CTRL2_BP_POS)
#define ILI9325_DISP_CTRL2_BP(value) ((ILI9325_DISP_CTRL2_BP_MSK & \
	((value) << ILI9325_DISP_CTRL2_BP_POS)))
#define ILI9325_DISP_CTRL2_FP_POS     8
#define ILI9325_DISP_CTRL2_FP_MSK    (0xfu << ILI9325_DISP_CTRL2_FP_POS)
#define ILI9325_DISP_CTRL2_FP(value) ((ILI9325_DISP_CTRL2_FP_MSK & \
	((value) << ILI9325_DISP_CTRL2_FP_POS)))

/** ILI9325_DISP_CTRL3 : (Offset: 0x09) Display Control 3 */
#define ILI9325_DISP_CTRL3            (0x09u)
#define ILI9325_DISP_CTRL3_ISC_POS     0
#define ILI9325_DISP_CTRL3_ISC_MSK    (0xfu << ILI9325_DISP_CTRL3_ISC_POS)
#define ILI9325_DISP_CTRL3_ISC(value) ((ILI9325_DISP_CTRL3_ISC_MSK & \
	((value) << ILI9325_DISP_CTRL3_ISC_POS)))
#define ILI9325_DISP_CTRL3_PTG_POS     4
#define ILI9325_DISP_CTRL3_PTG_MSK    (0x3u << ILI9325_DISP_CTRL3_PTG_POS)
#define ILI9325_DISP_CTRL3_PTG(value) ((ILI9325_DISP_CTRL3_PTG_MSK & \
	((value) << ILI9325_DISP_CTRL3_PTG_POS)))
#define ILI9325_DISP_CTRL3_PTS_POS     8
#define ILI9325_DISP_CTRL3_PTS_MSK    (0x7u << ILI9325_DISP_CTRL3_PTS_POS)
#define ILI9325_DISP_CTRL3_PTS(value) ((ILI9325_DISP_CTRL3_PTS_MSK & \
	((value) << ILI9325_DISP_CTRL3_PTS_POS)))

/** ILI9325_DISP_CTRL4 : (Offset: 0x0A) Display Control 4 */
#define ILI9325_DISP_CTRL4            (0x0Au)
#define ILI9325_DISP_CTRL4_FMI_POS     0
#define ILI9325_DISP_CTRL4_FMI_MSK    (0x7u << ILI9325_DISP_CTRL4_FMI_POS)
#define ILI9325_DISP_CTRL4_FMI(value) ((ILI9325_DISP_CTRL4_FMI_MSK & \
	((value) << ILI9325_DISP_CTRL4_FMI_POS)))
#define ILI9325_DISP_CTRL4_FMARKOE    (0x1u << 3)

/** ILI9325_RGB_DISP_INTERFACE_CTRL1 : (Offset: 0x0C) RGB Display
 *Interface Control 1 */
#define ILI9325_RGB_DISP_INTERFACE_CTRL1            (0x0Cu)
#define ILI9325_RGB_DISP_INTERFACE_CTRL1_RIM_POS     0
#define ILI9325_RGB_DISP_INTERFACE_CTRL1_RIM_MSK    (0x3u << \
	ILI9325_RGB_DISP_INTERFACE_CTRL1_RIM_POS)
#define ILI9325_RGB_DISP_INTERFACE_CTRL1_RIM(value) (( \
		ILI9325_RGB_DISP_INTERFACE_CTRL1_RIM_MSK & \
		((value) << ILI9325_RGB_DISP_INTERFACE_CTRL1_RIM_POS)))
#define ILI9325_RGB_DISP_INTERFACE_CTRL1_DM0        (0x1u << 4)
#define ILI9325_RGB_DISP_INTERFACE_CTRL1_DM1        (0x1u << 5)
#define ILI9325_RGB_DISP_INTERFACE_CTRL1_DM_POS      4
#define ILI9325_RGB_DISP_INTERFACE_CTRL1_DM_MSK     (0x3u << \
	ILI9325_RGB_DISP_INTERFACE_CTRL1_DM_POS)
#define ILI9325_RGB_DISP_INTERFACE_CTRL1_DM(value)  (( \
		ILI9325_RGB_DISP_INTERFACE_CTRL1_DM_MSK & \
		((value) << ILI9325_RGB_DISP_INTERFACE_CTRL1_DM_POS)))
#define ILI9325_RGB_DISP_INTERFACE_CTRL1_RM         (0x1u << 8)
#define ILI9325_RGB_DISP_INTERFACE_CTRL1_ENC_POS     12
#define ILI9325_RGB_DISP_INTERFACE_CTRL1_ENC_MSK    (0x7u << \
	ILI9325_RGB_DISP_INTERFACE_CTRL1_ENC_POS)
#define ILI9325_RGB_DISP_INTERFACE_CTRL1_ENC(value) (( \
		ILI9325_RGB_DISP_INTERFACE_CTRL1_ENC_MSK & \
		((value) < ILI9325_RGB_DISP_INTERFACE_CTRL1_ENC_POS)))

/** ILI9325_FRAME_MAKER_POS : (Offset: 0x0D) Frame Maker Position */
#define ILI9325_FRAME_MAKER_POS            (0x0Du)
#define ILI9325_FRAME_MAKER_POS_FMP_POS     0
#define ILI9325_FRAME_MAKER_POS_FMP_MSK    (0x1ffu << \
	ILI9325_FRAME_MAKER_POS_FMP_POS)
#define ILI9325_FRAME_MAKER_POS_FMP(value) ((ILI9325_FRAME_MAKER_POS_FMP_MSK & \
	((value) << ILI9325_FRAME_MAKER_POS_FMP_POS)))

/** ILI9325_RGB_DISP_INTERFACE_CTRL2 : (Offset: 0x0F) RGB Display
 * Interface Control 2 */
#define ILI9325_RGB_DISP_INTERFACE_CTRL2      (0x0Fu)
#define ILI9325_RGB_DISP_INTERFACE_CTRL2_EPL  (0x1u << 0)
#define ILI9325_RGB_DISP_INTERFACE_CTRL2_DPL  (0x1u << 1)
#define ILI9325_RGB_DISP_INTERFACE_CTRL2_HSPL (0x1u << 3)
#define ILI9325_RGB_DISP_INTERFACE_CTRL2_VSPL (0x1u << 4)

/** ILI9325_POWER_CTRL1 : (Offset: 0x10) Power Control 1 */
#define ILI9325_POWER_CTRL1           (0x10u)
#define ILI9325_POWER_CTRL1_STB       (0x1u << 0)
#define ILI9325_POWER_CTRL1_SLP       (0x1u << 1)
#define ILI9325_POWER_CTRL1_DSTB      (0x1u << 2)
#define ILI9325_POWER_CTRL1_AP_POS     4
#define ILI9325_POWER_CTRL1_AP_MSK    (0x7u << ILI9325_POWER_CTRL1_AP_POS)
#define ILI9325_POWER_CTRL1_AP(value) ((ILI9325_POWER_CTRL1_AP_MSK & \
	((value) << ILI9325_POWER_CTRL1_AP_POS)))
#define ILI9325_POWER_CTRL1_APE       (0x1u << 7)
#define ILI9325_POWER_CTRL1_BT_POS     8
#define ILI9325_POWER_CTRL1_BT_MSK    (0x7u << ILI9325_POWER_CTRL1_BT_POS)
#define ILI9325_POWER_CTRL1_BT(value) ((ILI9325_POWER_CTRL1_BT_MSK & \
	((value) << ILI9325_POWER_CTRL1_BT_POS)))
#define ILI9325_POWER_CTRL1_SAP       (0x1u << 12)

/** ILI9325_POWER_CTRL2 : (Offset: 0x11) Power Control 2 */
#define ILI9325_POWER_CTRL2            (0x11u)
#define ILI9325_POWER_CTRL2_VC_POS      0
#define ILI9325_POWER_CTRL2_VC_MSK     (0x7u << ILI9325_POWER_CTRL2_VC_POS)
#define ILI9325_POWER_CTRL2_VC(value)  ((ILI9325_POWER_CTRL2_VC_MSK & \
	((value) << ILI9325_POWER_CTRL2_VC_POS)))
#define ILI9325_POWER_CTRL2_DC0_POS     4
#define ILI9325_POWER_CTRL2_DC0_MSK    (0x7u << ILI9325_POWER_CTRL2_DC0_POS)
#define ILI9325_POWER_CTRL2_DC0(value) ((ILI9325_POWER_CTRL2_DC0_MSK & \
	((value) << ILI9325_POWER_CTRL2_DC0_POS)))
#define ILI9325_POWER_CTRL2_DC1_POS     8
#define ILI9325_POWER_CTRL2_DC1_MSK    (0x7u << ILI9325_POWER_CTRL2_DC1_POS)
#define ILI9325_POWER_CTRL2_DC1(value) ((ILI9325_POWER_CTRL2_DC1_MSK & \
	((value) << ILI9325_POWER_CTRL2_DC1_POS)))

/** ILI9325_POWER_CTRL3 : (Offset: 0x12) Power Control 3 */
#define ILI9325_POWER_CTRL3            (0x12u)
#define ILI9325_POWER_CTRL3_VRH_POS     0
#define ILI9325_POWER_CTRL3_VRH_MSK    (0xfu << ILI9325_POWER_CTRL3_VRH_POS)
#define ILI9325_POWER_CTRL3_VRH(value) ((ILI9325_POWER_CTRL3_VRH_MSK & \
	((value) << ILI9325_POWER_CTRL3_VRH_POS)))
#define ILI9325_POWER_CTRL3_PON        (0x1u << 4)
#define ILI9325_POWER_CTRL3_VCIRE      (0x1u << 7)

/** ILI9325_POWER_CTRL4 : (Offset: 0x13) Power Control 4 */
#define ILI9325_POWER_CTRL4            (0x13u)
#define ILI9325_POWER_CTRL4_VDV_POS     8
#define ILI9325_POWER_CTRL4_VDV_MSK    (0x1fu << ILI9325_POWER_CTRL4_VDV_POS)
#define ILI9325_POWER_CTRL4_VDV(value) ((ILI9325_POWER_CTRL4_VDV_MSK & \
	((value) << ILI9325_POWER_CTRL4_VDV_POS)))

/** ILI9325_HORIZONTAL_GRAM_ADDR_SET : (Offset: 0x20) Horizontal GRAM
 * Address Set */
#define ILI9325_HORIZONTAL_GRAM_ADDR_SET           (0x20u)
#define ILI9325_HORIZONTAL_GRAM_ADDR_SET_AD_POS     0
#define ILI9325_HORIZONTAL_GRAM_ADDR_SET_AD_MSK    (0xffu << \
	ILI9325_HORIZONTAL_GRAM_ADDR_SET_AD_POS)
#define ILI9325_HORIZONTAL_GRAM_ADDR_SET_AD(value) (( \
		ILI9325_HORIZONTAL_GRAM_ADDR_SET_AD_MSK & \
		((value) << ILI9325_HORIZONTAL_GRAM_ADDR_SET_AD_POS)))

/** ILI9325_VERTICAL_GRAM_ADDR_SET : (Offset: 0x21) Vertical  GRAM
 * Address Set */
#define ILI9325_VERTICAL_GRAM_ADDR_SET           (0x21u)
#define ILI9325_VERTICAL_GRAM_ADDR_SET_AD_POS     0
#define ILI9325_VERTICAL_GRAM_ADDR_SET_AD_MSK    (0xffu << \
	ILI9325_VERTICAL_GRAM_ADDR_SET_AD_POS)
#define ILI9325_VERTICAL_GRAM_ADDR_SET_AD(value) (( \
		ILI9325_VERTICAL_GRAM_ADDR_SET_AD_MSK &	\
		((value) << ILI9325_VERTICAL_GRAM_ADDR_SET_AD_POS)))

/** ILI9325_GRAM_DATA_REG : (Offset: 0x22) GRAM Data Register */
#define ILI9325_GRAM_DATA_REG (0x22u)

/** ILI9325_POWER_CTRL7 : (Offset: 0x29) Power Control 7 */
#define ILI9325_POWER_CTRL7            (0x29u)
#define ILI9325_POWER_CTRL7_VCM_POS     0
#define ILI9325_POWER_CTRL7_VCM_MSK    (0x3fu << ILI9325_POWER_CTRL7_VCM_POS)
#define ILI9325_POWER_CTRL7_VCM(value) ((ILI9325_POWER_CTRL7_VCM_MSK & \
	((value) << ILI9325_POWER_CTRL7_VCM_POS)))

/** ILI9325_FRAME_RATE_AND_COLOR_CTRL : (Offset: 0x2B) Frame Rate and
 * Color Control */
#define ILI9325_FRAME_RATE_AND_COLOR_CTRL            (0x2Bu)
#define ILI9325_FRAME_RATE_AND_COLOR_CTRL_FRS_POS     0
#define ILI9325_FRAME_RATE_AND_COLOR_CTRL_FRS_MSK    (0xfu << \
	ILI9325_FRAME_RATE_AND_COLOR_CTRL_FRS_POS)
#define ILI9325_FRAME_RATE_AND_COLOR_CTRL_FRS(value) ((	\
		ILI9325_FRAME_RATE_AND_COLOR_CTRL_FRS_MSK & \
		((value) << ILI9325_FRAME_RATE_AND_COLOR_CTRL_FRS_POS)))

/** ILI9325_GAMMA_CTL1 : (Offset: 0x30) Gamma Control 1 */
#define ILI9325_GAMMA_CTL1            (0x30u)
#define ILI9325_GAMMA_CTL1_KP0_POS     0
#define ILI9325_GAMMA_CTL1_KP0_MSK    (0x7u << ILI9325_GAMMA_CTL1_KP0_POS)
#define ILI9325_GAMMA_CTL1_KP0(value) ((ILI9325_GAMMA_CTL1_KP0_MSK & \
	((value) << ILI9325_GAMMA_CTL1_KP0_POS)))
#define ILI9325_GAMMA_CTL1_KP1_POS     8
#define ILI9325_GAMMA_CTL1_KP1_MSK    (0x7u << ILI9325_GAMMA_CTL1_KP1_POS)
#define ILI9325_GAMMA_CTL1_KP1(value) ((ILI9325_GAMMA_CTL1_KP1_MSK & \
	((value) << ILI9325_GAMMA_CTL1_KP1_POS)))

/** ILI9325_GAMMA_CTL2 : (Offset: 0x31) Gamma Control 2 */
#define ILI9325_GAMMA_CTL2            (0x31u)
#define ILI9325_GAMMA_CTL2_KP2_POS     0
#define ILI9325_GAMMA_CTL2_KP2_MSK    (0x7u << ILI9325_GAMMA_CTL2_KP2_POS)
#define ILI9325_GAMMA_CTL2_KP2(value) ((ILI9325_GAMMA_CTL2_KP2_MSK & \
	((value) << ILI9325_GAMMA_CTL2_KP2_POS)))
#define ILI9325_GAMMA_CTL2_KP3_POS     8
#define ILI9325_GAMMA_CTL2_KP3_MSK    (0x7u << ILI9325_GAMMA_CTL2_KP3_POS)
#define ILI9325_GAMMA_CTL2_KP3(value) ((ILI9325_GAMMA_CTL2_KP3_MSK & \
	((value) << ILI9325_GAMMA_CTL2_KP3_POS)))

/** ILI9325_GAMMA_CTL3 : (Offset: 0x32) Gamma Control 3 */
#define ILI9325_GAMMA_CTL3            (0x32u)
#define ILI9325_GAMMA_CTL3_KP4_POS     0
#define ILI9325_GAMMA_CTL3_KP4_MSK    (0x7u << ILI9325_GAMMA_CTL3_KP4_POS)
#define ILI9325_GAMMA_CTL3_KP4(value) ((ILI9325_GAMMA_CTL3_KP4_MSK & \
	((value) << ILI9325_GAMMA_CTL3_KP4_POS)))
#define ILI9325_GAMMA_CTL3_KP5_POS     8
#define ILI9325_GAMMA_CTL3_KP5_MSK    (0x7u << ILI9325_GAMMA_CTL3_KP5_POS)
#define ILI9325_GAMMA_CTL3_KP5(value) ((ILI9325_GAMMA_CTL3_KP5_MSK & \
	((value) << ILI9325_GAMMA_CTL3_KP5_POS)))

/** ILI9325_GAMMA_CTL4 : (Offset: 0x35) Gamma Control 4 */
#define ILI9325_GAMMA_CTL4            (0x35u)
#define ILI9325_GAMMA_CTL4_RP0_POS     0
#define ILI9325_GAMMA_CTL4_RP0_MSK    (0x7u << ILI9325_GAMMA_CTL4_RP0_POS)
#define ILI9325_GAMMA_CTL4_RP0(value) ((ILI9325_GAMMA_CTL4_RP0_MSK & \
	((value) << ILI9325_GAMMA_CTL4_RP0_POS)))
#define ILI9325_GAMMA_CTL4_RP1_POS     8
#define ILI9325_GAMMA_CTL4_RP1_MSK    (0x7u << ILI9325_GAMMA_CTL4_RP1_POS)
#define ILI9325_GAMMA_CTL4_RP1(value) ((ILI9325_GAMMA_CTL4_RP1_MSK & \
	((value) << ILI9325_GAMMA_CTL4_RP1_POS)))

/** ILI9325_GAMMA_CTL5 : (Offset: 0x36) Gamma Control 5 */
#define ILI9325_GAMMA_CTL5             (0x36u)
#define ILI9325_GAMMA_CTL5_VRP0_POS     0
#define ILI9325_GAMMA_CTL5_VRP0_MSK    (0xfu << ILI9325_GAMMA_CTL5_VRP0_POS)
#define ILI9325_GAMMA_CTL5_VRP0(value) ((ILI9325_GAMMA_CTL5_VRP0_MSK & \
	((value) << ILI9325_GAMMA_CTL5_VRP0_POS)))
#define ILI9325_GAMMA_CTL5_VRP1_POS     8
#define ILI9325_GAMMA_CTL5_VRP1_MSK    (0x1fu << ILI9325_GAMMA_CTL5_VRP1_POS)
#define ILI9325_GAMMA_CTL5_VRP1(value) ((ILI9325_GAMMA_CTL5_VRP1_MSK & \
	((value) << ILI9325_GAMMA_CTL5_VRP1_POS)))

/** ILI9325_GAMMA_CTL6 : (Offset: 0x37) Gamma Control 6*/
#define ILI9325_GAMMA_CTL6            (0x37u)
#define ILI9325_GAMMA_CTL6_KN0_POS     0
#define ILI9325_GAMMA_CTL6_KN0_MSK    (0x7u << ILI9325_GAMMA_CTL6_KN0_POS)
#define ILI9325_GAMMA_CTL6_KN0(value) ((ILI9325_GAMMA_CTL6_KN0_MSK & \
	((value) << ILI9325_GAMMA_CTL6_KN0_POS)))
#define ILI9325_GAMMA_CTL6_KN1_POS     8
#define ILI9325_GAMMA_CTL6_KN1_MSK    (0x7u << ILI9325_GAMMA_CTL6_KN1_POS)
#define ILI9325_GAMMA_CTL6_KN1(value) ((ILI9325_GAMMA_CTL6_KN1_MSK \
	& ((value) << ILI9325_GAMMA_CTL6_KN1_POS)))

/** ILI9325_GAMMA_CTL7 : (Offset: 0x38) Gamma Control 7*/
#define ILI9325_GAMMA_CTL7            (0x38u)
#define ILI9325_GAMMA_CTL7_KN2_POS     0
#define ILI9325_GAMMA_CTL7_KN2_MSK    (0x7u << ILI9325_GAMMA_CTL7_KN2_POS)
#define ILI9325_GAMMA_CTL7_KN2(value) ((ILI9325_GAMMA_CTL7_KN2_MSK & \
	((value) << ILI9325_GAMMA_CTL7_KN2_POS)))
#define ILI9325_GAMMA_CTL7_KN3_POS     8
#define ILI9325_GAMMA_CTL7_KN3_MSK    (0x7u << ILI9325_GAMMA_CTL7_KN3_POS)
#define ILI9325_GAMMA_CTL7_KN3(value) ((ILI9325_GAMMA_CTL7_KN3_MSK & \
	((value) << ILI9325_GAMMA_CTL7_KN3_POS)))

/** ILI9325_GAMMA_CTL8 : (Offset: 0x39) Gamma Control 8*/
#define ILI9325_GAMMA_CTL8            (0x39u)
#define ILI9325_GAMMA_CTL8_KN4_POS     0
#define ILI9325_GAMMA_CTL8_KN4_MSK    (0x7u << ILI9325_GAMMA_CTL8_KN4_POS)
#define ILI9325_GAMMA_CTL8_KN4(value) ((ILI9325_GAMMA_CTL8_KN4_MSK & \
	((value) << ILI9325_GAMMA_CTL8_KN4_POS)))
#define ILI9325_GAMMA_CTL8_KN5_POS     8
#define ILI9325_GAMMA_CTL8_KN5_MSK    (0x7u << ILI9325_GAMMA_CTL8_KN5_POS)
#define ILI9325_GAMMA_CTL8_KN5(value) ((ILI9325_GAMMA_CTL8_KN5_MSK & \
	((value) << ILI9325_GAMMA_CTL8_KN5_POS)))

/** ILI9325_GAMMA_CTL9 : (Offset: 0x3C) Gamma Control 9*/
#define ILI9325_GAMMA_CTL9            (0x3Cu)
#define ILI9325_GAMMA_CTL9_RN0_POS     0
#define ILI9325_GAMMA_CTL9_RN0_MSK    (0x7u << ILI9325_GAMMA_CTL9_RN0_POS)
#define ILI9325_GAMMA_CTL9_RN0(value) ((ILI9325_GAMMA_CTL9_RN0_MSK & \
	((value) << ILI9325_GAMMA_CTL9_RN0_POS)))
#define ILI9325_GAMMA_CTL9_RN1_POS     8
#define ILI9325_GAMMA_CTL9_RN1_MSK    (0x7u << ILI9325_GAMMA_CTL9_RN1_POS)
#define ILI9325_GAMMA_CTL9_RN1(value) ((ILI9325_GAMMA_CTL9_RN1_MSK & \
	((value) << ILI9325_GAMMA_CTL9_RN1_POS)))

/** ILI9325_GAMMA_CTL10 : (Offset: 0x3D) Gamma Control 10*/
#define ILI9325_GAMMA_CTL10             (0x3Du)
#define ILI9325_GAMMA_CTL10_VRN0_POS     0
#define ILI9325_GAMMA_CTL10_VRN0_MSK    (0xfu << ILI9325_GAMMA_CTL10_VRN0_POS)
#define ILI9325_GAMMA_CTL10_VRN0(value) ((ILI9325_GAMMA_CTL10_VRN0_MSK & \
	((value) << ILI9325_GAMMA_CTL10_VRN0_POS)))
#define ILI9325_GAMMA_CTL10_VRN1_POS     8
#define ILI9325_GAMMA_CTL10_VRN1_MSK    (0x1fu << \
	ILI9325_GAMMA_CTL10_VRN1_POS)
#define ILI9325_GAMMA_CTL10_VRN1(value) ((ILI9325_GAMMA_CTL10_VRN1_MSK & \
	((value) << ILI9325_GAMMA_CTL10_VRN1_POS)))

/**
 * ILI9325_HORIZONTAL_ADDR_START : (Offset: 0x50) Horizontal Address
 * Start Position
 */
#define ILI9325_HORIZONTAL_ADDR_START            (0x50u)
#define ILI9325_HORIZONTAL_ADDR_START_HSA_POS     0
#define ILI9325_HORIZONTAL_ADDR_START_HSA_MSK    (0xffu << \
	ILI9325_HORIZONTAL_ADDR_START_HSA_POS)
#define ILI9325_HORIZONTAL_ADDR_START_HSA(value) (( \
		ILI9325_HORIZONTAL_ADDR_START_HSA_MSK &	\
		((value) << ILI9325_HORIZONTAL_ADDR_START_HSA_POS)))

/**
 * ILI9325_HORIZONTAL_ADDR_END : (Offset: 0x51) Horizontal Address End
 * Position
 */
#define ILI9325_HORIZONTAL_ADDR_END            (0x51u)
#define ILI9325_HORIZONTAL_ADDR_END_HEA_POS     0
#define ILI9325_HORIZONTAL_ADDR_END_HEA_MSK    (0xffu << \
	ILI9325_HORIZONTAL_ADDR_END_HEA_POS)
#define ILI9325_HORIZONTAL_ADDR_END_HEA(value) (( \
		ILI9325_HORIZONTAL_ADDR_END_HEA_MSK & \
		((value) << ILI9325_HORIZONTAL_ADDR_END_HEA_POS)))

/**
 * ILI9325_VERTICAL_ADDR_START : (Offset: 0x52) Vertical Address Start
 * Position
 */
#define ILI9325_VERTICAL_ADDR_START            (0x52u)
#define ILI9325_VERTICAL_ADDR_START_VSA_POS     0
#define ILI9325_VERTICAL_ADDR_START_VSA_MSK    (0x1ffu << \
	ILI9325_VERTICAL_ADDR_START_VSA_POS)
#define ILI9325_VERTICAL_ADDR_START_VSA(value) (( \
		ILI9325_VERTICAL_ADDR_START_VSA_MSK & \
		((value) << ILI9325_VERTICAL_ADDR_START_VSA_POS)))

/**
 * ILI9325_VERTICAL_ADDR_END : (Offset: 0x53) Vertical Address End
 * Position
 */
#define ILI9325_VERTICAL_ADDR_END            (0x53u)
#define ILI9325_VERTICAL_ADDR_END_VEA_POS     0
#define ILI9325_VERTICAL_ADDR_END_VEA_MSK    (0x1ffu <<	\
	ILI9325_VERTICAL_ADDR_END_VEA_POS)
#define ILI9325_VERTICAL_ADDR_END_VEA(value) ((ILI9325_VERTICAL_ADDR_END_VEA_MSK \
	& ((value) << ILI9325_VERTICAL_ADDR_END_VEA_POS)))

/**
 * ILI9325_DRIVER_OUTPUT_CTRL2 : (Offset: 0x60) Driver Output
 * Control 2
 */
#define ILI9325_DRIVER_OUTPUT_CTRL2            (0x60u)
#define ILI9325_DRIVER_OUTPUT_CTRL2_SCN_POS     0
#define ILI9325_DRIVER_OUTPUT_CTRL2_SCN_MSK    (0x3fu << \
	ILI9325_DRIVER_OUTPUT_CTRL2_SCN_POS)
#define ILI9325_DRIVER_OUTPUT_CTRL2_SCN(value) (( \
		ILI9325_DRIVER_OUTPUT_CTRL2_SCN_MSK & \
		((value) << ILI9325_DRIVER_OUTPUT_CTRL2_SCN_POS)))
#define ILI9325_DRIVER_OUTPUT_CTRL2_NL_POS      8
#define ILI9325_DRIVER_OUTPUT_CTRL2_NL_MSK     (0x3fu << \
	ILI9325_DRIVER_OUTPUT_CTRL2_NL_POS)
#define ILI9325_DRIVER_OUTPUT_CTRL2_NL(value)  (( \
		ILI9325_DRIVER_OUTPUT_CTRL2_NL_MSK & \
		((value) << ILI9325_DRIVER_OUTPUT_CTRL2_NL_POS)))
#define ILI9325_DRIVER_OUTPUT_CTRL2_GS         (0x1u << 15)

/**
 * ILI9325_BASE_IMG_DISP_CTRL : (Offset: 0x61) Base Image Display
 * Control
 */
#define ILI9325_BASE_IMG_DISP_CTRL     (0x61u)
#define ILI9325_BASE_IMG_DISP_CTRL_REV (0x1u << 0)
#define ILI9325_BASE_IMG_DISP_CTRL_VLE (0x1u << 1)
#define ILI9325_BASE_IMG_DISP_CTRL_NDL (0x1u << 2)

/**
 * ILI9325_VERTICAL_SCROLL_CTRL : (Offset: 0x6A) Vertical Scroll
 * Control
 */
#define ILI9325_VERTICAL_SCROLL_CTRL           (0x6Au)
#define ILI9325_VERTICAL_SCROLL_CTRL_VL_POS     0
#define ILI9325_VERTICAL_SCROLL_CTRL_VL_MSK    (0x1ffu << \
	ILI9325_VERTICAL_SCROLL_CTRL_VL_POS)
#define ILI9325_VERTICAL_SCROLL_CTRL_VL(value) (( \
		ILI9325_VERTICAL_SCROLL_CTRL_VL_MSK & \
		((value) << ILI9325_VERTICAL_SCROLL_CTRL_VL_POS)))

/**
 * ILI9325_PARTIAL_IMG1_DISP_POS : (Offset: 0x80) Partial Image 1
 * Display Position
 */
#define ILI9325_PARTIAL_IMG1_DISP_POS              (0x80u)
#define ILI9325_PARTIAL_IMG1_DISP_POS_PTDP0_POS     0
#define ILI9325_PARTIAL_IMG1_DISP_POS_PTDP0_MSK    (0x1ffu << \
	ILI9325_PARTIAL_IMG1_DISP_POS_PTDP0_POS)
#define ILI9325_PARTIAL_IMG1_DISP_POS_PTDP0(value) (( \
		ILI9325_PARTIAL_IMG1_DISP_POS_PTDP0_MSK & \
		((value) << ILI9325_PARTIAL_IMG1_DISP_POS_PTDP0_POS)))

/**
 * ILI9325_PARTIAL_IMG1_AREA_START_LINE : (Offset: 0x81) Partial Image
 * 1 Area (Start Line)
 */
#define ILI9325_PARTIAL_IMG1_AREA_START_LINE              (0x81u)
#define ILI9325_PARTIAL_IMG1_AREA_START_LINE_PTSA0_POS     0
#define ILI9325_PARTIAL_IMG1_AREA_START_LINE_PTSA0_MSK    (0x1ffu << \
	ILI9325_PARTIAL_IMG1_AREA_START_LINE_PTSA0_POS)
#define ILI9325_PARTIAL_IMG1_AREA_START_LINE_PTSA0(value) (( \
		ILI9325_PARTIAL_IMG1_AREA_START_LINE_PTSA0_MSK & \
		((value) << \
		ILI9325_PARTIAL_IMG1_AREA_START_LINE_PTSA0_POS)))

/**
 * ILI9325_PARTIAL_IMG1_AREA_END_LINE : (Offset: 0x82) Partial Image 1
 * Area (End Line)
 */
#define ILI9325_PARTIAL_IMG1_AREA_END_LINE              (0x82u)
#define ILI9325_PARTIAL_IMG1_AREA_END_LINE_PTEA0_POS     0
#define ILI9325_PARTIAL_IMG1_AREA_END_LINE_PTEA0_MSK    (0x1ffu << \
	ILI9325_PARTIAL_IMG1_AREA_END_LINE_PTEA0_POS)
#define ILI9325_PARTIAL_IMG1_AREA_END_LINE_PTEA0(value) (( \
		ILI9325_PARTIAL_IMG1_AREA_END_LINE_PTEA0_MSK & \
		((value) << \
		ILI9325_PARTIAL_IMG1_AREA_END_LINE_PTEA0_POS)))

/**
 * ILI9325_PARTIAL_IMG2_DISP_POS : (Offset: 0x83) Partial Image 2
 * Display Position
 */
#define ILI9325_PARTIAL_IMG2_DISP_POS              (0x83u)
#define ILI9325_PARTIAL_IMG2_DISP_POS_PTDP1_POS     0
#define ILI9325_PARTIAL_IMG2_DISP_POS_PTDP1_MSK    (0x1ffu << \
	ILI9325_PARTIAL_IMG2_DISP_POS_PTDP1_POS)
#define ILI9325_PARTIAL_IMG2_DISP_POS_PTDP1(value) (( \
		ILI9325_PARTIAL_IMG2_DISP_POS_PTDP1_MSK & \
		((value) << ILI9325_PARTIAL_IMG2_DISP_POS_PTDP1_POS)))

/**
 * ILI9325_PARTIAL_IMG2_AREA_START_LINE : (Offset: 0x84) Partial Image
 * 2 Area (Start Line)
 */
#define ILI9325_PARTIAL_IMG2_AREA_START_LINE              (0x84u)
#define ILI9325_PARTIAL_IMG2_AREA_START_LINE_PTSA1_POS     0
#define ILI9325_PARTIAL_IMG2_AREA_START_LINE_PTSA1_MSK    (0x1ffu << \
	ILI9325_PARTIAL_IMG2_AREA_START_LINE_PTSA1_POS)
#define ILI9325_PARTIAL_IMG2_AREA_START_LINE_PTSA1(value) (( \
		ILI9325_PARTIAL_IMG2_AREA_START_LINE_PTSA1_MSK & \
		((value) << \
		ILI9325_PARTIAL_IMG2_AREA_START_LINE_PTSA1_POS)))

/**
 * ILI9325_PARTIAL_IMG2_AREA_END_LINE : (Offset: 0x85) Partial Image 2
 * Area (End Line)
 */
#define ILI9325_PARTIAL_IMG2_AREA_END_LINE              (0x85u)
#define ILI9325_PARTIAL_IMG2_AREA_END_LINE_PTEA1_POS     0
#define ILI9325_PARTIAL_IMG2_AREA_END_LINE_PTEA1_MSK    (0x1ffu << \
	ILI9325_PARTIAL_IMG2_AREA_END_LINE_PTEA1_POS)
#define ILI9325_PARTIAL_IMG2_AREA_END_LINE_PTEA1(value) (( \
		ILI9325_PARTIAL_IMG2_AREA_END_LINE_PTEA1_MSK & \
		((value) << \
		ILI9325_PARTIAL_IMG2_AREA_END_LINE_PTEA1_POS)))

/**
 * ILI9325_PANEL_INTERFACE_CTRL1 : (Offset: 0x90) Panel Interface
 * Control 1
 */
#define ILI9325_PANEL_INTERFACE_CTRL1             (0x90u)
#define ILI9325_PANEL_INTERFACE_CTRL1_RTNI_POS     0
#define ILI9325_PANEL_INTERFACE_CTRL1_RTNI_MSK    (0x1fu << \
	ILI9325_PANEL_INTERFACE_CTRL1_RTNI_POS)
#define ILI9325_PANEL_INTERFACE_CTRL1_RTNI(value) (( \
		ILI9325_PANEL_INTERFACE_CTRL1_RTNI_MSK & \
		((value) << ILI9325_PANEL_INTERFACE_CTRL1_RTNI_POS)))
#define ILI9325_PANEL_INTERFACE_CTRL1_DIVI_POS     8
#define ILI9325_PANEL_INTERFACE_CTRL1_DIVI_MSK    (0x3u << \
	ILI9325_PANEL_INTERFACE_CTRL1_DIVI_POS)
#define ILI9325_PANEL_INTERFACE_CTRL1_DIVI(value) (( \
		ILI9325_PANEL_INTERFACE_CTRL1_DIVI_MSK & \
		((value) << ILI9325_PANEL_INTERFACE_CTRL1_DIVI_POS)))

/**
 * ILI9325_PANEL_INTERFACE_CTRL2 : (Offset: 0x92) Panel Interface
 * Control 2
 */
#define ILI9325_PANEL_INTERFACE_CTRL2             (0x92u)
#define ILI9325_PANEL_INTERFACE_CTRL2_NOWI_POS     8
#define ILI9325_PANEL_INTERFACE_CTRL2_NOWI_MSK    (0x7u << \
	ILI9325_PANEL_INTERFACE_CTRL2_NOWI_POS)
#define ILI9325_PANEL_INTERFACE_CTRL2_NOWI(value) (( \
		ILI9325_PANEL_INTERFACE_CTRL2_NOWI_MSK & \
		((value) << ILI9325_PANEL_INTERFACE_CTRL2_NOWI_POS)))

/**
 * ILI9325_PANEL_INTERFACE_CTRL4 : (Offset: 0x95) Panel Interface
 * Control 4
 */
#define ILI9325_PANEL_INTERFACE_CTRL4             (0x95u)
#define ILI9325_PANEL_INTERFACE_CTRL4_RTNE_POS     0
#define ILI9325_PANEL_INTERFACE_CTRL4_RTNE_MSK    (0x3fu << \
	ILI9325_PANEL_INTERFACE_CTRL4_RTNE_POS)
#define ILI9325_PANEL_INTERFACE_CTRL4_RTNE(value) (( \
		ILI9325_PANEL_INTERFACE_CTRL4_RTNE_MSK & \
		((value) << ILI9325_PANEL_INTERFACE_CTRL4_RTNE_POS)))
#define ILI9325_PANEL_INTERFACE_CTRL4_DIVE_POS     8
#define ILI9325_PANEL_INTERFACE_CTRL4_DIVE_MSK    (0x3u << \
	ILI9325_PANEL_INTERFACE_CTRL4_DIVE_POS)
#define ILI9325_PANEL_INTERFACE_CTRL4_DIVE(value) (( \
		ILI9325_PANEL_INTERFACE_CTRL4_DIVE_MSK & \
		((value) << ILI9325_PANEL_INTERFACE_CTRL4_DIVE_POS)))

/** ILI9325_OTP_VCM_PROG_CTRL : (Offset: 0xA1) OTP VCM Programming Control */
#define ILI9325_OTP_VCM_PROG_CTRL                (0xA1u)
#define ILI9325_OTP_VCM_PROG_CTRL_VCM_OTP_POS     0
#define ILI9325_OTP_VCM_PROG_CTRL_VCM_OTP_MSK    (0x3fu << \
	ILI9325_OTP_VCM_PROG_CTRL_VCM_OTP_POS)
#define ILI9325_OTP_VCM_PROG_CTRL_VCM_OTP(value) (( \
		ILI9325_OTP_VCM_PROG_CTRL_VCM_OTP_MSK &	\
		((value) << ILI9325_OTP_VCM_PROG_CTRL_VCM_OTP_POS)))
#define ILI9325_OTP_VCM_PROG_CTRL_OTP_PGM_EN     (0x1u << 11)

/** ILI9325_OTP_VCM_STATUS_AND_ENABLE : (Offset: 0xA2) OTP VCM Status
 * and Enable   */
#define ILI9325_OTP_VCM_STATUS_AND_ENABLE                (0xA2u)
#define ILI9325_OTP_VCM_STATUS_AND_ENABLE_VCM_EN         (0x1u << 0)
#define ILI9325_OTP_VCM_STATUS_AND_ENABLE_VCM_D_POS       8
#define ILI9325_OTP_VCM_STATUS_AND_ENABLE_VCM_D_MSK      (0x3fu << \
	ILI9325_OTP_VCM_STATUS_AND_ENABLE_VCM_D_POS)
#define ILI9325_OTP_VCM_STATUS_AND_ENABLE_VCM_D(value)   (( \
		ILI9325_OTP_VCM_STATUS_AND_ENABLE_VCM_D_MSK & ((value) << \
		ILI9325_OTP_VCM_STATUS_AND_ENABLE_VCM_D_POS)))
#define ILI9325_OTP_VCM_STATUS_AND_ENABLE_PGM_CNT_POS     14
#define ILI9325_OTP_VCM_STATUS_AND_ENABLE_PGM_CNT_MSK    (0x3u << \
	ILI9325_OTP_VCM_STATUS_AND_ENABLE_PGM_CNT_POS)
#define ILI9325_OTP_VCM_STATUS_AND_ENABLE_PGM_CNT(value) (( \
		ILI9325_OTP_VCM_STATUS_AND_ENABLE_PGM_CNT_MSK &	\
		((value) << \
		IILI9325_OTP_VCM_STATUS_AND_ENABLE_PGM_CNT_POS)))

/** ILI9325_OTP_PROG_ID_KEY : (Offset: 0xA5) OTP Programming ID Key */
#define ILI9325_OTP_PROG_ID_KEY            (0xA5u)
#define ILI9325_OTP_PROG_ID_KEY_KEY_POS     0
#define ILI9325_OTP_PROG_ID_KEY_KEY_MSK    (0xffffu << \
	ILI9325_OTP_PROG_ID_KEY_KEY_POS)
#define ILI9325_OTP_PROG_ID_KEY_KEY(value) ((ILI9325_OTP_PROG_ID_KEY_KEY_MSK & \
	((value) << ILI9325_OTP_PROG_ID_KEY_KEY_POS)))

/** @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/** @endcond */

#endif /* ILI9325_REGS_H_INCLUDED */
