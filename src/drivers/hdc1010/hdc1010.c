/***************************************************
  This is a library for the HDC1000 Humidity & Temp Sensor

  Designed specifically to work with the HDC1000 sensor from Adafruit
  ----> https://www.adafruit.com/products/2635

  These sensors use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "hdc1010.h"

int hdc1010_begin(uint8_t addr)
{
    hdc1010_i2caddr = addr;
    hdc1010_reset();
    if (hdc1010_read16(HDC1000_MANUFID) != 0x5449) return 0;
    if (hdc1010_read16(HDC1000_DEVICEID) != 0x1000) return 0;
    return 1;
}

void hdc1010_reset(void)
{
    // reset, and select 14 bit temp & humidity
    uint16_t config = HDC1000_CONFIG_RST | HDC1000_CONFIG_MODE | HDC1000_CONFIG_TRES_14 | HDC1000_CONFIG_HRES_14;
    hdc1010_writeConfig(config);
}

float hdc1010_readTemperature(void)
{
    float temp = (hdc1010_read32(HDC1000_TEMP) >> 16);
    temp /= 65536;
    temp *= 165;
    temp -= 40;

    return temp;
}

float hdc1010_readHumidity(void)
{
    float hum = (hdc1010_read32(HDC1000_TEMP) & 0xFFFF);

    hum /= 65536;
    hum *= 100;

    return hum;
}

void hdc1010_drySensor(void)
{
    uint16_t origConfig = hdc1010_read16(HDC1000_CONFIG);

    // reset, heat up, and select 14 bit temp & humidity
    uint16_t newConfig =  HDC1000_CONFIG_RST | HDC1000_CONFIG_HEAT | HDC1000_CONFIG_MODE | HDC1000_CONFIG_TRES_14 | HDC1000_CONFIG_HRES_14;
    hdc1010_writeConfig(newConfig);
    usleep(15e3);

    // take 1000 readings & toss
    for ( int i = 0; i < 1000; i++){
        hdc1010_read32(HDC1000_TEMP);
        usleep(1e3);
    }

    origConfig |= HDC1000_CONFIG_RST;
    hdc1010_writeConfig(origConfig);
    usleep(15e3);
}

void hdc1010_writeConfig(uint16_t config)
{
    uint8_t reg_data[2] = {config>>8, config&0xFF};
    i2c_write_n(hdc1010_i2caddr, HDC1000_CONFIG, reg_data, 2);
    usleep(15e3);
}

/*********************************************************************/

uint16_t hdc1010_read16(uint8_t addr)
{
    uint8_t reg_data[2] = {0};
    i2c_read_n(hdc1010_i2caddr, addr, reg_data, 2, 50);
    uint16_t r = 0;
    r = reg_data[0];
    r <<= 8;
    r |= reg_data[1];
    return r;
}

uint32_t hdc1010_read32(uint8_t addr)
{
    uint8_t reg_data[4] = {0};
    i2c_read_n(hdc1010_i2caddr, addr, reg_data, 4, 50);
    uint32_t r = 0;
    r = reg_data[0];
    r <<= 8;
    r |= reg_data[1];
    r <<= 8;
    r |= reg_data[2];
    r <<= 8;
    r |= reg_data[3];
    return r;
}