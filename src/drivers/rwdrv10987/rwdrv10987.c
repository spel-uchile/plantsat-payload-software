/**
 * @file rwdrv10987.c
 * @mainpage RW I2C Command Interface
 * @author Gustavo Diaz H.
 * @date 23 Dec 2019
 * @brief Define basic set_speed, get_speed and get_current commands
 *
 * @copyright GPL
 *
 */

#include "rwdrv10987.h"

/**************************************************************************/
/*!
    @brief  setup and initialize communication with the hardware
*/
/**************************************************************************/
int rwdrv10987_init(void)
{
    rwdrv10987_set_speed(MOTOR1_ID, 0);
    rwdrv10987_set_speed(MOTOR2_ID, 0);
    rwdrv10987_set_speed(MOTOR3_ID, 0);
    return 1;
}

/**************************************************************************/
/*!
    @brief get speed of the specified motor from the drv10987 driver
    @param motor_id:[1-3]
*/
/**************************************************************************/
uint16_t rwdrv10987_get_speed(uint8_t motor_id)
{
    //send sample speed cmd
    uint8_t reg_tx[2] = {0x00, 0x00};
    if(motor_id == MOTOR1_ID){
        i2c_write_n(BIuC_ADDR, SAMPLE_SPEED_CODE_MOTOR1, reg_tx, 2);
    }
    else if(motor_id == MOTOR2_ID){
        i2c_write_n(BIuC_ADDR, SAMPLE_SPEED_CODE_MOTOR2, reg_tx, 2);
    }
    else if(motor_id == MOTOR3_ID){
        i2c_write_n(BIuC_ADDR, SAMPLE_SPEED_CODE_MOTOR3, reg_tx, 2);
    }
    else{
        printf("[RWDRV10987:Error]: Bad id");
        return -1;
    }
    usleep(1e3);
    //read data regs
    uint8_t reg_rx[2];
    i2c_read_from_n(BIuC_ADDR, reg_rx, 2);

    uint16_t speed = (reg_rx[0]<<8) | reg_rx[1];
    usleep(100e3);
    return speed;
}

/**************************************************************************/
/*!
    @brief get current of the specified motor from the drv10987 driver
    @param motor_id:[1-3]
*/
/**************************************************************************/
float rwdrv10987_get_current(uint8_t motor_id)
{
    //send sample speed cmd
    uint8_t reg_tx[2] = {0x00, 0x00};
    if(motor_id == MOTOR1_ID){
        i2c_write_n(BIuC_ADDR, SAMPLE_CURRENT_CODE_MOTOR1, reg_tx, 2);
    }
    else if(motor_id == MOTOR2_ID){
        i2c_write_n(BIuC_ADDR, SAMPLE_CURRENT_CODE_MOTOR2, reg_tx, 2);
    }
    else if(motor_id == MOTOR3_ID){
        i2c_write_n(BIuC_ADDR, SAMPLE_CURRENT_CODE_MOTOR3, reg_tx, 2);
    }
    else{
        printf("[RWDRV10987:Error]: Bad id");
        return -1;
    }
    //read data regs
    uint8_t reg_rx[2];
    i2c_read_from_n(BIuC_ADDR, reg_rx, 2);

    uint16_t current_aux = ((reg_rx[0]&0x07)<<8) | reg_rx[1];
    float current = 3000*(current_aux-1023)/2048.0; //[mA]

    return current;
}

/**************************************************************************/
/*!
    @brief set speed of the specified motor through the drv10987 driver
    @param motor_id:[1-3]
*/
/**************************************************************************/
int8_t rwdrv10987_set_speed(uint8_t motor_id, uint16_t speed)
{
    int8_t res;
    uint8_t reg_tx[2] = {speed & 0xff, speed >> 8};
    if(motor_id == MOTOR1_ID)
    {
        res = i2c_write_n(BIuC_ADDR, SET_SPEED_CODE_MOTOR1, reg_tx, 2);
    }
    else if(motor_id == MOTOR2_ID)
    {
        res = i2c_write_n(BIuC_ADDR, SET_SPEED_CODE_MOTOR2, reg_tx, 2);
    }
    else if(motor_id == MOTOR3_ID)
    {
        res = i2c_write_n(BIuC_ADDR, SET_SPEED_CODE_MOTOR3, reg_tx, 2);
    }
    else
    {
        printf("[RWDRV10987:Error]: Bad id");
        return -1;
    }
    usleep(150e3);
    return res;
}