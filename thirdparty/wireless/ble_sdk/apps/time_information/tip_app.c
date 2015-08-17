/**
 * \file
 *
 * \brief Time Information Profile Application
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

 /**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the Time Information Profile Application
 */
/***********************************************************************************
 *									Includes		                               *
 **********************************************************************************/
#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "tip_app.h"
#include "profiles.h"
#include "console_serial.h"
#include "timer_hw.h"
#include "conf_extint.h"
#include "ble_manager.h"
#include "ble_utils.h"
#include "current_time.h"
#include "next_dst.h"
#include "reference_time.h"


/***********************************************************************************
 *									Types			                               *
 **********************************************************************************/
extern gatt_cts_handler_t cts_handle;

extern gatt_dst_handler_t dst_handle;

extern gatt_rtu_handler_t rtu_handle;

extern at_ble_connected_t ble_connected_dev_info[MAX_DEVICE_CONNECTED];

volatile bool button_pressed = false;

/***********************************************************************************
 *									Implementations                               *
 **********************************************************************************/
/**
 * @brief Button Press Callback 
 */
void button_cb(void)
{
 button_pressed = true;
}

/**
 * @brief Timer Callback
 */
void timer_callback_handler(void)
{
	/* Free to use for User Application */
}

/**
 * @brief Main Function for Time Information Callback
 */
int main (void)
{

#if SAMG55
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
#elif SAM0
	system_init();
#endif
	/* Initializing the console  */
	serial_console_init();
	
	/* Initializing the button */
	button_init();
	
	/* Initializing the hardware timer */
	hw_timer_init();
	
	/*Registration of timer callback*/
	hw_timer_register_callback(timer_callback_handler);
	
	DBG_LOG("Time Profile Application");

	
	/* initialize the BLE chip  and Set the device mac address */
	ble_device_init(NULL);
	
	while(1)
	{
		ble_event_task();
		if (button_pressed)
		{
			delay_ms(200);
			if(tis_current_time_read( ble_connected_dev_info[0].handle, cts_handle.curr_char_handle ) == AT_BLE_SUCCESS)
			{
				LED_Toggle(LED0);
			}
			if(tis_current_time_read( ble_connected_dev_info[0].handle, cts_handle.lti_char_handle ) == AT_BLE_SUCCESS)
			{
				DBG_LOG("Local Time info request success");
			}
			if(tis_current_time_read( ble_connected_dev_info[0].handle, cts_handle.rti_char_handle ) == AT_BLE_SUCCESS)
			{
				DBG_LOG("Reference Time info request success");
			}
			if(tis_dst_change_read( ble_connected_dev_info[0].handle, dst_handle.dst_char_handle ) == AT_BLE_SUCCESS)
			{
				DBG_LOG("Dst read request success");
			}
			if(tis_rtu_update_read( ble_connected_dev_info[0].handle, rtu_handle.tp_control_char_handle, 20) == AT_BLE_SUCCESS)
			{
				DBG_LOG("RTU control point request success");
			}
			
			if(tis_rtu_update_read( ble_connected_dev_info[0].handle, rtu_handle.tp_state_char_handle, 20 ) == AT_BLE_SUCCESS)
			{
				DBG_LOG("RTU state request success");
			}
			button_pressed = false;
		}
	}
}


