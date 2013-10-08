/**
 * @file tal_ed.c
 *
 * @brief This file implements ED Scan
 *
 * $Id: tal_ed.c 35137 2013-09-20 14:47:30Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2012, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "tal_config.h"
#include "tal_internal.h"
#include "mac_build_config.h"


/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/**
 * The peak_ed_level is the maximum ED value received from the transceiver for
 * the specified Scan Duration.
 */
#if (MAC_SCAN_ED_REQUEST_CONFIRM == 1)
static int8_t max_ed_level[2];
static uint32_t sampler_counter[2];
#endif

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Starts ED Scan
 *
 * This function starts an ED Scan for the scan duration specified by the
 * MAC layer. The result is returned from the TAL by calling tal_ed_end_cb().
 *
 * @param trx_id Transceiver identifier
 * @param scan_duration Specifies the ED scan duration in superframe duration
 *
 * @return MAC_SUCCESS - ED scan duration timer started successfully
 *         TAL_BUSY - TAL is busy servicing the previous request from MAC
 *         TAL_TRX_ASLEEP - Transceiver is currently sleeping
 *         FAILURE otherwise
 */
#if (MAC_SCAN_ED_REQUEST_CONFIRM == 1) || (defined DOXYGEN)
retval_t tal_ed_start(trx_id_t trx_id, uint8_t scan_duration)
{
    uint16_t sample_duration;
    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;

    //debug_text(PSTR("tal_ed_start()"));

    /*
     * Check if the TAL is in idle state. Only in idle state it can
     * accept and ED request from the MAC.
     */
    if (tal_state[trx_id] == TAL_SLEEP)
    {
        return TAL_TRX_ASLEEP;
    }

    if (TAL_IDLE != tal_state[trx_id])
    {
        Assert("TAL is TAL_BUSY" == 0);
        return TAL_BUSY;
    }

    max_ed_level[trx_id] = -127;   // set to min value

    /* Store TRX state before entering Tx transaction */
    if ((trx_state[trx_id] == RF_RX) || (trx_state[trx_id] == RF_TXPREP))
    {
        trx_default_state[trx_id] = RF_RX;
    }

    switch_to_txprep(trx_id);

    /* Disable BB */
    pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_BBEN, 0);

    /* Setup and start energy detection, ensure AGC is not hold */
    pal_trx_bit_write(rf_reg_offset + SR_RF09_AGCC_FRZC, 0);

    /* Setup energy measurement averaging duration */
    sample_duration = ED_SAMPLE_DURATION_SYM * tal_pib[trx_id].SymbolDuration_us;
    //debug_text_val(PSTR("sample_duration = "), sample_duration);
    set_ed_sample_duration(trx_id, sample_duration);

    /* Calculate the number of samples */
    sampler_counter[trx_id] = aBaseSuperframeDuration
                              * ((1UL << scan_duration) + 1);
    sampler_counter[trx_id] = sampler_counter[trx_id] / ED_SAMPLE_DURATION_SYM;
#ifdef REDUCED_ED_SAMPLE_COUNTER // used for testing
    /* used for debugging purposes only */
    sampler_counter[trx_id] = REDUCED_ED_SAMPLE_COUNTER;
#endif
    //debug_text_val(PSTR("sampler_counter = "), (uint16_t)sampler_counter[trx_id]);

    /* Set RF to Rx */
    pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_RX);
    trx_state[trx_id] = RF_RX;

    tal_state[trx_id] = TAL_ED_SCAN;

    //debug_text(PSTR("tal_ed_start: start scan"));

    /* Start energy measurement */
    pal_trx_bit_write(rf_reg_offset + SR_RF09_EDC_EDM, RF_EDCONT);

    return MAC_SUCCESS;
}
#endif /* #if (MAC_SCAN_ED_REQUEST_CONFIRM == 1) */


/**
 * @brief Handles ED scan end interrupt
 *
 * This function handles an ED done interrupt from the transceiver.
 *
 * @param trx_id Transceiver identifier
 */
void handle_ed_end_irq(trx_id_t trx_id)
{
    //debug_text(PSTR("handle_ed_end_irq()"));

    /* Capture ED value for current frame / ED scan */
    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    tal_current_ed_val[trx_id] = pal_trx_reg_read(rf_reg_offset + RG_RF09_EDV);
    //debug_text_val(PSTR("tal_current_ed_val = "),
                  // (uint8_t)tal_current_ed_val[trx_id]);

    if (tal_state[trx_id] == TAL_CCA)
    {
        cca_done_handling(trx_id);
        return;
    }

#if (MAC_SCAN_ED_REQUEST_CONFIRM == 1)
    if (tal_state[trx_id] == TAL_ED_SCAN)
    {
        /*
         * Update the peak ED value received, if greater than the previously
         * read ED value.
         */
        if (tal_current_ed_val[trx_id] > max_ed_level[trx_id])
        {
            max_ed_level[trx_id] = tal_current_ed_val[trx_id];
        }

        sampler_counter[trx_id]--;
        //debug_text_val(PSTR("remaining sampler_counter = "),
                      // (uint16_t)sampler_counter[trx_id]);
        if (sampler_counter[trx_id] == 0)
        {
            /* Keep RF in Rx state */
            /* Stop continuous energy detection */
            pal_trx_bit_write(rf_reg_offset + SR_RF09_EDC_EDM, RF_EDAUTO);
            /* Restore ED average duration for CCA */
            set_ed_sample_duration(trx_id, tal_pib[trx_id].CCADuration_us);
            /* Switch BB on again */
            uint16_t bb_reg_offset = BB_BASE_ADDR_OFFSET * trx_id;
            pal_trx_bit_write(bb_reg_offset + SR_BBC0_PC_BBEN, 1);
            tal_state[trx_id] = TAL_IDLE;
            /* Set trx state for leaving ED scan */
            if (trx_default_state[trx_id] == RF_RX)
            {
                switch_to_rx(trx_id);
            }
            else
            {
                pal_trx_reg_write(rf_reg_offset + RG_RF09_CMD, RF_TRXOFF);
            }
            /* Scale result to 0xFF */
            uint8_t ed = max_ed_level[trx_id];//scale_ed_value(max_ed_level[trx_id]); sriram
            tal_ed_end_cb(trx_id, ed);
        }
        else
        {
            /* Wait for the next interrupt */
        }
    }
#endif /* #if (MAC_SCAN_ED_REQUEST_CONFIRM == 1) */
}


/**
 * @brief Sets the energy measurement duration
 *
 * @param trx_id Transceiver identifier
 * @param sample_duration_us Sample duration in us
 */
void set_ed_sample_duration(trx_id_t trx_id, uint16_t sample_duration_us)
{
    uint8_t dtb;
    uint8_t df;

    if ((sample_duration_us % 128) == 0)
    {
        dtb = 3;
        df = sample_duration_us / 128;
    }
    else if ((sample_duration_us % 32) == 0)
    {
        dtb = 2;
        df = sample_duration_us / 32;
    }
    else if ((sample_duration_us % 8) == 0)
    {
        dtb = 1;
        df = sample_duration_us / 8;
    }
    else
    {
        dtb = 0;
        df = sample_duration_us / 2;
    }

    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    pal_trx_reg_write(rf_reg_offset + RG_RF09_EDD, ((df << 2) | dtb));
}


/**
 * @brief Scale ED value
 *
 * This function scales the trx ED value to the range 0x00 - 0xFF.
 *
 * @param ed RF215 register value EDV.
 *
 * @return Scaled ED value
 */
uint8_t scale_ed_value(uint8_t ed)
{
    uint8_t result;
    int8_t e = (int8_t)ed;

    if (e > 0)
    {
        result = 0xFF;
    }
    else if (e == 127)
    {
        result = 0x00;
    }
    else if (e < -100)
    {
        result = 0x00;
    }
    else
    {
        float my = (e + 100) * (float)255 / (float)100;
        result = (uint8_t)my;
    }

    return result;
}


#if (MAC_SCAN_ED_REQUEST_CONFIRM == 1)
/**
 * @brief Stops ED Scan
 *
 * This function stops an ED Scan and completes it by calling tal_ed_end_cb().
 *
 * @param trx_id Transceiver identifier
 */
void stop_ed_scan(trx_id_t trx_id)
{
    /* Stop continuous energy detection */
    uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
    pal_trx_bit_write(rf_reg_offset + SR_RF09_EDC_EDM, RF_EDAUTO);
    sampler_counter[trx_id] = 0;
    /* Clear any pending ED IRQ */
    TAL_RF_IRQ_CLR(trx_id, RF_IRQ_EDC);
    handle_ed_end_irq(trx_id);
}
#endif


/* EOF */
