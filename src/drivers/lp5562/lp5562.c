#include "lp5562.h"


int lp5562_reset(uint8_t addr, uint8_t reset){
    return i2c_write_n(addr, LP5562_REG_RESET, &reset, 1);
}

int lp5562_set_pwm_red(uint8_t addr, uint8_t pwm){
    return i2c_write_n(addr, LP5562_REG_R_PWM, &pwm, 1);
}

int lp5562_set_pwm_blue(uint8_t addr, uint8_t pwm){
return i2c_write_n(addr, LP5562_REG_B_PWM, &pwm, 1);
}

int lp5562_set_current_red(uint8_t addr, uint8_t current){
    return i2c_write_n(addr, LP5562_REG_R_CURRENT, &current, 1);
}

int lp5562_set_current_blue(uint8_t addr, uint8_t current){
    return i2c_write_n(addr, LP5562_REG_B_CURRENT, &current, 1);
}

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

    lp5562_reset(addr, 0xFF);

    lp5562_chip_en( addr, 0x40);
    usleep(1000);

    lp5562_set_cfg(addr, 0x43);

    lp5562_set_opmode(addr, 0x00);
    lp5562_set_mapping(addr, 0x00);

    lp5562_set_current_red(addr, 0);
    lp5562_set_current_blue(addr, 0);

    lp5562_set_pwm_red(addr, 0);
    lp5562_set_pwm_blue(addr, 0);


    // disable power save mode
    //lp5562_set_cfg(addr, 0x01);

    return 0;
}


