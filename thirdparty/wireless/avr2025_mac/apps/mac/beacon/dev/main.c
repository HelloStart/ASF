/**
 * @file main.c
 *
 * @brief MAC Example Beacon Application - Device
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
 */

/**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the IEEE 802.15.4 MAC Beacon Application -
 *Device
 * \section main_files Application Files
 *      - main.c                 Application main file.
 * \section intro Application Introduction
 * The basic MAC Example Beacon Application deploys a beacon-enabled network
 *with star topology.
 * The coordinator starts a PAN at channel DEFAULT_CHANNEL with the PAN ID
 *DEFAULT_PAN_ID.
 *
 * The coordinator starts a beaconing network and transmits user data within
 *beacon payload of transmitted beacon frames.
 * The device scans for this network,sync up with the coordinator and associates
 *to the coordinator.The devices receive
 * these beacon frames, extract the receveived user data from the coordinator
 *,print the received data on the hyper
 * terminal and also sends the extracted beacon payload back to the coordinator.
 * The coordinator also transmits broadcast data frames periodically.The devices
 *receive these broadcast data frames
 * and increase a counter.
 * The coordinator also puts the data in the indirect queue periodically and
 *transmits data frames based on the
 * periodic poll request from the device.
 * The results of the proper data transmission/reception are printed to a
 *terminal program via Serial I/O (UART or USB).
 * For demonstration purposes the coordinator's configuration is limited to
 *associate maximum of 100 devices.
 * \note For Two Processor Approach the Application needs to be flashed in the
 *Host board(eg.SAM4L-Xplained Pro) and the Serial-if application (Beacon
 *FFD)(MAC Stack)
 * needs to be flashed in the NCP(Network CoProcessor) board.
 * \section api_modules Application Dependent Modules
 * - \subpage api
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR .
 * Other compilers may or may not work.
 *
 * \section references References
 * 1)  IEEE Std 802.15.4-2006 Part 15.4: Wireless Medium Access Control (MAC)
 *     and Physical Layer (PHY) Specifications for Low-Rate Wireless Personal
 *Area
 *     Networks (WPANs).\n\n
 * 2)  AVR Wireless Support <A href="http://avr@atmel.com">avr@atmel.com</A>.\n
 *
 * \section contactinfo Contact Information
 * For further information,visit
 * <A href="http://www.atmel.com/avr">www.atmel.com</A>.\n
 */

#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include "conf_board.h"
#include "avr2025_mac.h"
#include "delay.h"
#include "common_sw_timer.h"
#include "sio2host.h"
#include "mac.h"
#include "tal.h"
#include "ieee_const.h"
#include "mac_internal.h"
#include <asf.h>

/* === TYPES =============================================================== */
/**
 * This enum stores the current state of the application.
 */
typedef enum app_state_tag {
	APP_IDLE = 0,
	APP_SCAN_DONE,
	APP_ASSOC_IN_PROGRESS,
	APP_DEVICE_RUNNING
}
app_state_t;

/* === MACROS ============================================================== */

#define DEFAULT_PAN_ID                  CCPU_ENDIAN_TO_LE16(0xBABE)

/** Defines the short address of the coordinator. */
#define COORD_SHORT_ADDR                CCPU_ENDIAN_TO_LE16(0XCAFE)

#define CHANNEL_OFFSET                  (11)

#define SCAN_CHANNEL                    (1ul << current_channel)

/** Defines the short scan duration time. */
#define SCAN_DURATION_SHORT             (5)
/** Defines the long scan duration time. */
#define SCAN_DURATION_LONG              (6)

/*
 * This is the timeout in ms once the node tries to synchronize with the
 * coordinator. This timeout is set so that the node should be able to receive
 * at least a beacon frame from its parent.
 * In case the Beacon Order is increased to a high value, this timeout might
 * need to be increased as well.
 */
#define TIMER_SYNC_BEFORE_ASSOC_MS      (3000)

#define PAYLOAD_LEN                     (104)

#if (LED_COUNT >= 3)
#define LED_START                       (LED0)
#define LED_NWK_SETUP                   (LED1)
#define LED_DATA                        (LED2)
#elif (LED_COUNT == 2)
#define LED_START                       (LED0)
#define LED_NWK_SETUP                   (LED0)
#define LED_DATA                        (LED1)
#else
#define LED_START                       (LED0)
#define LED_NWK_SETUP                   (LED0)
#define LED_DATA                        (LED0)
#define LED_DATA                        (LED0)
#endif

#ifdef MAC_SECURITY_ZIP
/* MAC security macros */
#define KEY_INDEX_1                     (1)
#define KEY_INDEX_2                     (2)
#define KEY_INDEX_3                     (3)
#define LOOKUP_DATA_SIZE_1              (1) // Size is 9 octets
#define FRAME_TYPE_DATA                 (1)
#define CMD_FRAME_ID_NA                 (0) // CommandFrameIdentifier is n/a
#define ZIP_SEC_MIN                     (5) // SecurityMinimum for ZIP is 5
#define DEV_OVERRIDE_SEC_MIN            (1) // DeviceOverrideSecurityMinimum: True
#define ZIP_KEY_ID_MODE                 (1) // ZIP uses KeyIdMode 1
#endif
/* === GLOBALS ============================================================= */

/* This structure stores the short and extended address of the coordinator. */
static wpan_addr_spec_t coord_addr_spec;

/* This variable counts the number of received broadcast data frames. */
static uint32_t bc_rx_cnt;

/* This variable counts the number of received indirect data frames. */
static uint32_t indirect_rx_cnt;

/** This variable stores the current state of the node. */
static app_state_t app_state = APP_IDLE;

/** This array stores the current msdu payload. */
static uint8_t msdu_payload[PAYLOAD_LEN];

static uint8_t current_channel;
static uint8_t current_channel_page;
static uint32_t channels_supported;

static uint8_t APP_TIMER;

#ifdef MAC_SECURITY_ZIP
/*
 * This is implemented as an array of bytes, but actually this is a
 * 128-bit variable. This is the reason why the array needs to be filled
 * in in reverse order than expected.
 */
static uint8_t default_key[3][16] = {{
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF
},
{
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xD0
},
{
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xD1
}
};

#define INDEX_0                         (0)
#define INDEX_1                         (1)
#define INDEX_2                         (2)
#define EMPTY_DEV_HANDLE                (0xFF) // key device desc is invalid
#define KEY_INFO_FRAME                  (0xDE)
#define NO_SECURITY                     (0)

static uint8_t default_key_source[8] = {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static uint8_t deviceShortAddress = 0xFF;

#endif

/* === PROTOTYPES ========================================================== */

/**
 * @brief Callback function indicating network search
 *
 * @param parameter Pointer to callback parameter
 *                  (not used in this application, but could be if desired).
 */
static void network_search_indication_cb(void *parameter);

/**
 * @brief Callback function for switching off the LED after data reception
 *
 * @param parameter Pointer to callback parameter
 *                  (not used in this application, but could be if desired).
 */
static void rx_data_led_off_cb(void *parameter);

/** Alert to indicate something has gone wrong in the application */
static void app_alert(void);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Main function of the device application
 */
int main(void)
{
	irq_initialize_vectors();
	//sysclk_init();

	/* Initialize the board.
	 * The board-specific conf_board.h file contains the configuration of
	 * the board initialization.
	 */
	//board_init();
	system_init();
	delay_init();
#ifdef SIO_HUB
	sio2host_init(); 
#endif
	sw_timer_init();

	if (MAC_SUCCESS != wpan_init()) {
		app_alert();
	}

	/* Initialize LEDs. */
	LED_On(LED_START);     /* indicating application is started */
	LED_Off(LED_NWK_SETUP); /* indicating network is started */
	LED_Off(LED_DATA);     /* indicating data transmission */

	cpu_irq_enable();

#ifdef SIO_HUB
	/* Initialize the serial interface used for communication with terminal
	 *program. */

	/* To Make sure the Hyper Terminal to the System */
	sio2host_getchar();

	printf("\nBeacon_Application\r\n\n");
	printf("\nDevice\r\n\n");
#endif

	sw_timer_get_id(&APP_TIMER);

	wpan_mlme_reset_req(true);

	while (true) {
		wpan_task();
	}
}

#if defined(ENABLE_TSTAMP)

/*
 * Callback function usr_mcps_data_conf
 *
 * @param msduHandle  Handle of MSDU handed over to MAC earlier
 * @param status      Result for requested data transmission request
 * @param Timestamp   The time, in symbols, at which the data were transmitted
 *                    (only if time stamping is enabled).
 *
 */
void usr_mcps_data_conf(uint8_t msduHandle,
		uint8_t status,
		uint32_t Timestamp)
#else
void usr_mcps_data_conf(uint8_t msduHandle,
		uint8_t status)
#endif  /* ENABLE_TSTAMP */
{
}

/*
 * @brief Callback function usr_mcps_data_ind
 *
 * @param SrcAddrSpec      Pointer to source address specification
 * @param DstAddrSpec      Pointer to destination address specification
 * @param msduLength       Number of octets contained in MSDU
 * @param msdu             Pointer to MSDU
 * @param mpduLinkQuality  LQI measured during reception of the MPDU
 * @param DSN              DSN of the received data frame.
 * @param Timestamp        The time, in symbols, at which the data were
 *received.
 *                         (only if timestamping is enabled).
 */
#ifdef MAC_SECURITY_ZIP
void usr_mcps_data_ind(wpan_addr_spec_t *SrcAddrSpec,
				wpan_addr_spec_t *DstAddrSpec,
				uint8_t msduLength,
				uint8_t *msdu,
				uint8_t mpduLinkQuality,
				uint8_t DSN,
#ifdef ENABLE_TSTAMP
				uint32_t Timestamp,
				#endif  /* ENABLE_TSTAMP */
				uint8_t SecurityLevel,
				uint8_t KeyIdMode,
				uint8_t KeyIndex)
#else /* No MAC_SECURITY */

void usr_mcps_data_ind(wpan_addr_spec_t *SrcAddrSpec,
		wpan_addr_spec_t *DstAddrSpec,
		uint8_t msduLength,
		uint8_t *msdu,
		uint8_t mpduLinkQuality,
#ifdef ENABLE_TSTAMP
		uint8_t DSN,
		uint32_t Timestamp)
#else
		uint8_t DSN)
#endif  /* ENABLE_TSTAMP */
#endif
{
#ifdef SIO_HUB
	//char sio_array[255];
#ifdef SIO_HUB
const char Display_Received_Frame[] = "Frame received: %lu\r\n";
#endif
#endif

	if (DstAddrSpec->Addr.short_address == BROADCAST) {
		bc_rx_cnt++;
#ifdef SIO_HUB
		printf("Broadcast ");
		printf(Display_Received_Frame, bc_rx_cnt);
#endif
	} else {
		indirect_rx_cnt++;
#ifdef SIO_HUB
		printf("Indirect Data ");
		printf("Frame received: ");
		for (uint8_t i = 0; i < msduLength; i++) {
			printf("%c", msdu[i]);
		}
		printf("\n\r");
#endif
	}

	/*
	 * Dummy data has been received successfully.
	 * Application code could be added here ...
	 */
	LED_On(LED_DATA);

	/* Start a timer switching off the LED. */
	sw_timer_start(APP_TIMER,
			500000,
			SW_TIMEOUT_RELATIVE,
			(FUNC_PTR)rx_data_led_off_cb,
			NULL);

	/* Keep compiler happy. */
	SrcAddrSpec = SrcAddrSpec;
	DstAddrSpec = DstAddrSpec;
	msduLength = msduLength;
	msdu = msdu;
	mpduLinkQuality = mpduLinkQuality;
	DSN = DSN;
#ifdef ENABLE_TSTAMP
	Timestamp = Timestamp;
#endif  /* ENABLE_TSTAMP */
}

#if ((MAC_PURGE_REQUEST_CONFIRM == 1) && (MAC_INDIRECT_DATA_BASIC == 1))

/*
 * Callback function usr_mcps_purge_conf
 *
 * @param msduHandle           Handle (id) of MSDU to be purged.
 * @param status               Result of requested purge operation.
 *
 * @return void
 *
 */
void usr_mcps_purge_conf(uint8_t msduHandle,
		uint8_t status)
{
}

#endif  /* ((MAC_PURGE_REQUEST_CONFIRM == 1) && (MAC_INDIRECT_DATA_BASIC == 1))
         **/

#if (MAC_ASSOCIATION_REQUEST_CONFIRM == 1)

/*
 * @brief Callback function usr_mlme_associate_conf
 *
 * @param AssocShortAddress    Short address allocated by the coordinator
 * @param status               Result of requested association operation
 */
void usr_mlme_associate_conf(uint16_t AssocShortAddress,
		uint8_t status)
{
	if (status == MAC_SUCCESS) {
#ifdef SIO_HUB
		printf("Connected to beacon-enabled network\r\n");
#endif
		/* Set proper state of application. */
		app_state = APP_DEVICE_RUNNING;

		/* Stop timer used for search indication */
		sw_timer_stop(APP_TIMER);

		LED_On(LED_NWK_SETUP);
		   uint8_t mac_dev_table_entries = 1;

	                 wpan_mlme_set_req(macDeviceTableEntries,
	                 NO_PIB_INDEX,
	                 &mac_dev_table_entries);

	} else {
		LED_Off(LED_NWK_SETUP);

		/* Set proper state of application. */
		app_state = APP_IDLE;

		/* Something went wrong; restart. */
		wpan_mlme_reset_req(true);
	}

	/* Keep compiler happy. */
	AssocShortAddress = AssocShortAddress;
#ifdef MAC_SECURITY_ZIP
    deviceShortAddress = AssocShortAddress;
#endif
}

#endif  /* (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) */

#if (MAC_ASSOCIATION_INDICATION_RESPONSE == 1)

/*
 * @brief Callback function usr_mlme_associate_ind
 *
 * @param DeviceAddress         Extended address of device requesting
 *association
 * @param CapabilityInformation Capabilities of device requesting association
 */
void usr_mlme_associate_ind(uint64_t DeviceAddress,
		uint8_t CapabilityInformation)
{
	/* Keep compiler happy. */
	DeviceAddress = DeviceAddress;
	CapabilityInformation = CapabilityInformation;
}

#endif  /* (MAC_ASSOCIATION_INDICATION_RESPONSE == 1) */

#if (MAC_BEACON_NOTIFY_INDICATION == 1)

/*
 * Callback function usr_mlme_beacon_notify_ind
 *
 * @param BSN            Beacon sequence number.
 * @param PANDescriptor  Pointer to PAN descriptor for received beacon.
 * @param PendAddrSpec   Pending address specification in received beacon.
 * @param AddrList       List of addresses of devices the coordinator has
 *pending data.
 * @param sduLength      Length of beacon payload.
 * @param sdu            Pointer to beacon payload.
 */
void usr_mlme_beacon_notify_ind(uint8_t BSN,
		wpan_pandescriptor_t *PANDescriptor,
		uint8_t PendAddrSpec,
		uint8_t *AddrList,
		uint8_t sduLength,
		uint8_t *sdu)
{
	if (APP_SCAN_DONE == app_state) {
		/* Set proper state of application. */
		app_state = APP_ASSOC_IN_PROGRESS;

		/*
		 * Associate to our coordinator.
		 * Use: bool wpan_mlme_associate_req(uint8_t LogicalChannel,
		 *                                   uint8_t ChannelPage,
		 *                                   wpan_addr_spec_t
		 **CoordAddrSpec,
		 *                                   uint8_t
		 *CapabilityInformation);
		 * This request will cause a mlme associate confirm message ->
		 * usr_mlme_associate_conf.
		 */
		wpan_mlme_associate_req(current_channel,
				current_channel_page,
				&coord_addr_spec,
				WPAN_CAP_ALLOCADDRESS);
	} else if (APP_DEVICE_RUNNING == app_state) {
		/* This is the standard portion once the node is associated
		 * with the application.
		 */

		/*
		 * Extract the beacon payload from our coordinator and feed it
		 *back
		 * to the coordinator via a data frame.
		 */

		/* Use: bool wpan_mcps_data_req(uint8_t SrcAddrMode,
		 *                              wpan_addr_spec_t *DstAddrSpec,
		 *                              uint8_t msduLength,
		 *                              uint8_t *msdu,
		 *                              uint8_t msduHandle,
		 *                              uint8_t TxOptions);
		 *
		 * This request will cause a mcps data confirm message ->
		 * usr_mcps_data_conf
		 */
		if (sduLength > PAYLOAD_LEN) {
			sduLength = PAYLOAD_LEN;
		}

		/* Copy payload from beacon frame. */
		memcpy(&msdu_payload, sdu, sduLength);

		uint8_t src_addr_mode = WPAN_ADDRMODE_SHORT;
		static uint8_t msdu_handle = 0;

		msdu_handle++;      /* Increment handle */
		
#ifdef MAC_SECURITY_ZIP
		wpan_mcps_data_req(src_addr_mode,
				&coord_addr_spec,
				sduLength,  // msduLength
				&msdu_payload[0],
				msdu_handle,
				WPAN_TXOPT_ACK,
				ZIP_SEC_MIN,     // SecurityLevel
				NULL,
				ZIP_KEY_ID_MODE, // KeyIdMode
				deviceShortAddress);  // KeyIndex
#else		
		wpan_mcps_data_req(src_addr_mode,
				&coord_addr_spec,
				sduLength,
				&msdu_payload[0],
				msdu_handle,
				WPAN_TXOPT_ACK);
#endif

#ifdef SIO_HUB
		{
			static uint32_t rx_cnt;
			const char Rx_Beacon_Payload[] = "Rx beacon payload (%lu): ";

			/* Print received payload. */
			rx_cnt++;
			printf(Rx_Beacon_Payload, rx_cnt);

			/* Set last element to 0. */
			if (sduLength == PAYLOAD_LEN) {
				msdu_payload[PAYLOAD_LEN - 1] = '\0';
			} else {
				msdu_payload[sduLength] = '\0';
			}
			
			for (uint8_t i = 0; i < sduLength; i++) {
				printf("%c", msdu_payload[i]);
			}			
			printf("\r\n");
		}
#endif  /* SIO_HUB */
	}

	/* Keep compiler happy. */
	BSN = BSN;
	PANDescriptor = PANDescriptor;
	PendAddrSpec = PendAddrSpec;
	AddrList = AddrList;
}

#endif  /* (MAC_BEACON_NOTIFY_INDICATION == 1) */

#if ((MAC_ORPHAN_INDICATION_RESPONSE == 1) || \
	(MAC_ASSOCIATION_INDICATION_RESPONSE == 1))

/*
 * @brief Callback function usr_mlme_comm_status_ind
 *
 * @param SrcAddrSpec      Pointer to source address specification
 * @param DstAddrSpec      Pointer to destination address specification
 * @param status           Result for related response operation
 */
void usr_mlme_comm_status_ind(wpan_addr_spec_t *SrcAddrSpec,
		wpan_addr_spec_t *DstAddrSpec,
		uint8_t status)
{
	if (status == MAC_SUCCESS) {
		/*
		 * Now the association of the device has been successful and its
		 * information, like address, could  be stored.
		 * But for the sake of simple handling it has been done
		 * during assignment of the short address within the function
		 * assign_new_short_addr()
		 */
	}

	/* Keep compiler happy. */
	SrcAddrSpec = SrcAddrSpec;
	DstAddrSpec = DstAddrSpec;
}

#endif  /* ((MAC_ORPHAN_INDICATION_RESPONSE == 1) ||
         *(MAC_ASSOCIATION_INDICATION_RESPONSE == 1)) */

#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)

/*
 * Callback function usr_mlme_disassociate_conf
 *
 * @param status             Result of requested disassociate operation.
 * @param DeviceAddrSpec     Pointer to wpan_addr_spec_t structure for device
 *                           that has either requested disassociation or been
 *                           instructed to disassociate by its coordinator.
 *
 * @return void
 */
void usr_mlme_disassociate_conf(uint8_t status,
		wpan_addr_spec_t *DeviceAddrSpec)
{
}

#endif /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)*/

#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)

/*
 * Callback function usr_mlme_disassociate_ind
 *
 * @param DeviceAddress        Extended address of device which initiated the
 *                             disassociation request.
 * @param DisassociateReason   Reason for the disassociation. Valid values:
 *                           - @ref WPAN_DISASSOC_BYPARENT,
 *                           - @ref WPAN_DISASSOC_BYCHILD.
 *
 * @return void
 */
void usr_mlme_disassociate_ind(uint64_t DeviceAddress,
		uint8_t DisassociateReason)
{
}

#endif  /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) */

#if (MAC_GET_SUPPORT == 1)

/*
 * Callback function usr_mlme_get_conf
 *
 * @param status            Result of requested PIB attribute get operation.
 * @param PIBAttribute      Retrieved PIB attribute.
 * @param PIBAttributeIndex Index of the PIB attribute to be read.
 * @param PIBAttributeValue Pointer to data containing retrieved PIB attribute,
 *
 * @return void
 */
void usr_mlme_get_conf(uint8_t status,
			uint8_t PIBAttribute,
#ifdef MAC_SECURITY_ZIP
			uint8_t PIBAttributeIndex,
#endif  /* MAC_SECURITY_ZIP */
			void *PIBAttributeValue)
{
#ifdef MAC_SECURITY_ZIP
mac_key_table_t *key_table = (mac_key_table_t *)PIBAttributeValue;
#endif
	if ((status == MAC_SUCCESS) && (PIBAttribute == phyCurrentPage)) {
		current_channel_page = *(uint8_t *)PIBAttributeValue;
#ifdef MAC_SECURITY_ZIP
		wpan_mlme_get_req(phyChannelsSupported,NO_PIB_INDEX);
#else
		wpan_mlme_get_req(phyChannelsSupported);
#endif
	} else if ((status == MAC_SUCCESS) &&
			(PIBAttribute == phyChannelsSupported)) {
		uint8_t index;

		channels_supported = *(uint32_t *)PIBAttributeValue;

		for (index = 0; index < 32; index++) {
			if (channels_supported & (1 << index)) {
				current_channel = index + CHANNEL_OFFSET;
				break;
			}
		}
#ifdef SIO_HUB
		printf("Searching network in Channel - %d\r\n",
				current_channel);
#endif

		/*
		 * Initiate an active scan over all channels to determine
		 * which channel is used by the coordinator.
		 * Use: bool wpan_mlme_scan_req(uint8_t ScanType,
		 *                              uint32_t ScanChannels,
		 *                              uint8_t ScanDuration,
		 *                              uint8_t ChannelPage);
		 *
		 * This request leads to a scan confirm message ->
		 *usr_mlme_scan_conf
		 * Scan for about 50 ms on each channel -> ScanDuration = 1
		 * Scan for about 1/2 second on each channel -> ScanDuration = 5
		 * Scan for about 1 second on each channel -> ScanDuration = 6
		 */
		wpan_mlme_scan_req(MLME_SCAN_TYPE_ACTIVE,
				SCAN_CHANNEL,
				SCAN_DURATION_SHORT,
				current_channel_page);

		/* Indicate network scanning by a LED flashing. */
		sw_timer_start(APP_TIMER,
				500000,
				SW_TIMEOUT_RELATIVE,
				(FUNC_PTR)network_search_indication_cb,
				NULL);
	}
	else if((status == MAC_SUCCESS) &&
	     (PIBAttribute == macKeyTable)){
			 for (uint8_t i = 0; i < key_table->KeyDeviceListEntries; i++)
			 {
				 if (EMPTY_DEV_HANDLE == (key_table->KeyDeviceList[i].DeviceDescriptorHandle))
				 {
					 key_table->KeyDeviceList[i].DeviceDescriptorHandle = 0x00;
					 key_table->KeyDeviceList[i].UniqueDevice = true;
					 break;
				 }
			 }
			 wpan_mlme_set_req(macKeyTable, deviceShortAddress - 1, (uint8_t *)PIBAttributeValue);
		
	}
}

#endif  /* (MAC_GET_SUPPORT == 1) */

#if (MAC_ORPHAN_INDICATION_RESPONSE == 1)

/*
 * Callback function usr_mlme_orphan_ind
 *
 * @param OrphanAddress     Address of orphaned device.
 *
 * @return void
 *
 */
void usr_mlme_orphan_ind(uint64_t OrphanAddress)
{
}

#endif  /* (MAC_ORPHAN_INDICATION_RESPONSE == 1) */

#if (MAC_INDIRECT_DATA_BASIC == 1)

/*
 * Callback function that must be implemented by application (NHLE) for MAC
 *service
 * MLME-POLL.confirm.
 *
 * @param status           Result of requested poll operation.
 *
 * @return void
 *
 */
void usr_mlme_poll_conf(uint8_t status)
{
}

#endif  /* (MAC_INDIRECT_DATA_BASIC == 1) */

/*
 * @brief Callback function usr_mlme_reset_conf
 *
 * @param status Result of the reset procedure
 */
void usr_mlme_reset_conf(uint8_t status)
{
	if (status == MAC_SUCCESS) {
#ifdef MAC_SECURITY_ZIP
	/*	wpan_mlme_get_req(phyCurrentPage,NO_PIB_INDEX);*/
		wpan_mlme_set_req(macDefaultKeySource,
		NO_PIB_INDEX,
		&default_key_source);
#else
		wpan_mlme_get_req(phyCurrentPage);
#endif
	} else {
		/* Set proper state of application. */
		app_state = APP_IDLE;

		/* Something went wrong; restart. */
		wpan_mlme_reset_req(true);
	}
}

#if (MAC_RX_ENABLE_SUPPORT == 1)

/*
 * Callback function usr_mlme_rx_enable_conf
 *
 * @param status           Result of requested receiver enable operation.
 *
 * @return void
 */
void usr_mlme_rx_enable_conf(uint8_t status)
{
}

#endif  /* (MAC_RX_ENABLE_SUPPORT == 1) */

#if ((MAC_SCAN_ED_REQUEST_CONFIRM == 1)      ||	\
	(MAC_SCAN_ACTIVE_REQUEST_CONFIRM == 1)  || \
	(MAC_SCAN_PASSIVE_REQUEST_CONFIRM == 1) || \
	(MAC_SCAN_ORPHAN_REQUEST_CONFIRM == 1))

/*
 * @brief Callback function usr_mlme_scan_conf
 *
 * @param status            Result of requested scan operation
 * @param ScanType          Type of scan performed
 * @param ChannelPage       Channel page on which the scan was performed
 * @param UnscannedChannels Bitmap of unscanned channels
 * @param ResultListSize    Number of elements in ResultList
 * @param ResultList        Pointer to array of scan results
 */
void usr_mlme_scan_conf(uint8_t status,
		uint8_t ScanType,
		uint8_t ChannelPage,
		uint32_t UnscannedChannels,
		uint8_t ResultListSize,
		void *ResultList)
{
	if (status == MAC_SUCCESS) {
		wpan_pandescriptor_t *coordinator;
		uint8_t i;

		/*
		 * Analyze the ResultList.
		 * Assume that the first entry of the result list is our
		 *coodinator.
		 */
		coordinator = (wpan_pandescriptor_t *)ResultList;

		for (i = 0; i < ResultListSize; i++) {
			/*
			 * Check if the PAN descriptor belongs to our
			 *coordinator.
			 * Check if coordinator allows association.
			 */
			if ((coordinator->LogicalChannel == current_channel) &&
					(coordinator->ChannelPage ==
					current_channel_page) &&
					(coordinator->CoordAddrSpec.PANId ==
					DEFAULT_PAN_ID) &&
					((coordinator->SuperframeSpec &
					((uint16_t)1 <<
					ASSOC_PERMIT_BIT_POS)) ==
					((uint16_t)1 << ASSOC_PERMIT_BIT_POS))
					) {
				/* Store the coordinator's address information.
				 **/
				coord_addr_spec.AddrMode = WPAN_ADDRMODE_SHORT;
				coord_addr_spec.PANId = DEFAULT_PAN_ID;
				ADDR_COPY_DST_SRC_16(
						coord_addr_spec.Addr.short_address,
						coordinator->CoordAddrSpec.Addr.short_address);
#ifdef SIO_HUB
				printf("Found network\r\n");
#endif

				/* Set proper state of application. */
				app_state = APP_SCAN_DONE;

				/*
				 * Set the PAN-Id of the scanned network.
				 * This is required in order to perform a proper
				 *sync
				 * before assocation.
				 * Use: bool wpan_mlme_set_req(uint8_t
				 *PIBAttribute,
				 *                             void
				 **PIBAttributeValue);
				 *
				 * This request leads to a set confirm message
				 *-> usr_mlme_set_conf
				 */
				uint16_t pan_id;
				pan_id = DEFAULT_PAN_ID;
				wpan_mlme_set_req(macPANId,NO_PIB_INDEX, &pan_id);
				
				return;
			}

			/* Get the next PAN descriptor. */
			coordinator++;
		}

		/*
		 * If here, the result list does not contain our expected
		 *coordinator.
		 * Let's scan again.
		 */
		wpan_mlme_scan_req(MLME_SCAN_TYPE_ACTIVE,
				SCAN_CHANNEL,
				SCAN_DURATION_SHORT,
				current_channel_page);
	} else if (status == MAC_NO_BEACON) {
		/*
		 * No beacon is received; no coordiantor is located.
		 * Scan again, but used longer scan duration.
		 */
		wpan_mlme_scan_req(MLME_SCAN_TYPE_ACTIVE,
				SCAN_CHANNEL,
				SCAN_DURATION_LONG,
				current_channel_page);
	} else {
		/* Set proper state of application. */
		app_state = APP_IDLE;

		/* Something went wrong; restart. */
		wpan_mlme_reset_req(true);
	}

	/* Keep compiler happy. */
	ScanType = ScanType;
	ChannelPage = ChannelPage;
	UnscannedChannels = UnscannedChannels;
}

#endif

/*
 * @brief Callback function usr_mlme_set_conf
 *
 * @param status        Result of requested PIB attribute set operation
 * @param PIBAttribute  Updated PIB attribute
 */

void usr_mlme_set_conf(uint8_t status, uint8_t PIBAttribute, uint8_t PIBAttributeIndex)
{
    if (status != MAC_SUCCESS)
    {
        // something went wrong; restart
        wpan_mlme_reset_req(true);
    }
    else
    {
        switch (PIBAttribute)
        {
            case macPANId:
            {
				/*
				 * Set the Coordinator Short Address of the scanned network.
				 * This is required in order to perform a proper sync
				 * before assocation.
				 */
				wpan_mlme_set_req(macCoordShortAddress,NO_PIB_INDEX, &coord_addr_spec.Addr);
				
			}
            break;

            case macCoordShortAddress:
                 {
					wpan_mlme_sync_req(current_channel,
							current_channel_page,
							1);
                }
                 break;

            case macDefaultKeySource:
                 {
	                 uint8_t mac_sec_level_table_entries = 1;

	                 wpan_mlme_set_req(macSecurityLevelTableEntries,
	                 NO_PIB_INDEX,
	                 &mac_sec_level_table_entries);
                 }
                 break;

             case macSecurityLevelTableEntries:
                 {
	                 uint8_t mac_sec_level_table[4] = {FRAME_TYPE_DATA,      // FrameType: Data
		                 CMD_FRAME_ID_NA,      // CommandFrameIdentifier: N/A
		                 ZIP_SEC_MIN,          // SecurityMinimum: 5
		                 DEV_OVERRIDE_SEC_MIN  // DeviceOverrideSecurityMinimum: True
	                 };

	                 wpan_mlme_set_req(macSecurityLevelTable,
	                 0,    // Index: 0
	                 &mac_sec_level_table);
                 }
                 break;

             case macSecurityLevelTable:
                 {
	               uint8_t mac_key_table_entries = 3;

	                 wpan_mlme_set_req(macKeyTableEntries,
	                 NO_PIB_INDEX,
	                 &mac_key_table_entries);
                 }
                 break;

             case macKeyTableEntries:
                 {
	                                    uint8_t mac_key_table[40] =
	                                    {
		                                    // KeyIdLookupList[1].LookupData : macDefaultKeySource || g_Sec_KeyIndex_1
		                                    default_key_source[0], // LookupData[0]
		                                    default_key_source[1], // LookupData[1]
		                                    default_key_source[2], // LookupData[2]
		                                    default_key_source[3], // LookupData[3]
		                                    default_key_source[4], // LookupData[4]
		                                    default_key_source[5], // LookupData[5]
		                                    default_key_source[6], // LookupData[6]
		                                    default_key_source[7], // LookupData[7]
		                                    KEY_INDEX_1,           // LookupData[8]
		                                    LOOKUP_DATA_SIZE_1, // LookupDataSize: 0x01 : Size 9 octets
		                                    1,              // KeyIdLookupListEntries = 1
		                                    // KeyDeviceList[1]
		                                    EMPTY_DEV_HANDLE,              // DeviceDescriptorHandle
		                                    false,              // UniqueDevice - Key is unique per node
		                                    false,              // Blacklisted
		                                    EMPTY_DEV_HANDLE,              // DeviceDescriptorHandle
		                                    false,              // UniqueDevice - Key is unique per node
		                                    false,              // Blacklisted
		                                    EMPTY_DEV_HANDLE,              // DeviceDescriptorHandle
		                                    false,              // UniqueDevice - Key is unique per node
		                                    false,              // Blacklisted
		                                    MAC_ZIP_MAX_KEY_DEV_LIST_ENTRIES,              // KeyDeviceListEntries
		                                    //  KeyUsageList
		                                    FRAME_TYPE_DATA,    // FrameType - Data frames
		                                    CMD_FRAME_ID_NA,    // CommandFrameIdentifier not used in ZIP
		                                    1,                  // KeyUsageListEntries
		                                    // Key
		                                    default_key[0][0],
		                                    default_key[0][1],
		                                    default_key[0][2],
		                                    default_key[0][3],
		                                    default_key[0][4],
		                                    default_key[0][5],
		                                    default_key[0][6],
		                                    default_key[0][7],
		                                    default_key[0][8],
		                                    default_key[0][9],
		                                    default_key[0][10],
		                                    default_key[0][11],
		                                    default_key[0][12],
		                                    default_key[0][13],
		                                    default_key[0][14],
		                                    default_key[0][15],
	                                    };
	                                    uint8_t mac_key_table1[40] =
	                                    {
		                                    // KeyIdLookupList[1].LookupData : macDefaultKeySource || g_Sec_KeyIndex_1
		                                    default_key_source[0], // LookupData[0]
		                                    default_key_source[1], // LookupData[1]
		                                    default_key_source[2], // LookupData[2]
		                                    default_key_source[3], // LookupData[3]
		                                    default_key_source[4], // LookupData[4]
		                                    default_key_source[5], // LookupData[5]
		                                    default_key_source[6], // LookupData[6]
		                                    default_key_source[7], // LookupData[7]
		                                    KEY_INDEX_2,           // LookupData[8]
		                                    LOOKUP_DATA_SIZE_1, // LookupDataSize: 0x01 : Size 9 octets
		                                    1,              // KeyIdLookupListEntries = 1
		                                    // KeyDeviceList[1]
		                                    EMPTY_DEV_HANDLE,              // DeviceDescriptorHandle
		                                    false,              // UniqueDevice - Key is unique per node
		                                    false,              // Blacklisted
		                                    EMPTY_DEV_HANDLE,              // DeviceDescriptorHandle
		                                    false,              // UniqueDevice - Key is unique per node
		                                    false,              // Blacklisted
		                                    EMPTY_DEV_HANDLE,              // DeviceDescriptorHandle
		                                    false,              // UniqueDevice - Key is unique per node
		                                    false,              // Blacklisted
		                                    MAC_ZIP_MAX_KEY_DEV_LIST_ENTRIES,              // KeyDeviceListEntries
		                                    //  KeyUsageList
		                                    FRAME_TYPE_DATA,    // FrameType - Data frames
		                                    CMD_FRAME_ID_NA,    // CommandFrameIdentifier not used in ZIP
		                                    1,                  // KeyUsageListEntries
		                                    // Key
		                                    default_key[1][0],
		                                    default_key[1][1],
		                                    default_key[1][2],
		                                    default_key[1][3],
		                                    default_key[1][4],
		                                    default_key[1][5],
		                                    default_key[1][6],
		                                    default_key[1][7],
		                                    default_key[1][8],
		                                    default_key[1][9],
		                                    default_key[1][10],
		                                    default_key[1][11],
		                                    default_key[1][12],
		                                    default_key[1][13],
		                                    default_key[1][14],
		                                    default_key[1][15],
	                                    };
	                                    uint8_t mac_key_table2[40] =
	                                    {
		                                    // KeyIdLookupList[1].LookupData : macDefaultKeySource || g_Sec_KeyIndex_1
		                                    default_key_source[0], // LookupData[0]
		                                    default_key_source[1], // LookupData[1]
		                                    default_key_source[2], // LookupData[2]
		                                    default_key_source[3], // LookupData[3]
		                                    default_key_source[4], // LookupData[4]
		                                    default_key_source[5], // LookupData[5]
		                                    default_key_source[6], // LookupData[6]
		                                    default_key_source[7], // LookupData[7]
		                                    KEY_INDEX_3,           // LookupData[8]
		                                    LOOKUP_DATA_SIZE_1, // LookupDataSize: 0x01 : Size 9 octets
		                                    1,              // KeyIdLookupListEntries = 1
		                                    // KeyDeviceList[1]
		                                    EMPTY_DEV_HANDLE,              // DeviceDescriptorHandle
		                                    false,              // UniqueDevice - Key is unique per node
		                                    false,              // Blacklisted
		                                    EMPTY_DEV_HANDLE,              // DeviceDescriptorHandle
		                                    false,              // UniqueDevice - Key is unique per node
		                                    false,              // Blacklisted
		                                    EMPTY_DEV_HANDLE,              // DeviceDescriptorHandle
		                                    false,              // UniqueDevice - Key is unique per node
		                                    false,              // Blacklisted
		                                    MAC_ZIP_MAX_KEY_DEV_LIST_ENTRIES,              // KeyDeviceListEntries
		                                    //  KeyUsageList
		                                    FRAME_TYPE_DATA,    // FrameType - Data frames
		                                    CMD_FRAME_ID_NA,    // CommandFrameIdentifier not used in ZIP
		                                    1,                  // KeyUsageListEntries
		                                    // Key
		                                    default_key[2][0],
		                                    default_key[2][1],
		                                    default_key[2][2],
		                                    default_key[2][3],
		                                    default_key[2][4],
		                                    default_key[2][5],
		                                    default_key[2][6],
		                                    default_key[2][7],
		                                    default_key[2][8],
		                                    default_key[2][9],
		                                    default_key[2][10],
		                                    default_key[2][11],
		                                    default_key[2][12],
		                                    default_key[2][13],
		                                    default_key[2][14],
		                                    default_key[2][15],
	                                    };
	                                    wpan_mlme_set_req(macKeyTable,
	                                    INDEX_0,    // Index: 0
	                                    &mac_key_table);

	                                    wpan_mlme_set_req(macKeyTable,
	                                    INDEX_1,    // Index: 1
	                                    &mac_key_table1);
	                                    wpan_mlme_set_req(macKeyTable,
	                                    INDEX_2,    // Index: 2
	                                    &mac_key_table2);
                                    }
                 break;

             case macKeyTable:
                 {
					/*
					* Sync with beacon frames from our coordinator.
					* Use: bool wpan_mlme_sync_req(uint8_t LogicalChannel,
					*                              uint8_t ChannelPage,
					*                              bool TrackBeacon);
					*
					* This does not lead to an immediate reaction.
					*
					* In case we receive beacon frames from our coordinator
					*including
					* a beacon payload, this is indicated in the callback function
					* usr_mlme_beacon_notify_ind().
					*
					* In case the device cannot find its coordinator or later
					*looses
					* synchronization with its parent, this is indicated in the
					* callback function usr_mlme_sync_loss_ind().
					*/
					if(PIBAttributeIndex == INDEX_2 && app_state == APP_IDLE){
						wpan_mlme_get_req(phyCurrentPage,NO_PIB_INDEX);
				    }
                 }
                 break;

              case macDeviceTableEntries:
                 {
	                 uint8_t mac_dev_table[17] =
	                 {
		                 // DeviceDescriptor
		                 // PANId
		                 (uint8_t)tal_pib.PANId,
		                 (uint8_t)(tal_pib.PANId >> 8),
		                 // Device ShortAddress
		                 (uint8_t)mac_pib.mac_CoordShortAddress,
		                 (uint8_t)(mac_pib.mac_CoordShortAddress >> 8),
		                 // Device ExtAddress
		                 (uint8_t)mac_pib.mac_CoordExtendedAddress,
		                 (uint8_t)(mac_pib.mac_CoordExtendedAddress >> 8),
		                 (uint8_t)(mac_pib.mac_CoordExtendedAddress >> 16),
		                 (uint8_t)(mac_pib.mac_CoordExtendedAddress >> 24),
		                 (uint8_t)(mac_pib.mac_CoordExtendedAddress >> 32),
		                 (uint8_t)(mac_pib.mac_CoordExtendedAddress >> 40),
		                 (uint8_t)(mac_pib.mac_CoordExtendedAddress >> 48),
		                 (uint8_t)(mac_pib.mac_CoordExtendedAddress >> 56),
		                 // FrameCounter
		                 0, 0, 0, 0,
		                 // Exempt
		                 0
	                 };

	                 wpan_mlme_set_req(macDeviceTable,
	                 0,    // Index: 0
	                 &mac_dev_table);
                 }
                 break;

             case macDeviceTable:
                 {
	                 /* Use DSN start value as in ZIP test spec. */
	                 uint8_t new_dsn = 0x0;

	                 wpan_mlme_set_req(macDSN,
	                 NO_PIB_INDEX,
	                 &new_dsn);
                 }
                 break;

              case macDSN:
                 {
	                 /* Use DSN start value as in ZIP test spec. */
	                 uint32_t frame_counter = 1;

	                 wpan_mlme_set_req(macFrameCounter,
	                 NO_PIB_INDEX,
	                 &frame_counter);
                 }
                 break;

              case macFrameCounter:
				 {
					 wpan_mlme_get_req(macKeyTable,deviceShortAddress-1);
				 }
                 break;

                 default:
                 // undesired PIB attribute; restart
                 wpan_mlme_reset_req(true);
                 break;
        }
    }

    /* Keep compiler happy. */
    PIBAttributeIndex = PIBAttributeIndex;
}



#if (MAC_START_REQUEST_CONFIRM == 1)
void usr_mlme_start_conf(uint8_t status)
{
}

#endif  /* (MAC_START_REQUEST_CONFIRM == 1) */

/*
 * Callback function usr_mlme_sync_loss_ind
 *
 * @param LossReason     Reason for synchronization loss.
 * @param PANId          The PAN identifier with which the device lost
 *                       synchronization or to which it was realigned.
 * @param LogicalChannel The logical channel on which the device lost
 *                       synchronization or to which it was realigned.
 * @param ChannelPage    The channel page on which the device lost
 *                       synchronization or to which it was realigned.
 */
void usr_mlme_sync_loss_ind(uint8_t LossReason,
		uint16_t PANId,
		uint8_t LogicalChannel,
		uint8_t ChannelPage)
{
	/*
	 * Once we lost sync this the coordinator we need to re-sync.
	 * Since we the network parameter are not supposed ot change,
	 * use the already known parameters form our coordinator.
	 */
	wpan_mlme_sync_req(LogicalChannel,
			ChannelPage,
			1);

	/* Keep compiler happy. */
	LossReason = LossReason;
	PANId = PANId;
}

/* Alert to indicate something has gone wrong in the application */
static void app_alert(void)
{
	while (1) {
		#if LED_COUNT > 0
		LED_Toggle(LED0);
		#endif

		#if LED_COUNT > 1
		LED_Toggle(LED1);
		#endif

		#if LED_COUNT > 2
		LED_Toggle(LED2);
		#endif

		#if LED_COUNT > 3
		LED_Toggle(LED3);
		#endif

		#if LED_COUNT > 4
		LED_Toggle(LED4);
		#endif

		#if LED_COUNT > 5
		LED_Toggle(LED5);
		#endif

		#if LED_COUNT > 6
		LED_Toggle(LED6);
		#endif

		#if LED_COUNT > 7
		LED_Toggle(LED7);
		#endif
		delay_us(0xFFFF);
	}
}

/*
 * @brief Callback function indicating network search
 *
 * @param parameter Pointer to callback parameter
 *                  (not used in this application, but could be if desired).
 */
static void network_search_indication_cb(void *parameter)
{
	LED_Toggle(LED_NWK_SETUP);

	/* Re-start timer again. */
	sw_timer_start(APP_TIMER,
			500000,
			SW_TIMEOUT_RELATIVE,
			(FUNC_PTR)network_search_indication_cb,
			NULL);

	parameter = parameter; /* Keep compiler happy. */
}

/*
 * @brief Callback function for switching off the LED after data reception
 *
 * @param parameter Pointer to callback parameter
 *                  (not used in this application, but could be if desired).
 */
static void rx_data_led_off_cb(void *parameter)
{
	LED_Off(LED_DATA);

	parameter = parameter; /* Keep compiler happy. */
}
