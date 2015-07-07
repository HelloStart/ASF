/**
 * \file
 *
 * \brief Component description for PM
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

#ifndef _SAML22_PM_COMPONENT_
#define _SAML22_PM_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR PM */
/* ========================================================================== */
/** \addtogroup SAML22_PM Power Manager */
/*@{*/

#define PM_U2240
#define REV_PM                      0x210

/* -------- PM_CTRLA : (PM Offset: 0x00) (R/W  8) Control A -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  :2;               /*!< bit:  0.. 1  Reserved                           */
    uint8_t  IORET:1;          /*!< bit:      2  I/O Retention                      */
    uint8_t  :5;               /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_CTRLA_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PM_CTRLA_OFFSET             0x00         /**< \brief (PM_CTRLA offset) Control A */
#define PM_CTRLA_RESETVALUE         0x00ul       /**< \brief (PM_CTRLA reset_value) Control A */

#define PM_CTRLA_IORET_Pos          2            /**< \brief (PM_CTRLA) I/O Retention */
#define PM_CTRLA_IORET              (0x1ul << PM_CTRLA_IORET_Pos)
#define PM_CTRLA_MASK               0x04ul       /**< \brief (PM_CTRLA) MASK Register */

/* -------- PM_SLEEPCFG : (PM Offset: 0x01) (R/W  8) Sleep Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  SLEEPMODE:3;      /*!< bit:  0.. 2  Sleep Mode                         */
    uint8_t  :5;               /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_SLEEPCFG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PM_SLEEPCFG_OFFSET          0x01         /**< \brief (PM_SLEEPCFG offset) Sleep Configuration */
#define PM_SLEEPCFG_RESETVALUE      0x02ul       /**< \brief (PM_SLEEPCFG reset_value) Sleep Configuration */

#define PM_SLEEPCFG_SLEEPMODE_Pos   0            /**< \brief (PM_SLEEPCFG) Sleep Mode */
#define PM_SLEEPCFG_SLEEPMODE_Msk   (0x7ul << PM_SLEEPCFG_SLEEPMODE_Pos)
#define PM_SLEEPCFG_SLEEPMODE(value) (PM_SLEEPCFG_SLEEPMODE_Msk & ((value) << PM_SLEEPCFG_SLEEPMODE_Pos))
#define   PM_SLEEPCFG_SLEEPMODE_IDLE0_Val 0x0ul  /**< \brief (PM_SLEEPCFG) CPU clock is OFF */
#define   PM_SLEEPCFG_SLEEPMODE_IDLE1_Val 0x1ul  /**< \brief (PM_SLEEPCFG) AHB clock is OFF */
#define   PM_SLEEPCFG_SLEEPMODE_IDLE2_Val 0x2ul  /**< \brief (PM_SLEEPCFG) APB clock are OFF */
#define   PM_SLEEPCFG_SLEEPMODE_STANDBY_Val 0x4ul  /**< \brief (PM_SLEEPCFG) All Clocks are OFF */
#define   PM_SLEEPCFG_SLEEPMODE_BACKUP_Val 0x5ul  /**< \brief (PM_SLEEPCFG) Only Backup domain is powered ON */
#define   PM_SLEEPCFG_SLEEPMODE_OFF_Val   0x6ul  /**< \brief (PM_SLEEPCFG) All power domains are powered OFF */
#define PM_SLEEPCFG_SLEEPMODE_IDLE0 (PM_SLEEPCFG_SLEEPMODE_IDLE0_Val << PM_SLEEPCFG_SLEEPMODE_Pos)
#define PM_SLEEPCFG_SLEEPMODE_IDLE1 (PM_SLEEPCFG_SLEEPMODE_IDLE1_Val << PM_SLEEPCFG_SLEEPMODE_Pos)
#define PM_SLEEPCFG_SLEEPMODE_IDLE2 (PM_SLEEPCFG_SLEEPMODE_IDLE2_Val << PM_SLEEPCFG_SLEEPMODE_Pos)
#define PM_SLEEPCFG_SLEEPMODE_STANDBY (PM_SLEEPCFG_SLEEPMODE_STANDBY_Val << PM_SLEEPCFG_SLEEPMODE_Pos)
#define PM_SLEEPCFG_SLEEPMODE_BACKUP (PM_SLEEPCFG_SLEEPMODE_BACKUP_Val << PM_SLEEPCFG_SLEEPMODE_Pos)
#define PM_SLEEPCFG_SLEEPMODE_OFF   (PM_SLEEPCFG_SLEEPMODE_OFF_Val << PM_SLEEPCFG_SLEEPMODE_Pos)
#define PM_SLEEPCFG_MASK            0x07ul       /**< \brief (PM_SLEEPCFG) MASK Register */

/* -------- PM_PLCFG : (PM Offset: 0x02) (R/W  8) Performance Level Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  PLSEL:2;          /*!< bit:  0.. 1  Performance Level Select           */
    uint8_t  :5;               /*!< bit:  2.. 6  Reserved                           */
    uint8_t  PLDIS:1;          /*!< bit:      7  Performance Level Disable          */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_PLCFG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PM_PLCFG_OFFSET             0x02         /**< \brief (PM_PLCFG offset) Performance Level Configuration */
#define PM_PLCFG_RESETVALUE         0x00ul       /**< \brief (PM_PLCFG reset_value) Performance Level Configuration */

#define PM_PLCFG_PLSEL_Pos          0            /**< \brief (PM_PLCFG) Performance Level Select */
#define PM_PLCFG_PLSEL_Msk          (0x3ul << PM_PLCFG_PLSEL_Pos)
#define PM_PLCFG_PLSEL(value)       (PM_PLCFG_PLSEL_Msk & ((value) << PM_PLCFG_PLSEL_Pos))
#define   PM_PLCFG_PLSEL_PL0_Val          0x0ul  /**< \brief (PM_PLCFG) Performance Level 0 */
#define   PM_PLCFG_PLSEL_PL1_Val          0x1ul  /**< \brief (PM_PLCFG) Performance Level 1 */
#define   PM_PLCFG_PLSEL_PL2_Val          0x2ul  /**< \brief (PM_PLCFG) Performance Level 2 */
#define PM_PLCFG_PLSEL_PL0          (PM_PLCFG_PLSEL_PL0_Val        << PM_PLCFG_PLSEL_Pos)
#define PM_PLCFG_PLSEL_PL1          (PM_PLCFG_PLSEL_PL1_Val        << PM_PLCFG_PLSEL_Pos)
#define PM_PLCFG_PLSEL_PL2          (PM_PLCFG_PLSEL_PL2_Val        << PM_PLCFG_PLSEL_Pos)
#define PM_PLCFG_PLDIS_Pos          7            /**< \brief (PM_PLCFG) Performance Level Disable */
#define PM_PLCFG_PLDIS              (0x1ul << PM_PLCFG_PLDIS_Pos)
#define PM_PLCFG_MASK               0x83ul       /**< \brief (PM_PLCFG) MASK Register */

/* -------- PM_INTENCLR : (PM Offset: 0x04) (R/W  8) Interrupt Enable Clear -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  PLRDY:1;          /*!< bit:      0  Performance Level Interrupt Enable */
    uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_INTENCLR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PM_INTENCLR_OFFSET          0x04         /**< \brief (PM_INTENCLR offset) Interrupt Enable Clear */
#define PM_INTENCLR_RESETVALUE      0x00ul       /**< \brief (PM_INTENCLR reset_value) Interrupt Enable Clear */

#define PM_INTENCLR_PLRDY_Pos       0            /**< \brief (PM_INTENCLR) Performance Level Interrupt Enable */
#define PM_INTENCLR_PLRDY           (0x1ul << PM_INTENCLR_PLRDY_Pos)
#define PM_INTENCLR_MASK            0x01ul       /**< \brief (PM_INTENCLR) MASK Register */

/* -------- PM_INTENSET : (PM Offset: 0x05) (R/W  8) Interrupt Enable Set -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  PLRDY:1;          /*!< bit:      0  Performance Level Ready interrupt Enable */
    uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_INTENSET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PM_INTENSET_OFFSET          0x05         /**< \brief (PM_INTENSET offset) Interrupt Enable Set */
#define PM_INTENSET_RESETVALUE      0x00ul       /**< \brief (PM_INTENSET reset_value) Interrupt Enable Set */

#define PM_INTENSET_PLRDY_Pos       0            /**< \brief (PM_INTENSET) Performance Level Ready interrupt Enable */
#define PM_INTENSET_PLRDY           (0x1ul << PM_INTENSET_PLRDY_Pos)
#define PM_INTENSET_MASK            0x01ul       /**< \brief (PM_INTENSET) MASK Register */

/* -------- PM_INTFLAG : (PM Offset: 0x06) (R/W  8) Interrupt Flag Status and Clear -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { // __I to avoid read-modify-write on write-to-clear register
  struct {
    __I uint8_t  PLRDY:1;          /*!< bit:      0  Performance Level Ready            */
    __I uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PM_INTFLAG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PM_INTFLAG_OFFSET           0x06         /**< \brief (PM_INTFLAG offset) Interrupt Flag Status and Clear */
#define PM_INTFLAG_RESETVALUE       0x00ul       /**< \brief (PM_INTFLAG reset_value) Interrupt Flag Status and Clear */

#define PM_INTFLAG_PLRDY_Pos        0            /**< \brief (PM_INTFLAG) Performance Level Ready */
#define PM_INTFLAG_PLRDY            (0x1ul << PM_INTFLAG_PLRDY_Pos)
#define PM_INTFLAG_MASK             0x01ul       /**< \brief (PM_INTFLAG) MASK Register */

/* -------- PM_STDBYCFG : (PM Offset: 0x08) (R/W 16) Standby Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t :6;               /*!< bit:  0.. 5  Reserved                           */
    uint16_t VREGSMOD:2;       /*!< bit:  6.. 7  Voltage Regulator Standby mode     */
    uint16_t :2;               /*!< bit:  8.. 9  Reserved                           */
    uint16_t BBIASHS:2;        /*!< bit: 10..11  Back Bias for HMCRAMCHS            */
    uint16_t :4;               /*!< bit: 12..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PM_STDBYCFG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PM_STDBYCFG_OFFSET          0x08         /**< \brief (PM_STDBYCFG offset) Standby Configuration */
#define PM_STDBYCFG_RESETVALUE      0x0400ul     /**< \brief (PM_STDBYCFG reset_value) Standby Configuration */

#define PM_STDBYCFG_VREGSMOD_Pos    6            /**< \brief (PM_STDBYCFG) Voltage Regulator Standby mode */
#define PM_STDBYCFG_VREGSMOD_Msk    (0x3ul << PM_STDBYCFG_VREGSMOD_Pos)
#define PM_STDBYCFG_VREGSMOD(value) (PM_STDBYCFG_VREGSMOD_Msk & ((value) << PM_STDBYCFG_VREGSMOD_Pos))
#define   PM_STDBYCFG_VREGSMOD_AUTO_Val   0x0ul  /**< \brief (PM_STDBYCFG) Automatic mode */
#define   PM_STDBYCFG_VREGSMOD_PERFORMANCE_Val 0x1ul  /**< \brief (PM_STDBYCFG) Performance oriented */
#define   PM_STDBYCFG_VREGSMOD_LP_Val     0x2ul  /**< \brief (PM_STDBYCFG) Low Power oriented */
#define PM_STDBYCFG_VREGSMOD_AUTO   (PM_STDBYCFG_VREGSMOD_AUTO_Val << PM_STDBYCFG_VREGSMOD_Pos)
#define PM_STDBYCFG_VREGSMOD_PERFORMANCE (PM_STDBYCFG_VREGSMOD_PERFORMANCE_Val << PM_STDBYCFG_VREGSMOD_Pos)
#define PM_STDBYCFG_VREGSMOD_LP     (PM_STDBYCFG_VREGSMOD_LP_Val   << PM_STDBYCFG_VREGSMOD_Pos)
#define PM_STDBYCFG_BBIASHS_Pos     10           /**< \brief (PM_STDBYCFG) Back Bias for HMCRAMCHS */
#define PM_STDBYCFG_BBIASHS_Msk     (0x3ul << PM_STDBYCFG_BBIASHS_Pos)
#define PM_STDBYCFG_BBIASHS(value)  (PM_STDBYCFG_BBIASHS_Msk & ((value) << PM_STDBYCFG_BBIASHS_Pos))
#define PM_STDBYCFG_MASK            0x0CC0ul     /**< \brief (PM_STDBYCFG) MASK Register */

/** \brief PM hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  __IO PM_CTRLA_Type             CTRLA;       /**< \brief Offset: 0x00 (R/W  8) Control A */
  __IO PM_SLEEPCFG_Type          SLEEPCFG;    /**< \brief Offset: 0x01 (R/W  8) Sleep Configuration */
  __IO PM_PLCFG_Type             PLCFG;       /**< \brief Offset: 0x02 (R/W  8) Performance Level Configuration */
       RoReg8                    Reserved1[0x1];
  __IO PM_INTENCLR_Type          INTENCLR;    /**< \brief Offset: 0x04 (R/W  8) Interrupt Enable Clear */
  __IO PM_INTENSET_Type          INTENSET;    /**< \brief Offset: 0x05 (R/W  8) Interrupt Enable Set */
  __IO PM_INTFLAG_Type           INTFLAG;     /**< \brief Offset: 0x06 (R/W  8) Interrupt Flag Status and Clear */
       RoReg8                    Reserved2[0x1];
  __IO PM_STDBYCFG_Type          STDBYCFG;    /**< \brief Offset: 0x08 (R/W 16) Standby Configuration */
} Pm;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAML22_PM_COMPONENT_ */
