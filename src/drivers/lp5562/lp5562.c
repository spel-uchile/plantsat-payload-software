#include "lp5562.h"

// resets the lp5562 - all register to default values
int lp5562_reset(uint8_t addr, uint8_t reset){
    return i2c_write_n(addr, LP5562_REG_RESET, &reset, 1);
}

// sets the pwm from 0% (0x00) to 100% (0xFF) for red led
int lp5562_set_pwm_red(uint8_t addr, uint8_t pwm){
    return i2c_write_n(addr, LP5562_REG_R_PWM, &pwm, 1);
}

// sets the pwm from 0% (0x00) to 100% (0xFF) for blue led
int lp5562_set_pwm_blue(uint8_t addr, uint8_t pwm){
return i2c_write_n(addr, LP5562_REG_B_PWM, &pwm, 1);
}

// sets the current from 0.0 mA (0) to 25.5 ma (255) for red led
int lp5562_set_current_red(uint8_t addr, uint8_t current){
    return i2c_write_n(addr, LP5562_REG_R_CURRENT, &current, 1);
}

// sets the current from 0.0 mA (0) to 25.5 ma (255) for blue led
int lp5562_set_current_blue(uint8_t addr, uint8_t current){
    return i2c_write_n(addr, LP5562_REG_B_CURRENT, &current, 1);
}

// Set operation mode
// Used mainly to TURN OFF Engines 1, 2 and 3
int lp5562_set_opmode(uint8_t addr, uint8_t mode){
    return i2c_write_n(addr, LP5562_REG_OP_MODE, &mode, 1);
}


int lp5562_set_cfg(uint8_t addr, uint8_t cfg){
    return i2c_write_n(addr, LP5562_REG_CONFIG, &cfg, 1);
}

int lp5562_set_mapping(uint8_t addr, uint8_t map){
    return i2c_write_n(addr, LP5562_LED_MAP, &map, 1);
}

int lp5562_chip_en(uint8_t addr, uint8_t enable){
    return i2c_write_n(addr, LP5562_REG_ENABLE, &enable, 1);
}

int lp5562_begin(uint8_t addr){

    lp5562_reset(addr, LP5562_RESET);

    lp5562_chip_en(addr, LP552_DEFAULT_CHIP_ENABLE);
    usleep(1000);

    lp5562_set_cfg(addr, LP5562_DEFAULT_CFG);

    lp5562_set_opmode(addr, LP5562_DEFAULT_OP_MODE);
    lp5562_set_mapping(addr, LP5562_DEFAULT_MAP);

    lp5562_set_current_red(addr, 0);
    lp5562_set_current_blue(addr, 0);

    lp5562_set_pwm_red(addr, 0);
    lp5562_set_pwm_blue(addr, 0);


    // disable power save mode
    //lp5562_set_cfg(addr, 0x01);

    return 0;
}


