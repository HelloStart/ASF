/**
 * \file
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#ifndef TC_HEADER_H_INCLUDED
#define TC_HEADER_H_INCLUDED


#ifndef __ASSEMBLY__
#include <stdint.h>
#ifndef __cplusplus
typedef volatile const uint8_t  RoReg8;  /**< Read only  8-bit register (volatile const unsigned int) */
#else
typedef volatile       uint8_t  RoReg8;  /**< Read only  8-bit register (volatile const unsigned int) */
#endif
#define CAST(type, value) ((type *)(value))
#define REG_ACCESS(type, address) (*(type*)(address)) /**< C code: Register value */
#else
#define CAST(type, value) (value)
#define REG_ACCESS(type, address) (address) /**< Assembly code: Register address */
#endif



#define REV_TC         0x110

#ifndef __ASSEMBLY__

#ifndef __EXCLUDE_TC_FROM_CMSIS__

/* -------- TC_CTRLA : (TC Offset: 0x00) (R/W 16) Control A Register -------- */
typedef union {
  struct {
    uint16_t SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t MODE:2;           /*!< bit:  2.. 3  Timer Counter Mode                 */
    uint16_t _reserved0:1;     /*!< bit:      4  Reserved                           */
    uint16_t WAVEGEN:2;        /*!< bit:  5.. 6  Waveform Generation Operation      */
    uint16_t _reserved1:1;     /*!< bit:      7  Reserved                           */
    uint16_t PRESCALER:3;      /*!< bit:  8..10  Prescaler                          */
    uint16_t RUNSTDBY:1;       /*!< bit:     11  Run during Standby                 */
    uint16_t PRESCSYNC:2;      /*!< bit: 12..13  Prescaler and Counter Synchronization */
    uint16_t _reserved2:2;     /*!< bit: 14..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} TC_CTRLA_Type;


#define   TC_CTRLBSET_CMD_NONE      (0x0u <<  6)
#define   TC_CTRLBSET_CMD_RETRIGGER (0x1u <<  6)
#define   TC_CTRLBSET_CMD_STOP      (0x2u <<  6)


#define TC_CTRLA_OFFSET             0x00         /**< \brief (TC_CTRLA offset) Control A Register */
#define TC_CTRLA_RESETVALUE         0x0          /**< \brief (TC_CTRLA reset_value) Control A Register */

#define TC_CTRLA_SWRST              (0x1u <<  0) /**< \brief (TC_CTRLA) Software Reset */
#define TC_CTRLA_ENABLE             (0x1u <<  1) /**< \brief (TC_CTRLA) Enable */
#define TC_CTRLA_MODE_Pos           2
#define TC_CTRLA_MODE_Msk           (0x3u << TC_CTRLA_MODE_Pos) /**< \brief (TC_CTRLA) Timer Counter Mode */
#define TC_CTRLA_MODE(value)        ((TC_CTRLA_MODE_Msk & ((value) << TC_CTRLA_MODE_Pos)))
#define   TC_CTRLA_MODE_COUNT16     (0x0u <<  2) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_MODE_COUNT8      (0x1u <<  2) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_MODE_COUNT32     (0x2u <<  2) /**< \brief (TC_CTRLA)  */
#define TC_CTRLA_WAVEGEN_Pos        5
#define TC_CTRLA_WAVEGEN_Msk        (0x3u << TC_CTRLA_WAVEGEN_Pos) /**< \brief (TC_CTRLA) Waveform Generation Operation */
#define TC_CTRLA_WAVEGEN(value)     ((TC_CTRLA_WAVEGEN_Msk & ((value) << TC_CTRLA_WAVEGEN_Pos)))
#define   TC_CTRLA_WAVEGEN_NFRQ     (0x0u <<  5) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_WAVEGEN_MFRQ     (0x1u <<  5) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_WAVEGEN_NPWM     (0x2u <<  5) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_WAVEGEN_MPWM     (0x3u <<  5) /**< \brief (TC_CTRLA)  */
#define TC_CTRLA_PRESCALER_Pos      8
#define TC_CTRLA_PRESCALER_Msk      (0x7u << TC_CTRLA_PRESCALER_Pos) /**< \brief (TC_CTRLA) Prescaler */
#define TC_CTRLA_PRESCALER(value)   ((TC_CTRLA_PRESCALER_Msk & ((value) << TC_CTRLA_PRESCALER_Pos)))
#define TC_CTRLA_RUNSTDBY           (0x1u << 11) /**< \brief (TC_CTRLA) Run during Standby */
#define TC_CTRLA_PRESCSYNC_Pos      12
#define TC_CTRLA_PRESCSYNC_Msk      (0x3u << TC_CTRLA_PRESCSYNC_Pos) /**< \brief (TC_CTRLA) Prescaler and Counter Synchronization */
#define TC_CTRLA_PRESCSYNC(value)   ((TC_CTRLA_PRESCSYNC_Msk & ((value) << TC_CTRLA_PRESCSYNC_Pos)))
#define   TC_CTRLA_PRESCSYNC_GCLK   (0x0u << 12) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_PRESCSYNC_PRESC  (0x1u << 12) /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_PRESCSYNC_RESYNC (0x2u << 12) /**< \brief (TC_CTRLA)  */
#define TC_CTRLA_MASK               0x3F6Fu /**< \brief (TC_CTRLA) MASK Register */

/* -------- TC_READREQ : (TC Offset: 0x02) (R/W 16) Read Request Register -------- */
typedef union {
  struct {
    uint16_t ADDR:5;           /*!< bit:  0.. 4  Address                            */
    uint16_t _reserved0:9;     /*!< bit:  5..13  Reserved                           */
    uint16_t RCONT:1;          /*!< bit:     14  Read Continuously                  */
    uint16_t RREQ:1;           /*!< bit:     15  Read Request                       */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} TC_READREQ_Type;

#define TC_READREQ_OFFSET           0x02         /**< \brief (TC_READREQ offset) Read Request Register */
#define TC_READREQ_RESETVALUE       0x0          /**< \brief (TC_READREQ reset_value) Read Request Register */

#define TC_READREQ_ADDR_Pos         0
#define TC_READREQ_ADDR_Msk         (0x1Fu << TC_READREQ_ADDR_Pos) /**< \brief (TC_READREQ) Address */
#define TC_READREQ_ADDR(value)      ((TC_READREQ_ADDR_Msk & ((value) << TC_READREQ_ADDR_Pos)))
#define TC_READREQ_RCONT            (0x1u << 14) /**< \brief (TC_READREQ) Read Continuously */
#define TC_READREQ_RREQ             (0x1u << 15) /**< \brief (TC_READREQ) Read Request */
#define TC_READREQ_MASK             0xC01Fu /**< \brief (TC_READREQ) MASK Register */

/* -------- TC_CTRLBCLR : (TC Offset: 0x04) (R/W  8) Control B Clear Register -------- */
typedef union {
  struct {
    uint8_t  DIR:1;            /*!< bit:      0  Counter Direction                  */
    uint8_t  _reserved0:1;     /*!< bit:      1  Reserved                           */
    uint8_t  ONESHOT:1;        /*!< bit:      2  One-Shot on Counter                */
    uint8_t  _reserved1:3;     /*!< bit:  3.. 5  Reserved                           */
    uint8_t  CMD:2;            /*!< bit:  6.. 7  Command                            */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_CTRLBCLR_Type;

#define TC_CTRLBCLR_OFFSET          0x04         /**< \brief (TC_CTRLBCLR offset) Control B Clear Register */
#define TC_CTRLBCLR_RESETVALUE      0x0          /**< \brief (TC_CTRLBCLR reset_value) Control B Clear Register */

#define TC_CTRLBCLR_DIR             (0x1u <<  0) /**< \brief (TC_CTRLBCLR) Counter Direction */
#define TC_CTRLBCLR_ONESHOT         (0x1u <<  2) /**< \brief (TC_CTRLBCLR) One-Shot on Counter */
#define TC_CTRLBCLR_CMD_Pos         6
#define TC_CTRLBCLR_CMD_Msk         (0x3u << TC_CTRLBCLR_CMD_Pos) /**< \brief (TC_CTRLBCLR) Command */
#define TC_CTRLBCLR_CMD(value)      ((TC_CTRLBCLR_CMD_Msk & ((value) << TC_CTRLBCLR_CMD_Pos)))
#define   TC_CTRLBCLR_CMD_NONE      (0x0u <<  6) /**< \brief (TC_CTRLBCLR)  */
#define   TC_CTRLBCLR_CMD_RETRIGGER (0x1u <<  6) /**< \brief (TC_CTRLBCLR)  */
#define   TC_CTRLBCLR_CMD_STOP      (0x2u <<  6) /**< \brief (TC_CTRLBCLR)  */
#define TC_CTRLBCLR_MASK            0xC5u /**< \brief (TC_CTRLBCLR) MASK Register */

/* -------- TC_CTRLBSET : (TC Offset: 0x05) (R/W  8) Control B Set Register -------- */
typedef union {
  struct {
    uint8_t  DIR:1;            /*!< bit:      0  Counter Direction                  */
    uint8_t  _reserved0:1;     /*!< bit:      1  Reserved                           */
    uint8_t  ONESHOT:1;        /*!< bit:      2  One-Shot on Counter                */
    uint8_t  _reserved1:3;     /*!< bit:  3.. 5  Reserved                           */
    uint8_t  CMD:2;            /*!< bit:  6.. 7  Command                            */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_CTRLBSET_Type;

#define TC_CTRLBSET_OFFSET          0x05         /**< \brief (TC_CTRLBSET offset) Control B Set Register */
#define TC_CTRLBSET_RESETVALUE      0x0          /**< \brief (TC_CTRLBSET reset_value) Control B Set Register */

#define TC_CTRLBSET_DIR             (0x1u <<  0) /**< \brief (TC_CTRLBSET) Counter Direction */
#define TC_CTRLBSET_ONESHOT         (0x1u <<  2) /**< \brief (TC_CTRLBSET) One-Shot on Counter */
#define TC_CTRLBSET_CMD_Pos         6
#define TC_CTRLBSET_CMD_Msk         (0x3u << TC_CTRLBSET_CMD_Pos) /**< \brief (TC_CTRLBSET) Command */
#define TC_CTRLBSET_CMD(value)      ((TC_CTRLBSET_CMD_Msk & ((value) << TC_CTRLBSET_CMD_Pos)))
#define TC_CTRLBSET_MASK            0xC5u /**< \brief (TC_CTRLBSET) MASK Register */

/* -------- TC_CTRLC : (TC Offset: 0x06) (R/W  8) Control C Register -------- */
typedef union {
  struct {
    uint8_t  INVEN:2;          /*!< bit:  0.. 1  Output Waveform Invert Enable      */
    uint8_t  _reserved0:2;     /*!< bit:  2.. 3  Reserved                           */
    uint8_t  CPTEN:2;          /*!< bit:  4.. 5  Capture Channel  Enable            */
    uint8_t  _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_CTRLC_Type;

#define TC_CTRLC_OFFSET             0x06         /**< \brief (TC_CTRLC offset) Control C Register */
#define TC_CTRLC_RESETVALUE         0x0          /**< \brief (TC_CTRLC reset_value) Control C Register */

#define TC_CTRLC_INVEN_Pos          0
#define TC_CTRLC_INVEN_Msk          (0x3u << TC_CTRLC_INVEN_Pos) /**< \brief (TC_CTRLC) Output Waveform Invert Enable */
#define TC_CTRLC_INVEN(value)       ((TC_CTRLC_INVEN_Msk & ((value) << TC_CTRLC_INVEN_Pos)))
#define TC_CTRLC_CPTEN_Pos          4
#define TC_CTRLC_CPTEN_Msk          (0x3u << TC_CTRLC_CPTEN_Pos) /**< \brief (TC_CTRLC) Capture Channel  Enable */
#define TC_CTRLC_CPTEN(value)       ((TC_CTRLC_CPTEN_Msk & ((value) << TC_CTRLC_CPTEN_Pos)))
#define TC_CTRLC_MASK               0x33u /**< \brief (TC_CTRLC) MASK Register */

/* -------- TC_DEBUG : (TC Offset: 0x08) (R/W  8) Debug Register -------- */
typedef union {
  struct {
    uint8_t  DBGRUN:1;         /*!< bit:      0  Run During Debug                   */
    uint8_t  _reserved0:7;     /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_DEBUG_Type;

#define TC_DEBUG_OFFSET             0x08         /**< \brief (TC_DEBUG offset) Debug Register */
#define TC_DEBUG_RESETVALUE         0x0          /**< \brief (TC_DEBUG reset_value) Debug Register */

#define TC_DEBUG_DBGRUN             (0x1u <<  0) /**< \brief (TC_DEBUG) Run During Debug */
#define TC_DEBUG_MASK               0x1u /**< \brief (TC_DEBUG) MASK Register */

/* -------- TC_EVCTRL : (TC Offset: 0x0A) (R/W 16) Event Control Register -------- */
typedef union {
  struct {
    uint16_t EVACT:3;          /*!< bit:  0.. 2  Event Action                       */
    uint16_t _reserved0:1;     /*!< bit:      3  Reserved                           */
    uint16_t TCINV:1;          /*!< bit:      4  TC Event Input Polarity            */
    uint16_t TCEI:1;           /*!< bit:      5  TC Event Enable                    */
    uint16_t _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
    uint16_t OVFEO:1;          /*!< bit:      8  Event Output Enable                */
    uint16_t _reserved2:3;     /*!< bit:  9..11  Reserved                           */
    uint16_t MCEO:2;           /*!< bit: 12..13  MC Event Output Enable             */
    uint16_t _reserved3:2;     /*!< bit: 14..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} TC_EVCTRL_Type;

#define TC_EVCTRL_OFFSET            0x0A         /**< \brief (TC_EVCTRL offset) Event Control Register */
#define TC_EVCTRL_RESETVALUE        0x0          /**< \brief (TC_EVCTRL reset_value) Event Control Register */

#define TC_EVCTRL_EVACT_Pos         0
#define TC_EVCTRL_EVACT_Msk         (0x7u << TC_EVCTRL_EVACT_Pos) /**< \brief (TC_EVCTRL) Event Action */
#define TC_EVCTRL_EVACT(value)      ((TC_EVCTRL_EVACT_Msk & ((value) << TC_EVCTRL_EVACT_Pos)))
#define   TC_EVCTRL_EVACT_OFF       (0x0u <<  0) /**< \brief (TC_EVCTRL)  */
#define   TC_EVCTRL_EVACT_RETRIGGER (0x1u <<  0) /**< \brief (TC_EVCTRL)  */
#define   TC_EVCTRL_EVACT_COUNT     (0x2u <<  0) /**< \brief (TC_EVCTRL)  */
#define   TC_EVCTRL_EVACT_START     (0x3u <<  0) /**< \brief (TC_EVCTRL)  */
#define   TC_EVCTRL_EVACT_PPW       (0x5u <<  0) /**< \brief (TC_EVCTRL)  */
#define   TC_EVCTRL_EVACT_PWP       (0x6u <<  0) /**< \brief (TC_EVCTRL)  */
#define TC_EVCTRL_TCINV             (0x1u <<  4) /**< \brief (TC_EVCTRL) TC Event Input Polarity */
#define TC_EVCTRL_TCEI              (0x1u <<  5) /**< \brief (TC_EVCTRL) TC Event Enable */
#define TC_EVCTRL_OVFEO             (0x1u <<  8) /**< \brief (TC_EVCTRL) Event Output Enable */
#define TC_EVCTRL_MCEO_Pos          12
#define TC_EVCTRL_MCEO_Msk          (0x3u << TC_EVCTRL_MCEO_Pos) /**< \brief (TC_EVCTRL) MC Event Output Enable */
#define TC_EVCTRL_MCEO(value)       ((TC_EVCTRL_MCEO_Msk & ((value) << TC_EVCTRL_MCEO_Pos)))
#define TC_EVCTRL_MASK              0x3137u /**< \brief (TC_EVCTRL) MASK Register */

/* -------- TC_INTENCLR : (TC Offset: 0x0C) (R/W  8) Interrupt Enable Clear Register -------- */
typedef union {
  struct {
    uint8_t  OVF:1;            /*!< bit:      0  OVF Interrupt Disable              */
    uint8_t  ERR:1;            /*!< bit:      1  ERR Interrupt Disable              */
    uint8_t  _reserved0:1;     /*!< bit:      2  Reserved                           */
    uint8_t  READY:1;          /*!< bit:      3  READY Interrupt Disable            */
    uint8_t  MC:2;             /*!< bit:  4.. 5  MC Interrupt Disable               */
    uint8_t  _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_INTENCLR_Type;

#define TC_INTENCLR_OFFSET          0x0C         /**< \brief (TC_INTENCLR offset) Interrupt Enable Clear Register */
#define TC_INTENCLR_RESETVALUE      0x0          /**< \brief (TC_INTENCLR reset_value) Interrupt Enable Clear Register */

#define TC_INTENCLR_OVF             (0x1u <<  0) /**< \brief (TC_INTENCLR) OVF Interrupt Disable */
#define TC_INTENCLR_ERR             (0x1u <<  1) /**< \brief (TC_INTENCLR) ERR Interrupt Disable */
#define TC_INTENCLR_READY           (0x1u <<  3) /**< \brief (TC_INTENCLR) READY Interrupt Disable */
#define TC_INTENCLR_MC_Pos          4
#define TC_INTENCLR_MC_Msk          (0x3u << TC_INTENCLR_MC_Pos) /**< \brief (TC_INTENCLR) MC Interrupt Disable */
#define TC_INTENCLR_MC(value)       ((TC_INTENCLR_MC_Msk & ((value) << TC_INTENCLR_MC_Pos)))
#define TC_INTENCLR_MASK            0x3Bu /**< \brief (TC_INTENCLR) MASK Register */

/* -------- TC_INTENSET : (TC Offset: 0x0D) (R/W  8) Interrupt Enable Set Register -------- */
typedef union {
  struct {
    uint8_t  OVF:1;            /*!< bit:      0  OVF Interrupt Enable               */
    uint8_t  ERR:1;            /*!< bit:      1  ERR Interrupt Enable               */
    uint8_t  _reserved0:1;     /*!< bit:      2  Reserved                           */
    uint8_t  READY:1;          /*!< bit:      3  READY Interrupt Enable             */
    uint8_t  MC:2;             /*!< bit:  4.. 5  MC Interrupt Enable                */
    uint8_t  _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_INTENSET_Type;

#define TC_INTENSET_OFFSET          0x0D         /**< \brief (TC_INTENSET offset) Interrupt Enable Set Register */
#define TC_INTENSET_RESETVALUE      0x0          /**< \brief (TC_INTENSET reset_value) Interrupt Enable Set Register */

#define TC_INTENSET_OVF             (0x1u <<  0) /**< \brief (TC_INTENSET) OVF Interrupt Enable */
#define TC_INTENSET_ERR             (0x1u <<  1) /**< \brief (TC_INTENSET) ERR Interrupt Enable */
#define TC_INTENSET_READY           (0x1u <<  3) /**< \brief (TC_INTENSET) READY Interrupt Enable */
#define TC_INTENSET_MC_Pos          4
#define TC_INTENSET_MC_Msk          (0x3u << TC_INTENSET_MC_Pos) /**< \brief (TC_INTENSET) MC Interrupt Enable */
#define TC_INTENSET_MC(value)       ((TC_INTENSET_MC_Msk & ((value) << TC_INTENSET_MC_Pos)))
#define TC_INTENSET_MASK            0x3Bu /**< \brief (TC_INTENSET) MASK Register */

/* -------- TC_INTFLAG : (TC Offset: 0x0E) (R/W  8) Interrupt Flag Status and Clear Register -------- */
typedef union {
  struct {
    uint8_t  OVF:1;            /*!< bit:      0  OVF Interrupt Flag                 */
    uint8_t  ERR:1;            /*!< bit:      1  ERR Interrupt Flag                 */
    uint8_t  _reserved0:1;     /*!< bit:      2  Reserved                           */
    uint8_t  READY:1;          /*!< bit:      3  READY Interrupt Flag               */
    uint8_t  MC:2;             /*!< bit:  4.. 5  MC Interrupt Flag                  */
    uint8_t  _reserved1:2;     /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_INTFLAG_Type;

#define TC_INTFLAG_OFFSET           0x0E         /**< \brief (TC_INTFLAG offset) Interrupt Flag Status and Clear Register */
#define TC_INTFLAG_RESETVALUE       0x0          /**< \brief (TC_INTFLAG reset_value) Interrupt Flag Status and Clear Register */

#define TC_INTFLAG_OVF              (0x1u <<  0) /**< \brief (TC_INTFLAG) OVF Interrupt Flag */
#define TC_INTFLAG_ERR              (0x1u <<  1) /**< \brief (TC_INTFLAG) ERR Interrupt Flag */
#define TC_INTFLAG_READY            (0x1u <<  3) /**< \brief (TC_INTFLAG) READY Interrupt Flag */
#define TC_INTFLAG_MC_Pos           4
#define TC_INTFLAG_MC_Msk           (0x3u << TC_INTFLAG_MC_Pos) /**< \brief (TC_INTFLAG) MC Interrupt Flag */
#define TC_INTFLAG_MC(value)        ((TC_INTFLAG_MC_Msk & ((value) << TC_INTFLAG_MC_Pos)))
#define TC_INTFLAG_MASK             0x3Bu /**< \brief (TC_INTFLAG) MASK Register */

/* -------- TC_STATUS : (TC Offset: 0x0F) (R/   8) Status Register -------- */
typedef union {
  struct {
    uint8_t  _reserved0:3;     /*!< bit:  0.. 2  Reserved                           */
    uint8_t  STOP:1;           /*!< bit:      3  Stop Status Flag                   */
    uint8_t  SLAVE:1;          /*!< bit:      4  Slave Status Flag                  */
    uint8_t  _reserved1:2;     /*!< bit:  5.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7  Synchronization Busy Status        */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_STATUS_Type;

#define TC_STATUS_OFFSET            0x0F         /**< \brief (TC_STATUS offset) Status Register */
#define TC_STATUS_RESETVALUE        0x8          /**< \brief (TC_STATUS reset_value) Status Register */

#define TC_STATUS_STOP              (0x1u <<  3) /**< \brief (TC_STATUS) Stop Status Flag */
#define TC_STATUS_SLAVE             (0x1u <<  4) /**< \brief (TC_STATUS) Slave Status Flag */
#define TC_STATUS_SYNCBUSY          (0x1u <<  7) /**< \brief (TC_STATUS) Synchronization Busy Status */
#define TC_STATUS_MASK              0x98u /**< \brief (TC_STATUS) MASK Register */

/* -------- TC_COUNT16_COUNT : (TC Offset: 0x10) (R/W 16) COUNT16 Count Register -------- */
typedef union {
  uint16_t reg;                /*!< Type      used for register access              */
} TC_COUNT16_COUNT_Type;

#define TC_COUNT16_COUNT_OFFSET     0x10         /**< \brief (TC_COUNT16_COUNT offset) Count Register */
#define TC_COUNT16_COUNT_RESETVALUE 0x0          /**< \brief (TC_COUNT16_COUNT reset_value) Count Register */
#define TC_COUNT16_COUNT_MASK       0xFFFFu      /**< \brief (TC_COUNT16_COUNT) MASK Register */

/* -------- TC_COUNT32_COUNT : (TC Offset: 0x10) (R/W 32) COUNT32 Count Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} TC_COUNT32_COUNT_Type;

#define TC_COUNT32_COUNT_OFFSET     0x10         /**< \brief (TC_COUNT32_COUNT offset) Count Register */
#define TC_COUNT32_COUNT_RESETVALUE 0x0          /**< \brief (TC_COUNT32_COUNT reset_value) Count Register */
#define TC_COUNT32_COUNT_MASK       0xFFFFFFFFu  /**< \brief (TC_COUNT32_COUNT) MASK Register */

/* -------- TC_COUNT8_COUNT : (TC Offset: 0x10) (R/W  8) COUNT8 Count Register -------- */
typedef union {
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_COUNT8_COUNT_Type;

#define TC_COUNT8_COUNT_OFFSET      0x10         /**< \brief (TC_COUNT8_COUNT offset) Count Register */
#define TC_COUNT8_COUNT_RESETVALUE  0x0          /**< \brief (TC_COUNT8_COUNT reset_value) Count Register */
#define TC_COUNT8_COUNT_MASK        0xFFu        /**< \brief (TC_COUNT8_COUNT) MASK Register */

/* -------- TC_COUNT8_PER : (TC Offset: 0x14) (R/W  8) COUNT8 Period Register -------- */
typedef union {
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_COUNT8_PER_Type;

#define TC_COUNT8_PER_OFFSET        0x14         /**< \brief (TC_COUNT8_PER offset) Period Register */
#define TC_COUNT8_PER_RESETVALUE    0x0          /**< \brief (TC_COUNT8_PER reset_value) Period Register */
#define TC_COUNT8_PER_MASK          0xFFu        /**< \brief (TC_COUNT8_PER) MASK Register */

/* -------- TC_COUNT16_CC : (TC Offset: 0x18) (R/W 16) COUNT16 Compare and Capture Register -------- */
typedef union {
  uint16_t reg;                /*!< Type      used for register access              */
} TC_COUNT16_CC_Type;

#define TC_COUNT16_CC_OFFSET        0x18         /**< \brief (TC_COUNT16_CC offset) Compare and Capture Register */
#define TC_COUNT16_CC_RESETVALUE    0x0          /**< \brief (TC_COUNT16_CC reset_value) Compare and Capture Register */
#define TC_COUNT16_CC_MASK          0xFFFFu      /**< \brief (TC_COUNT16_CC) MASK Register */

/* -------- TC_COUNT32_CC : (TC Offset: 0x18) (R/W 32) COUNT32 Compare and Capture Register -------- */
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} TC_COUNT32_CC_Type;

#define TC_COUNT32_CC_OFFSET        0x18         /**< \brief (TC_COUNT32_CC offset) Compare and Capture Register */
#define TC_COUNT32_CC_RESETVALUE    0x0          /**< \brief (TC_COUNT32_CC reset_value) Compare and Capture Register */
#define TC_COUNT32_CC_MASK          0xFFFFFFFFu  /**< \brief (TC_COUNT32_CC) MASK Register */

/* -------- TC_COUNT8_CC : (TC Offset: 0x18) (R/W  8) COUNT8 Compare and Capture Register -------- */
typedef union {
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_COUNT8_CC_Type;

#define TC_COUNT8_CC_OFFSET         0x18         /**< \brief (TC_COUNT8_CC offset) Compare and Capture Register */
#define TC_COUNT8_CC_RESETVALUE     0x0          /**< \brief (TC_COUNT8_CC reset_value) Compare and Capture Register */
#define TC_COUNT8_CC_MASK           0xFFu        /**< \brief (TC_COUNT8_CC) MASK Register */

#define TC_CC32_NUM                 2
#define TC_CC8_NUM                  2
#define TC_CC16_NUM                 2

/** \brief TC_COUNT8 hardware registers */
typedef struct { /* 8-bit Counter Mode */
  __IO TC_CTRLA_Type             CTRLA;       /**< \brief Offset: 0x00 (R/W 16) Control A Register */
  __IO TC_READREQ_Type           READREQ;     /**< \brief Offset: 0x02 (R/W 16) Read Request Register */
  __IO TC_CTRLBCLR_Type          CTRLBCLR;    /**< \brief Offset: 0x04 (R/W  8) Control B Clear Register */
  __IO TC_CTRLBSET_Type          CTRLBSET;    /**< \brief Offset: 0x05 (R/W  8) Control B Set Register */
  __IO TC_CTRLC_Type             CTRLC;       /**< \brief Offset: 0x06 (R/W  8) Control C Register */
       RoReg8                    Reserved1[0x1];
  __IO TC_DEBUG_Type             DEBUG;       /**< \brief Offset: 0x08 (R/W  8) Debug Register */
       RoReg8                    Reserved2[0x1];
  __IO TC_EVCTRL_Type            EVCTRL;      /**< \brief Offset: 0x0A (R/W 16) Event Control Register */
  __IO TC_INTENCLR_Type          INTENCLR;    /**< \brief Offset: 0x0C (R/W  8) Interrupt Enable Clear Register */
  __IO TC_INTENSET_Type          INTENSET;    /**< \brief Offset: 0x0D (R/W  8) Interrupt Enable Set Register */
  __IO TC_INTFLAG_Type           INTFLAG;     /**< \brief Offset: 0x0E (R/W  8) Interrupt Flag Status and Clear Register */
  __I  TC_STATUS_Type            STATUS;      /**< \brief Offset: 0x0F (R/   8) Status Register */
  __IO TC_COUNT8_COUNT_Type      COUNT;       /**< \brief Offset: 0x10 (R/W  8) Count Register */
       RoReg8                    Reserved3[0x3];
  __IO TC_COUNT8_PER_Type        PER;         /**< \brief Offset: 0x14 (R/W  8) Period Register */
       RoReg8                    Reserved4[0x3];
  __IO TC_COUNT8_CC_Type         CC[TC_CC8_NUM]; /**< \brief Offset: 0x18 (R/W  8) Compare and Capture Register */
       TC_COUNT8_CC_Type         Reserved5[4 - TC_CC8_NUM];
} TcCount8;

/** \brief TC_COUNT16 hardware registers */
typedef struct { /* 16-bit Counter Mode */
  __IO TC_CTRLA_Type             CTRLA;       /**< \brief Offset: 0x00 (R/W 16) Control A Register */
  __IO TC_READREQ_Type           READREQ;     /**< \brief Offset: 0x02 (R/W 16) Read Request Register */
  __IO TC_CTRLBCLR_Type          CTRLBCLR;    /**< \brief Offset: 0x04 (R/W  8) Control B Clear Register */
  __IO TC_CTRLBSET_Type          CTRLBSET;    /**< \brief Offset: 0x05 (R/W  8) Control B Set Register */
  __IO TC_CTRLC_Type             CTRLC;       /**< \brief Offset: 0x06 (R/W  8) Control C Register */
       RoReg8                    Reserved1[0x1];
  __IO TC_DEBUG_Type             DEBUG;       /**< \brief Offset: 0x08 (R/W  8) Debug Register */
       RoReg8                    Reserved2[0x1];
  __IO TC_EVCTRL_Type            EVCTRL;      /**< \brief Offset: 0x0A (R/W 16) Event Control Register */
  __IO TC_INTENCLR_Type          INTENCLR;    /**< \brief Offset: 0x0C (R/W  8) Interrupt Enable Clear Register */
  __IO TC_INTENSET_Type          INTENSET;    /**< \brief Offset: 0x0D (R/W  8) Interrupt Enable Set Register */
  __IO TC_INTFLAG_Type           INTFLAG;     /**< \brief Offset: 0x0E (R/W  8) Interrupt Flag Status and Clear Register */
  __I  TC_STATUS_Type            STATUS;      /**< \brief Offset: 0x0F (R/   8) Status Register */
  __IO TC_COUNT16_COUNT_Type     COUNT;       /**< \brief Offset: 0x10 (R/W 16) Count Register */
       RoReg8                    Reserved3[0x6];
  __IO TC_COUNT16_CC_Type        CC[TC_CC16_NUM]; /**< \brief Offset: 0x18 (R/W 16) Compare and Capture Register */
       TC_COUNT16_CC_Type        Reserved4[4 - TC_CC16_NUM];
} TcCount16;

/** \brief TC_COUNT32 hardware registers */
typedef struct { /* 32-bit Counter Mode */
  __IO TC_CTRLA_Type             CTRLA;       /**< \brief Offset: 0x00 (R/W 16) Control A Register */
  __IO TC_READREQ_Type           READREQ;     /**< \brief Offset: 0x02 (R/W 16) Read Request Register */
  __IO TC_CTRLBCLR_Type          CTRLBCLR;    /**< \brief Offset: 0x04 (R/W  8) Control B Clear Register */
  __IO TC_CTRLBSET_Type          CTRLBSET;    /**< \brief Offset: 0x05 (R/W  8) Control B Set Register */
  __IO TC_CTRLC_Type             CTRLC;       /**< \brief Offset: 0x06 (R/W  8) Control C Register */
       RoReg8                    Reserved1[0x1];
  __IO TC_DEBUG_Type             DEBUG;       /**< \brief Offset: 0x08 (R/W  8) Debug Register */
       RoReg8                    Reserved2[0x1];
  __IO TC_EVCTRL_Type            EVCTRL;      /**< \brief Offset: 0x0A (R/W 16) Event Control Register */
  __IO TC_INTENCLR_Type          INTENCLR;    /**< \brief Offset: 0x0C (R/W  8) Interrupt Enable Clear Register */
  __IO TC_INTENSET_Type          INTENSET;    /**< \brief Offset: 0x0D (R/W  8) Interrupt Enable Set Register */
  __IO TC_INTFLAG_Type           INTFLAG;     /**< \brief Offset: 0x0E (R/W  8) Interrupt Flag Status and Clear Register */
  __I  TC_STATUS_Type            STATUS;      /**< \brief Offset: 0x0F (R/   8) Status Register */
  __IO TC_COUNT32_COUNT_Type     COUNT;       /**< \brief Offset: 0x10 (R/W 32) Count Register */
       RoReg8                    Reserved3[0x4];
  __IO TC_COUNT32_CC_Type        CC[TC_CC32_NUM]; /**< \brief Offset: 0x18 (R/W 32) Compare and Capture Register */
       TC_COUNT32_CC_Type        Reserved4[2 - TC_CC32_NUM];
} TcCount32;

typedef union {
       TcCount8                  COUNT8;      /**< \brief Offset: 0x00 8-bit Counter Mode */
       TcCount16                 COUNT16;     /**< \brief Offset: 0x00 16-bit Counter Mode */
       TcCount32                 COUNT32;     /**< \brief Offset: 0x00 32-bit Counter Mode */
} Tc;

#endif /*  __EXCLUDE_TC_FROM_CMSIS__ */
#endif /* __ASSEMBLY__ */

#endif /* TC_HEADER_H_INCLUDED */
