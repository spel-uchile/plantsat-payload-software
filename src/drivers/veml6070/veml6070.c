/*!
 * @file [Adapted from] Adafruit_VEML6070.cpp
 *
 * @mainpage Adafruit VEML6070 UV sensor
 *
 * @section intro_sec Introduction
 *
 * Designed specifically to work with the VEML6070 sensor from Adafruit
 * ----> https://www.adafruit.com/products/2899
 *
 * These sensors use I2C to communicate, 2 pins (SCL+SDA) are required
 * to interface with the breakout.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section dependencies Dependencies
 *
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#include "veml6070.h"
//#define VEML6070_DEBUG

/**************************************************************************/
/*!
    @brief  setup and initialize communication with the hardware
    @param itime the integration time to use for the data
    @param twoWire Optional pointer to the desired TwoWire I2C object. Defaults to &Wire
*/
/**************************************************************************/
void veml6070_begin(veml6070_integrationtime_t itime) {
    #ifdef VEML6070_DEBUG
        printf("begin\n");
    #endif
    //default setting
    veml6070_commandRegister.reg = 0x02;
    veml6070_commandRegister.bit.IT = itime;

    veml6070_clearAck();
    veml6070_writeCommand();
}

/**************************************************************************/
/*!
    @brief  Set the threshold-based interrupt feature
    @param  state true to enable, false to disable
    @param  level 1 for threshold value of 145, 0 for 102 (default)
*/
/**************************************************************************/
void veml6070_setInterrupt(int state, int level) {
    veml6070_commandRegister.bit.ACK = state;
    veml6070_commandRegister.bit.ACK_THD = level;

    veml6070_clearAck();
    veml6070_writeCommand();
}

/**************************************************************************/
/*!
    @brief  Clear possible interrupt state (ACK active) by reading register
            If set, MUST be cleared before device will respond at other
            I2C addresses.
            See datasheet rev 1.7, p.7, and app note p. 13 (example code)
    @return True if ACK was active (interrupt triggered)
*/
/**************************************************************************/
int veml6070_clearAck() {
    printf("clearAck\n");
    uint8_t reg_data[1] = {0};
    return i2c_read_from(VEML6070_ADDR_ARA, reg_data);
}

/**************************************************************************/
/*!
    @brief  read the chips UV sensor
    @return the UV reading as a 16 bit integer
*/
/**************************************************************************/
uint16_t veml6070_readUV() {
    #ifdef VEML6070_DEBUG
        printf("readUV\n");
    #endif

    veml6070_waitForNext();

    uint8_t reg_data1[1] = {0};
    int ec = i2c_read_from(VEML6070_ADDR_H, reg_data1);
    if (ec) return -1;

    uint8_t reg_data2[1] = {0};
    int ec2 = i2c_read_from(VEML6070_ADDR_L, reg_data2);
    if (ec2) return -1;

    uint16_t uvi = 0;
    uvi = reg_data1[0];
    uvi <<= 8;
    uvi |= reg_data2[0];

    return uvi;
}

/**************************************************************************/
/*!
    @brief  wait for one integration period (with ~10% clock error margin)
*/
/**************************************************************************/
void veml6070_waitForNext() {
    #ifdef VEML6070_DEBUG
        printf("waitForNext\n");
    #endif
    // Map the integration time code to the correct multiple (datasheet p. 8)
    // {0 -> 1, 1 -> 2; 2 -> 4; 3 -> 8}
    uint8_t itCount = 1;
    for (uint8_t i = veml6070_commandRegister.bit.IT; i > 0; i--) { itCount *= 2; }

    for (uint8_t i = 0; i < itCount; i++) {
        usleep(125e3);  // Depends on RSET = 270K, note actual time is shorter
        // than 62.5ms for RSET = 300K in datasheet table
    }
}

/**************************************************************************/
/*!
    @brief  enter or exit sleep (shutdown) mode. While in sleep mode
      the chip draws ~1uA
    @param state true to enter sleep mode, false to exit
*/
/**************************************************************************/
void veml6070_sleep(int state) {
    veml6070_commandRegister.bit.SD = state;

    veml6070_writeCommand();
}


/**************************************************************************/
/*!
    @brief write current internal veml6070_commandRegister value to device
*/
/**************************************************************************/
void veml6070_writeCommand() {
    #ifdef VEML6070_DEBUG
        printf("writeCommand\n");
        printf("VEML6070_ADDR_L = %d\n", VEML6070_ADDR_L);
        printf("veml6070_commandRegister.reg = %d\n", veml6070_commandRegister.reg);
    #endif
    i2c_write_addr(VEML6070_ADDR_L, veml6070_commandRegister.reg);
}