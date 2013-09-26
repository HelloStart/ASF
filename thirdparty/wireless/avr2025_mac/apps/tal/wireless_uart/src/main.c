/**
 * @file main.c
 *
 * @brief  Main of TAL Example - Chat Application
 *
 * $Id: main.c 35141 2013-09-20 15:11:51Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2013, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include "pal.h"
#include "tal.h"
#include "app_config.h"
#include "sio_handler.h"
#include "app_common.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

#if (BOARD_TYPE == EVAL215)
#define LED_APP         LED_0
#define LED_TX_1GHZ     LED_1
#define LED_RX_1GHZ     LED_2
#define LED_TX_2GHZ     LED_3
#define LED_RX_2GHZ     LED_4
#endif
#if (BOARD_TYPE == RZ600_233_SAM4SEK)
#define LED_APP         LED_0
#define LED_TX          LED_1
#define LED_RX          LED_2
#endif
#define LED_TIME        500000UL

/* === EXTERNALS =========================================================== */

/* === GLOBALS ============================================================= */

#ifdef MULTI_TRX_SUPPORT
app_state_t app_state[2] = {APP_IDLE, APP_IDLE};
#else
app_state_t app_state[1] = {APP_IDLE};
#endif
trx_id_t current_trx_id = RF09;

/* === PROTOTYPES ========================================================== */

static void app_task(void);
static void app_init(void);
static void led_tx_off(void *parameter);
static void led_rx_off(void *parameter);
#ifdef MULTI_TRX_SUPPORT
static void switch_tx_band(trx_id_t id);
#endif

/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Main function of the Chat application
 */
int main(void)
{
    /* Initialize the TAL layer */
    if (tal_init() != MAC_SUCCESS)
    {
        // something went wrong during initialization
        pal_alert();
    }

    /* Calibrate MCU's RC oscillator */
    pal_calibrate_rc_osc();

    /* Initialize LEDs */
    pal_led_init();
    pal_led(LED_APP, LED_ON);     // indicating application is started

    /* Initialize buttons */
    pal_button_init();

    /*
     * The stack is initialized above, hence the global interrupts are enabled
     * here.
     */
    pal_global_irq_enable();

    /* Initialize the serial interface used for communication with terminal program */
    if (pal_sio_init(SIO_CHANNEL) != MAC_SUCCESS)
    {
        // something went wrong during initialization
        pal_alert();
    }

#if ((!defined __ICCAVR__) && (!defined __ICCARM__) && (!(defined __GNUC__ && defined __AVR32__))&& (!defined __GNUARM__))
    fdevopen(_sio_putchar, _sio_getchar);
#endif

    /* Setting stdin & stdout as unbuffered for ARM GCC*/
#if (defined __GNUARM__)
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
#endif

    app_init();

    print_chat_menu();

    /* Endless while loop */
    while (1)
    {
        pal_task(); /* Handle platform specific tasks, like serial interface */
        tal_task(); /* Handle transceiver specific tasks */
        app_task(); /* Application task */
    }
}


/**
 * @brief Initializes the application
 */
static void app_init(void)
{
    /* Configure the TAL PIBs; e.g. set short address */
    uint16_t pan_id = OWN_PAN_ID;
#ifdef MULTI_TRX_SUPPORT
    tal_pib_set(RF09, macPANId, (pib_value_t *)&pan_id);
    tal_pib_set(RF24, macPANId, (pib_value_t *)&pan_id);
    uint16_t addr = OWN_SHORT_ADDR;
    tal_pib_set(RF09, macShortAddress, (pib_value_t *)&addr);
    tal_pib_set(RF24, macShortAddress, (pib_value_t *)&addr);

    /* Configure PHY for sub-1GHz */
    phy_t phy;
    phy.modulation = OQPSK;
    phy.phy_mode.oqpsk.chip_rate = CHIP_RATE_1000;
    phy.phy_mode.oqpsk.rate_mode = OQPSK_RATE_MOD_0;
    phy.freq_band = US_915;
    phy.ch_spacing = OQPSK_915_CH_SPAC;
    phy.freq_f0 = OQPSK_915_F0;
    if (tal_pib_set(RF09, phySetting, (pib_value_t *)&phy) != MAC_SUCCESS)
    {
        pal_alert();
    }

    /* Configure PHY for 2.4GHz */
#if 1
    phy.modulation = OFDM;
    phy.phy_mode.ofdm.interl = true;
    phy.phy_mode.ofdm.option = OFDM_OPT_1;
    phy.phy_mode.ofdm.mcs_val = MCS3;
    phy.freq_band = WORLD_2450;
    phy.ch_spacing = OFDM_2450_OPT1_CH_SPAC;
    phy.freq_f0 = OFDM_2450_OPT1_F0;
#else
    phy.modulation = LEG_OQPSK;
    phy.phy_mode.leg_oqpsk.chip_rate = CHIP_RATE_2000;
    phy.freq_band = WORLD_2450;
    phy.ch_spacing = LEG_2450_CH_SPAC;
    phy.freq_f0 = LEG_2450_F0;
#endif
    if (tal_pib_set(RF24, phySetting, (pib_value_t *)&phy) != MAC_SUCCESS)
    {
        pal_alert();
    }

#else /* #ifdef MULTI_TRX_SUPPORT */
    tal_pib_set(macPANId, (pib_value_t *)&pan_id);
    uint16_t addr = OWN_SHORT_ADDR;
    tal_pib_set(macShortAddress, (pib_value_t *)&addr);
#endif

    /* Init tx frame info structure value that do not change during program execution */
    init_tx_frame();

    /* Switch receiver(s) on */
#ifdef MULTI_TRX_SUPPORT
    tal_rx_enable(RF09, PHY_RX_ON);
    tal_rx_enable(RF24, PHY_RX_ON);
#else
    tal_rx_enable(PHY_RX_ON);
#endif
}


/**
 * @brief Application task
 */
static void app_task(void)
{
    int input = sio_getchar_nowait();
    if (input != -1)
    {
        switch (input)
        {
#ifdef MULTI_TRX_SUPPORT
            case SUB1_CHAR:
                switch_tx_band(RF09);
                break;
            case TWO_G_CHAR:
                switch_tx_band(RF24);
                break;
#endif
            default:
                if (input != 0xFF)
                {
                    get_chat_input(input);
                }
                break;
        }
    }
#ifdef MULTI_TRX_SUPPORT
    if (pal_button_read(BUTTON_0) == BUTTON_PRESSED)
    {
        switch_tx_band(RF09);
    }
    if (pal_button_read(BUTTON_1) == BUTTON_PRESSED)
    {
        switch_tx_band(RF24);
    }
#endif
}


#ifdef MULTI_TRX_SUPPORT
/**
 * @brief Switch frequency band for transmission
 */
static void switch_tx_band(trx_id_t id)
{
    led_id_t tx_led_id, rx_led_id;
    current_trx_id = id;
    printf("\nActive transmitter frequency band: ");

    if (id == RF09)
    {
        printf("sub-1GHz\n> ");
        tx_led_id = LED_TX_1GHZ;
        rx_led_id = LED_RX_1GHZ;
    }
    else
    {
        printf("2.4GHz\n> ");
        tx_led_id = LED_TX_2GHZ;
        rx_led_id = LED_RX_2GHZ;
    }

    for (uint8_t i = 0; i < 20; i++)
    {
        pal_led(tx_led_id, LED_TOGGLE);
        pal_led(rx_led_id, LED_TOGGLE);
        pal_timer_delay(50000);
        pal_timer_delay(50000);
    }
    pal_led(tx_led_id, LED_OFF);
    pal_led(rx_led_id, LED_OFF);
}
#endif


#ifdef MULTI_TRX_SUPPORT
/**
 * @brief User call back function for frame reception
 *
 * @param trx_id   Transceiver identifier
 * @param rx_frame Pointer to received frame structure of type frame_info_t
 *                 or to received frame array
 */
void tal_rx_frame_cb(trx_id_t trx_id, frame_info_t *rx_frame)
#else
void tal_rx_frame_cb(frame_info_t *rx_frame)
#endif
{
#ifndef MULTI_TRX_SUPPORT
    trx_id_t trx_id = RF09;
#endif

    /* Indicate received frame by LED */
#if (BOARD_TYPE == EVAL215)
    if (trx_id == RF09)
    {
        pal_led(LED_RX_1GHZ, LED_ON);
    }
    else
    {
        pal_led(LED_RX_2GHZ, LED_ON);
    }
#else
    pal_led(LED_RX, LED_ON);
#endif
    pal_timer_start(T_APP_LED_RX, LED_TIME, TIMEOUT_RELATIVE,
                    (FUNC_PTR())led_rx_off, NULL);

    chat_handle_incoming_frame(trx_id, rx_frame);

    /* Free buffer of incoming frame */
    bmm_buffer_free(rx_frame->buffer_header);
}


#ifdef MULTI_TRX_SUPPORT
/**
 * @brief User call back function for frame transmission
 *
 * @param trx_id Transceiver identifier
 * @param status Status of frame transmission attempt
 * @param frame Pointer to frame structure of type frame_info_t
 */
void tal_tx_frame_done_cb(trx_id_t trx_id, retval_t status, frame_info_t *frame)
#else
void tal_tx_frame_done_cb(retval_t status, frame_info_t *frame)
#endif
{
#ifndef MULTI_TRX_SUPPORT
    trx_id_t trx_id = RF09;
#endif

    /* Indicate transmitted frame by LED */
#if (BOARD_TYPE == EVAL215)
    if (trx_id == RF09)
    {
        pal_led(LED_TX_1GHZ, LED_ON);
    }
    else
    {
        pal_led(LED_TX_2GHZ, LED_ON);
    }
#else
    pal_led(LED_TX, LED_ON);
#endif
    pal_timer_start(T_APP_LED_TX, LED_TIME, TIMEOUT_RELATIVE,
                    (FUNC_PTR())led_tx_off, NULL);

    chat_tx_done_cb(trx_id, status, frame);
}


/**
 * @brief User call back function for finished ED Scan
 */
#ifdef MULTI_TRX_SUPPORT
void tal_ed_end_cb(trx_id_t trx_id, uint8_t energy_level)
#else
void tal_ed_end_cb(uint8_t energy_level)
#endif
{
    /* Keep compiler happy */
#ifdef MULTI_TRX_SUPPORT
    trx_id = trx_id;
#endif
    energy_level = energy_level;
}


/**
 * @brief User call back function to switch RX LEDs off
 */
static void led_rx_off(void *parameter)
{
#if (BOARD_TYPE == EVAL215)
    pal_led(LED_RX_1GHZ, LED_OFF);
    pal_led(LED_RX_2GHZ, LED_OFF);
#else
    pal_led(LED_RX, LED_OFF);
#endif
    /* Keep compiler happy. */
    parameter = parameter;
}


/**
 * @brief User call back function to switch TX LEDs off
 */
static void led_tx_off(void *parameter)
{
#if (BOARD_TYPE == EVAL215)
    pal_led(LED_TX_1GHZ, LED_OFF);
    pal_led(LED_TX_2GHZ, LED_OFF);
#else
    pal_led(LED_TX, LED_OFF);
#endif
    /* Keep compiler happy. */
    parameter = parameter;
}



/**
 * @brief Get text for return value
 *
 * @param ret return value
 */
char *get_retval_text(retval_t ret)
{
    char *text;

    switch (ret)
    {
        case MAC_SUCCESS:
            text = "MAC_SUCCESS";
            break;
        case TAL_TRX_ASLEEP:
            text = "TAL_TRX_ASLEEP";
            break;
        case TAL_TRX_AWAKE:
            text = "TAL_TRX_AWAKE";
            break;
        case FAILURE:
            text = "FAILURE";
            break;
        case TAL_BUSY:
            text = "TAL_BUSY";
            break;
        case TAL_FRAME_PENDING:
            text = "TAL_FRAME_PENDING";
            break;
        case MAC_CHANNEL_ACCESS_FAILURE:
            text = "MAC_CHANNEL_ACCESS_FAILURE";
            break;
        case MAC_DISABLE_TRX_FAILURE:
            text = "MAC_DISABLE_TRX_FAILURE";
            break;
        case MAC_FRAME_TOO_LONG:
            text = "MAC_FRAME_TOO_LONG";
            break;
        case MAC_INVALID_PARAMETER:
            text = "MAC_INVALID_PARAMETER";
            break;
        case MAC_NO_ACK:
            text = "MAC_NO_ACK";
            break;
        case MAC_UNSUPPORTED_ATTRIBUTE:
            text = "MAC_UNSUPPORTED_ATTRIBUTE";
            break;
        case MAC_READ_ONLY:
            text = "MAC_READ_ONLY";
            break;
        default:
            text = "unknown retval";
            break;
    }
    return text;
}


/**
 * @brief Get text for TAL type
 *
 * @param tal_type TAL type
 */
char *get_tal_type_text(uint8_t tal_type)
{
    char *text;

    switch (tal_type)
    {
#ifdef ATMEGARFA1
        case ATMEGARFA1:
            text = "ATMEGARFA1";
            break;
#endif
#ifdef ATMEGARFR2
        case ATMEGARFR2:
            text = "ATMEGARFR2";
            break;
#endif
#ifdef AT86RF231
        case AT86RF231:
            text = "AT86RF231";
            break;
#endif
#ifdef AT86RF233
        case AT86RF233:
            text = "AT86RF233";
            break;
#endif
#ifdef AT86RF215
        case AT86RF215:
            text = "AT86RF215";
            break;
#endif
        default:
            text = "unknown TAL_TYPE";
            break;
    }

    return text;
}


/**
 * @brief Get text for PAL type
 *
 * @param pal_type PAL type
 */
char *get_pal_type_text(uint8_t pal_type)
{
    char *text;

    switch (pal_type)
    {
#ifdef ATMEGA128RFA1
        case ATMEGA128RFA1:
            text = "ATMEGA128RFA1";
            break;
#endif
#ifdef ATMEGA256RFR2
        case ATMEGA256RFR2:
            text = "ATMEGA256RFR2";
            break;
#endif
#ifdef AT91SAM4S16C
        case AT91SAM4S16C:
            text = "AT91SAM4S16C";
            break;
#endif
#ifdef AT91SAM4S16B
        case AT91SAM4S16B:
            text = "AT91SAM4S16B";
            break;
#endif
#ifdef ATXMEGA256A3
        case ATXMEGA256A3:
            text = "ATXMEGA256A3";
            break;
#endif
        default:
            text = "unknown PAL type";
            break;
    }

    return text;
}


/**
 * @brief Get text for board type
 *
 * @param board Board type
 */
char *get_board_text(uint8_t board)
{
    char *text;

    switch (board)
    {
#ifdef RCB_6_3_SENS_TERM_BOARD
        case RCB_6_3_SENS_TERM_BOARD:
            text = "RCB_6_3_SENS_TERM_BOARD";
            break;
#endif
#ifdef RCB_6_3_2_SENS_TERM_BOARD
        case RCB_6_3_2_SENS_TERM_BOARD:
            text = "RCB_6_3_2_SENS_TERM_BOARD";
            break;
#endif
#ifdef RZ600_231_SAM4SEK
        case RZ600_231_SAM4SEK:
            text = "RZ600_231_SAM4SEK";
            break;
#endif
#ifdef RZ600_233_SAM4SEK
        case RZ600_233_SAM4SEK:
            text = "RZ600_233_SAM4SEK";
            break;
#endif
#ifdef REB_8_1_CBB
        case REB_8_1_CBB:
            text = "REB_8_1_CBB";
            break;
#endif
#ifdef RF233_SAM4_BAB
        case RF233_SAM4_BAB:
            text = "RF233_SAM4_BAB";
            break;
#endif
#ifdef RF233_SAM4_BAB_NATIVE_USB
        case RF233_SAM4_BAB_NATIVE_USB:
            text = "RF233_SAM4_BAB_NATIVE_USB";
            break;
#endif
#ifdef EVAL215
        case EVAL215:
            text = "EVAL215";
            break;
#endif
        default:
            text = "unknown board";
            break;
    }

    return text;
}


#ifdef MULTI_TRX_SUPPORT
/**
 * @brief Get text for transceiver identifier
 *
 * @param id Transceiver identifier
 */
char *get_trx_id_text(trx_id_t id)
{
    char *text;

    if (id == RF09)
    {
        text = "RF09";
    }
    else if (id == RF24)
    {
        text = "RF24";
    }
    else
    {
        text = "unknown trx id";
    }

    return text;
}
#endif


/* EOF */
