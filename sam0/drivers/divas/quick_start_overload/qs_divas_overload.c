/**
 * \file
 *
 * \brief SAM DIVAS Driver Quick Start
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
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
 
#include <asf.h>

//! [buffer]
#define BUF_LEN 8

const int32_t numerator_s[BUF_LEN] = {
	2046, 415, 26, 1, -1, -255, -3798, -65535};

const int32_t excepted_s[BUF_LEN] = {
	2046, 207, 8, 0, 0, -42, -542, -8191};

const int32_t excepted_s_m[BUF_LEN] = {
	0, 1, 2, 1, -1, -3, -4, -7};

const uint32_t numerator_u[BUF_LEN] = {
	0x00000001,
	0x0000005A,
	0x000007AB,
	0x00006ABC, 
	0x0004567D,
	0x0093846E, 
	0x20781945, 
	0x7FFFFFFF,
};

const uint32_t excepted_u[BUF_LEN] = {
	0x00000001,
	0x0000002d,
	0x0000028E,
	0x00001AAF,
	0x0000DE19,
	0x00189612,
	0x04A37153,
	0x0FFFFFFF, 
};

const uint32_t excepted_u_m[BUF_LEN] = {
	0, 0, 1, 0, 0, 2, 0, 7};

const uint32_t excepted_r[BUF_LEN] = {
	0x00000001,
	0x00000009,
	0x0000002C,
	0x000000A5,
	0x00000215,
	0x00000C25,
	0x00005B2B,
	0x0000B504,
};

static int32_t result_s[BUF_LEN], result_s_m[BUF_LEN];
static uint32_t result_u[BUF_LEN], result_u_m[BUF_LEN];
static uint32_t result_r[BUF_LEN];
static uint8_t result = 0;
//! [buffer]

//! [calculate]
static void signed_division(void)
{
	int32_t numerator, denominator;
	uint8_t i;
	
	for (i = 0; i < BUF_LEN; i++) {
		numerator = numerator_s[i];
		denominator = i + 1;
		result_s[i] = numerator / denominator;
		if(result_s[i] != excepted_s[i]) {
			result |= 0x01;
		}
	}
}

static void unsigned_division(void)
{
	uint32_t numerator, denominator;
	uint8_t i;
	
	for (i = 0; i < BUF_LEN; i++) {
		numerator = numerator_u[i];
		denominator = i + 1;
		result_u[i] = numerator / denominator;
		if(result_u[i] != excepted_u[i]) {
			result |= 0x02;
		}
	}
}

static void signed_division_mod(void)
{
	int32_t numerator, denominator;
	uint8_t i;
	
	for (i = 0; i < BUF_LEN; i++) {
		numerator = numerator_s[i];
		denominator = i + 1;
		result_s_m[i] = numerator % denominator;
		if(result_s_m[i] != excepted_s_m[i]) {
			result |= 0x04;
		}
	}
}

static void unsigned_division_mod(void)
{
	uint32_t numerator, denominator;
	uint8_t i;
	
	for (i = 0; i < BUF_LEN; i++) {
		numerator = numerator_u[i];
		denominator = i + 1;
		result_u_m[i] = numerator % denominator;
		if(result_u_m[i] != excepted_u_m[i]) {
			result |= 0x08;
		}
	}
}

static void squart_root(void)
{
	uint32_t operator;
	uint8_t i;
	
	for (i = 0; i < BUF_LEN; i++) {
		operator = numerator_u[i];
		result_r[i] = divas_sqrt(operator);
		if(result_r[i] != excepted_r[i]) {
			result |= 0x10;
		}
	}
}
//! [calculate]

int main(void)
{
	//! [setup_init]
	system_init();
	//! [setup_init]
	
	//! [main]
	//! [main_1]
	signed_division();
	//! [main_1]
	//! [main_2]
	unsigned_division();
	//! [main_2]
	//! [main_3]
	signed_division_mod();
	//! [main_3]
	//! [main_4]
	unsigned_division_mod();
	//! [main_4]
	//! [main_5]
	squart_root();
	//! [main_5]
	
	//! [main_6]
	while (true) {
		if(result) {
			port_pin_toggle_output_level(LED_0_PIN);
			/* Add a short delay to see LED toggle */
			volatile uint32_t delay = 50000;
			while(delay--) {
			}
		} else {
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
		}
	}
	//! [main_6]
	//! [main]
}
