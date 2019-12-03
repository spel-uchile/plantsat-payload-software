/**
 * @file  dataSchema.h
 * @author Camilo Rojas M - camrojas@uchile.cl
 * @date 2019
 * @copyright GNU GPL v3
 *
 * This file initilize some structs needed for data schema.
 */

#include "repoDataSchema.h"

struct map data_map[last_sensor] = {
        {"temp_data",      (uint16_t) (sizeof(temp_data_t)),     dat_drp_temp, dat_drp_ack_temp, "%u %f %f %f",                   "timestamp obc_temp_1 obc_temp_2 obc_temp_3"},
        { "ads_data",      (uint16_t) (sizeof(ads_data_t)),      dat_drp_ads,  dat_drp_ack_ads,  "%u %f %f %f %f %f %f",          "timestamp acc_x acc_y acc_z mag_x mag_y mag_z"},
        { "eps_data",      (uint16_t) (sizeof(eps_data_t)),      dat_drp_eps,  dat_drp_ack_eps,  "%u %u %u %u %d %d %d %d %d %d", "timestamp cursun cursys vbatt temp1 temp2 temp3 temp4 temp5 temp6"},
        { "mcp_data",      (uint16_t) (sizeof(mcp_data_t)),      dat_drp_mcp,  dat_drp_ack_mcp,  "%u %f",                         "timestamp temp"},
        { "bmp_data",      (uint16_t) (sizeof(bmp_data_t)),      dat_drp_bmp,  dat_drp_ack_bmp,  "%u %f %f %f",                   "timestamp temp prs alt"},
        { "hdc_data",      (uint16_t) (sizeof(hdc_data_t)),      dat_drp_hdc,  dat_drp_ack_hdc,  "%u %f %f",                      "timestamp temp hum"},
        { "veml_data",     (uint16_t) (sizeof(veml_data_t)),     dat_drp_veml, dat_drp_ack_veml, "%u %u",                         "timestamp uv"},
        { "apds_data",     (uint16_t) (sizeof(apds_data_t)),     dat_drp_apds, dat_drp_ack_apds, "%u %d %d %d %d %d",             "timestamp red green blue als ir"},
        { "langmuir_data", (uint16_t) (sizeof(langmuir_data_t)), dat_drp_lang, dat_drp_ack_lang, "%u %f %f %f %d",                "timestamp sweep_voltage plasma_voltage plasma_temperature particles_counter"}
};


void dat_status_to_struct(dat_status_t *status)
{
    assert(status != NULL);
    DAT_CPY_SYSTEM_VAR(status, dat_obc_opmode);        ///< General operation mode
    DAT_CPY_SYSTEM_VAR(status, dat_obc_last_reset);    ///< Last reset source
    DAT_CPY_SYSTEM_VAR(status, dat_obc_hrs_alive);     ///< Hours since first boot
    DAT_CPY_SYSTEM_VAR(status, dat_obc_hrs_wo_reset);  ///< Hours since last reset
    DAT_CPY_SYSTEM_VAR(status, dat_obc_reset_counter); ///< Number of reset since first boot
    DAT_CPY_SYSTEM_VAR(status, dat_obc_sw_wdt);        ///< Software watchdog timer counter
    DAT_CPY_SYSTEM_VAR_F(status, dat_obc_temp_1);        ///< Temperature value of the first sensor
    DAT_CPY_SYSTEM_VAR_F(status, dat_obc_temp_2);        ///< Temperature value of the second sensor
    DAT_CPY_SYSTEM_VAR_F(status, dat_obc_temp_3);        ///< Temperature value of the gyroscope

    DAT_CPY_SYSTEM_VAR(status, dat_dep_deployed);      ///< Was the satellite deployed?
    DAT_CPY_SYSTEM_VAR(status, dat_dep_ant_deployed);  ///< Was the antenna deployed?
    DAT_CPY_SYSTEM_VAR(status, dat_dep_date_time);     ///< Deployment unix time

    DAT_CPY_SYSTEM_VAR(status, dat_rtc_date_time);     /// RTC current unix time

    DAT_CPY_SYSTEM_VAR(status, dat_com_count_tm);      ///< number of TM sent
    DAT_CPY_SYSTEM_VAR(status, dat_com_count_tc);      ///< number of received TC
    DAT_CPY_SYSTEM_VAR(status, dat_com_last_tc);       ///< Unix time of the last received tc
    DAT_CPY_SYSTEM_VAR(status, dat_com_freq);          ///< Frequency [Hz]
    DAT_CPY_SYSTEM_VAR(status, dat_com_tx_pwr);        ///< TX power (0: 25dBm, 1: 27dBm, 2: 28dBm, 3: 30dBm)
    DAT_CPY_SYSTEM_VAR(status, dat_com_baud);          ///< Baudrate [bps]
    DAT_CPY_SYSTEM_VAR(status, dat_com_mode);          ///< Framing mode (1: RAW, 2: ASM, 3: HDLC, 4: Viterbi, 5: GOLAY, 6: AX25)
    DAT_CPY_SYSTEM_VAR(status, dat_com_bcn_period);    ///< Number of seconds between beacon packets

    DAT_CPY_SYSTEM_VAR(status, dat_fpl_last);          ///< Last executed flight plan (unix time)
    DAT_CPY_SYSTEM_VAR(status, dat_fpl_queue);         ///< Flight plan queue length

    DAT_CPY_SYSTEM_VAR_F(status, dat_ads_acc_x);         ///< Gyroscope acceleration value along the x axis
    DAT_CPY_SYSTEM_VAR_F(status, dat_ads_acc_y);         ///< Gyroscope acceleration value along the y axis
    DAT_CPY_SYSTEM_VAR_F(status, dat_ads_acc_z);         ///< Gyroscope acceleration value along the z axis
    DAT_CPY_SYSTEM_VAR_F(status, dat_ads_mag_x);         ///< Magnetometer x axis
    DAT_CPY_SYSTEM_VAR_F(status, dat_ads_mag_y);         ///< Magnetometer y axis
    DAT_CPY_SYSTEM_VAR_F(status, dat_ads_mag_z);         ///< Magnetometer z axis

    DAT_CPY_SYSTEM_VAR(status, dat_eps_vbatt);         ///< Voltage of battery [mV]
    DAT_CPY_SYSTEM_VAR(status, dat_eps_cur_sun);       ///< Current from boost converters [mA]
    DAT_CPY_SYSTEM_VAR(status, dat_eps_cur_sys);       ///< Current out of battery [mA]
    DAT_CPY_SYSTEM_VAR(status, dat_eps_temp_bat0);     ///< Battery temperature sensor

    DAT_CPY_SYSTEM_VAR(status, dat_drp_temp);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_ads);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_eps);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_mcp);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_bmp);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_hdc);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_veml);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_apds);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_lang);

    DAT_CPY_SYSTEM_VAR(status, dat_drp_ack_temp);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_ack_ads);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_ack_eps);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_ack_mcp);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_ack_bmp);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_ack_hdc);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_ack_veml);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_ack_apds);
    DAT_CPY_SYSTEM_VAR(status, dat_drp_ack_lang);
}

void dat_print_status(dat_status_t *status)
{
    DAT_PRINT_SYSTEM_VAR(status, dat_obc_opmode);        ///< General operation mode
    DAT_PRINT_SYSTEM_VAR(status, dat_obc_last_reset);    ///< Last reset source
    DAT_PRINT_SYSTEM_VAR(status, dat_obc_hrs_alive);     ///< Hours since first boot
    DAT_PRINT_SYSTEM_VAR(status, dat_obc_hrs_wo_reset);  ///< Hours since last reset
    DAT_PRINT_SYSTEM_VAR(status, dat_obc_reset_counter); ///< Number of reset since first boot
    DAT_PRINT_SYSTEM_VAR(status, dat_obc_sw_wdt);        ///< Software watchdog timer counter
    DAT_PRINT_SYSTEM_VAR_F(status, dat_obc_temp_1);      ///< Temperature value of the first sensor
    DAT_PRINT_SYSTEM_VAR_F(status, dat_obc_temp_2);      ///< Temperature value of the second sensor
    DAT_PRINT_SYSTEM_VAR_F(status, dat_obc_temp_3);      ///< Temperature value of the gyroscope

    DAT_PRINT_SYSTEM_VAR(status, dat_dep_deployed);      ///< Was the satellite deployed?
    DAT_PRINT_SYSTEM_VAR(status, dat_dep_ant_deployed);  ///< Was the antenna deployed?
    DAT_PRINT_SYSTEM_VAR(status, dat_dep_date_time);     ///< Deployment unix time

    DAT_PRINT_SYSTEM_VAR(status, dat_rtc_date_time);     /// RTC current unix time

    DAT_PRINT_SYSTEM_VAR(status, dat_com_count_tm);      ///< number of TM sent
    DAT_PRINT_SYSTEM_VAR(status, dat_com_count_tc);      ///< number of received TC
    DAT_PRINT_SYSTEM_VAR(status, dat_com_last_tc);       ///< Unix time of the last received tc
    DAT_PRINT_SYSTEM_VAR(status, dat_com_freq);          ///< Frequency [Hz]
    DAT_PRINT_SYSTEM_VAR(status, dat_com_tx_pwr);        ///< TX power (0: 25dBm, 1: 27dBm, 2: 28dBm, 3: 30dBm)
    DAT_PRINT_SYSTEM_VAR(status, dat_com_baud);          ///< Baudrate [bps]
    DAT_PRINT_SYSTEM_VAR(status, dat_com_mode);          ///< Framing mode (1: RAW, 2: ASM, 3: HDLC, 4: Viterbi, 5: GOLAY, 6: AX25)
    DAT_PRINT_SYSTEM_VAR(status, dat_com_bcn_period);    ///< Number of seconds between beacon packets

    DAT_PRINT_SYSTEM_VAR(status, dat_fpl_last);          ///< Last executed flight plan (unix time)
    DAT_PRINT_SYSTEM_VAR(status, dat_fpl_queue);         ///< Flight plan queue length

    DAT_PRINT_SYSTEM_VAR_F(status, dat_ads_acc_x);         ///< Gyroscope acceleration value along the x axis
    DAT_PRINT_SYSTEM_VAR_F(status, dat_ads_acc_y);         ///< Gyroscope acceleration value along the y axis
    DAT_PRINT_SYSTEM_VAR_F(status, dat_ads_acc_z);         ///< Gyroscope acceleration value along the z axis
    DAT_PRINT_SYSTEM_VAR_F(status, dat_ads_mag_x);         ///< Magnetometer x axis
    DAT_PRINT_SYSTEM_VAR_F(status, dat_ads_mag_y);         ///< Magnetometer y axis
    DAT_PRINT_SYSTEM_VAR_F(status, dat_ads_mag_z);         ///< Magnetometer z axis

    DAT_PRINT_SYSTEM_VAR(status, dat_eps_vbatt);         ///< Voltage of battery [mV]
    DAT_PRINT_SYSTEM_VAR(status, dat_eps_cur_sun);       ///< Current from boost converters [mA]
    DAT_PRINT_SYSTEM_VAR(status, dat_eps_cur_sys);       ///< Current out of battery [mA]
    DAT_PRINT_SYSTEM_VAR(status, dat_eps_temp_bat0);     ///< Battery temperature sensor

    DAT_PRINT_SYSTEM_VAR(status, dat_drp_temp);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_ads);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_eps);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_mcp);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_bmp);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_hdc);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_veml);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_apds);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_lang);

    DAT_PRINT_SYSTEM_VAR(status, dat_drp_ack_temp);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_ack_ads);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_ack_eps);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_ack_mcp);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_ack_bmp);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_ack_hdc);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_ack_veml);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_ack_apds);
    DAT_PRINT_SYSTEM_VAR(status, dat_drp_ack_lang);
}

