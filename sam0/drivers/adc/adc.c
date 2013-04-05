/**
 * \file
 *
 * \brief SAM D20 Peripheral Analog-to-Digital Converter Driver
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

#include "adc.h"


/**
* \internal Configure MUX settings for the analog pins
*
* This function will set the given ADC input pins
* to the analog function in the pin mux, giving
* the ADC access to the analog signal
*
* \param [in] pin pin number to configure
*/
static inline void _adc_configure_ain_pin(uint32_t pin)
{
	struct system_pinmux_config config;
	system_pinmux_get_config_defaults(&config);
	config.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;

	/* Analog functions are at mux setting 7 */
	config.mux_position = 7;
	/* Pins above Pin23 are internal signals */
	if (pin <= ADC_INPUTCTRL_MUXPOS_PIN23) {
		if (pin < ADC_INPUTCTRL_MUXPOS_PIN8) {
			/* PORT A */
			system_pinmux_pin_set_config(pin, &config);
		} else {
			/* PORT B */
			system_pinmux_pin_set_config(pin + 32, &config);
		}
	}
}

/**
 * \internal Writes an ADC configuration to the hardware module
 *
 * Writes out a given ADC module configuration to the hardware module.
 *
 * \param[out] module_inst  Pointer to the ADC software instance struct
 * \param[in]  config       Pointer to configuration struct
 *
 * \return Status of the configuration procedure
 * \retval STATUS_OK               The configuration was successful
 * \retval STATUS_ERR_INVALID_ARG  Invalid argument(s) were provided
 */
static enum status_code _adc_set_config(
		struct adc_module *const module_inst,
		struct adc_config *const config)
{
	uint8_t adjres;
	enum adc_average_samples average;
	struct system_gclk_chan_config gclk_chan_conf;

	/* Get the hardware module pointer */
	Adc *const adc_module = module_inst->hw;

	/* Configure GCLK channel and enable clock */
	gclk_chan_conf.source_generator = config->clock_source;

	/* Set the GCLK channel to run in standby mode */
	gclk_chan_conf.run_in_standby = config->run_in_standby;

	/* Apply configuration and enable the GCLK channel */
	system_gclk_chan_set_config(ADC_GCLK_ID, &gclk_chan_conf);
	system_gclk_chan_enable(ADC_GCLK_ID);

	/* Configure analog input pins */
	_adc_configure_ain_pin(config->positive_input);
	_adc_configure_ain_pin(config->negative_input);

	/* Configure run in standby */
	adc_module->CTRLA.reg = (config->run_in_standby << ADC_CTRLA_RUNSTDBY_Pos);

	/* Configure reference */
	adc_module->REFCTRL.reg =
			(config->reference_compensation_enable << ADC_REFCTRL_REFCOMP_Pos) |
			(config->reference);

	/* Set adjusting result and number of samples */
	switch (config->oversampling_and_decimation) {

	case ADC_OVERSAMPLING_AND_DECIMATION_DISABLE:
		adjres = 0x00;
		average = config->average_samples;
		break;

	case ADC_OVERSAMPLING_AND_DECIMATION_1BIT:
		adjres = 0x01;
		average = ADC_AVGCTRL_SAMPLENUM_4;
		break;

	case ADC_OVERSAMPLING_AND_DECIMATION_2BIT:
		adjres = 0x02;
		average = ADC_AVGCTRL_SAMPLENUM_16;
		break;

	case ADC_OVERSAMPLING_AND_DECIMATION_3BIT:
		adjres = 0x01;
		average = ADC_AVGCTRL_SAMPLENUM_64;
		break;

	case ADC_OVERSAMPLING_AND_DECIMATION_4BIT:
		adjres = 0x00;
		average = ADC_AVGCTRL_SAMPLENUM_256;
		break;

	default:
		/* Unknown. Abort. */
		return STATUS_ERR_INVALID_ARG;
	}

	adc_module->AVGCTRL.reg = ADC_AVGCTRL_ADJRES(adjres) | average;

	/* Check validity of sample length value */
	if (config->sample_length > 63) {
		return STATUS_ERR_INVALID_ARG;
	} else {
		/* Configure sample length */
		adc_module->SAMPCTRL.reg = (config->sample_length << ADC_SAMPCTRL_SAMPLEN_Pos);
	}

	while (adc_is_syncing(module_inst)) {
		/* Wait for synchronization */
	}

	/* Configure CTRLB */
	adc_module->CTRLB.reg =
			config->clock_prescaler |
			config->resolution |
			(config->correction.correction_enable << ADC_CTRLB_CORREN_Pos) |
			(config->freerunning << ADC_CTRLB_FREERUN_Pos) |
			(config->left_adjust << ADC_CTRLB_LEFTADJ_Pos) |
			(config->differential_mode << ADC_CTRLB_DIFFMODE_Pos);

	/* Check validity of window thresholds */
	if (config->window.window_mode != ADC_WINDOW_MODE_DISABLE) {
		switch (config->resolution) {
		case ADC_RESOLUTION_8BIT:
			if (config->differential_mode &&
					(config->window.window_lower_value > 127 ||
					config->window.window_lower_value < -128 ||
					config->window.window_upper_value > 127 ||
					config->window.window_upper_value < -128)) {
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			} else if (config->window.window_lower_value > 255 ||
					config->window.window_upper_value > 255){
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			}
			break;
		case ADC_RESOLUTION_10BIT:
			if (config->differential_mode &&
					(config->window.window_lower_value > 511 ||
					config->window.window_lower_value < -512 ||
					config->window.window_upper_value > 511 ||
					config->window.window_upper_value > -512)) {
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			} else if (config->window.window_lower_value > 1023 ||
					config->window.window_upper_value > 1023){
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			}
			break;
		case ADC_RESOLUTION_12BIT:
			if (config->differential_mode &&
					(config->window.window_lower_value > 2047 ||
					config->window.window_lower_value < -2048 ||
					config->window.window_upper_value > 2047 ||
					config->window.window_upper_value < -2048)) {
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			} else if (config->window.window_lower_value > 4095 ||
					config->window.window_upper_value > 4095){
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			}
			break;
		case ADC_RESOLUTION_16BIT:
			if (config->differential_mode && (config->window.window_lower_value > 32767 ||
					config->window.window_lower_value < -32768 ||
					config->window.window_upper_value > 32767 ||
					config->window.window_upper_value < -32768)) {
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			} else if (config->window.window_lower_value > 65535 ||
					config->window.window_upper_value > 65535){
				/* Invalid value */
				return STATUS_ERR_INVALID_ARG;
			}
			break;
		}
	}

	while (adc_is_syncing(module_inst)) {
		/* Wait for synchronization */
	}

	/* Configure window mode */
	adc_module->WINCTRL.reg = config->window.window_mode;

	while (adc_is_syncing(module_inst)) {
		/* Wait for synchronization */
	}

	/* Configure lower threshold */
	adc_module->WINLT.reg = config->window.window_lower_value << ADC_WINLT_WINLT_Pos;

	while (adc_is_syncing(module_inst)) {
		/* Wait for synchronization */
	}

	/* Configure lower threshold */
	adc_module->WINUT.reg = config->window.window_upper_value << ADC_WINUT_WINUT_Pos;

	uint8_t inputs_to_scan = config->pin_scan.inputs_to_scan;
	if (inputs_to_scan > 0) {
		/*
		* Number of input sources included is the value written to INPUTSCAN
		* plus 1.
		*/
		inputs_to_scan--;
	}
	if (inputs_to_scan > ADC_INPUTCTRL_INPUTSCAN_Msk ||
			config->pin_scan.offset_start_scan > ADC_INPUTCTRL_INPUTOFFSET_Msk) {
		/* Invalid number of input pins or input offset */
		return STATUS_ERR_INVALID_ARG;
	}

	while (adc_is_syncing(module_inst)) {
		/* Wait for synchronization */
	}

	/* Configure pin scan mode and positive and negative input pins */
	adc_module->INPUTCTRL.reg =
			config->gain_factor |
			(config->pin_scan.offset_start_scan << ADC_INPUTCTRL_INPUTOFFSET_Pos) |
			(inputs_to_scan << ADC_INPUTCTRL_INPUTSCAN_Pos) |
			config->negative_input |
			config->positive_input;

	/* Configure events */
	adc_module->EVCTRL.reg =
			config->event.event_action |
			(config->event.generate_event_on_window_monitor  << ADC_EVCTRL_WINMONEO_Pos) |
			(config->event.generate_event_on_conversion_done << ADC_EVCTRL_RESRDYEO_Pos);


	/* Disable all interrupts */
	adc_module->INTENCLR.reg =
			(1 << ADC_INTENCLR_READY_Pos)   | (1 << ADC_INTENCLR_WINMON_Pos) |
			(1 << ADC_INTENCLR_OVERRUN_Pos) | (1 << ADC_INTENCLR_RESRDY_Pos);

	if (config->correction.correction_enable){
		/* Make sure gain_correction value is valid */
		if (config->correction.gain_correction > ADC_GAINCORR_GAINCORR_Msk) {
			return STATUS_ERR_INVALID_ARG;
		} else {
			/* Set gain correction value */
			adc_module->GAINCORR.reg = config->correction.gain_correction <<
					ADC_GAINCORR_GAINCORR_Pos;
		}

		/* Make sure offset correction value is valid */
		if (config->correction.offset_correction > 2047 ||
				config->correction.offset_correction < -2048) {
			return STATUS_ERR_INVALID_ARG;
		} else {
			/* Set offset correction value */
			adc_module->OFFSETCORR.reg = config->correction.offset_correction <<
					ADC_OFFSETCORR_OFFSETCORR_Pos;
		}
	}

	return STATUS_OK;
}

/**
 * \brief Initializes the ADC
 *
 * Initializes the ADC device struct and the hardware module based on the
 * given configuration struct values.
 *
 * \param[out] module_inst Pointer to the ADC software instance struct
 * \param[in]  module      Pointer to the ADC module instance
 * \param[in]  config      Pointer to the configuration struct
 *
 * \return Status of the initialization procedure
 * \retval STATUS_OK                The initialization was successful
 * \retval STATUS_ERR_INVALID_ARG   Invalid argument(s) were provided
 * \retval STATUS_BUSY          The module is busy with a reset operation
 * \retval STATUS_ERR_DENIED        The module is enabled
 */
enum status_code adc_init(
		struct adc_module *const module_inst,
		Adc *module,
		struct adc_config *config)
{
	/* Associate the software module instance with the hardware module */
	module_inst->hw = module;

	if (module->CTRLA.reg & ADC_CTRLA_SWRST) {
		/* We are in the middle of a reset. Abort. */
		return STATUS_BUSY;
	}

	if (module->CTRLA.reg & ADC_CTRLA_ENABLE) {
		/* Module must be disabled before initialization. Abort. */
		return STATUS_ERR_DENIED;
	}
#if ADC_CALLBACK_MODE == true
	for (uint8_t i = 0; i < ADC_CALLBACK_N; i++) {
		module_inst->callback[i] = NULL;
	};

	module_inst->registered_callback_mask = 0;
	module_inst->enabled_callback_mask = 0;
	module_inst->remaining_conversions = 0;
	module_inst->job_status = STATUS_OK;

	_adc_instances[0] = module_inst;

	if(config->event.event_action == ADC_EVENT_ACTION_DISABLED) {
		module_inst->software_trigger = true;
	} else {
		module_inst->software_trigger = false;
	}
#endif

	/* Write configuration to module */
	return _adc_set_config(module_inst, config);;
}
