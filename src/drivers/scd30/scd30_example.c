/*
  Reading CO2, humidity and temperature from the SCD30
  By: Nathan Seidle
  SparkFun Electronics
  Date: May 22nd, 2018
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.
  Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/14751
  This example prints the current CO2 level, relative humidity, and temperature in C.
  Hardware Connections:
  If needed, attach a Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the device into an available Qwiic port
  Open the serial monitor at 9600 baud to see the output
*/

#include "scd30.h"

int main()
{
    printf("SDC30 Example\n");

    if (scd30_begin()) {
        scd30_found = 1;
        printf("SCD30 found!\n");
    } else {
        printf("SCD30 not found! Check wiring?\n");
        return 1;
    }

    while (1)
    {
        if (scd30_found)
        {
            if (scd30_dataAvailable())
            {
                printf("CO2: %d [ppm]\t", scd30_getCO2());
                printf("Temperature: %d [°C]\t", scd30_getTemperature());
                printf("Humidity: %d [%%]\t", scd30_getHumidity());
                printf("-------------------------------------\n");
            }
            else
            {
                printf("No data\n");
            }

        }
        usleep(500e3);
    }
}