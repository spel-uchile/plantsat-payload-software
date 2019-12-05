/*
 * APDS9250.h
 *
 * Arduino library for the Broadcom/Avago APDS-9250 ambient light sensor, IR
 * sensor, and RGB color sensor.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPLv2.0
 *
 */

#ifndef __APDS9250_H
#define __APDS9250_H

#include "i2c.h"

/*
 * LSB = Least Significant Byte
 * ISB = Intermediately Significant Byte (yes, I made this one up)
 * MSB = Most Significant Byte
 */

#define APDS9250_I2C_ADDR 0x52 /* Default (only?) i2c address */

/*
 *  (t) - Triggers new measurement
 * (ro) - Read only register
 * (rw) - Read/write register
 */
#define APDS9250_REG_MAIN_CTRL        0x00 /* (t)(rw) LS operation mode control, SW reset */
#define APDS9250_REG_LS_MEAS_RATE     0x04 /* (t)(rw) LS measurement rate and resolution in active mode */
#define APDS9250_REG_LS_GAIN          0x05 /* (t)(rw) LS analog gain range */
#define APDS9250_REG_PART_ID          0x06 /*    (ro) Part number ID and revision ID */
#define APDS9250_REG_MAIN_STATUS      0x07 /*    (ro) Power-on status, interrupt status, data status */
#define APDS9250_REG_LS_DATA_IR_0     0x0A /*    (ro) IR    ADC measurement data - LSB */
#define APDS9250_REG_LS_DATA_IR_1     0x0B /*    (ro) IR    ADC measurement data - ISB */
#define APDS9250_REG_LS_DATA_IR_2     0x0C /*    (ro) IR    ADC measurement data - MSB */
#define APDS9250_REG_LS_DATA_GREEN_0  0x0D /*    (ro) Green ADC measurement data - LSB */
#define APDS9250_REG_LS_DATA_GREEN_1  0x0E /*    (ro) Green ADC measurement data - ISB */
#define APDS9250_REG_LS_DATA_GREEN_2  0x0F /*    (ro) Green ADC measurement data - MSB */
#define APDS9250_REG_LS_DATA_BLUE_0   0x10 /*    (ro) Blue  ADC measurement data - LSB */
#define APDS9250_REG_LS_DATA_BLUE_1   0x11 /*    (ro) Blue  ADC measurement data - ISB */
#define APDS9250_REG_LS_DATA_BLUE_2   0x12 /*    (ro) Blue  ADC measurement data - MSB */
#define APDS9250_REG_LS_DATA_RED_0    0x13 /*    (ro) Red   ADC measurement data - LSB */
#define APDS9250_REG_LS_DATA_RED_1    0x14 /*    (ro) Red   ADC measurement data - ISB */
#define APDS9250_REG_LS_DATA_RED_2    0x15 /*    (ro) Red   ADC measurement data - MSB */
#define APDS9250_REG_INT_CFG          0x19 /*    (rw) Interrupt configuration */
#define APDS9250_REG_INT_PERSISTENCE  0x1A /*    (rw) Interrupt persist setting */
#define APDS9250_REG_LS_THRES_UP_0    0x21 /*    (rw) LS interrupt upper threshold - LSB */
#define APDS9250_REG_LS_THRES_UP_1    0x22 /*    (rw) LS interrupt upper threshold - ISB */
#define APDS9250_REG_LS_THRES_UP_2    0x23 /*    (rw) LS interrupt upper threshold - MSB */
#define APDS9250_REG_LS_THRES_LOW_0   0x24 /*    (rw) LS interrupt lower threshold - LSB */
#define APDS9250_REG_LS_THRES_LOW_1   0x25 /*    (rw) LS interrupt lower threshold - ISB */
#define APDS9250_REG_LS_THRES_LOW_2   0x26 /*    (rw) LS interrupt lower threshold - MSB */
#define APDS9250_REG_LS_THRES_VAR     0x27 /*    (rw) LS interrupt variance threshold */

#define APDS9250_CTRL_SW_RESET    (1 << 4) /* Trigger software reset */
#define APDS9250_CTRL_CS_MODE_ALS (0 << 2) /* Channel Select 0 - ALS & IR mode (default) */
#define APDS9250_CTRL_CS_MODE_RGB (1 << 2) /* Channel Select 1 - RGB & IR mode */
#define APDS9250_CTRL_CS_MASK     (1 << 2) /* Channel Select mask */
#define APDS9250_CTRL_LS_EN       (1 << 1) /* Light Sensor enabled */

/* Sensor resolution, with minimum integration time */
#define APDS9250_RESOLUTION_20BIT (0 << 4) /* 20 bit resolution, 400ms integration time */
#define APDS9250_RESOLUTION_19BIT (1 << 4) /* 19 bit resolution, 200ms integration time */
#define APDS9250_RESOLUTION_18BIT (2 << 4) /* 18 bit resolution, 100ms integration time (default) */
#define APDS9250_RESOLUTION_17BIT (3 << 4) /* 17 bit resolution, 50ms integration time */
#define APDS9250_RESOLUTION_16BIT (4 << 4) /* 16 bit resolution, 25ms integration time */
#define APDS9250_RESOLUTION_13BIT (5 << 4) /* 13 bit resolution, 3.125ms integration time */
#define APDS9250_RESOLUTION_MASK  (7 << 4) /* Mask for resolution bits */

/* Sensor measurement rate -- I think it overrides the integration window above? */
#define APDS9250_MEAS_RATE_25MS   (0 << 0) /* 25ms integration time */
#define APDS9250_MEAS_RATE_50MS   (1 << 0) /* 50ms integration time */
#define APDS9250_MEAS_RATE_100MS  (2 << 0) /* 100ms integration time */
#define APDS9250_MEAS_RATE_200MS  (3 << 0) /* 200ms integration time */
#define APDS9250_MEAS_RATE_500MS  (4 << 0) /* 500ms integration time */
#define APDS9250_MEAS_RATE_1000MS (5 << 0) /* 1000ms integration time */
#define APDS9250_MEAS_RATE_2000MS (6 << 0) /* 2000ms integration time */
#define APDS9250_MEAS_RATE_DUP    (7 << 0) /* 2000ms integration time (duplicate) */
#define APDS9250_MEAS_RATE_MASK   (7 << 0) /* Mask for resolution bits */

#define APDS9250_LS_GAIN_1X       (0 << 0) /* Gain 1x */
#define APDS9250_LS_GAIN_3X       (1 << 0) /* Gain 3x */
#define APDS9250_LS_GAIN_6X       (2 << 0) /* Gain 6x */
#define APDS9250_LS_GAIN_9X       (3 << 0) /* Gain 9x */
#define APDS9250_LS_GAIN_18X      (4 << 0) /* Gain 18x */
#define APDS9250_LS_GAIN_MASK     (7 << 0) /* Gain mask */

typedef enum apds9250_chan {
    APDS9250_CHAN_ALS = 0,
    APDS9250_CHAN_RGB = 1
} apds9250_chan_t;

typedef enum apds9250_res {
    APDS9250_RES_20BIT = 0,
    APDS9250_RES_19BIT = 1,
    APDS9250_RES_18BIT = 2,
    APDS9250_RES_17BIT = 3,
    APDS9250_RES_16BIT = 4,
    APDS9250_RES_13BIT = 5
} apds9250_res_t;

typedef enum apds9250_rate {
    APDS9250_RATE_25MS = 0,
    APDS9250_RATE_50MS = 1,
    APDS9250_RATE_100MS = 2,
    APDS9250_RATE_200MS = 3,
    APDS9250_RATE_500MS = 4,
    APDS9250_RATE_1000MS = 5,
    APDS9250_RATE_2000MS = 6
} apds9250_rate_t;

typedef enum apds9250_gain {
    APDS9250_GAIN_1X = 0,
    APDS9250_GAIN_3X = 1,
    APDS9250_GAIN_6X = 2,
    APDS9250_GAIN_9X = 3,
    APDS9250_GAIN_18X = 4,
} apds9250_gain_t;

int apds9250_begin();
int apds9250_enable();
int apds9250_reset();
apds9250_chan_t apds9250_getMode();
apds9250_chan_t apds9250_setMode(apds9250_chan_t newMode);
apds9250_res_t apds9250_getResolution();
apds9250_res_t apds9250_setResolution(apds9250_res_t newRes);
apds9250_rate_t apds9250_getMeasRate();
apds9250_rate_t apds9250_setMeasRate(apds9250_rate_t newRate);
apds9250_gain_t apds9250_getGain();
apds9250_gain_t apds9250_setGain(apds9250_gain_t newGain);
void apds9250_setModeALS();
void apds9250_setModeRGB();

uint32_t apds9250_getRawRedData();
uint32_t apds9250_getRawGreenData();
uint32_t apds9250_getRawBlueData();
uint32_t apds9250_getRawIRData();
uint32_t apds9250_getRawALSData();

static const uint8_t apds9250_addr = APDS9250_I2C_ADDR;

void apds9250_getMeasureRateReg();
void apds9250_setMeasureRateReg();

apds9250_chan_t apds9250_mode;
apds9250_res_t apds9250_res;
apds9250_rate_t apds9250_meas_rate;
apds9250_gain_t apds9250_gain;

uint32_t apds9250_raw_r;
uint32_t apds9250_raw_g;
uint32_t apds9250_raw_b;
uint32_t apds9250_raw_als;
uint32_t apds9250_raw_ir;

uint32_t apds9250_read20(uint8_t reg);

apds9250_chan_t apds9250_modeFromReg(uint8_t reg_value);
apds9250_res_t apds9250_resFromReg(uint8_t reg_value);
apds9250_rate_t apds9250_measRateFromReg(uint8_t reg_value);
apds9250_gain_t apds9250_gainFromReg(uint8_t reg_value);

uint8_t apds9250_modeToReg(apds9250_chan_t newMode);
uint8_t apds9250_resToReg(apds9250_res_t newRes);
uint8_t apds9250_measRateToReg(apds9250_rate_t newMeasRate);
uint8_t apds9250_gainToReg(apds9250_gain_t newGain);

#endif //__APDS9250_H