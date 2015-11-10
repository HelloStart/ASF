/**
* \file
*
* \brief Battery Information Service - Application
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
/*
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/

/****************************************************************************************
*							        Includes	                                     	*
****************************************************************************************/


#include <asf.h>
#include "console_serial.h"
#include "at_ble_api.h"
#include "platform.h"
#include "battery.h"
#include "timer_hw.h"
#include "ble_utils.h"
#include "battery_info.h"
#include "ble_manager.h"
//#include "conf_extint.h"

/****************************************************************************************
*							        Macros	                                     		*
****************************************************************************************/
/** @brief APP_BAS_FAST_ADV between 0x0020 and 0x4000 in 0.625 ms units (20ms to 10.24s). */
#define APP_BAS_FAST_ADV				(100) //100 ms

/** @brief APP_BAS_ADV_TIMEOUT Advertising time-out between 0x0001 and 0x3FFF in seconds, 0x0000 disables time-out.*/
#define APP_BAS_ADV_TIMEOUT				(1000) // 100 Secs

/** @brief scan_resp_len is the length of the scan response data */
#define SCAN_RESP_LEN					(10)

/** @brief ADV_DATA_LEN */
#define ADV_DATA_LEN					(18)

/** @brief ADV_TYPE_LEN */
#define ADV_TYPE_LEN					(0x01)

/** @brief BAS_ADV_DATA_UUID_LEN the size of  BAS service uuid */
#define BAS_ADV_DATA_UUID_LEN			(2)

/** @brief BAS_ADV_DATA_UUID_TYPE the total sizeof BAS service uuid*/
#define BAS_ADV_DATA_UUID_TYPE			(0x03)

/** @brief BAS_ADV_DATA_NAME_LEN the  length of the device name */
#define BAS_ADV_DATA_NAME_LEN			(9)

/** @brief BAS_ADV_DATA_NAME_TYPE the gap ad data type */
#define BAS_ADV_DATA_NAME_TYPE			(0x09)

/* @brief BAS_ADV_DATA_NAME_DATA the actual name of device */
#define BAS_ADV_DATA_NAME_DATA			("ATMEL-BAS")

/* === GLOBALS ============================================================ */

#define BATTERY_UPDATE_INTERVAL	(1) //1 second
#define BATTERY_MAX_LEVEL		(100)
#define BATTERY_MIN_LEVEL		(0)

volatile unsigned char app_stack_patch[1024];
	
uint8_t db_mem[1024] = {0};
bat_gatt_service_handler_t bas_service_handler;

bool volatile timer_cb_done = false;
bool volatile flag = true;
bool volatile battery_flag = true;

bool isCharChanged = false;
at_ble_characteristic_changed_t bas_char_handle;

/**
* \Timer callback handler called on timer expiry
*/
static void timer_callback_handler(void)
{
	timer_cb_done = true;
	send_plf_int_msg_ind(USER_TIMER_CALLBACK,TIMER_EXPIRED_CALLBACK_TYPE_DETECT,NULL,0);
}

/* Advertisement data set and Advertisement start */
static at_ble_status_t battery_service_advertise(void)
{
	uint8_t idx = 0;
	uint8_t scan_rsp_data[SCAN_RESP_LEN] = {0x09,0xff, 0x00, 0x06, 0xd6, 0xb2, 0xf0, 0x05, 0xf0, 0xf8};
	uint8_t adv_data [ BAS_ADV_DATA_NAME_LEN + BAS_ADV_DATA_UUID_LEN   + (2*2)];
	
	adv_data[idx++] = BAS_ADV_DATA_UUID_LEN + ADV_TYPE_LEN;
	adv_data[idx++] = BAS_ADV_DATA_UUID_TYPE;

	/* Appending the UUID */
	adv_data[idx++] = (uint8_t)BAT_SERVICE_UUID;
	adv_data[idx++] = (uint8_t)(BAT_SERVICE_UUID >> 8);
	
	//Appending the complete name to the Ad packet
	adv_data[idx++] = BAS_ADV_DATA_NAME_LEN + ADV_TYPE_LEN;
	adv_data[idx++] = BAS_ADV_DATA_NAME_TYPE;
	
	memcpy(&adv_data[idx], BAS_ADV_DATA_NAME_DATA, BAS_ADV_DATA_NAME_LEN );
	idx += BAS_ADV_DATA_NAME_LEN;
	
	/* Adding the advertisement data and scan response data */
	if(!(at_ble_adv_data_set(adv_data, idx, scan_rsp_data, SCAN_RESP_LEN) == AT_BLE_SUCCESS) )
	{
		DBG_LOG("Failed to set adv data");
	}
	
	/* Start of advertisement */
	if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY, APP_BAS_FAST_ADV, APP_BAS_ADV_TIMEOUT, 0) == AT_BLE_SUCCESS)
	{
		DBG_LOG("BLE Started Adv");
		return AT_BLE_SUCCESS;
	}
	else
	{
		DBG_LOG("BLE Adv start Failed");
	}
	return AT_BLE_FAILURE;
}

/* Callback registered for AT_BLE_PAIR_DONE event from stack */
static void ble_paired_app_event(at_ble_handle_t conn_handle)
{
	
}

/* Callback registered for AT_BLE_DISCONNECTED event from stack */
static void ble_disconnected_app_event(at_ble_handle_t conn_handle)
{
	flag = false;
	timer_cb_done = false;
	hw_timer_stop();
	battery_service_advertise();
}

/* Callback registered for AT_BLE_NOTIFICATION_CONFIRMED event from stack */
static void ble_notification_confirmed_app_event(at_ble_cmd_complete_event_t *notification_status)
{
	if(!notification_status->status)
	{
		flag = true;
		DBG_LOG_DEV("sending notification to the peer success");				
	}
}

/* Callback registered for AT_BLE_CHARACTERISTIC_CHANGED event from stack */
static at_ble_status_t ble_char_changed_app_event(at_ble_characteristic_changed_t *char_handle)
{
	isCharChanged = true;
	memcpy(&bas_char_handle, char_handle, sizeof(at_ble_characteristic_changed_t));
	send_plf_int_msg_ind(USER_TIMER_CALLBACK,TIMER_EXPIRED_CALLBACK_TYPE_DETECT,NULL,0);
}

void button_cb(void)
{
	/* For user usage */
}

/**
* \Battery Service Application main function
*/

int main(void)
{
	uint8_t battery_level = BATTERY_MIN_LEVEL;	
	uint8_t status;
	
	#if SAMG55
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
	#elif SAM0
	system_init();
	#endif
	
	memset(db_mem, 0, sizeof(uint8_t)*1024);
	memset(&bas_service_handler, 0, sizeof(bat_gatt_service_handler_t));
	memset(&bas_char_handle, 0, sizeof(at_ble_characteristic_changed_t));

	flag = false;
	battery_flag = true;
	timer_cb_done = false;
	isCharChanged = false;

	/* Initialize the button */
	//button_init();
	
	/* Initialize serial console */
	serial_console_init();
	
	/* Initialize the hardware timer */
	hw_timer_init();
	
	/* Register the callback */
	hw_timer_register_callback(timer_callback_handler);
	
	DBG_LOG("Initializing Battery Service Application");
		
	/* initialize the ble chip  and Set the device mac address */
	ble_device_init(NULL);
	
	/* Initialize the battery service */
	bat_init_var();
	bat_init_service(&bas_service_handler, &battery_level);
	
	/* Define the primary service in the GATT server database */
	if((status = bat_primary_service_define(&bas_service_handler))!= AT_BLE_SUCCESS)
	{
		DBG_LOG("defining battery service failed %d", status);
	}
	
	battery_service_advertise();
	
	/* Register callback for paired event */
	register_ble_paired_event_cb(ble_paired_app_event);
	
	/* Register callback for disconnected event */
	register_ble_disconnected_event_cb(ble_disconnected_app_event);
	
	/* Register callback for notification confirmed event */
	register_ble_notification_confirmed_cb(ble_notification_confirmed_app_event);
	
	/* Register callback for characteristic changed event */
	register_ble_characteristic_changed_cb(ble_char_changed_app_event);
	
	acquire_sleep_lock();
	
	/* Capturing the events  */ 
	while (1) {
		/* BLE Event Task */
		ble_event_task();

		if(timer_cb_done)
		{
			/* send the notification and Update the battery level  */			
			if(flag)
			{
				if(bat_update_char_value(&bas_service_handler, battery_level, &flag) == AT_BLE_SUCCESS)
				{
					DBG_LOG("Battery Level:%d%%", battery_level);
				}
				if(battery_level == BATTERY_MAX_LEVEL)
				{
					battery_flag = false;
				}
				else if(battery_level == BATTERY_MIN_LEVEL)
				{
					battery_flag = true;
				}
				if(battery_flag)
				{
					battery_level++;
				}
				else
				{
					battery_level--;
				}				
				//hw_timer_start(BATTERY_UPDATE_INTERVAL);
				timer_cb_done = false;
			}
		}
		
		if(isCharChanged == true)
		{
			bat_char_changed_event(&bas_service_handler, &bas_char_handle, &flag);	
	
			if( flag )
			{
				hw_timer_start(BATTERY_UPDATE_INTERVAL);
			}
			else
			{
				hw_timer_stop();
			}
		}
	}	
}


