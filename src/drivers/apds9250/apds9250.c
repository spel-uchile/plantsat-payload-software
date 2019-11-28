/*
 * APDS9250.cpp
 *
 * Arduino library for the Broadcom/Avago APDS-9250 ambient light sensor, IR
 * sensor, and RGB color sensor.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPLv2.0
 *
 * Adapted to c by
 * Gustavo Diaz <g.hernan.diaz@gmail.com>
 */

#include <apds9250.h>


/*
 * Initialize and detect the sensor on the i2c bus. Returns false if it wasn't
 * able to find one.
 */
int apds9250_begin() {
    //Sets some default values for sanity.
    apds9250_mode      = APDS9250_CHAN_ALS;
    apds9250_res       = APDS9250_RES_18BIT;
    apds9250_meas_rate = APDS9250_RATE_100MS;
    apds9250_gain      = APDS9250_GAIN_3X;
    apds9250_raw_r     = 0;
    apds9250_raw_g     = 0;
    apds9250_raw_b     = 0;
    apds9250_raw_als   = 0;
    apds9250_raw_ir    = 0;
    return apds9250_enable();
}

int apds9250_reset() {
    uint8_t reg_data[1] = {APDS9250_REG_MAIN_CTRL};
    int ec = i2c_write_n(APDS9250_I2C_ADDR, APDS9250_REG_MAIN_CTRL, reg_data, 1);
    return !ec;
}

/*
 * Enable sensor by triggering software reset and setting light sensor as
 * active.
 */
int apds9250_enable() {
    uint8_t reg_data[1] = {APDS9250_CTRL_LS_EN};
    int ec = i2c_write_n(APDS9250_I2C_ADDR, APDS9250_REG_MAIN_CTRL, reg_data, 1);
    return !ec;
}

/*
 * Get current sensor mode (basically, what's in the green register).
 */
apds9250_chan_t apds9250_getMode() {
    uint8_t temp = read8(APDS9250_REG_MAIN_CTRL);
    apds9250_mode = apds9250_modeFromReg(temp);
    return apds9250_mode;
}

/*
 * Set current sensor mode (basically, what's in the green register), returing
 * new mode value.
 */
apds9250_chan_t apds9250_setMode(apds9250_chan_t newMode) {
    uint8_t temp = APDS9250_CTRL_LS_EN;
    temp |= apds9250_modeToReg(newMode);
    uint8_t reg_data[1] = {temp};
    int ec = i2c_write_n(APDS9250_I2C_ADDR, APDS9250_REG_MAIN_CTRL, reg_data, 1);
    return apds9250_getMode();
}

/*
 * Set sensing mode to Ambient Light Sensor (ALS) + IR.
 */
void apds9250_setModeALS() {
    apds9250_setMode(APDS9250_CHAN_ALS);
}

/*
 * Set sensing mode to RGB+IR.
 */
void apds9250_setModeRGB() {
    apds9250_setMode(APDS9250_CHAN_RGB);
}

/*
 * Get ADC resolution bits.
 */
apds9250_res_t apds9250_getResolution() {
    apds9250_getMeasureRateReg();
    return apds9250_res;
}

/*
 * Set ADC resolution bits, returning new resolution value.
 */
apds9250_res_t apds9250_setResolution(apds9250_res_t newRes) {
    apds9250_res = newRes;
    apds9250_setMeasureRateReg();
    return apds9250_getResolution();
}

/*
 * Get ADC integration time.
 */
apds9250_rate_t apds9250_getMeasRate() {
    apds9250_getMeasureRateReg();
    return apds9250_meas_rate;
}

/*
 * Set ADC integration time, returning new rate. I *think* this overrides the
 * minimum times for the higher resolutions, but the data sheet is not 100%
 * clear.
 */
apds9250_rate_t apds9250_setMeasRate(apds9250_rate_t newRate) {
    apds9250_meas_rate = newRate;
    apds9250_setMeasureRateReg();
    return apds9250_getMeasRate();
}

/*
 * Get ADC gain
 */
apds9250_gain_t apds9250_getGain() {
    uint8_t temp = read8(APDS9250_REG_LS_GAIN);
    apds9250_gain = apds9250_gainFromReg(temp);
    return apds9250_gain;
}

/*
 * Set ADC gain
 */
apds9250_gain_t apds9250_setGain(apds9250_gain_t newGain) {
    uint8_t reg_data[1] = {apds9250_gainToReg(newGain)};
    int ec = i2c_write_n(APDS9250_I2C_ADDR, APDS9250_REG_LS_GAIN, reg_data, 1);
    return apds9250_getGain();
}

/*
 * Get raw data from the red channel. TODO autoscale somehow?
 */
uint32_t apds9250_getRawRedData() {
    apds9250_raw_r = apds9250_read20(APDS9250_REG_LS_DATA_RED_0);
    return apds9250_raw_r;
}

/*
 * Get raw data from the green channel. TODO autoscale somehow?
 */
uint32_t apds9250_getRawGreenData() {
    apds9250_raw_g = apds9250_read20(APDS9250_REG_LS_DATA_GREEN_0);
    return apds9250_raw_g;
}

/*
 * Get raw data from the blue channel. TODO autoscale somehow?
 */
uint32_t apds9250_getRawBlueData() {
    apds9250_raw_b = apds9250_read20(APDS9250_REG_LS_DATA_BLUE_0);
    return apds9250_raw_b;
}

/*
 * Get raw data from the IR channel. TODO autoscale somehow?
 */
uint32_t apds9250_getRawIRData() {
    apds9250_raw_ir = apds9250_read20(APDS9250_REG_LS_DATA_IR_0);
    return apds9250_raw_ir;
}

/*
 * Get raw ALS data (theoretically plain ol lux) TODO autoscale?
 */
uint32_t apds9250_getRawALSData() {
    apds9250_raw_als = apds9250_read20(APDS9250_REG_LS_DATA_GREEN_0);
    return apds9250_raw_als;
}

/*
 * Read 20 bits bytes over I2C from the given register, and repack into a 32 bit int and return.
 */
uint32_t apds9250_read20(uint8_t reg) {
    uint8_t lsb, isb, msb;

    uint8_t reg_data[3];

    i2c_read_n(apds9250_addr, reg, reg_data, 3, 0);
    lsb = reg_data[0];
    isb = reg_data[1];
    msb = reg_data[2];

    uint32_t data = (((uint32_t)(msb & 7)) << 16) | (((uint32_t)isb) << 8) | lsb;

    return data;
}

/*
 * Get LS_MEAS_RATE register values.
 */
void apds9250_getMeasureRateReg() {
    uint8_t temp = read8(APDS9250_REG_LS_MEAS_RATE);
    apds9250_res = apds9250_resFromReg(temp);
    apds9250_meas_rate = apds9250_measRateFromReg(temp);
}

/*
 * Set LS_MEAS_RATE register values.
 */
void apds9250_setMeasureRateReg() {
    uint8_t temp = 0;
    temp |= apds9250_resToReg(apds9250_res) << 4;
    temp |= apds9250_measRateToReg(apds9250_meas_rate);

    uint8_t reg_data[1] = {temp};
    int ec = i2c_write_n(APDS9250_I2C_ADDR, APDS9250_REG_LS_MEAS_RATE, reg_data, 1);
}

/*
 * Convert register data to enum type for mode.
 */
apds9250_chan_t apds9250_modeFromReg(uint8_t reg_value) {
    switch (reg_value & APDS9250_CTRL_CS_MASK) {
        case APDS9250_CTRL_CS_MODE_RGB:
            return APDS9250_CHAN_RGB;
        case APDS9250_CTRL_CS_MODE_ALS:
            return APDS9250_CHAN_ALS;
        default:
            return APDS9250_CHAN_ALS;
    }
}

/*
 * Convert register data to enum type for resolution.
 */
apds9250_res_t apds9250_resFromReg(uint8_t reg_value) {
    switch (reg_value & APDS9250_RESOLUTION_MASK) {
        case APDS9250_RESOLUTION_20BIT:
            return APDS9250_RES_20BIT;
        case APDS9250_RESOLUTION_19BIT:
            return APDS9250_RES_19BIT;
        case APDS9250_RESOLUTION_18BIT:
            return APDS9250_RES_18BIT;
        case APDS9250_RESOLUTION_17BIT:
            return APDS9250_RES_17BIT;
        case APDS9250_RESOLUTION_16BIT:
            return APDS9250_RES_16BIT;
        case APDS9250_RESOLUTION_13BIT:
            return APDS9250_RES_13BIT;
        default:
            return APDS9250_RES_18BIT;
    }
}

/*
 * Convert register data to enum type for measurement rate.
 */
apds9250_rate_t apds9250_measRateFromReg(uint8_t reg_value) {
    switch (reg_value & APDS9250_MEAS_RATE_MASK) {
        case APDS9250_MEAS_RATE_25MS:
            return APDS9250_RATE_25MS;
        case APDS9250_MEAS_RATE_50MS:
            return APDS9250_RATE_50MS;
        case APDS9250_MEAS_RATE_100MS:
            return APDS9250_RATE_100MS;
        case APDS9250_MEAS_RATE_200MS:
            return APDS9250_RATE_200MS;
        case APDS9250_MEAS_RATE_500MS:
            return APDS9250_RATE_500MS;
        case APDS9250_MEAS_RATE_1000MS:
            return APDS9250_RATE_1000MS;
        case APDS9250_MEAS_RATE_2000MS:
            return APDS9250_RATE_2000MS;
        case APDS9250_MEAS_RATE_DUP:
            return APDS9250_RATE_2000MS;
        default:
            return APDS9250_RATE_100MS;
    }
}

/*
 * Convert register data to enum type for gain.
 */
apds9250_gain_t apds9250_gainFromReg(uint8_t reg_value) {
    switch (reg_value & APDS9250_LS_GAIN_MASK) {
        case APDS9250_LS_GAIN_1X:
            return APDS9250_GAIN_1X;
        case APDS9250_LS_GAIN_3X:
            return APDS9250_GAIN_3X;
        case APDS9250_LS_GAIN_6X:
            return APDS9250_GAIN_6X;
        case APDS9250_LS_GAIN_9X:
            return APDS9250_GAIN_9X;
        case APDS9250_LS_GAIN_18X:
            return APDS9250_GAIN_18X;
        default:
            return APDS9250_GAIN_3X;
    }
}

uint8_t apds9250_resToReg(apds9250_res_t newRes) {
    switch (newRes) {
        case APDS9250_RES_20BIT:
            return APDS9250_RESOLUTION_20BIT;
        case APDS9250_RES_19BIT:
            return APDS9250_RESOLUTION_19BIT;
        case APDS9250_RES_18BIT:
            return APDS9250_RESOLUTION_18BIT;
        case APDS9250_RES_17BIT:
            return APDS9250_RESOLUTION_17BIT;
        case APDS9250_RES_16BIT:
            return APDS9250_RESOLUTION_16BIT;
        case APDS9250_RES_13BIT:
            return APDS9250_RESOLUTION_13BIT;
        default:
            return APDS9250_RESOLUTION_18BIT;
    }
}

uint8_t apds9250_measRateToReg(apds9250_rate_t newMeasRate) {
    switch (newMeasRate) {
        case APDS9250_RATE_25MS:
            return APDS9250_MEAS_RATE_25MS;
        case APDS9250_RATE_50MS:
            return APDS9250_MEAS_RATE_50MS;
        case APDS9250_RATE_100MS:
            return APDS9250_MEAS_RATE_100MS;
        case APDS9250_RATE_200MS:
            return APDS9250_MEAS_RATE_200MS;
        case APDS9250_RATE_500MS:
            return APDS9250_MEAS_RATE_500MS;
        case APDS9250_RATE_1000MS:
            return APDS9250_MEAS_RATE_1000MS;
        case APDS9250_RATE_2000MS:
            return APDS9250_MEAS_RATE_2000MS;
        default:
            return APDS9250_MEAS_RATE_100MS;
    }
}

uint8_t apds9250_modeToReg(apds9250_chan_t newMode) {
    switch (newMode) {
        case APDS9250_CHAN_ALS:
            return APDS9250_CTRL_CS_MODE_ALS;
        case APDS9250_CHAN_RGB:
            return APDS9250_CTRL_CS_MODE_RGB;
        default:
            return APDS9250_CTRL_CS_MODE_ALS;
    }
}

uint8_t apds9250_gainToReg(apds9250_gain_t newGain) {
    switch (newGain) {
        case APDS9250_GAIN_1X:
            return APDS9250_LS_GAIN_1X;
        case APDS9250_GAIN_3X:
            return APDS9250_LS_GAIN_3X;
        case APDS9250_GAIN_6X:
            return APDS9250_LS_GAIN_6X;
        case APDS9250_GAIN_9X:
            return APDS9250_LS_GAIN_9X;
        case APDS9250_GAIN_18X:
            return APDS9250_LS_GAIN_18X;
        default:
            return APDS9250_LS_GAIN_3X;
    }
}