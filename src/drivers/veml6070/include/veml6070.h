/*!
 * @file [adapted from] Adafruit_VEML6070.h
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
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */
#include "i2c.h"

// really unusual way of getting data, your read from two different addrs!

#define VEML6070_ADDR_H     (0x39) ///< High address
#define VEML6070_ADDR_L     (0x38) ///< Low address
#define VEML6070_ADDR_ARA   (0x0C) ///< Alert Resp Address (read to clear condition)


/**************************************************************************/
/*!
    @brief  integration time definitions
*/
/**************************************************************************/
typedef enum veml6070_integrationtime {
    VEML6070_HALF_T,
    VEML6070_1_T,
    VEML6070_2_T,
    VEML6070_4_T,
} veml6070_integrationtime_t;


/**************************************************************************/
/*!
    @brief  Class that stores state and functions for interacting with VEML6070 sensor IC
*/
/**************************************************************************/
void veml6070_begin(veml6070_integrationtime_t itime);
void veml6070_setInterrupt(int state, int level);
int veml6070_clearAck();
uint16_t veml6070_readUV(void);
void veml6070_waitForNext(void);
void veml6070_sleep(int state);

void veml6070_writeCommand(void);

typedef union {
    uint8_t reg;
    struct {
        uint8_t SD:1;
        uint8_t :1;
        uint8_t IT:2;
        uint8_t ACK_THD:1;
        uint8_t ACK:1;
    } bit;

} veml6070_commandRegister_t;

veml6070_commandRegister_t veml6070_commandRegister;