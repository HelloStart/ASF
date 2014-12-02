/**
 * \file
 *
 * \brief SAM Power Driver Quick Start
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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
#include <asf.h>
#include "conf_test.h"

static void performance_level_switch_test(void)
{
	struct system_gclk_gen_config gclk_conf;

	/* Switch to PL2 */
	system_switch_performance_level(SYSTEM_PERFORMANCE_LEVEL_2);
	system_flash_set_waitstates(2);
	
	/* Switch Clock freq to 48MHz */
	system_gclk_gen_get_config_defaults(&gclk_conf);
	gclk_conf.source_clock = SYSTEM_CLOCK_SOURCE_DFLL;
	gclk_conf.division_factor = 1;      
	gclk_conf.run_in_standby  = false; 
	gclk_conf.output_enable   = true; 
	system_gclk_gen_set_config(GCLK_GENERATOR_0, &gclk_conf);
}

static void config_clockoutput_extwake_pin(void)
{
	struct system_pinmux_config pin_conf;
	system_pinmux_get_config_defaults(&pin_conf);

	pin_conf.mux_position = CONF_GCLK0_OUTPUT_PINMUX;
	pin_conf.direction    = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
	system_pinmux_pin_set_config(CONF_GCLK0_OUTPUT_PIN, &pin_conf);
	pin_conf.mux_position = CONF_GCLK1_OUTPUT_PINMUX;
	system_pinmux_pin_set_config(CONF_GCLK1_OUTPUT_PIN, &pin_conf);

	pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
	pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_UP;
	pin_conf.mux_position = CONF_EXT_WAKEUP_PINMUX;
	system_pinmux_pin_set_config(CONF_EXT_WAKEUP_PIN, &pin_conf);
}

static void configure_extint_channel(void)
{

	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin           = BUTTON_0_EIC_PIN;
	config_extint_chan.gpio_pin_mux       = BUTTON_0_EIC_MUX;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	extint_chan_set_config(BUTTON_0_EIC_LINE, &config_extint_chan);
 	extint_chan_enable_callback(BUTTON_0_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	system_interrupt_enable_global();
}


static void supc_battery_backup_power_switch_test(void)
{
	struct system_battery_backup_power_switch_config config;
	system_battery_backup_power_switch_get_config_defaults(&config);
	config.wake_enabled         = true;
	config.battery_power_switch = SYSTEM_BATTERY_POWER_SWITCH_AUTOMATIC;
	system_battery_backup_power_switch_set_config(&config);
	
}

static void delay_time_s(uint32_t time)
{
	uint32_t i,j;
	
	for(j=0;i<time;j++)
		for(i=0;i<1000000;i++)
			;
}
int main(void)
{

		
/* Check if the RESET is caused by exit BACKUP mode */
			if (system_get_reset_cause() == SYSTEM_RESET_CAUSE_BACKUP
				&& system_get_backup_exit_source() == SYSTEM_RESET_BACKKUP_EXIT_EXTWAKE
				/*&& (system_get_pin_wakeup_cause() & (1 << CONF_EXT_WAKEUP_PIN))*/){
		
				/* GCLK0 is running at 4MHz,GCLK1 is disabled due to the BACKUP reset */
					
				/* The	GCLK0 freq output pin and LED0 are still ok due to they are retained */
		
				system_init();
			//port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
			config_clockoutput_extwake_pin();
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
			delay_time_s(1);  
			port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);
			delay_time_s(1);  
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
			delay_time_s(1);  
			port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);
			delay_time_s(1);
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
			delay_time_s(1);
			port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);
			 //   performance_level_switch_test();
			
			while(1);
			}

#if 1

			/*Wait automatic power switch between  VDD and VBAT power.*/
	if (system_get_reset_cause() == SYSTEM_RESET_CAUSE_BACKUP
		&& system_get_backup_exit_source() == SYSTEM_RESET_BACKKUP_EXIT_BBPS){

		/* GCLK0 is running at 4MHz,GCLK1 is disabled due to the BACKUP reset */	
		/* The  GCLK0 freq output pin and LED0 are still ok due to they are retained */
		system_init();
		port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
		config_clockoutput_extwake_pin();
		performance_level_switch_test();
		while(1);
	} 
	
	#endif

	system_io_retension_disable();
	system_init();
	port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
	supc_battery_backup_power_switch_test();

	delay_time_s(2);
		//supc_battery_backup_power_switch_test();

		port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);

		/* Now XOSC32K(32KHz),OSC16M(4MHz) and DFLL(48MHz) are running,
		   GCLK0 is running at 4MHz,
		   GCLK1 is runing at 4MHz.
		 */
	 
		/*Set GCLK0 and GCLK1 freq output pin and BACKUP extwake pin
		*/
		config_clockoutput_extwake_pin(); 


                /*  GCLK0/GCLK1 are running at 4MHz.*/
                struct system_standby_config config;
		configure_extint_channel();
		
		system_standby_get_config_defaults(&config);
		config.power_domain        = SYSTEM_POWER_DOMAIN_DEFAULT;
	    config.linked_power_domain = SYSTEM_LINKED_POWER_DOMAIN_DEFAULT;

		while (extint_chan_is_detected(BUTTON_0_EIC_LINE))
			extint_chan_clear_detected(BUTTON_0_EIC_LINE);
		
		//system_standby_set_config(&config);
		system_set_sleepmode(SYSTEM_SLEEPMODE_STANDBY);
		system_sleep();
	//! [main]
           port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);      
#if 0
	while (true) {
		
		//! [main_1]
		//if (extint_chan_is_detected(BUTTON_0_EIC_LINE))
		 {
		
			extint_chan_clear_detected(BUTTON_0_EIC_LINE);
                      //  port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
                        break;
		}
	}
#endif
		

		extint_chan_disable_callback(BUTTON_0_EIC_LINE,
					EXTINT_CALLBACK_TYPE_DETECT);
						
extint_chan_clear_detected(BUTTON_0_EIC_LINE);
      //  port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);
		//performance_level_switch_test();
		
/*Set EXTWAKE pin*/
		system_set_pin_wakeup_polarity_low(1<<CONF_EXT_WAKEUP_PIN);
		system_enable_pin_wakeup(1<<CONF_EXT_WAKEUP_PIN);
		system_set_pin_wakeup_debounce_counter(SYSTEM_WAKEUP_DEBOUNCE_2CK32);

            delay_time_s(1);  
			port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);
			delay_time_s(1);  
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
			delay_time_s(1);  
			port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);
		/*Enter BACKUP mode*/
		system_set_sleepmode(SYSTEM_SLEEPMODE_BACKUP);
		//system_io_retension_enable();
		system_sleep();

#if 0
	/* The reset cause is POR , start testing performance level and backup mode*/
	if (system_get_reset_cause() == SYSTEM_RESET_CAUSE_POR){

		system_init();
		port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);

		/* Now XOSC32K(32KHz),OSC16M(4MHz) and DFLL(48MHz) are running,
		   GCLK0 is running at 4MHz,
		   GCLK1 is runing at 4MHz.
		 */
	 
		/*Set GCLK0 and GCLK1 freq output pin and BACKUP extwake pin
		*/
		config_clockoutput_extwake_pin(); 

		performance_level_switch_test();
	
		/* Check if prformance level is PL2 and GCLK0 is runing at 48MHz 
			and GCLK1 is running at 4MHz*/
		if(system_get_performance_level() != SYSTEM_PERFORMANCE_LEVEL_2
		|| system_cpu_clock_get_hz() < 47000000){
			port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);
			while(1) ;
		}

		/*Set EXTWAKE pin*/
		system_set_pin_wakeup_polarity_high(CONF_EXT_WAKEUP_PIN);
		system_enable_pin_wakeup(CONF_EXT_WAKEUP_PIN);

		/* Enable I/O retention , so that the I/O lines is retained
			When the device exits the BACKUP mode */
		system_io_retension_enable();

		/*Enter BACKUP mode*/
		system_set_sleepmode(SYSTEM_SLEEPMODE_BACKUP);
		system_sleep();
	}else{
		port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);
		while(1) ;
	}

	/*Waiting EXTWAKE PIN to HIGH to exit from BACKUP mode*/

	/* Check if the RESET is caused by exit BACKUP mode */
	if (system_get_reset_cause() == SYSTEM_RESET_CAUSE_BACKUP
		&& system_get_backup_exit_source() == SYSTEM_RESET_BACKKUP_EXIT_EXTWAKE
		&& (system_get_pin_wakeup_cause() & (CONF_EXT_WAKEUP_PIN << RSTC_WKEN_WKEN_Pos))){

		/* GCLK0 is running at 4MHz,GCLK1 is disabled due to the BACKUP reset */
			
		/* The  GCLK0 freq output pin and LED0 are still ok due to they are retained */

		system_init();
		/*  GCLK0/GCLK1 are running at 4MHz.*/
                struct system_standby_config config;
		configure_extint_channel();
		system_standby_get_config_defaults(&config);
		system_standby_set_config(&config);
		system_set_sleepmode(SYSTEM_SLEEPMODE_STANDBY);
		system_sleep();
	} else {
		port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);
		while(1) ;
	}

	/*Wait BUTTON0 pressed to exit from STANDBY mode*/
		
	/* If BUTTON0 is pressed, GCLK0/GCLK1 are running at 4MHz.
	  GCLK0 freq output pin and LED0 are still ok due to they are retained.*/
	 
	supc_battery_backup_power_switch_test();

	/*Wait automatic power switch between  VDD and VBAT power.*/
	if (system_get_reset_cause() == SYSTEM_RESET_CAUSE_BACKUP
		&& system_get_backup_exit_source() == SYSTEM_RESET_BACKKUP_EXIT_BBPS){

		/* GCLK0 is running at 4MHz,GCLK1 is disabled due to the BACKUP reset */	
		/* The  GCLK0 freq output pin and LED0 are still ok due to they are retained */
		system_init();
		performance_level_switch_test();
	} else {
		while(1);
	}
        
#endif
	/* GCLK0 is  running at 48MHz and GCLK1 is running at 4MHz.*/
	while(1){

	}
}
