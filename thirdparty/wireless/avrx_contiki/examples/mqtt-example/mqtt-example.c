
/**
* Copyright (c) 2015 Atmel Corporation and 2012 � 2013, Thingsquare, http://www.thingsquare.com/. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. Neither the name of Atmel nor the name of Thingsquare nor the names of its contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an
* Atmel microcontroller or Atmel wireless product.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*
*
*/
/**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the WSN Demo Application Application
 * The WSNDemo application implements a typical wireless sensor network
 *scenario,
 * in which one central node collects the data from a network of sensors and
 *passes this data over a serial connection for further processing.
 * In the case of the WSNDemo this processing is performed by the WSNMonitor PC
 *application. The BitCloud� Quick Start Guide  provides a detailed description
 *of the WSNDemo application scenario, and instructions on how to use
 *WSNMonitor.
 *  However since BitCloud is a ZigBee� PRO stack, there are a few differences
 *in the protocol:
 * � Device types (Coordinator, Router and End Device) are simulated on the
 *application level; there is no such separation in Lightweight Mesh on the
 *stack level
 * � The value of the extended address field is set equal to the value of the
 *short address field
 * � For all frames, the LQI and RSSI fields are filled in by the coordinator
 *with the values of LQI and RSSI from the received frame. This means that nodes
 *that are not connected to the coordinator directly will have the same values
 *as the last node on the route to the coordinator
 * � Sensor data values are generated randomly on all platforms
 * � Sending data to the nodes on the network is not implemented and not
 *supported in this demo application
 */
#include <string.h>
#include "thsq.h"
#include "netstack-aes.h"
#include "dev/button-sensor.h"
#include "rpl/rpl-private.h"
#include "port.h"
#include "asf.h"
#include "mqtt.h"
#include "adc.h"

#define SEND_INTERVAL		(60 * CLOCK_SECOND)
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static struct mqtt_connection conn;
static struct mqtt_message* msg_ptr = 0;
static struct etimer light_sense_timer;
static struct etimer reconnect_timer;
//static uint16_t random_topic;
static char str_topic_state[30];
static char str_topic_sensor[30];
static char str_topic_led[30];
static char app_buffer[128];

/* IO1-XPro Board details */
static char temp_str[8];
static char light_str[8];
static char mac_adr_str[18];

char str[48];
volatile float temp_C;
float temp_F;

volatile double temp_res;

static uint16_t button_sensor_value=0;
static process_event_t led_updates_changed_event;
static process_event_t reconnect_event;
static uint8_t reconnecting = 0;
static uip_ipaddr_t google_ipv4_dns_server = {
    .u8 = {
      /* Google's IPv4 DNS in mapped in an IPv6 address (::ffff:8.8.8.8) */
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0xff, 0xff,
      0x08, 0x08, 0x08, 0x08,
    }
};


/* MQTT Configuration details */
#define HOST          "198.41.30.241"
#define VERSION	      "v1"
#define PRIORITY      "p0"
#define UUID	      "atmeld"



/*---------------------------------------------------------------------------*/

PROCESS(mqtt_example_process, "MQTT Example");
AUTOSTART_PROCESSES(&mqtt_example_process);

struct adc_module adc_instance;

void configure_adc(void)
{
	struct adc_config config_adc;
	
	adc_get_config_defaults(&config_adc);
	config_adc.clock_source                  = GCLK_GENERATOR_3;
	//config_adc.reference                     = ADC_REFERENCE_INT1V;
	config_adc.reference                     = ADC_REFERENCE_INTVCC1;
	
	adc_init(&adc_instance, ADC, &config_adc);
	adc_enable(&adc_instance);
}

uint16_t measure_light(void)
{
	uint16_t result;
	adc_start_conversion(&adc_instance);
	
	do {
		/* Wait for conversion to be done and read out result */
	} while (adc_read(&adc_instance, &result) == STATUS_BUSY);
	return result;
}


/*---------------------------------------------------------------------------*/
static void
mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void* data)
{
  uint16_t i;

  switch(event) {
    case MQTT_EVENT_CONNECTED: {
      printf("\r\n APP - Application has a MQTT connection\n");
      break;
    }
    case MQTT_EVENT_DISCONNECTED: {
      printf("\r\n APP - Disconnected from MQTT broker\n");
      break;
      }
    case MQTT_EVENT_PUBLISH: {
      msg_ptr = data;
      /* New led value */
      if(strcmp(msg_ptr->topic, str_topic_led) == 0) {
        msg_ptr->payload_chunk[msg_ptr->payload_length] = 0;

        if(strcmp((char *)(msg_ptr->payload_chunk), "on") == 0) {
		  printf("\r\n LED ON \n");
         //leds_on(LEDS_RED);
		 port_pin_set_output_level(PIN_PA22, true);
		 port_pin_set_output_level(PIN_PA23, true);
		 port_pin_set_output_level(PIN_PA13, true);
		 port_pin_set_output_level(PIN_PA19,false);
        }
        if(strcmp((char *)(msg_ptr->payload_chunk), "off") == 0) {
		  printf("\r\n LED OFF \n");
		  port_pin_set_output_level(PIN_PA22, false);
		  port_pin_set_output_level(PIN_PA23, false);
		  port_pin_set_output_level(PIN_PA13, false);
		  port_pin_set_output_level(PIN_PA19,true);
          //leds_off(LEDS_RED);
        }
      }

      /* Implement first_flag in publish message? */
      if(msg_ptr->first_chunk) {
        msg_ptr->first_chunk = 0;
        printf("\r\n APP - Application received a publish on topic '%s'. Payload "
               "size is %i bytes. Content:\n\n", msg_ptr->topic, msg_ptr->payload_length);
      }

      for(i = 0; i < msg_ptr->payload_chunk_length; i++) {
        printf("%c", msg_ptr->payload_chunk[i]);
      }
      if(msg_ptr->payload_left == 0) {
        printf("\n");
        printf("\n");
        printf("\r\n APP - Application received publish content succefully.\n");
      }
      break;
    }
    case MQTT_EVENT_SUBACK: {
      printf("\r\n APP - Application is subscribed to topic successfully\n");
      break;
    }
    case MQTT_EVENT_UNSUBACK: {
      printf("\r\n APP - Application is unsubscribed to topic successfully\n");
      break;
    }
    case MQTT_EVENT_PUBACK: {
      printf("\r\n APP - Publishing complete.\n");
      break;
    }
    default:
      printf("\r\n APP - Application got a unhandled MQTT event: %i\n", event);
      break;
  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(mqtt_example_process, ev, data)
{
    PROCESS_BEGIN();
	static unsigned long  timestmp;
	volatile uint16_t light;
	volatile uip_ds6_addr_t *lladdr;
  // ("%s%x%s%x","Node id: ",ipaddr," Count: ",count)

  sprintf(str_topic_state, "/%s/%s/%s%s",VERSION,PRIORITY,UUID,"/status");
  sprintf(str_topic_sensor, "/%s/%s/%s%s",VERSION,PRIORITY,UUID,"/sensor");
  printf("\r\n%s\n",str_topic_sensor);
  sprintf(str_topic_led, "/%s/%s/%s%s",VERSION,PRIORITY,UUID,"/led");

  //timestmp = clock_time();

  //configure_adc();
  //light = measure_light();

  lladdr = uip_ds6_get_link_local(-1);
  sprintf(mac_adr_str,"%02x%02x%02x%02x%02x%02x%02x%02x",lladdr->ipaddr.u8[8],lladdr->ipaddr.u8[9],lladdr->ipaddr.u8[10],lladdr->ipaddr.u8[11],lladdr->ipaddr.u8[12],lladdr->ipaddr.u8[13],lladdr->ipaddr.u8[14],lladdr->ipaddr.u8[15]);

  mdns_init();
  mdns_conf(&google_ipv4_dns_server);

  /* Turn off AES */
  netstack_aes_set_active(1);

  /* Allocate events */
  led_updates_changed_event = process_alloc_event();
  reconnect_event = process_alloc_event();
  /* At the moment it is up to the user to provide the underlying input and
   * output buffer.
   */
  etimer_set(&light_sense_timer, CLOCK_SECOND*5);
  printf("\r\nMQTT Client ID : %s%d",UUID,strlen(str_topic_sensor));
  mqtt_register(&conn, &mqtt_example_process, UUID, mqtt_event);

  mqtt_set_last_will(&conn, str_topic_state, "offline", MQTT_QOS_LEVEL_0);
  
  /* IO1 Xpro Board initilization */
  static struct etimer et;
  etimer_set(&et, CLOCK_SECOND);
  at30tse_init();
  volatile uint16_t thigh = 0;
  thigh = at30tse_read_register(AT30TSE_THIGH_REG,
  AT30TSE_NON_VOLATILE_REG, AT30TSE_THIGH_REG_SIZE);
   volatile uint16_t tlow = 0;
   tlow = at30tse_read_register(AT30TSE_TLOW_REG,
   AT30TSE_NON_VOLATILE_REG, AT30TSE_TLOW_REG_SIZE);
   at30tse_write_config_register(
   AT30TSE_CONFIG_RES(AT30TSE_CONFIG_RES_12_bit));
  //etimer_set(&periodic_timer, CLOCK_SECOND*20);8
   /* To keep compiler happy */
   thigh = thigh;
   tlow = tlow;
  
  /* Reconnect from here */

  while(1) {
      if(simple_rpl_parent() == NULL) {

      printf("\r\n Connecting to a Wireless Network...\r\n");
      etimer_set(&et, CLOCK_SECOND / 1);
      while(simple_rpl_parent() == NULL) {
        PROCESS_WAIT_UNTIL(etimer_expired(&et));
        etimer_reset(&et);
        //leds_toggle(LEDS_RED);
		//leds_on(LEDS_RED);
      }
      //leds_off(LEDS_RED);
      printf("\r\n Connected to Wireless network\r\n");
    }

    /* Reset reconnecting flag */
    reconnecting = 0;

    /* Connect to MQTT server */
    conn.auto_reconnect = 1;
    mqtt_connect(&conn, HOST, 1883, 20);
    PROCESS_WAIT_UNTIL(mqtt_connected(&conn));
    conn.auto_reconnect = 0;
    //printf("comes here 1********************\n");
	
	/* Publish to the online topic that we are online. */
    PROCESS_WAIT_UNTIL(mqtt_ready(&conn));
    mqtt_publish(&conn,
               NULL,
               str_topic_state,
               (uint8_t *)"online",
               strlen("online"),
               MQTT_QOS_LEVEL_0,
               MQTT_RETAIN_ON);
    //printf("comes here 2**************************\n");
    /* Subscribe to the led topic */
    PROCESS_WAIT_UNTIL(mqtt_ready(&conn));
    mqtt_subscribe(&conn,
                   NULL,
                   str_topic_led,
                   MQTT_QOS_LEVEL_0);
    //printf("comes here 3***************************\n");
    /* Main loop */
    while(1) {
      PROCESS_WAIT_EVENT();
      if(ev == reconnect_event) {
        mqtt_disconnect(&conn);
        reconnecting = 1;
        etimer_set(&reconnect_timer, CLOCK_SECOND*10);
      }
      if(reconnecting &&
         etimer_expired(&reconnect_timer)) {
        break;
      }


      if(etimer_expired(&light_sense_timer) && mqtt_ready(&conn)) {
	  button_sensor_value++;
        /* Send sensor data and blink led */
		//leds_toggle(LEDS_RED);

		// get timestamp
		timestmp = clock_time();
		
		// read temperature sensor
		temp_C = at30tse_read_temperature();	// Measure temp ( result in deg C
		// convert to Fahrenheit
		//temp_F = (temp_C*9.0+2.0)/5.0 +32.0;  // convert result to deg F ( add 2 for better rounding )
		temp_F = (temp_C*9.0)/5.0 +32.0;		// convert result to deg F
		int d1 = temp_F;						// Get the integer part .
		float f2 = temp_F - d1;					// Get fractional part
		//int d2 = trunc(f2 * 10000);			// Turn into integer
		int d2 = (int)(f2 * 10000);				// Or this one: Turn into integer.
		d2 = d2/100;

		// Print as parts, note that you need 0-padding for fractional bit.
		sprintf (temp_str, "%d.%d", d1, d2);
		//printf("%s\r\n",str);
		
		// measure light intensity
		//light = measure_light();
		//sprintf(light_str,"%d",light);
		
		// create payload for mqtt message
		//sprintf(app_buffer,"%s%lu%s%s%s%s%s%s%s","{\x22timestamp\x22: \x22",timestmp,
			//"\x22,\x22SENS_TEMPERATURE\x22: \x22",
			//temp_str,"\x22,\x22SENS_LIGHT_LEVEL\x22: \x22",
			//light_str,"\x22,\x22sender_id\x22: \x22",
		//mac_adr_str,"\x22}");        
        sprintf(app_buffer,"%s%s%s%s","temp: ",temp_str);
		//printf("\r\nAPP - Sending Light sensor value %s Temp sensor value %s app buffer size %d\n",light_str,temp_str,strlen(app_buffer));

        mqtt_publish(&conn,
               NULL,
               str_topic_sensor,
               (uint8_t *)app_buffer,
               strlen(app_buffer),
               MQTT_QOS_LEVEL_0,
               MQTT_RETAIN_OFF);
        etimer_restart(&light_sense_timer);
      }
    }
  }
  PROCESS_END();
}


/*---------------------------------------------------------------------------*/
