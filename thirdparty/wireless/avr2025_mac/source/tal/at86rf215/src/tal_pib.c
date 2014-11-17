/**
 * @file tal_pib.c
 *
 * @brief This file handles the TAL PIB attributes, set/get and initialization
 *
 * $Id: tal_pib.c 36439 2014-09-01 14:13:56Z uwalter $
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

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "tal_config.h"
#include "tal_pib.h"
#include "tal_internal.h"
#include "ieee_154g.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

static retval_t apply_channel_settings(trx_id_t trx_id);
#ifndef REDUCED_PARAM_CHECK
static retval_t check_valid_freq_range(trx_id_t trx_id);
#endif
static void set_tx_pwr(trx_id_t trx_id, int8_t tx_pwr);
static retval_t set_channel(trx_id_t trx_id, uint16_t ch);
static retval_t set_phy_based_on_channel_page(trx_id_t trx_id, ch_pg_t pg);
static ch_pg_t calc_ch_page(trx_id_t trx_id);

/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Initialize the TAL PIB
 *
 * This function initializes the TAL information base attributes
 * to their default values.
 *
 * @param trx_id Transceiver identifier
 */
void init_tal_pib(trx_id_t trx_id)
{
    debug_text(PSTR("init_tal_pib()"));

    tal_pib[trx_id].PANId = TAL_PANID_BC_DEF;
    tal_pib[trx_id].ShortAddress = TAL_SHORT_ADDRESS_DEF;

    if (trx_id == RF09)
    {
        tal_pib[RF09].MaxCSMABackoffs = TAL_RF09_MAX_CSMA_BACKOFFS_DEF;
        tal_pib[RF09].MinBE = TAL_RF09_MINBE_DEF;
        tal_pib[RF09].MaxBE = TAL_RF09_MAXBE_DEF;
        tal_pib[RF09].MaxFrameRetries = TAL_RF09_MAXFRAMERETRIES_DEF;
#ifdef RX_WHILE_BACKOFF
        tal_pib[RF09].MaxNumRxFramesDuringBackoff = TAL_RF09_MAX_FRAMES_DURING_BACKOFF_DEF;
#endif
        tal_pib[RF09].PrivatePanCoordinator = TAL_RF09_PAN_COORDINATOR_DEF;
        tal_pib[RF09].FCSType = TAL_RF09_FCS_TYPE_DEFAULT;
#if TAL_RF09_MODULATION_DEF == OFDM_DEF
        tal_pib[RF09].phy.freq_band = TAL_RF09_OFDM_FRQ_BAND_DEF;
        tal_pib[RF09].phy.freq_f0 = TAL_RF09_OFDM_FRQ_F0_DEF;
        tal_pib[RF09].phy.ch_spacing = TAL_RF09_OFDM_CH_SPACING_DEF;
        tal_pib[RF09].phy.modulation = OFDM;
        tal_pib[RF09].phy.phy_mode.ofdm.option = TAL_RF09_OFDM_OPT_DEF;
        tal_pib[RF09].CurrentChannel = TAL_RF09_OFDM_CURRENT_CHANNEL_DEF;
#elif TAL_RF09_MODULATION_DEF == FSK_DEF
        tal_pib[RF09].phy.freq_band = TAL_RF09_FSK_FRQ_BAND_DEF;
        tal_pib[RF09].phy.freq_f0 = TAL_RF09_FSK_FRQ_F0_DEF;
        tal_pib[RF09].phy.ch_spacing = TAL_RF09_FSK_CH_SPACING_DEF;
        tal_pib[RF09].phy.modulation = FSK;
        tal_pib[RF09].phy.phy_mode.fsk.data_rate = TAL_RF09_FSK_DATA_RATE_DEF;
        tal_pib[RF09].phy.phy_mode.fsk.mod_idx = TAL_RF09_FSK_MOD_IDX_DEF;
        tal_pib[RF09].phy.phy_mode.fsk.mod_type = TAL_RF09_FSK_TYPE_DEF;
        tal_pib[RF09].phy.phy_mode.fsk.op_mode = TAL_RF09_FSK_OP_DEF;
        tal_pib[RF09].CurrentChannel = TAL_RF09_FSK_CURRENT_CHANNEL_DEF;
#elif TAL_RF09_MODULATION_DEF == OQPSK_DEF
        tal_pib[RF09].phy.freq_band = TAL_RF09_OQPSK_FRQ_BAND_DEF;
        tal_pib[RF09].phy.freq_f0 = TAL_RF09_OQPSK_FRQ_F0_DEF;
        tal_pib[RF09].phy.ch_spacing = TAL_RF09_OQPSK_CH_SPACING_DEF;
        tal_pib[RF09].phy.modulation = OQPSK;
        tal_pib[RF09].phy.phy_mode.oqpsk.chip_rate = (oqpsk_chip_rate_t)TAL_RF09_OQPSK_RATE;
        tal_pib[RF09].CurrentChannel = TAL_RF09_OQPSK_CURRENT_CHANNEL_DEF;
#elif TAL_RF09_MODULATION_DEF == LEG_OQPSK_DEF
        tal_pib[RF09].phy.freq_band = TAL_RF09_LEG_OQPSK_FRQ_BAND_DEF;
        tal_pib[RF09].phy.freq_f0 = TAL_RF09_LEG_OQPSK_FRQ_F0_DEF;
        tal_pib[RF09].phy.ch_spacing = TAL_RF09_LEG_OQPSK_CH_SPACING_DEF;
        tal_pib[RF09].phy.modulation = LEG_OQPSK;
        tal_pib[RF09].phy.phy_mode.oqpsk.chip_rate = TAL_RF09_LEG_OQPSK_RATE;
        tal_pib[RF09].CurrentChannel = TAL_RF09_LEG_OQPSK_CURRENT_CHANNEL_DEF;
#endif
#ifdef SUPPORT_ACK_RATE_MODE_ADAPTION
        tal_pib[RF09].AdaptDataRateForACK = TAL_RF09_ADAPT_DATA_RATE_FOR_ACK;
#endif
#ifdef SUPPORT_FRAME_FILTER_CONFIGURATION
        tal_pib[RF09].frame_types = TAL_RF09_FRAMES_TYPES;
        tal_pib[RF09].frame_versions = TAL_RF09_FRAME_VERSIONS;
#endif
#ifdef SUPPORT_OQPSK
        tal_pib[RF09].OQPSKRateMode = TAL_RF09_OQPSK_RATE;
#endif
#ifdef SUPPORT_OFDM
        tal_pib[RF09].OFDMMCS = TAL_RF09_OFDM_MCS_VAL_DEF;
        tal_pib[RF09].OFDMInterleaving = TAL_RF09_OFDM_INTERL_DEF;
#endif
#ifdef SUPPORT_FSK
        tal_pib[RF09].FSKFECEnabled = TAL_RF09_FSK_FEC_ENABLE;
        tal_pib[RF09].FSKFECInterleavingRSC = TAL_RF09_FSK_FEC_INTERLEAVING_RSC;
        tal_pib[RF09].FSKFECScheme = TAL_RF09_FSK_FEC_SCHEME;
        tal_pib[RF09].FSKPreambleLength = TAL_RF09_FSK_PREAMBLE_LEN;
        tal_pib[RF09].MRFSKSFD = TAL_RF09_FSK_SFD;
        tal_pib[RF09].FSKScramblePSDU = TAL_RF09_FSK_SCRAMBLE_PSDU;
#ifdef SUPPORT_MODE_SWITCH
        tal_pib[RF09].ModeSwitchEnabled = TAL_RF09_MODE_SWITCH_ENABLE;
        tal_pib[RF09].ModeSwitchSettlingDelay = TAL_RF09_MODE_SWITCH_SETTLING_DELAY;
        tal_pib[RF09].ModeSwitchNewMode.modulation = TAL_RF09_NEW_MODE_MODULATION;
        tal_pib[RF09].ModeSwitchNewMode.phy_mode.ofdm.option = TAL_RF09_NEW_MODE_OPTION;
        tal_pib[RF09].ModeSwitchNewMode.rate.ofdm_mcs = (ofdm_mcs_t)TAL_RF09_NEW_MODE_OPTION;
        tal_pib[RF09].ModeSwitchNewMode.fec_enabled = TAL_RF09_NEW_MODE_FEC_ENABLED;
        tal_pib[RF09].ModeSwitchDuration = TAL_RF09_MODE_SWITCH_DURATION;
#endif
#ifdef SUPPORT_FSK_RAW_MODE
        tal_pib[RF09].FSKRawModeEnabled = TAL_RF09_FSK_RAW_MODE_ENABLED;
#endif
#endif /* #ifdef SUPPORT_FSK */
    }
    else    // RF24
    {
        tal_pib[RF24].MaxCSMABackoffs = TAL_RF24_MAX_CSMA_BACKOFFS_DEF;
        tal_pib[RF24].MinBE = TAL_RF24_MINBE_DEF;
        tal_pib[RF24].MaxBE = TAL_RF24_MAXBE_DEF;
        tal_pib[RF24].MaxFrameRetries = TAL_RF24_MAXFRAMERETRIES_DEF;
#ifdef RX_WHILE_BACKOFF
        tal_pib[RF24].MaxNumRxFramesDuringBackoff = TAL_RF24_MAX_FRAMES_DURING_BACKOFF_DEF;
#endif
        tal_pib[RF24].PrivatePanCoordinator = TAL_RF24_PAN_COORDINATOR_DEF;
        tal_pib[RF24].FCSType = TAL_RF24_FCS_TYPE_DEFAULT;
#if TAL_RF24_MODULATION_DEF == OFDM_DEF
        tal_pib[RF24].phy.freq_band = TAL_RF24_OFDM_FRQ_BAND_DEF;
        tal_pib[RF24].phy.freq_f0 = TAL_RF24_OFDM_FRQ_F0_DEF;
        tal_pib[RF24].phy.ch_spacing = TAL_RF24_OFDM_CH_SPACING_DEF;
        tal_pib[RF24].phy.modulation = OFDM;
        tal_pib[RF24].phy.phy_mode.ofdm.option = TAL_RF24_OFDM_OPT_DEF;
        tal_pib[RF24].CurrentChannel = TAL_RF24_OFDM_CURRENT_CHANNEL_DEF;
#elif TAL_RF24_MODULATION_DEF == FSK_DEF
        tal_pib[RF24].phy.freq_band = TAL_RF24_FSK_FRQ_BAND_DEF;
        tal_pib[RF24].phy.freq_f0 = TAL_RF24_FSK_FRQ_F0_DEF;
        tal_pib[RF24].phy.ch_spacing = TAL_RF24_FSK_CH_SPACING_DEF;
        tal_pib[RF24].phy.modulation = FSK;
        tal_pib[RF24].phy.phy_mode.fsk.data_rate = TAL_RF24_FSK_DATA_RATE_DEF;
        tal_pib[RF24].phy.phy_mode.fsk.mod_idx = TAL_RF24_FSK_MOD_IDX_DEF;
        tal_pib[RF24].phy.phy_mode.fsk.mod_type = TAL_RF24_FSK_TYPE_DEF;
        tal_pib[RF24].phy.phy_mode.fsk.op_mode = TAL_RF24_FSK_OP_DEF;
        tal_pib[RF24].CurrentChannel = TAL_RF24_FSK_CURRENT_CHANNEL_DEF;
#elif TAL_RF24_MODULATION_DEF == OQPSK_DEF
        tal_pib[RF24].phy.freq_band = TAL_RF24_OQPSK_FRQ_BAND_DEF;
        tal_pib[RF24].phy.freq_f0 = TAL_RF24_OQPSK_FRQ_F0_DEF;
        tal_pib[RF24].phy.ch_spacing = TAL_RF24_OQPSK_CH_SPACING_DEF;
        tal_pib[RF24].phy.modulation = OQPSK;
        tal_pib[RF24].phy.phy_mode.oqpsk.chip_rate = (oqpsk_chip_rate_t)TAL_RF24_OQPSK_RATE;
        tal_pib[RF24].CurrentChannel = TAL_RF24_OQPSK_CURRENT_CHANNEL_DEF;
#elif TAL_RF24_MODULATION_DEF == LEG_OQPSK_DEF
        tal_pib[RF24].phy.freq_band = TAL_RF24_LEG_OQPSK_FRQ_BAND_DEF;
        tal_pib[RF24].phy.freq_f0 = TAL_RF24_LEG_OQPSK_FRQ_F0_DEF;
        tal_pib[RF24].phy.ch_spacing = TAL_RF24_LEG_OQPSK_CH_SPACING_DEF;
        tal_pib[RF24].phy.modulation = LEG_OQPSK;
        tal_pib[RF24].phy.phy_mode.oqpsk.chip_rate = TAL_RF24_LEG_OQPSK_RATE;
        tal_pib[RF24].CurrentChannel = TAL_RF24_LEG_OQPSK_CURRENT_CHANNEL_DEF;
#endif
#ifdef SUPPORT_ACK_RATE_MODE_ADAPTION
        tal_pib[RF24].AdaptDataRateForACK = TAL_RF24_ADAPT_DATA_RATE_FOR_ACK;
#endif
#ifdef SUPPORT_FRAME_FILTER_CONFIGURATION
        tal_pib[RF24].frame_types = TAL_RF24_FRAMES_TYPES;
        tal_pib[RF24].frame_versions = TAL_RF24_FRAME_VERSIONS;
#endif
#ifdef SUPPORT_OQPSK
        tal_pib[RF24].OQPSKRateMode = TAL_RF24_OQPSK_RATE;
#endif
#ifdef SUPPORT_OFDM
        tal_pib[RF24].OFDMMCS = TAL_RF24_OFDM_MCS_VAL_DEF;
        tal_pib[RF24].OFDMInterleaving = TAL_RF24_OFDM_INTERL_DEF;
#endif
#ifdef SUPPORT_FSK
        tal_pib[RF24].FSKFECEnabled = TAL_RF24_FSK_FEC_ENABLE;
        tal_pib[RF24].FSKFECInterleavingRSC = TAL_RF24_FSK_FEC_INTERLEAVING_RSC;
        tal_pib[RF24].FSKFECScheme = TAL_RF24_FSK_FEC_SCHEME;
        tal_pib[RF24].FSKPreambleLength = TAL_RF24_FSK_PREAMBLE_LEN;
        tal_pib[RF24].MRFSKSFD = TAL_RF24_FSK_SFD;
        tal_pib[RF24].FSKScramblePSDU = TAL_RF24_FSK_SCRAMBLE_PSDU;
#ifdef SUPPORT_MODE_SWITCH
        tal_pib[RF24].ModeSwitchEnabled = TAL_RF24_MODE_SWITCH_ENABLE;
        tal_pib[RF24].ModeSwitchSettlingDelay = TAL_RF24_MODE_SWITCH_SETTLING_DELAY;
        tal_pib[RF24].ModeSwitchNewMode.modulation = TAL_RF24_NEW_MODE_MODULATION;
        tal_pib[RF24].ModeSwitchNewMode.phy_mode.ofdm.option = TAL_RF24_NEW_MODE_OPTION;
        tal_pib[RF24].ModeSwitchNewMode.rate.ofdm_mcs = (ofdm_mcs_t)TAL_RF24_NEW_MODE_OPTION;
        tal_pib[RF24].ModeSwitchNewMode.fec_enabled = TAL_RF24_NEW_MODE_FEC_ENABLED;
        tal_pib[RF24].ModeSwitchDuration = TAL_RF24_MODE_SWITCH_DURATION;
#ifdef SUPPORT_FSK_RAW_MODE
        tal_pib[RF24].FSKRawModeEnabled = TAL_RF24_FSK_RAW_MODE_ENABLED;
#endif
#endif
#endif /* #ifdef SUPPORT_FSK */
    }

#ifdef PROMISCUOUS_MODE
    tal_pib[trx_id].PromiscuousMode = false;
#endif
#ifdef MEASURE_ON_AIR_DURATION
    tal_pib[trx_id].OnAirDuration = 0;
#endif
#ifdef SUPPORT_LEGACY_OQPSK
    tal_pib[trx_id].HighRateEnabled = false;
#endif
#ifdef SUPPORT_FSK_RAW_MODE
    tal_pib[trx_id].FSKRawModeRxLength = aMaxPHYPacketSize_4g;
#endif

}


/**
 * @brief Configures the PHY
 *
 * This function configures the PHY related settings.
 *
 * @param trx_id Transceiver identifier
 *
 * @return MAC_SUCCESS if the configuration was successful
 */
retval_t config_phy(trx_id_t trx_id)
{
    retval_t status = MAC_SUCCESS;

    status = conf_trx_modulation(trx_id);
    if (status == MAC_SUCCESS)
    {
        status = apply_channel_settings(trx_id);
        /* Set CCA measurement average duration */
        if (status == MAC_SUCCESS)
        {
            set_ed_sample_duration(trx_id, tal_pib[trx_id].CCADuration_us);
#if (defined SUPPORT_LEGACY_OQPSK) || (!defined BASIC_MODE)
            uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
#endif
#ifdef SUPPORT_LEGACY_OQPSK
            pal_trx_bit_write(reg_offset + SR_BBC0_PC_FCST, tal_pib[trx_id].FCSType);
#endif
#ifndef BASIC_MODE
            pal_trx_reg_write(reg_offset + RG_BBC0_AMEDT, tal_pib[trx_id].CCAThreshold);
            pal_trx_write(reg_offset + RG_BBC0_AMAACKTL,
                          (uint8_t *)&tal_pib[trx_id].ACKTiming, 2);
#endif
        }
    }

    return status;
}


/**
 * @brief Calculates PIB values that depend on the current PHY configuration
 *
 * @param trx_id Transceiver identifier
 */
void calculate_pib_values(trx_id_t trx_id)
{
    debug_text(PSTR("calculate_pib_values()"));

    /* Do not change the following order; some values are used to calculate others. */
    tal_pib[trx_id].SymbolDuration_us = tal_get_symbol_duration_us(trx_id);

#ifdef SUPPORT_LEGACY_OQPSK
    if (tal_pib[trx_id].phy.modulation == LEG_OQPSK)
    {
        tal_pib[trx_id].FCSType = FCS_TYPE_2_OCTETS;
        tal_pib[trx_id].FCSLen = 2;
        tal_pib[trx_id].MaxPHYPacketSize = aMaxPHYPacketSize;
    }
    else // SUN PHYs
#endif
    {
        tal_pib[trx_id].FCSType = FCS_TYPE_4_OCTETS;
        tal_pib[trx_id].FCSLen = 4;
        tal_pib[trx_id].MaxPHYPacketSize = aMaxPHYPacketSize_4g;
    }
    tal_pib[trx_id].CurrentPage = calc_ch_page(trx_id);
    uint32_t ch;
    if (get_supported_channels_tuple(trx_id, &ch) == MAC_SUCCESS)
    {
        tal_pib[trx_id].SupportedChannels = ch;
    }
    else
    {
        tal_pib[trx_id].SupportedChannels = 0;
    }
    //float rate = get_data_rate(trx_id);
    //debug_text_val(PSTR("rate"), (uint16_t)rate);
    tal_pib[trx_id].OctetDuration_us = (uint16_t)(((float)1000 * 8) / get_data_rate(trx_id));
    debug_text_val(PSTR("OctetDuration_us"), tal_pib[trx_id].OctetDuration_us);
    tal_pib[trx_id].CCADuration_us = get_cca_duration_us(trx_id);
    debug_text_val(PSTR("CCADuration_us"), tal_pib[trx_id].CCADuration_us);
    tal_pib[trx_id].CCADuration_sym = tal_pib[trx_id].CCADuration_us / tal_pib[trx_id].SymbolDuration_us;
    debug_text_val(PSTR("CCADuration_sym"), tal_pib[trx_id].CCADuration_sym);
    tal_pib[trx_id].CCAThreshold = get_cca_thres(trx_id);
    tal_pib[trx_id].ACKWaitDuration = get_AckWaitDuration_us(trx_id);
    debug_text_val(PSTR("ACKWaitDuration"), tal_pib[trx_id].ACKWaitDuration);
#ifdef IQ_RADIO
    /*
     * Extent ACK wait duration for chip mode, since some artificial delay
     * is used.
     */
    tal_pib[trx_id].ACKWaitDuration += 200;
#endif
    tal_pib[trx_id].ACKTiming = get_AckTiming_us(trx_id);
    debug_text_val(PSTR("ACKTiming"), tal_pib[trx_id].ACKTiming);
#ifdef MEASURE_ON_AIR_DURATION
    tal_pib[trx_id].ACKDuration_us = tal_pib[trx_id].SymbolDuration_us * get_ack_duration_sym(trx_id);
#endif
}


/**
 * @brief Write all shadow PIB variables to the transceiver
 *
 * This function writes all shadow PIB variables to the transceiver.
 * It is assumed that the radio does not sleep and has gone through a reset.
 *
 * @param trx_id Transceiver identifier
 */
void write_all_tal_pib_to_trx(trx_id_t trx_id)
{
    debug_text_val(PSTR("write_all_tal_pib_to_trx trx_id = "), trx_id);
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

    if (tal_pib[trx_id].FCSType != FCS_TYPE_4_OCTETS) // Compared against reset value
    {
        pal_trx_bit_write(reg_offset + SR_BBC0_PC_FCST, tal_pib[trx_id].FCSType);
    }

    /* Setup frame filter registers */
    pal_trx_write(reg_offset + RG_BBC0_MACPID0F0,
                  (uint8_t *)&tal_pib[trx_id].PANId, 2);
    pal_trx_write(reg_offset + RG_BBC0_MACSHA0F0,
                  (uint8_t *)&tal_pib[trx_id].ShortAddress, 2);
    pal_trx_write(reg_offset + RG_BBC0_MACEA0,
                  (uint8_t *)&tal_pib[trx_id].IeeeAddress, 8);
    if (tal_pib[trx_id].PrivatePanCoordinator)
    {
        pal_trx_bit_write(reg_offset + SR_BBC0_AFC1_PANC0,
                          tal_pib[trx_id].PrivatePanCoordinator);
    }
#ifdef SUPPORT_FRAME_FILTER_CONFIGURATION
    uint8_t temp[2];
    temp[0] = tal_pib[trx_id].frame_types;
    temp[1] = tal_pib[trx_id].frame_versions;
    pal_trx_write(reg_offset + RG_BBC0_AFFTM, temp, 2);
#endif
#ifdef SUPPORT_ACK_RATE_MODE_ADAPTION
    pal_trx_bit_write(reg_offset + SR_BBC0_AMCS_AACKDR,
                      tal_pib[trx_id].AdaptDataRateForACK);
#endif
#ifdef PROMISCUOUS_MODE
    if (tal_pib[trx_id].PromiscuousMode)
    {
        debug_text(PSTR("Promiscuous mode enabled"));
        pal_trx_bit_write(reg_offset + SR_BBC0_AFC0_PM, 1);
        tal_rx_enable(trx_id, PHY_RX_ON);
    }
#endif
#ifdef SUPPORT_OQPSK
    pal_trx_bit_write(reg_offset + SR_BBC0_OQPSKPHRTX_MOD,
                      tal_pib[trx_id].OQPSKRateMode);
#endif
#ifdef SUPPORT_FSK
    set_fsk_pibs(trx_id);
#ifdef SUPPORT_FSK_RAW_MODE
    configure_raw_mode(trx_id, tal_pib[trx_id].FSKRawModeEnabled);
    pal_trx_write(reg_offset + RG_BBC0_FSKRRXFLL,
                  (uint8_t *)&tal_pib[trx_id].FSKRawModeRxLength, 2);
#endif
#endif /* #ifdef SUPPORT_FSK */
#ifdef SUPPORT_OFDM
    pal_trx_bit_write(reg_offset + SR_BBC0_OFDMPHRTX_MCS,
                      tal_pib[trx_id].OFDMMCS);
    pal_trx_bit_write(reg_offset + SR_BBC0_OFDMC_POI,
                      tal_pib[trx_id].OFDMInterleaving);
#endif /* #ifdef SUPPORT_OFDM */
}


#ifdef SUPPORT_FSK
/**
 * @brief Sets FSK related PIBs
 *
 * @param trx_id Transceiver identifier
 */
void set_fsk_pibs(trx_id_t trx_id)
{
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

    pal_trx_bit_write(reg_offset + SR_BBC0_FSKPHRTX_SFD,
                      tal_pib[trx_id].FSKFECEnabled);
    pal_trx_bit_write(reg_offset + SR_BBC0_FSKC2_FECIE,
                      tal_pib[trx_id].FSKFECInterleavingRSC);
    pal_trx_bit_write(reg_offset + SR_BBC0_FSKC2_FECS,
                      tal_pib[trx_id].FSKFECScheme);
#ifdef SUPPORT_MODE_SWITCH
    pal_trx_bit_write(reg_offset + SR_BBC0_FSKC2_MSE,
                      tal_pib[trx_id].ModeSwitchEnabled);
#endif
    pal_trx_reg_write(reg_offset + RG_BBC0_FSKPLL,
                      (uint8_t)(tal_pib[trx_id].FSKPreambleLength & 0xFF));
    pal_trx_bit_write(reg_offset + SR_BBC0_FSKC1_FSKPLH,
                      (uint8_t)(tal_pib[trx_id].FSKPreambleLength >> 8));
    set_sfd(trx_id);
    pal_trx_bit_write(reg_offset + SR_BBC0_FSKPHRTX_DW,
                      tal_pib[trx_id].FSKScramblePSDU);
}
#endif


#if (defined SUPPORT_FSK) && ((defined SUPPORT_FSK_RAW_MODE) || (defined SUPPORT_MODE_SWITCH))
/**
 * @brief Configures FSK raw mode
 *
 * @param trx_id Transceiver identifier
 * @param enable true if raw mode should be enabled, else false
 */
void configure_raw_mode(trx_id_t trx_id, bool enable)
{
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

    /* Assuming SFD0 is used for uncoded and SFD1 is used for coded SFD */

    uint8_t fskc4 = pal_trx_reg_read(reg_offset + RG_BBC0_FSKC4);
    fskc4 &= 0xF0;

    if (enable) // Raw mode
    {
        fskc4 |= (1 << 0) | (3 << 2); /* Uncoded raw mode and coded raw mode */
        pal_trx_bit_write(reg_offset + SR_BBC0_PC_TXAFCS, 0);
    }
    else // IEEE mode
    {
        fskc4 |= (0 << 0) | (2 << 2); /* Uncoded IEEE mode and coded IEEE mode */
        pal_trx_bit_write(reg_offset + SR_BBC0_PC_TXAFCS, 1);
    }
    pal_trx_reg_write(reg_offset + RG_BBC0_FSKC4, fskc4);
}
#endif


/**
 * @brief Sets the transmit power
 *
 * @param trx_id Transceiver identifier
 * @param tx_pwr Transmit power in dBm
 */
static void set_tx_pwr(trx_id_t trx_id, int8_t tx_pwr)
{
    /* Limits the transmit power to its borders */
    if (tx_pwr > 14)
    {
        tx_pwr = 14;
    }
    else if (tx_pwr < -17)
    {
        tx_pwr = -17;
    }

    tal_pib[trx_id].TransmitPower = tx_pwr;

    /* Scale dBm value to register value */
    uint8_t val = (uint8_t)(tal_pib[trx_id].TransmitPower + 17);
    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
#ifdef IQ_RADIO
    pal_trx_bit_write(RF215_RF, reg_offset + SR_RF09_PAC_TXPWR, val);
#else
    pal_trx_bit_write(reg_offset + SR_RF09_PAC_TXPWR, val);
#endif
}


/**
 * @brief Check if freq spacing and frequency are in the supported range
 *
 * @param trx_id Transceiver identifier
 *
 * @return MAC_SUCCESS if frequency spacing and frequency are supported
           else FAILURE
 */
#ifndef REDUCED_PARAM_CHECK
static retval_t check_valid_freq_range(trx_id_t trx_id)
{
    retval_t status = MAC_SUCCESS;

    uint32_t freq = tal_pib[trx_id].phy.freq_f0 /*//vk +
                    (tal_pib[trx_id].phy.ch_spacing * tal_pib[trx_id].CurrentChannel)*/;
    uint32_t spacing = tal_pib[trx_id].phy.ch_spacing;

    if ((spacing % 25000) > 0)
    {
        status = FAILURE;
    }
    else
    {
        if (trx_id == RF09)
        {
            if (((freq < 389500000) || (freq > 1020000000)) ||
                ((freq > 510000000) && (freq < 779000000)))
            {
                status = FAILURE;
            }
        }
        else /* RF24 */
        {
            if ((freq < 2400000000) || (freq > 2483500000))
            {
                status = FAILURE;
            }
        }
    }

    return status;
}
#endif


/**
 * @brief Applies channel settings by writing them to the transceiver
 *
 * @param trx_id Transceiver identifier
 *
 * @return MAC_SUCCESS if channel setting is successful
                       else FAILURE
 */
static retval_t apply_channel_settings(trx_id_t trx_id)
{
	retval_t status = check_valid_freq_range(trx_id);
	if (status == MAC_SUCCESS)
	{
		uint32_t freq = tal_pib[trx_id].phy.freq_f0;
		uint32_t spacing = tal_pib[trx_id].phy.ch_spacing;
		uint16_t rf_reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
		uint16_t reg_val;
		uint8_t temp_val;

		/* Offset handling for 2.4GHz only */
		if (trx_id == RF24)
		{
			freq -= 1500000000;
		}
		reg_val = (uint16_t)(freq / 25000);
		pal_trx_write(rf_reg_offset + RG_RF09_CCF0L, (uint8_t *)&reg_val, 2);

		/* Set channel spacing */
		spacing /= 25000; // adjust to register scaling
		pal_trx_reg_write(rf_reg_offset + RG_RF09_CS, (uint8_t)spacing);

		/*
		* Set channel and channel mode.
		* Touching the CNM register forces the calculation of the actual frequency.
		*/
		//#ifdef	SUPPORT_LEGACY_OQPSK
		if(tal_pib[trx_id].phy.modulation == LEG_OQPSK)
		{
			uint16_t value;
			if ((tal_pib[trx_id].phy.freq_band == CHINA_780))
			{
				if(tal_pib[trx_id].CurrentChannel>3)
				{	 
					value = tal_pib[trx_id].CurrentChannel=3; //check
				}
				else
				{
					value = tal_pib[trx_id].CurrentChannel;
				}
			}
			else if ((tal_pib[trx_id].phy.freq_band == WORLD_2450))
			{
				if((tal_pib[trx_id].CurrentChannel >26) || (tal_pib[trx_id].CurrentChannel < 11))
				{
					tal_pib[trx_id].CurrentChannel=11;
					value = tal_pib[trx_id].CurrentChannel-11;
				}
				else
				{
					value = tal_pib[trx_id].CurrentChannel-11;
				}
	 
			}
			else if ((tal_pib[trx_id].phy.freq_band == US_915))
			{
				if((tal_pib[trx_id].CurrentChannel >10) || (tal_pib[trx_id].CurrentChannel < 1))
				{
					tal_pib[trx_id].CurrentChannel=1;
					value = tal_pib[trx_id].CurrentChannel-1;
				}
				else
				{
					value = tal_pib[trx_id].CurrentChannel-1;
				}
			}
	
			pal_trx_write(rf_reg_offset + RG_RF09_CNL, (uint8_t *)&value, 2);
		}
		else
		{
			uint16_t max_ch = get_sun_max_ch_no(trx_id);
			if(tal_pib[trx_id].CurrentChannel > (max_ch-1))
			{
				tal_pib[trx_id].CurrentChannel = (max_ch-1);		
			}
			if ((tal_pib[trx_id].phy.freq_band == EU_863)&&(tal_pib[trx_id].phy.modulation == OQPSK))
			{
				if(tal_pib[trx_id].CurrentChannel == 0)
				{
					freq = 868300000;
				}			
				else if(tal_pib[trx_id].CurrentChannel == 1)
				{
					freq = 868950000;
				}
				else if(tal_pib[trx_id].CurrentChannel == 2)
				{
					freq = 869525000;
				}	
				temp_val = 0;
				reg_val = (uint16_t)(freq / 25000);					
				pal_trx_write(rf_reg_offset + RG_RF09_CCF0L, (uint8_t *)&reg_val, 2);
				pal_trx_write(rf_reg_offset + RG_RF09_CNL,
				(uint8_t *)&temp_val, 2); // write cnl as 0 to get the same freq as freq			
			}
			else
			{
			pal_trx_write(rf_reg_offset + RG_RF09_CNL,
			(uint8_t *)&tal_pib[trx_id].CurrentChannel, 2);
			}
		}					  
		/* Wait until channel set is completed */
		if (trx_state[trx_id] == RF_TXPREP)
		{
			while (TAL_RF_IS_IRQ_SET(trx_id, RF_IRQ_TRXRDY) == 0)
			{
			/* Wait until new channel is set */
			}
			TAL_RF_IRQ_CLR(trx_id, RF_IRQ_TRXRDY);
			//debug_text(PSTR("RF_IRQ_TRXRDY: channel change completed"));
			}
		}

	return status;
	
 /*   debug_text_val(PSTR("apply_channel_settings(), trx_id ="), trx_id);

    / * Check if spacing and frequency are within the correct range. * /
#ifdef REDUCED_PARAM_CHECK
    retval_t status = MAC_SUCCESS;
#else
    retval_t status = check_valid_freq_range(trx_id);
#endif
    if (status == MAC_SUCCESS)
    {
        uint32_t freq = tal_pib[trx_id].phy.freq_f0;
        uint32_t spacing = tal_pib[trx_id].phy.ch_spacing;
        uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
        uint16_t reg_val;

        / * Offset handling for 2.4GHz only * /
        if (trx_id == RF24)
        {
            freq -= 1500000000;
        }
        reg_val = (uint16_t)(freq / 25000);
#ifdef IQ_RADIO
        pal_trx_write(RF215_RF, reg_offset + RG_RF09_CCF0L, (uint8_t *)&reg_val, 2);
#endif
        pal_trx_write(reg_offset + RG_RF09_CCF0L, (uint8_t *)&reg_val, 2);

        / * Set channel spacing * /
        spacing /= 25000; // adjust to register scaling
#ifdef IQ_RADIO
        pal_trx_reg_write(RF215_RF, reg_offset + RG_RF09_CS, (uint8_t)spacing);
#endif
        pal_trx_reg_write(reg_offset + RG_RF09_CS, (uint8_t)spacing);

        / *
         * Set channel and channel mode.
         * Touching the CNM register forces the calculation of the actual frequency.
         * /
#ifdef IQ_RADIO
        pal_trx_write(RF215_RF, reg_offset + RG_RF09_CNL,
                      (uint8_t *)&tal_pib[trx_id].CurrentChannel, 2);
#endif
        pal_trx_write(reg_offset + RG_RF09_CNL,
                      (uint8_t *)&tal_pib[trx_id].CurrentChannel, 2);

        / * Wait until channel set is completed * /
        if (trx_state[trx_id] == RF_TXPREP)
        {
            wait_for_txprep(trx_id);
            debug_text(PSTR("RF_IRQ_TRXRDY: channel change completed"));
        }
    }

    return status;*/
}


/**
 * @brief Gets a TAL PIB attribute
 *
 * This function is called to retrieve the transceiver information base
 * attributes.
 *
 * @param[in] trx_id Transceiver identifier
 * @param[in] attribute TAL infobase attribute ID
 * @param[out] value TAL infobase attribute value
 *
 * @return
 *      - @ref MAC_UNSUPPORTED_ATTRIBUTE if the TAL infobase attribute is not found
 *      - @ref MAC_SUCCESS otherwise
 * @ingroup apiTalApi
 */
#if (HIGHEST_STACK_LAYER == TAL) || (defined DOXYGEN)
retval_t tal_pib_get(trx_id_t trx_id, uint8_t attribute, uint8_t *value)
{
    retval_t status = MAC_SUCCESS;

    switch (attribute)
    {
        case phySetting:
            memcpy(value, &tal_pib[trx_id].phy, sizeof(phy_t));
            break;

        case phyCurrentChannel:
            *(uint16_t *)value = tal_pib[trx_id].CurrentChannel;
            break;

        case phyChannelsSupported:
            if (tal_pib[trx_id].phy.modulation == LEG_OQPSK)
            {
                *(uint32_t *)value = tal_pib[trx_id].SupportedChannels;
            }
            else
            {
                /* @ToDo: Add support for SUN PHY modes */
                status = MAC_UNSUPPORTED_ATTRIBUTE;
            }
            break;

        case phyTransmitPower:
            *(int8_t *)value = tal_pib[trx_id].TransmitPower;
            break;

        case phyCurrentPage:
            *(uint8_t *)value = tal_pib[trx_id].CurrentPage;
            break;

        case phyCurrentSUNPageEntry:
            *(uint8_t *)value = tal_pib[trx_id].CurrentSUNPageEntry;
            break;

        case phyCCADuration: /* CCA duration in symbols */
            *(uint8_t *)value = tal_pib[trx_id].CCADuration_sym;
            break;

        case phyCCATimeMethod:
            *(bool *)value = tal_pib[trx_id].CCATimeMethod;
            break;

        case phyCCAThreshold:
            *(uint8_t *)value = tal_pib[trx_id].CCAThreshold;
            break;

#ifdef SUPPORT_OQPSK
        case phyOQPSKRateMode:
            *(oqpsk_rate_mode_t *)value = tal_pib[trx_id].OQPSKRateMode;
            break;
#endif

#ifdef SUPPORT_FSK
        case phyFSKFECEnabled:
            *(bool *)value = tal_pib[trx_id].FSKFECEnabled;
            break;

        case phyFSKFECInterleavingRSC:
            *(bool *)value = tal_pib[trx_id].FSKFECInterleavingRSC;
            break;

        case phyFSKFECScheme:
            *(bool *)value = tal_pib[trx_id].FSKFECScheme;
            break;

        case phyFSKPreambleLength:
            *(uint16_t *)value = tal_pib[trx_id].FSKPreambleLength;
            break;

        case phyMRFSKSFD:
            *(bool *)value = tal_pib[trx_id].MRFSKSFD;
            break;

        case phyFSKScramblePSDU:
            *(bool *)value = tal_pib[trx_id].FSKScramblePSDU;
            break;

#ifdef SUPPORT_MODE_SWITCH
        case phyFSKModeSwitchEnabled:
            *(bool *)value = tal_pib[trx_id].ModeSwitchEnabled;
            break;

        case phyModeSwitchSettlingDelay:
            *(uint16_t *)value = tal_pib[trx_id].ModeSwitchSettlingDelay;

        case phyModeSwitchNewMode:
            *(new_phy_t *)value = tal_pib[trx_id].ModeSwitchNewMode;
            break;

        case phyModeSwitchDuration:
            *(uint32_t *)value = tal_pib[trx_id].ModeSwitchDuration;
            break;
#endif

#ifdef SUPPORT_FSK_RAW_MODE
        case phyFSKRawModeEnabled:
            *(bool *)value = tal_pib[trx_id].FSKRawModeEnabled;
            break;

        case phyFSKRawModeRxLength:
            *(uint16_t *)value = tal_pib[trx_id].FSKRawModeRxLength;
            break;
#endif /* #ifdef SUPPORT_FSK_RAW_MODE */

#endif /* #ifdef SUPPORT_FSK */

#ifdef SUPPORT_OFDM
        case phyOFDMMCS:
            *(ofdm_mcs_t *)value = tal_pib[trx_id].OFDMMCS;
            break;

        case phyOFDMInterleaving:
            *(bool *)value = tal_pib[trx_id].OFDMInterleaving;
            break;
#endif /* #ifdef SUPPORT_OFDM */

#ifdef RX_WHILE_BACKOFF
        case macMaxNumRxFramesDuringBackoff:
            *value = tal_pib[trx_id].MaxNumRxFramesDuringBackoff;
            break;
#endif

        case macMaxCSMABackoffs:
            *value = tal_pib[trx_id].MaxCSMABackoffs;
            break;

        case macMinBE:
            *value = tal_pib[trx_id].MinBE;
            break;

        case macMaxBE:
            *value = tal_pib[trx_id].MaxBE;
            break;

        case phyCCAMode:
            *value = tal_pib[trx_id].CCAMode;
            break;

        case macFCSType:
            *(bool *)value = tal_pib[trx_id].FCSType;
            break;

        case macAckWaitDuration:
            *(uint16_t *)value = tal_pib[trx_id].ACKWaitDuration;
            break;

        case macACKTiming:
            *(uint16_t *)value = tal_pib[trx_id].ACKTiming;
            break;

        case macMaxFrameRetries:
            *value = tal_pib[trx_id].MaxFrameRetries;
            break;

        case phyMaxFrameDuration:
            *(uint16_t *)value = tal_pib[trx_id].MaxFrameDuration;
            break;

        case phySymbolsPerOctet:
            *value = tal_pib[trx_id].SymbolsPerOctet;
            break;

#ifdef PROMISCUOUS_MODE
        case macPromiscuousMode:
            *(bool *)value = tal_pib[trx_id].PromiscuousMode;
            break;
#endif

        case macPANId:
            *(uint16_t *)value = tal_pib[trx_id].PANId;
            break;

        case macShortAddress:
            *(uint16_t *)value = tal_pib[trx_id].ShortAddress;
            break;

        case macIeeeAddress:
            *(uint64_t *)value = tal_pib[trx_id].IeeeAddress;
            break;

        case mac_i_pan_coordinator:
            *(bool *)value = tal_pib[trx_id].PrivatePanCoordinator;
            break;

#ifdef MEASURE_ON_AIR_DURATION
        case phyOnAirDuration:
            *(uint32_t *)value = tal_pib[trx_id].OnAirDuration;
            break;
#endif

#ifdef SUPPORT_ACK_RATE_MODE_ADAPTION
        case macAdaptDataRateForACK:
            *(bool *)value = tal_pib[trx_id].AdaptDataRateForACK;
            break;
#endif

#ifdef SUPPORT_FRAME_FILTER_CONFIGURATION
        case macFrameFilterFrameTypes:
            *(uint8_t *)value = tal_pib[trx_id].frame_types;
            break;

        case macFrameFilterFrameVersions:
            *(uint8_t *)value = tal_pib[trx_id].frame_versions;
            break;
#endif

#ifdef MEASURE_TIME_OF_FLIGHT
        case phyTimeOfFlight:
            *(uint32_t *)value = tal_pib[trx_id].TimeOfFlight;
            break;
#endif

#ifdef SUPPORT_LEGACY_OQPSK
        case phyHighRateEnabled:
            *(bool *)value = tal_pib[trx_id].HighRateEnabled;
            break;
#endif

        default:
            /* Invalid attribute id */
            status = MAC_UNSUPPORTED_ATTRIBUTE;
            break;
    }

    return status;
} /* tal_pib_get() */
#endif  /* (HIGHEST_STACK_LAYER == TAL) */


/**
 * @brief Sets a TAL PIB attribute
 *
 * This function is called to set the transceiver information base
 * attributes.
 *
 * @param trx_id Transceiver identifier
 * @param attribute TAL infobase attribute ID
 * @param value TAL infobase attribute value to be set
 *
 * @return
 *      - @ref MAC_UNSUPPORTED_ATTRIBUTE if the TAL info base attribute is not found
 *      - @ref TAL_BUSY if the TAL is not in TAL_IDLE state. An exception is
 *         macBeaconTxTime which can be accepted by TAL even if TAL is not
 *         in TAL_IDLE state.
 *      - @ref MAC_SUCCESS if the attempt to set the PIB attribute was successful
 *      - @ref TAL_TRX_ASLEEP if trx is in SLEEP mode and access to trx is required
 * @ingroup apiTalApi
 */
retval_t tal_pib_set(trx_id_t trx_id, uint8_t attribute, pib_value_t *value)
{
    debug_text(PSTR("tal_pib_set()"));

    retval_t status = MAC_SUCCESS;

    /*
     * Do not allow any changes while TAL is in SLEEP or busy.
     */
    if (tal_state[trx_id] == TAL_SLEEP)
    {
        debug_text(PSTR("TAL_TRX_ASLEEP"));
        return TAL_TRX_ASLEEP;
    }

    if (tal_state[trx_id] != TAL_IDLE)
    {
        debug_text_val(PSTR("TAL is busy or asleep, tal_state = "), tal_state[trx_id]);
        return TAL_BUSY;
    }

    uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;

    switch (attribute)
    {
        case phySetting:
            debug_text(PSTR("PIB attribute: phySetting"));
            {
                /* Store previous settings */
                phy_t previous_phy;
                memcpy(&previous_phy, &tal_pib[trx_id].phy, sizeof(phy_t));
                memcpy(&tal_pib[trx_id].phy, value, sizeof(phy_t));
                /* Update channel: if PHY is changed, lowest channel is set. */
                uint32_t ch;
                if (get_supported_channels_tuple(trx_id, &ch) == MAC_SUCCESS)
                {
                   // tal_pib[trx_id].CurrentChannel = (uint16_t)(ch & 0xFFFF);
                }
                else
                {
                    tal_pib[trx_id].CurrentChannel = 0;
                }
                calculate_pib_values(trx_id);
                status = config_phy(trx_id);
                if (status != MAC_SUCCESS)
                {
                    /* Restore previous values */
                    memcpy(&tal_pib[trx_id].phy, &previous_phy, sizeof(phy_t));
                    calculate_pib_values(trx_id);
                }
            }
            break;

        case phyCurrentChannel:
            status = set_channel(trx_id, value->pib_value_16bit);
            break;

        case phyTransmitPower:
            {
                int8_t pwr = (int8_t)value->pib_value_8bit;
                /* Limit tal_pib[trx_id].TransmitPower to max/min trx values */
                set_tx_pwr(trx_id, pwr);
            }
            break;

        case phyCurrentPage:
            status = set_phy_based_on_channel_page(trx_id, (ch_pg_t)value->pib_value_8bit);
			if (MAC_SUCCESS != status) {
				tal_pib[trx_id].CurrentPage = value->pib_value_8bit;
			}
			else {
				return MAC_INVALID_PARAMETER;
			}
		    break;

        case phyCCADuration: /* CCA duration in symbols */
            tal_pib[trx_id].CCADuration_sym = value->pib_value_8bit;
            tal_pib[trx_id].CCADuration_us = calculate_cca_duration_us(trx_id);
            set_ed_sample_duration(trx_id, tal_pib[trx_id].CCADuration_us);
            break;

        case phyCCATimeMethod:
            tal_pib[trx_id].CCATimeMethod = value->pib_value_bool;
            tal_pib[trx_id].CCADuration_us = calculate_cca_duration_us(trx_id);
            set_ed_sample_duration(trx_id, tal_pib[trx_id].CCADuration_us);
            break;

        case phyCCAThreshold:
            tal_pib[trx_id].CCAThreshold = value->pib_value_8bit;
#ifndef BASIC_MODE
            pal_trx_reg_write(reg_offset + RG_BBC0_AMEDT, tal_pib[trx_id].CCAThreshold);
#endif
            break;

#ifdef SUPPORT_OQPSK
        case phyOQPSKRateMode:
            tal_pib[trx_id].OQPSKRateMode = (oqpsk_rate_mode_t)value->pib_value_8bit;
            pal_trx_bit_write(reg_offset + SR_BBC0_OQPSKPHRTX_MOD,
                              tal_pib[trx_id].OQPSKRateMode);
            if (tal_pib[trx_id].phy.modulation == OQPSK)
            {
                calculate_pib_values(trx_id);
#ifndef BASIC_MODE
                pal_trx_reg_write(reg_offset + RG_BBC0_AMEDT, tal_pib[trx_id].CCAThreshold);
#endif
            }
            break;
#endif

#ifdef SUPPORT_FSK
        case phyFSKFECEnabled:
            tal_pib[trx_id].FSKFECEnabled = value->pib_value_bool;
            /* Assuming reset values of FSKC4_CSFD0 and FSKC4_CSFD1 */
            pal_trx_bit_write(reg_offset + SR_BBC0_FSKPHRTX_SFD,
                              tal_pib[trx_id].FSKFECEnabled);
            break;

        case phyFSKFECInterleavingRSC:
            tal_pib[trx_id].FSKFECInterleavingRSC = value->pib_value_bool;
            pal_trx_bit_write(reg_offset + SR_BBC0_FSKC2_FECIE,
                              tal_pib[trx_id].FSKFECInterleavingRSC);
            break;

        case phyFSKFECScheme:
            tal_pib[trx_id].FSKFECScheme = value->pib_value_bool;
            pal_trx_bit_write(reg_offset + SR_BBC0_FSKC2_FECS,
                              tal_pib[trx_id].FSKFECScheme);
            break;

        case phyFSKPreambleLength:
            tal_pib[trx_id].FSKPreambleLength = value->pib_value_16bit;
            pal_trx_reg_write(reg_offset + RG_BBC0_FSKPLL,
                              (uint8_t)(tal_pib[trx_id].FSKPreambleLength & 0xFF));
            pal_trx_bit_write(reg_offset + SR_BBC0_FSKC1_FSKPLH,
                              (uint8_t)(tal_pib[trx_id].FSKPreambleLength >> 8));
            break;

        case phyMRFSKSFD:
            tal_pib[trx_id].MRFSKSFD = value->pib_value_bool;
            set_sfd(trx_id);
            break;

        case phyFSKScramblePSDU:
            tal_pib[trx_id].FSKScramblePSDU = value->pib_value_bool;
            pal_trx_bit_write(reg_offset + SR_BBC0_FSKPHRTX_DW,
                              tal_pib[trx_id].FSKScramblePSDU);
            break;

#ifdef SUPPORT_MODE_SWITCH
        case phyFSKModeSwitchEnabled:
            tal_pib[trx_id].ModeSwitchEnabled = value->pib_value_bool;
            pal_trx_bit_write(reg_offset + SR_BBC0_FSKC2_MSE,
                              tal_pib[trx_id].ModeSwitchEnabled);
            break;

        case phyModeSwitchSettlingDelay:
            tal_pib[trx_id].ModeSwitchSettlingDelay = value->pib_value_16bit;
            break;

        case phyModeSwitchNewMode:
            memcpy(&tal_pib[trx_id].ModeSwitchNewMode, value, sizeof(new_phy_t));
            break;

        case phyModeSwitchDuration:
            tal_pib[trx_id].ModeSwitchDuration = value->pib_value_32bit;
            break;
#endif /* #ifdef SUPPORT_MODE_SWITCH */
#ifdef SUPPORT_FSK_RAW_MODE
        case phyFSKRawModeEnabled:
            tal_pib[trx_id].FSKRawModeEnabled = value->pib_value_bool;
            configure_raw_mode(trx_id, tal_pib[trx_id].FSKRawModeEnabled);
            break;

        case phyFSKRawModeRxLength:
            tal_pib[trx_id].FSKRawModeRxLength = value->pib_value_16bit;
            pal_trx_write(reg_offset + RG_BBC0_FSKRRXFLL,
                          (uint8_t *)&tal_pib[trx_id].FSKRawModeRxLength, 2);
            break;
#endif /* #ifdef SUPPORT_FSK_RAW_MODE */

#endif /* #ifdef SUPPORT_FSK */

#ifdef SUPPORT_OFDM
        case phyOFDMMCS:
            tal_pib[trx_id].OFDMMCS = (ofdm_mcs_t)value->pib_value_8bit;
            pal_trx_bit_write(reg_offset + SR_BBC0_OFDMPHRTX_MCS,
                              tal_pib[trx_id].OFDMMCS);
            if (tal_pib[trx_id].phy.modulation == OFDM)
            {
                int8_t previous_tx_pwr = tal_pib[trx_id].TransmitPower;
                calculate_pib_values(trx_id);
                status = conf_trx_modulation(trx_id);
                /* Restore previous power setting */
                set_tx_pwr(trx_id, previous_tx_pwr);
#ifndef BASIC_MODE
                pal_trx_reg_write(reg_offset + RG_BBC0_AMEDT,
                                  tal_pib[trx_id].CCAThreshold);
#endif
            }
            break;

        case phyOFDMInterleaving:
            tal_pib[trx_id].OFDMInterleaving = value->pib_value_bool;
            pal_trx_bit_write(reg_offset + SR_BBC0_OFDMC_POI,
                              tal_pib[trx_id].OFDMInterleaving);
            if (tal_pib[trx_id].phy.modulation == OFDM)
            {
                calculate_pib_values(trx_id);
                status = conf_trx_modulation(trx_id);
            }
            break;
#endif /* #ifdef SUPPORT_OFDM */

#ifdef RX_WHILE_BACKOFF
        case macMaxNumRxFramesDuringBackoff:
            tal_pib[trx_id].MaxNumRxFramesDuringBackoff = value->pib_value_8bit;
            break;
#endif

        case macMaxCSMABackoffs:
            tal_pib[trx_id].MaxCSMABackoffs = value->pib_value_8bit;
            break;

        case macMinBE:
            if (value->pib_value_8bit > tal_pib[trx_id].MaxBE)
            {
                status = MAC_INVALID_PARAMETER;
            }
            else
            {
                tal_pib[trx_id].MinBE = value->pib_value_8bit;
            }
            break;

        case macMaxBE:
            if ((value->pib_value_8bit < tal_pib[trx_id].MinBE) ||
                (value->pib_value_8bit > 8))
            {
                status = MAC_INVALID_PARAMETER;
            }
            else
            {
                tal_pib[trx_id].MaxBE = value->pib_value_8bit;
            }
            break;

        case phyCCAMode:
            if (value->pib_value_8bit != CCA_MODE_1_ED)
            {
                status = MAC_INVALID_PARAMETER;
            }
            break;

        case macFCSType:
            tal_pib[trx_id].FCSType = value->pib_value_bool;
            pal_trx_bit_write(reg_offset + SR_BBC0_PC_FCST, tal_pib[trx_id].FCSType);
            if (tal_pib[trx_id].FCSType == FCS_TYPE_4_OCTETS)
            {
                tal_pib[trx_id].FCSLen = 4;
            }
            else
            {
                tal_pib[trx_id].FCSLen = 2;
            }
            break;

        case macAckWaitDuration:
            tal_pib[trx_id].ACKWaitDuration = value->pib_value_16bit;
            break;

        case macACKTiming:
            tal_pib[trx_id].ACKTiming = value->pib_value_16bit;
            break;

        case macMaxFrameRetries:
            tal_pib[trx_id].MaxFrameRetries = value->pib_value_8bit;
            break;

#ifdef PROMISCUOUS_MODE
        case macPromiscuousMode:
            if (tal_state[trx_id] == TAL_IDLE)
            {
                tal_pib[trx_id].PromiscuousMode = value->pib_value_bool;
                pal_trx_bit_write(reg_offset + SR_BBC0_AFC0_PM,
                                  tal_pib[trx_id].PromiscuousMode);
                if (tal_pib[trx_id].PromiscuousMode)
                {
                    tal_rx_enable(trx_id, PHY_RX_ON);
                    debug_text(PSTR("Promiscuous mode enabled"));
                }
                else
                {
                    tal_rx_enable(trx_id, PHY_TRX_OFF);
                    debug_text(PSTR("Promiscuous mode disabled"));
                }
            }
            else
            {
                status = TAL_BUSY;
            }
            break;
#endif

        case macPANId:
            tal_pib[trx_id].PANId = value->pib_value_16bit;
            pal_trx_write(reg_offset + RG_BBC0_MACPID0F0,
                          (uint8_t *)&tal_pib[trx_id].PANId, 2);
            break;

        case macShortAddress:
            tal_pib[trx_id].ShortAddress = value->pib_value_16bit;
            pal_trx_write(reg_offset + RG_BBC0_MACSHA0F0,
                          (uint8_t *)&tal_pib[trx_id].ShortAddress, 2);
            break;

        case macIeeeAddress:
            tal_pib[trx_id].IeeeAddress = value->pib_value_64bit;
            pal_trx_write(reg_offset + RG_BBC0_MACEA0,
                          (uint8_t *)&tal_pib[trx_id].IeeeAddress, 8);
            break;

        case mac_i_pan_coordinator:
            tal_pib[trx_id].PrivatePanCoordinator = value->pib_value_bool;
            pal_trx_bit_write(reg_offset + SR_BBC0_AFC1_PANC0,
                              (uint8_t)tal_pib[trx_id].PrivatePanCoordinator);
            break;

#ifdef MEASURE_ON_AIR_DURATION
        case phyOnAirDuration:
            tal_pib[trx_id].OnAirDuration = value->pib_value_32bit;
            break;
#endif

#ifdef SUPPORT_ACK_RATE_MODE_ADAPTION
        case macAdaptDataRateForACK:
            tal_pib[trx_id].AdaptDataRateForACK = value->pib_value_bool;
            pal_trx_bit_write(reg_offset + SR_BBC0_AMCS_AACKDR,
                              tal_pib[trx_id].AdaptDataRateForACK);
            break;
#endif

#ifdef SUPPORT_FRAME_FILTER_CONFIGURATION
        case macFrameFilterFrameTypes:
            tal_pib[trx_id].frame_types = value->pib_value_8bit;
            pal_trx_reg_write(reg_offset + RG_BBC0_AFFTM, tal_pib[trx_id].frame_types);
            break;

        case macFrameFilterFrameVersions:
            tal_pib[trx_id].frame_versions = value->pib_value_8bit;
            pal_trx_bit_write(reg_offset + SR_BBC0_AFFVM_AFFVM, tal_pib[trx_id].frame_versions);
            break;
#endif

#ifdef SUPPORT_LEGACY_OQPSK
        case phyHighRateEnabled:
            tal_pib[trx_id].HighRateEnabled = value->pib_value_bool;
            pal_trx_bit_write(reg_offset + SR_BBC0_OQPSKC3_HRLEG,
                              tal_pib[trx_id].HighRateEnabled);
            if (tal_pib[trx_id].phy.modulation == LEG_OQPSK)
            {
                calculate_pib_values(trx_id);
            }
            break;
#endif

        default:
            status = MAC_UNSUPPORTED_ATTRIBUTE;
            break;
    }

    return status;
} /* tal_pib_set() */


/**
 * @brief Sets the provided channel
 *
 * @param trx_id Transceiver identifier
 * @param ch Channel number
 *
 * @return MAC_SUCCESS if setting was successful else MAC_INVALID_PARAMETER
 */
static retval_t set_channel(trx_id_t trx_id, uint16_t ch)
{
	uint16_t channel_to_set = ch;
    retval_t ret = MAC_SUCCESS;

#if (defined SUPPORT_LEGACY_OQPSK) && (!defined REDUCED_PARAM_CHECK)
    /* Adjust internal channel number to IEEE compliant numbering */
    if (tal_pib[trx_id].phy.modulation == LEG_OQPSK)
    {
                if (trx_id == RF24)
                {
	                if ((ch < 11) ||
	                (ch > 26))
	                {
		                ret = MAC_INVALID_PARAMETER;
		                /* no further processing of the channel value */
	                }
	                else
	                {
		                ch -= 11;
	                }
                }
                else // RF09
                {
	                if ((tal_pib[trx_id].phy.freq_band == US_915))
	                {
		                if ((ch < 1)&& (ch > 10))
		                {
			                
			                ret = MAC_INVALID_PARAMETER;
			                /* no further processing of the channel value */
		                }

		                ch -= 1;

	                }
	                else if (tal_pib[trx_id].phy.freq_band == CHINA_780)
	                {
		                if (ch > 3)
		                {
			                ret = MAC_INVALID_PARAMETER;
			                /* no further processing of the channel value */
		                }
		                
	                }
	                else{
		                
		                ret = MAC_INVALID_PARAMETER;
		                /* no further processing of the channel value */
	                }

                }
    }
#endif

    if (ret == MAC_SUCCESS)
    {
        tal_pib[trx_id].CurrentChannel = channel_to_set;

        rf_cmd_state_t previous_state = trx_state[trx_id];
        if (trx_state[trx_id] == RF_RX)
        {
            /* Set TXPREP and wait until it is reached. */
            switch_to_txprep(trx_id);
        }

        uint16_t reg_offset = RF_BASE_ADDR_OFFSET * trx_id;
#ifdef IQ_RADIO
        pal_trx_write(RF215_RF, reg_offset + RG_RF09_CNL,
                      (uint8_t *)&ch, 2);
#endif
        pal_trx_write(reg_offset + RG_RF09_CNL,
                      (uint8_t *)&ch, 2);

        if (trx_state[trx_id] == RF_TXPREP)
        {
            wait_for_txprep(trx_id);
            debug_text(PSTR("RF_IRQ_TRXRDY: channel change completed"));
        }

        /* restore previous TRX state */
        if (previous_state == RF_RX)
        {
            switch_to_rx(trx_id);
        }
    }

    return ret;
}


/**
 * @brief Sets PHY based on provided channel page
 *
 * @param trx_id Transceiver identifier
 * @param pg Channel page
 *
 * @return MAC_SUCCESS if setting was successful else MAC_INVALID_PARAMETER
 */
static retval_t set_phy_based_on_channel_page(trx_id_t trx_id, ch_pg_t pg)
{
    retval_t ret = MAC_SUCCESS;

    switch (pg)
    {
#ifdef SUPPORT_LEGACY_OQPSK
        case CH_PG_2003:
            if (trx_id == RF09)
            {
                ret = MAC_INVALID_PARAMETER;
            }
            else
            {
                tal_pib[trx_id].phy.ch_spacing = LEG_2450_CH_SPAC;
                tal_pib[trx_id].phy.freq_band = WORLD_2450;
                tal_pib[trx_id].phy.freq_f0 = LEG_2450_F0 ;//- (11 * LEG_2450_CH_SPAC);
                tal_pib[trx_id].phy.modulation = LEG_OQPSK;
                tal_pib[trx_id].phy.phy_mode.leg_oqpsk.chip_rate = CHIP_RATE_2000;
            }
            break;

        case CH_PG_2006:
            if (trx_id == RF24)
            {
                ret = MAC_INVALID_PARAMETER;
            }
            else
            {
                tal_pib[trx_id].phy.ch_spacing = LEG_915_CH_SPAC;
                tal_pib[trx_id].phy.freq_band = US_915;
                tal_pib[trx_id].phy.freq_f0 = LEG_915_F0/* - LEG_915_CH_SPAC*/; //vk
                tal_pib[trx_id].phy.modulation = LEG_OQPSK;
                tal_pib[trx_id].phy.phy_mode.leg_oqpsk.chip_rate = CHIP_RATE_1000;
            }
            break;
        case CH_PG_CHINA:
        if (trx_id == RF24)
        {
	        ret = MAC_INVALID_PARAMETER;
        }
        else
        {
	        tal_pib[trx_id].phy.ch_spacing = LEG_915_CH_SPAC;
	        tal_pib[trx_id].phy.freq_band = CHINA_780;
	        tal_pib[trx_id].phy.freq_f0 = LEG_780_F0/* - LEG_915_CH_SPAC*/; //vk
	        tal_pib[trx_id].phy.modulation = LEG_OQPSK;
	        tal_pib[trx_id].phy.phy_mode.leg_oqpsk.chip_rate = CHIP_RATE_1000;
        }
        break;
#endif

        default:
            ret = MAC_INVALID_PARAMETER;
            break;
    }
    if (ret == MAC_SUCCESS)
    {
        uint32_t ch;
        get_supported_channels_tuple(trx_id, &ch);
       // tal_pib[trx_id].CurrentChannel = (uint16_t)(ch & 0xFFFF);
        calculate_pib_values(trx_id);
        ret = config_phy(trx_id);
    }
    else
    {
        tal_pib[trx_id].CurrentPage = CH_PG_INVALID;
        tal_pib[trx_id].SupportedChannels = 0;
    }

    return ret;
}


/**
 * @brief Calculates the channel page based on trx_id and freq_band
 *
 * @param trx_id Transceiver identifier
 *
 * @return Channel page
 */
static ch_pg_t calc_ch_page(trx_id_t trx_id)
{
    ch_pg_t pg;

    if (tal_pib[trx_id].phy.modulation == LEG_OQPSK)
    {
        if (trx_id == RF09)
        {
            switch (tal_pib[trx_id].phy.freq_band)
            {
                case CHINA_470:
                    pg = CH_PG_CHINA;
                    break;

                case US_915:
                    pg = CH_PG_2006;
                    break;

                default:
                    pg = CH_PG_INVALID;
                    break;
            }
        }
        else // RF24
        {
            pg = CH_PG_2003;
        }
    }
    else
    {
        pg = CH_PG_SUN;
    }

    return pg;
}

/* EOF */
