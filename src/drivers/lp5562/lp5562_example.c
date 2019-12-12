
#include "lp5562.h"
#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv){

    int f;
    int index;

    uint8_t current_red = 10;
    uint8_t current_blue = 10;
    uint8_t addr = 0;

    while ((f = getopt(argc, argv, "a:r:b:")) != -1)

        switch (f) {

            //
            case 'a':
                addr = atoi(optarg);
                fprintf(stdout, "Address selected: %i\n", addr);
                break;

            case 'r':
                current_red = (uint8_t) ((atof(optarg))*10.0);
                fprintf(stdout, "Current for red led: %2.1f\n", current_red/10.0);
                break;

            case 'b':
                current_blue = (uint8_t) (((atof(optarg)))*10.0);
                fprintf(stdout, "Current for blue led: %2.1f\n", current_blue/10.0);
                break;

                // Display Help
                // TODO: completar ayuda
            case 'h':

                break;


            case '?':
                if (optopt == 'a' || optopt == 'r' || optopt == 'b')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 2;

            default:
                abort();
        }

    for (index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);


    fprintf(stdout, "Starting device...\n");
    lp5562_begin(addr);

    fprintf(stdout, "Setting red led...\n");
    lp5562_set_current_red(addr, current_red);
    if (current_red != 0) {
        lp5562_set_pwm_red(addr, 0xFF);
    }
    else {
        lp5562_set_pwm_red(addr, 0x00);
    }

    fprintf(stdout, "Setting blue led...\n");
    lp5562_set_current_blue(addr, current_blue);
    if (current_blue != 0) {
        lp5562_set_pwm_blue(addr, 0xFF);
    }
    else {
        lp5562_set_pwm_blue(addr, 0x00);
    }


}