/***************************************************
  This is an example for the HDC100x Humidity & Temp Sensor

  Designed specifically to work with the HDC1000 sensor from Adafruit
  ----> https://www.adafruit.com/products/2635

  These sensors use I2C to communicate, 2 pins are required to
  interface
 ****************************************************/

#include "apds9250.h"

typedef struct {
    uint32_t r;
    uint32_t g;
    uint32_t b;
    uint32_t ir;
    uint32_t als;
} apds9250_sensor_data_t;

int apds9250_found = 0;

apds9250_sensor_data_t apds9250_curr, apds9250_last;

int main()
{
    printf("APDS9250 test\n");

    if (apds9250_begin()) {
        apds9250_found = 1;
        printf("APDS9250 found!\n");
    } else {
        printf("APDS9250 not found! Check wiring?\n");
        return 1;
    }

    while (1) {
        if (apds9250_found) {

            printf("Resolution %d\t", (uint8_t)apds9250_getResolution());
            printf("Meas Rate %d\t", (uint8_t)apds9250_getMeasRate());
            printf("Gain %d\t\n", (uint8_t)apds9250_getGain());

            apds9250_setModeALS();
            usleep(500e3);
            apds9250_curr.als = apds9250_getRawALSData();
            apds9250_curr.ir = apds9250_getRawIRData();
            printf("ALS, IR: (%d, %d)\n", apds9250_curr.als, apds9250_curr.ir);

            apds9250_setModeRGB();
            usleep(500e3);
            apds9250_curr.r = apds9250_getRawRedData();
            apds9250_curr.g = apds9250_getRawGreenData();
            apds9250_curr.b = apds9250_getRawBlueData();
            printf("Red, Green, Blue: (%d, %d, %d)\n", apds9250_curr.r, apds9250_curr.g, apds9250_curr.b);
            printf("-------------------------------------\n");

        }

    }
}