/**
 * \file
 *
 * \brief ATMEGA256RFR2 Xplained Pro board init.
 *
 * To use this board, define BOARD=ATMEGA256RFR2_XPLAINED_PRO.
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
#include <board.h>
#include <compiler.h>
#include <conf_board.h>
#include "gpio.h"
#include "led.h"
#include "helper.h"

void board_init(void)
{
  
  board_identify();

	/* On board LED initialization */
	ioport_configure_pin(LED0_RCB,IOPORT_DIR_OUTPUT |  IOPORT_INIT_HIGH);
	ioport_configure_pin(LED1_RCB,IOPORT_DIR_OUTPUT |  IOPORT_INIT_HIGH);

	ioport_configure_pin(LED2_RCB,IOPORT_DIR_OUTPUT |  IOPORT_INIT_HIGH);

	/* On board Switch initialization */
	ioport_configure_pin(GPIO_PUSH_BUTTON_0,IOPORT_DIR_INPUT | IOPORT_PULL_UP);

    

#ifdef BREAKOUT_BOARD
  //Enable RCB_BB RS232 level converter

    DDRD = 0XD0; 	//(1 << 4) | (1 << 6) | (1 << 7);
    PORTD = 0XC0 ;  //(0 << 4) | (1 << 6) | (1 << 7);
#endif

}


