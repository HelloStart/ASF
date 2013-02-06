/**
 * \file
 *
 * \brief SAMD20 RTC Basic Usage Example
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

void config_rtc_calendar(void);

//! [initiate]
void config_rtc_calendar(void)
{

	/* Initialize RTC in calendar mode. */
//! [set_conf]
	struct rtc_calendar_conf config;
//! [set_conf]
//! [get_default]
	rtc_calendar_get_config_defaults(&config);
//! [get_default]

//! [time_struct]
	struct rtc_calendar_time alarm;
	rtc_calendar_get_time_defaults(&alarm);
	alarm.year      = 2013;
	alarm.month     = 1;
	alarm.day       = 1;
	alarm.hour      = 0;
	alarm.minute    = 0;
	alarm.second    = 2;
//! [time_struct]

//! [set_config]
	config.clock_24h = true;
	config.alarm[0].time = alarm;
	config.alarm[0].mask = RTC_CALENDAR_ALARM_MASK_YEAR;
//! [set_config]

//! [init_rtc]
	rtc_calendar_init(&config);
//! [init_rtc]

//! [enable]
	rtc_calendar_enable();
//! [enable]
}
//! [initiate]

int main(void)
{
//! [add_main]
	system_init();

	struct rtc_calendar_time time;
	time.year      = 2012;
	time.month     = 12;
	time.day       = 31;
	time.hour      = 22;
	time.minute    = 0;
	time.second    = 0;
	config_rtc_calendar();

	/* Set current time. */
	rtc_calendar_set_time(&time);

	rtc_calendar_swap_time_mode();

//! [add_main]
	while(1){
		/* Inf loop. */
		if (rtc_calendar_is_alarm_match(RTC_CALENDAR_ALARM_0)) {
			/* Do something */
			rtc_calendar_clear_alarm_match(RTC_CALENDAR_ALARM_0);
		}
	}

	return 0;
}
