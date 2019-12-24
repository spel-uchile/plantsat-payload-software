/**
* @file rwdrv10987_example.c
* @mainpage RW I2C Command Interface
* @author Gustavo Diaz H.
* @date 23 Dec 2019
* @brief This is an example for the DRV10987 motor driver
*
* @copyright GPL
*
*/

#include "rwdrv10987.h"

int main()
{
    printf("RW-DRV10987 test\n");
//    rwdrv10987_init(); //TODO: Check for correct initialization

//    rwdrv10987_set_speed(MOTOR1_ID, 0);
    rwdrv10987_set_speed(MOTOR2_ID, 90);
    rwdrv10987_set_speed(MOTOR3_ID, 400);


    while (1) {
//        uint16_t speed1 = rwdrv10987_get_speed(MOTOR1_ID);
        uint16_t speed2 = rwdrv10987_get_speed(MOTOR2_ID);
        uint16_t speed3 = rwdrv10987_get_speed(MOTOR3_ID);

//        printf("Motor 1 Speed = %d [RPM]\n", speed1);
        printf("Motor 2 Speed = %d [RPM]\n", speed2);
        printf("Motor 3 Speed = %d [RPM]\n", speed3);

//        uint16_t current1 = rwdrv10987_get_current(MOTOR1_ID);
        uint16_t current2 = rwdrv10987_get_current(MOTOR2_ID);
        uint16_t current3 = rwdrv10987_get_current(MOTOR3_ID);

//        printf("Motor 1 Speed = %d [RPM]\n", speed1);
        printf("Motor 2 Current = %d [mA]\n", current2);
        printf("Motor 3 Current = %d [mA]\n", current3);

        printf("----------------------------------------\n");
        usleep(100e3);
    }
}