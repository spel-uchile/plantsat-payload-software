/***************************************************
  This is an example for the HDC100x Humidity & Temp Sensor

  Designed specifically to work with the HDC1000 sensor from Adafruit
  ----> https://www.adafruit.com/products/2635

  These sensors use I2C to communicate, 2 pins are required to
  interface
 ****************************************************/

#include "hdc1010.h"

int main()
{
    printf("HDC1010 test\n");
    if (!hdc1010_begin(HDC1000_I2CADDR)) {
        printf("Could not find a valid HDC1010 sensor, check wiring!\n");
        while (1);
    }

    while (1) {
        printf("HDC1010 Temperature = %f °C\n", hdc1010_readTemperature());
        printf("HDC1010 Humidity = %f [%%]\n", hdc1010_readHumidity());
        printf("----------------------------------------\n");
        usleep(500e3);
    }
}