/**
 * \file
 *
 * \brief HID Keyboard Device Profile Application declarations
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
 *    Atmel micro controller product.
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
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel
 *Support</a>
 */

/**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the HID Keyboard Device Profile Application declarations
 */
/*- Includes -----------------------------------------------------------------------*/
#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "ble_manager.h"
#include "hid.h"
#include "hid_keyboard_app.h"
#include "device_info.h"
#include "ble_utils.h"
#include "console_serial.h"
#include "timer_hw.h"
#include "conf_extint.h"


/* =========================== GLOBALS ============================================================ */

/* Memory allocated for database */
uint8_t	au8DbMem[1024]	= {0}; 

/* Control point notification structure */
hid_control_mode_ntf_t hid_control_point_value; 

/* Report notification structure */
hid_report_ntf_t report_ntf_info;

/* Boot notification structure */
hid_boot_ntf_t  boot_ntf_info;

/* Protocol mode notification structure */
hid_proto_mode_ntf_t hid_proto_mode_value;

/* HID profile structure for application */
hid_prf_info_t hid_prf_data;

/* Keyboard character info to be displayed during demo */
uint8_t keyb_disp[12] = {0x0B, 0x08, 0x0F, 0x0F, 0x12, 0x2C, 0x04, 0x17, 0x10, 0x08, 0x0F, 0x28};

/* Keyboard character to be printed */
uint8_t keyb_id = 0;

/* Keyboard report value */
uint8_t app_keyb_report[8] = {0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};	
	
/* Keyboard key status */
uint8_t key_status = 0;	

/* keyboard report */
static uint8_t hid_app_keyb_report_map[] =
{
   0x05, 0x01,		/* Usage Page (Generic Desktop)      */
   0x09, 0x06,		/* Usage (Keyboard)                  */
   0xA1, 0x01,		/* Collection (Application)          */
   0x85, 0x01,		/* REPORT ID (1) - MANDATORY         */ 
   0x05, 0x07,		/* Usage Page (Keyboard)             */
   0x19, 224,		/* Usage Minimum (224)               */
   0x29, 231,		/* Usage Maximum (231)               */
   0x15, 0x00,		/* Logical Minimum (0)               */
   0x25, 0x01,		/* Logical Maximum (1)               */
   0x75, 0x01,		/* Report Size (1)                   */
   0x95, 0x08,		/* Report Count (8)                  */
   0x81, 0x02,		/* Input (Data, Variable, Absolute)  */
   0x81, 0x01,		/* Input (Constant)                  */
   0x19, 0x00,		/* Usage Minimum (0)                 */
   0x29, 101,		/* Usage Maximum (101)               */
   0x15, 0x00,		/* Logical Minimum (0)               */
   0x25, 101,		/* Logical Maximum (101)             */
   0x75, 0x08,		/* Report Size (8)                   */
   0x95, 0x06,		/* Report Count (6)                  */
   0x81, 0x00,		/* Input (Data, Array)               */
   0x05, 0x08,		/* Usage Page (LED)                  */
   0x19, 0x01,		/* Usage Minimum (1)                 */
   0x29, 0x05,		/* Usage Maximum (5)                 */
   0x15, 0x00,		/* Logical Minimum (0)               */
   0x25, 0x01,		/* Logical Maximum (1)               */
   0x75, 0x01,		/* Report Size (1)                   */
   0x95, 0x05,		/* Report Count (5)                  */
   0x91, 0x02,		/* Output (Data, Variable, Absolute) */
   0x95, 0x03,		/* Report Count (3)                  */
   0x91, 0x01,		/* Output (Constant)                 */
   0xC0				/* End Collection                    */
};

/* Callback called when host change the control point value */
void hid_prf_control_point_ntf_cb(hid_control_mode_ntf_t *hid_control_point_value_t)
{
	DBG_LOG_DEV("Control Point Notification Callback :: Service Instance %d Control Value %d", hid_control_point_value_t->serv_inst, hid_control_point_value_t->control_value);
}

/* Callback called when host change the protocol mode value */
void hid_prf_protocol_mode_ntf_cb(hid_proto_mode_ntf_t *protocol_mode)
{
	DBG_LOG_DEV("Protocol Mode Notification Callback :: Service Instance %d  New Protocol Mode  %d  Connection Handle %d", protocol_mode->serv_inst, protocol_mode->mode, protocol_mode->conn_handle);
}

/* Callback called when host enable/disable the notification for boot report (Mouse/Keyboard)
   Mouse Boot Value 4
   Keyboard Boot Value 6
*/
void hid_prf_boot_ntf_cb(hid_boot_ntf_t *boot_ntf_info_t)
{
	DBG_LOG_DEV("Boot Notification Callback :: Service Instance %d  Boot Value  %d  Notification(Enable/Disable) %d", boot_ntf_info_t->serv_inst, boot_ntf_info_t->boot_value, boot_ntf_info_t->ntf_conf);
}

/* Callback called when host enable/disable the notification for report (Mouse/Keyboard) */
void hid_prf_report_ntf_cb(hid_report_ntf_t *report_info)
{
	DBG_LOG_DEV("Report Notification Callback Service Instance %d  Report ID  %d  Notification(Enable/Disable) %d Connection Handle %d", report_info->serv_inst, report_info->report_ID, report_info->ntf_conf, report_info->conn_handle);
    report_ntf_info.serv_inst = report_info->serv_inst;
	report_ntf_info.report_ID = report_info->report_ID;
	report_ntf_info.ntf_conf = report_info->ntf_conf;
	report_ntf_info.conn_handle = report_info->conn_handle;
}

/* Callback called when report send over the air */
void hid_notification_confirmed_cb(uint8_t status)
{
	DBG_LOG_DEV("Keyboard report send to host status %d", status);
}

/* Callback called when user press the button for writing new characteristic value */
void button_cb(void)
{
	key_status = 1;
}

void timer_callback_handler(void)
{
 /* Application can use this for application timer events */	
}

/* Initialize the application information for HID profile*/
void hid_keyboard_app_init(void)
{
	
	hid_prf_data.hid_serv_instance = 1;
	hid_prf_data.hid_device = HID_KEYBOARD_MODE; 
	hid_prf_data.protocol_mode = HID_REPORT_PROTOCOL_MODE; 
	hid_prf_data.num_of_report = HID_NUM_OF_REPORT;
	
	/*Update the report information based on report id, User can allocate maximum HID_MAX_REPORT_NUM number of report*/
	hid_prf_data.report_id[0] = 1;  
	hid_prf_data.report_type[0] = INPUT_REPORT;  
	
	hid_prf_data.report_val[0] = &app_keyb_report[0];	
	hid_prf_data.report_len[0] = sizeof(app_keyb_report);
	hid_prf_data.report_map_info.report_map = hid_app_keyb_report_map;
	hid_prf_data.report_map_info.report_map_len = sizeof(hid_app_keyb_report_map);
	hid_prf_data.hid_device_info.bcd_hid = 0x0111;        
	hid_prf_data.hid_device_info.bcountry_code = 0x00;
	hid_prf_data.hid_device_info.flags = 0x02; 
	
	if(hid_prf_conf(&hid_prf_data)==HID_PRF_SUCESS)
	{
		DBG_LOG("HID Profile Configured");
	}
	else
	{
		DBG_LOG("HID Profile Configuration Failed");
	}
}

int main(void )
{
    at_ble_init_config_t pf_cfg;  
    platform_config busConfig;    
	
	/*Memory allocation for DB*/
	pf_cfg.memPool.memSize 		= sizeof(au8DbMem);
	pf_cfg.memPool.memStartAdd 	= &(au8DbMem[0]);
	
	/*Bus configuration*/
	busConfig.bus_type = UART;
	pf_cfg.plf_config = &busConfig;
	
#if SAMG55
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
#elif SAM0
	system_init();
#endif
	
	/* Initialize serial console */
	serial_console_init();
	
	/* Initialize button*/
	button_init();
	
	/* Initialize HW Timer*/
	hw_timer_init();
	hw_timer_register_callback(timer_callback_handler);
	
	DBG_LOG("Initializing HID Keyboard Application");
	
	/* Initialize the profile based on user input */
	hid_keyboard_app_init();
	
	/* initialize the ble chip  and Set the device mac address */
	
	ble_device_init(NULL, &pf_cfg);
	
	/* Register the notification handler */
	register_ble_notification_confirmed_cb(hid_notification_confirmed_cb);
	notify_report_ntf_handler(hid_prf_report_ntf_cb);
	notify_boot_ntf_handler(hid_prf_boot_ntf_cb);
	notify_protocol_mode_handler(hid_prf_protocol_mode_ntf_cb);
	notify_control_point_handler(hid_prf_control_point_ntf_cb);
	
	/* Capturing the events  */
	while(1)
	{
		ble_event_task();
		
		/* Check for key status */
		if(key_status)
		{
			delay_ms(KEY_PAD_DEBOUNCE_TIME);
			if((keyb_id == 0) || (keyb_id == 6))
			{
				app_keyb_report[0] = 0x02;
			}
			else
			{
				app_keyb_report[0] = 0x00;
			}
			
			app_keyb_report[2] = keyb_disp[keyb_id];
			hid_prf_report_update(report_ntf_info.conn_handle, report_ntf_info.serv_inst, report_ntf_info.report_ID, app_keyb_report, sizeof(app_keyb_report));
			
			app_keyb_report[2] = 0x00;
			hid_prf_report_update(report_ntf_info.conn_handle, report_ntf_info.serv_inst, report_ntf_info.report_ID, app_keyb_report, sizeof(app_keyb_report));	
			
			key_status = 0;
			
			if(keyb_id == 11)
			{
				keyb_id = 0;
			}
			else
			{
				++keyb_id;
			}
			
		}
		
	}

	return 0;
}



