/***************************************************
  This is an example for the VEML6070 Sensor
  These sensors use I2C to communicate
 ****************************************************/

#include "veml6070.h"

int main()
{
    printf("VEML6070 test\n");
    veml6070_begin(VEML6070_1_T);  // pass in the integration time constant
    //TODO: Check for correct initialization

    while (1) {
        printf("UV light level = %d [units]\n", veml6070_readUV());
        printf("----------------------------------------\n");
        usleep(1e3);
    }
}