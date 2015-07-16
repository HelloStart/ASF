/**
* \file
*
* \brief Proximity Monitor Profile
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
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/

/**
* \mainpage
* \section preface Preface
* This is the reference manual for the Proximity Monitor Profile
*/
/*- Includes ---------------------------------------------------------------*/
#include <asf.h>
#include "platform.h"
#include "console_serial.h"
#include "timer_hw.h"
#include "conf_extint.h"
#include "pxp_monitor.h"



#if defined TX_POWER_SERVICE
#include "tx_power.h"
#endif

#if defined LINK_LOSS_SERVICE
#include "link_loss.h"
#endif

#if defined IMMEDIATE_ALERT_SERVICE
#include "immediate_alert.h"
#endif

/* pxp reporter device address to connect */
at_ble_addr_t pxp_reporter_address;

uint8_t pxp_supp_scan_index[MAX_SCAN_DEVICE];
uint8_t scan_index=0;


extern at_ble_connected_t ble_connected_dev_info[MAX_DEVICE_CONNECTED];
extern uint8_t scan_response_count;

#if defined TX_POWER_SERVICE
gatt_char_handler_t txps_handle = 
				{0, 0, 0, AT_BLE_INVALID_PARAM, NULL};
uint8_t tx_power_char_data[MAX_TX_POWER_CHAR_SIZE];
#endif

#if defined LINK_LOSS_SERVICE
gatt_char_handler_t lls_handle = 
				{0, 0, 0, AT_BLE_INVALID_PARAM, NULL};
uint8_t lls_char_data[MAX_LLS_CHAR_SIZE];
#endif

#if defined IMMEDIATE_ALERT_SERVICE
gatt_char_handler_t ias_handle = 
				{0, 0, 0, AT_BLE_INVALID_PARAM, NULL};
uint8_t ias_char_data[MAX_IAS_CHAR_SIZE];
#endif


/* setting up the variable to declared pointer */
void pxp_monitor_init(void)
{
	lls_handle.char_data = lls_char_data;
	ias_handle.char_data = ias_char_data;
	txps_handle.char_data = tx_power_char_data;
}

at_ble_status_t pxp_monitor_connect_request(at_ble_scan_info_t * scan_buffer, uint8_t index)
{
	at_ble_scan_stop();
	memcpy((uint8_t *)&pxp_reporter_address, (uint8_t *)&scan_buffer[index].dev_addr, sizeof(at_ble_addr_t));
	if(gap_dev_connect(&pxp_reporter_address) == AT_BLE_SUCCESS)
	{
		DBG_LOG("PXP Connect request sent");
		return AT_BLE_SUCCESS;
	}
	else
	{
		DBG_LOG("PXP Connect request send failed");		
	}
	return AT_BLE_FAILURE;
}

/* */
at_ble_status_t pxp_monitor_scan_data_handler(at_ble_scan_info_t *scan_buffer, uint8_t scanned_dev_count)
{	
	scan_index = 0;
	if (scanned_dev_count)
	{
		uint8_t index;
		
		/* Stop the current scan active */
		at_ble_scan_stop();
		
		for (index = 0; index < scanned_dev_count; index++)
		{
			at_ble_uuid_t service_uuid;
			service_uuid.type = AT_BLE_UUID_16;
			service_uuid.uuid[1] = (LINK_LOSS_SERVICE_UUID >> 8);
			service_uuid.uuid[0] = (uint8_t)LINK_LOSS_SERVICE_UUID;
			
			if(scan_info_parse(&scan_buffer[index], &service_uuid, AD_TYPE_COMPLETE_LIST_UUID) == AT_BLE_SUCCESS)
			{
				/* Device Service UUID  matched */
				pxp_supp_scan_index[scan_index++]=index;
			}
		}
		
		if(scan_index)
		{
			/* Multiple supported device found*/
			if(scan_index > 1)
			{
				DBG_LOG("Supported device list");
				for(index=0;index<scan_index;index++)
				{
					DBG_LOG_CONT("%d ",pxp_supp_scan_index[index]);
				}
				
				index = getchar();
				index-=48;
				DBG_LOG("%d",index);
				return pxp_monitor_connect_request(scan_buffer, index);	
			}
			else
			{  /* Single device found */
				return pxp_monitor_connect_request(scan_buffer, pxp_supp_scan_index[0]);	
			}
						
		}
	}
	else
	{
		/* Try for rescan */
	}
	return AT_BLE_FAILURE;
}

at_ble_status_t pxp_disconnect_event_handler(at_ble_disconnected_t *disconnect)
{
	
	if(gap_dev_connect(&pxp_reporter_address) == AT_BLE_SUCCESS)
	{
		DBG_LOG("PXP Re-Connect request sent");
		return AT_BLE_SUCCESS;
	}
	else
	{
		DBG_LOG("PXP Re-Connect request send failed");
		return AT_BLE_FAILURE;
	}
}

 /**@brief After the successful connection handler
  *
  * After connecting to the peer device start the GATT primary discovery
  *
  * @param[in] at_ble_connected_t parameters of the established connection
  *
  * @return @ref AT_BLE_SUCCESS operation programmed successfully.
  * @return @ref AT_BLE_INVALID_PARAM if GATT discovery parameter are incorrect parameter.
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t pxp_monitor_connected_state_handler(at_ble_connected_t * conn_params)
{
	at_ble_status_t discovery_status = AT_BLE_FAILURE;
	if(conn_params->conn_status==AT_BLE_SUCCESS)
	{
		discovery_status = at_ble_primary_service_discover_all(ble_connected_dev_info[0].handle, GATT_DISCOVERY_STARTING_HANDLE, GATT_DISCOVERY_ENDING_HANDLE);
		
		if(discovery_status == AT_BLE_SUCCESS)
		{
			DBG_LOG("GATT Discovery request started ");
		}
		else
		{
			DBG_LOG("GATT Discovery request failed");
		}
	}
	return discovery_status;
}

void pxp_monitor_service_found_handler(at_ble_primary_service_found_t * primary_service_params)
{
	at_ble_uuid_t *pxp_service_uuid;
	pxp_service_uuid = &primary_service_params->service_uuid;
	if (pxp_service_uuid->type == AT_BLE_UUID_16)
	{
		uint16_t service_uuid;
		service_uuid = ((pxp_service_uuid->uuid[1] << 8) | pxp_service_uuid->uuid[0]);
		switch(service_uuid)
		{
			/* for link loss service Handler */
			case LINK_LOSS_SERVICE_UUID:
			{
				lls_handle.start_handle = primary_service_params->start_handle;
				lls_handle.end_handle = primary_service_params->end_handle;
				DBG_LOG("link loss service discovered  %04X %04X", primary_service_params->start_handle, primary_service_params->end_handle);
			}
			break;
			
			/* for Immediate Alert service Handler */
			case IMMEDIATE_ALERT_SERVICE_UUID:
			{
				ias_handle.start_handle = primary_service_params->start_handle;
				ias_handle.end_handle = primary_service_params->end_handle;
				DBG_LOG("Immediate Alert service discovered %04X %04X ", primary_service_params->start_handle, primary_service_params->end_handle);
			}
			break;
			
			/* for Tx Power service Handler */
			case TX_POWER_SERVICE_UUID:
			{
				txps_handle.start_handle = primary_service_params->start_handle;
				txps_handle.end_handle = primary_service_params->end_handle;
				DBG_LOG("Tx power service handler  %04X %04X", primary_service_params->start_handle, primary_service_params->end_handle);
			}
			break;
			
			default:
			break;
		}
	}
}

/**@brief Discover all Characteristics supported for Proximity Service in a connected device
*  and handles discovery complete
* Search will go from start_handle to end_handle, whenever a characteristic is found
* After search and discovery completes will initialize the alert level and read the tx power value as defined   
* @ref AT_BLE_CHARACTERISTIC_FOUND event is sent and @ref AT_BLE_DISCOVERY_COMPLETE is sent at end of discover operation.
*
* @param[in] discover_status discovery status on each 
* @param[in] start_handle start of the searched range 
* @param[in] end_handle   end of the searched range
*
*/
void pxp_monitor_discovery_complete_handler(at_ble_discovery_complete_t *discover_status)
{
	bool discover_char_flag = true;
	if(discover_status->status == AT_BLE_DISCOVERY_SUCCESS)
	{		
#if defined TX_POWER_SERVICE
		if ((txps_handle.char_discovery == AT_BLE_INVALID_PARAM) && (discover_char_flag))
		{
			if(at_ble_characteristic_discover_all(ble_connected_dev_info[0].handle, txps_handle.start_handle, txps_handle.end_handle) == AT_BLE_SUCCESS)
			{
				txps_handle.char_discovery = AT_BLE_SUCCESS;
				DBG_LOG("Tx Characteristic Discovery Started");
			}
			else
			{
				txps_handle.char_discovery = AT_BLE_FAILURE;
				DBG_LOG("Tx Characteristic Discovery Failed");
			}
			discover_char_flag = false;
		}
#endif

#if defined LINK_LOSS_SERVICE
		if ((lls_handle.char_discovery == AT_BLE_INVALID_PARAM) && (discover_char_flag))
		{
			if(at_ble_characteristic_discover_all(ble_connected_dev_info[0].handle, lls_handle.start_handle, lls_handle.end_handle) == AT_BLE_SUCCESS)
			{
				lls_handle.char_discovery = AT_BLE_SUCCESS;
				DBG_LOG("Link Loss Characteristic Discovery Started");
			}
			else
			{
				lls_handle.char_discovery = AT_BLE_FAILURE;
				DBG_LOG("Link Loss Characteristic Discovery Failed");
			}
			discover_char_flag = false;
		}
#endif

#if defined IMMEDIATE_ALERT_SERVICE
		if ((ias_handle.char_discovery == AT_BLE_INVALID_PARAM) && (discover_char_flag))
		{
			if(at_ble_characteristic_discover_all(ble_connected_dev_info[0].handle, ias_handle.start_handle, ias_handle.end_handle) == AT_BLE_SUCCESS)
			{
				ias_handle.char_discovery = AT_BLE_SUCCESS;
				DBG_LOG("Immediate Characteristic Discovery Started");
			}
			else
			{
				ias_handle.char_discovery = AT_BLE_FAILURE;
				DBG_LOG("Immediate Characteristic Discovery Failed");
			}
			discover_char_flag = false;
		}
#endif

		if(discover_char_flag)
		{
			DBG_LOG("GATT characteristic discovery completed");
#if defined LINK_LOSS_SERVICE
			// set link loss profile to high alert upon connection
			if(!(lls_alert_level_write(ble_connected_dev_info[0].handle, lls_handle.char_handle, LLS_ALERT_LEVEL) == AT_BLE_SUCCESS))
			{
				DBG_LOG("Link Loss write characteristics failed");
			}
#endif

#if defined TX_POWER_SERVICE
			if(!(txps_power_read(ble_connected_dev_info[0].handle,txps_handle.char_handle)== AT_BLE_SUCCESS))
			{
				DBG_LOG("Characteristic Read Request failed");
			}
#endif
		}
	}
}

void pxp_monitor_characteristic_read_response(at_ble_characteristic_read_response_t *char_read_resp)
{
		
#if defined TX_POWER_SERVICE
	txps_power_read_response(char_read_resp, &txps_handle);
#endif

#if defined LINK_LOSS_SERVICE
	lls_alert_read_response(char_read_resp, &lls_handle);
#endif
	
	//rssi_update(ble_connected_dev_info[0].handle);
	hw_timer_start(PXP_RSSI_UPDATE_INTERVAL);
	
}

void pxp_monitor_characteristic_found_handler(at_ble_characteristic_found_t *characteristic_found)
{
	uint16_t charac_16_uuid ;
	
	charac_16_uuid = (uint16_t)((characteristic_found->char_uuid.uuid[0]) | \
							(characteristic_found->char_uuid.uuid[1] << 8));	
	
	if(charac_16_uuid == TX_POWER_LEVEL_CHAR_UUID)
	{
		txps_handle.char_handle = characteristic_found->value_handle;
		DBG_LOG("Tx power characteristics %04X",txps_handle.char_handle);
	}
	
	else if((charac_16_uuid == ALERT_LEVEL_CHAR_UUID))
	{
		if((characteristic_found->char_handle > lls_handle.start_handle) && (characteristic_found->char_handle < lls_handle.end_handle))
		{
			lls_handle.char_handle = characteristic_found->value_handle;
			DBG_LOG("link loss characteristics %04X",lls_handle.char_handle);
		}
		else
		{
			ias_handle.char_handle = characteristic_found->value_handle;
			DBG_LOG("Immediate alert characteristics %04X",ias_handle.char_handle);
		}
	}
}






