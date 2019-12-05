
/**************************************************************************/
/*!
This is a demo for the Adafruit MCP9808 breakout
----> http://www.adafruit.com/products/1782
Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!
*/
/**************************************************************************/

#include "mcp9808.h"

int main()
{
    printf("MCP9808 Example\n");

    if (mcp9808_init()) {
        printf("MCP9808 found!\n");
    } else {
        printf("MCP9808 not found! Check wiring?\n");
        return 1;
    }

    mcp9808_set_resolution(3);
    // Mode Resolution SampleTime
    //  0    0.5°C       30 ms
    //  1    0.25°C      65 ms
    //  2    0.125°C     130 ms
    //  3    0.0625°C    250 ms

    while (1)
    {
        mcp9808_wake();
        printf("Resolution in mode: %d\t", mcp9808_get_resolution());
        printf("Temperature: %f [°C]\n", mcp9808_read_temp_c());
        usleep(500e3);
        mcp9808_shutdown();
        printf("-------------------------------------\n");
        usleep(200e3);
    }
}
