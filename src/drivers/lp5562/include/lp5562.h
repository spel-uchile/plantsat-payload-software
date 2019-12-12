#include "i2c.h"


#define LP5562_REG_ENABLE		0x00
#define LP5562_LOG_PWM          0x80
#define LP5562_CHIP_EN          0x40
#define LP552_DEFAULT_CHIP_ENABLE (LP5562_CHIP_EN)

#define LP5562_REG_OP_MODE		0x01
#define LP5562_DEFAULT_OP_MODE  0x00

#define LP5562_REG_B_PWM		0x02
#define LP5562_REG_G_PWM		0x03
#define LP5562_REG_R_PWM		0x04

#define LP5562_REG_B_CURRENT	0x05
#define LP5562_REG_G_CURRENT	0x06
#define LP5562_REG_R_CURRENT	0x07

#define LP5562_REG_CONFIG		0x08

#define LP5562_PWM_256			0x00
#define LP5562_PWM_558          0x40

#define LP5562_PWRSAVE_EN		0x20

#define LP5562_CLK_INT			0x01	/* Internal clock */
#define LP5562_CLK_EXT          0x00
#define LP5562_CLK_AUTO         0x02

#define LP5562_DEFAULT_CFG		(LP5562_PWM_558 | LP5562_CLK_INT)

#define LP5562_REG_RESET		0x0D
#define LP5562_RESET            0xFF

#define LP5562_REG_W_PWM		0x0E
#define LP5562_REG_W_CURRENT	0x0F

#define LP5562_LED_MAP          0x70
#define LP5562_DEFAULT_MAP      0x00



int lp5562_reset(uint8_t addr, uint8_t reset);
int lp5562_set_pwm_red(uint8_t addr, uint8_t pwm);
int lp5562_set_pwm_blue(uint8_t addr, uint8_t pwm);
int lp5562_set_current_red(uint8_t addr, uint8_t current);
int lp5562_set_current_blue(uint8_t addr, uint8_t current);
int lp5562_set_opmode(uint8_t addr, uint8_t mode);
int lp5562_set_cfg(uint8_t addr, uint8_t cfg);
int lp5562_set_mapping(uint8_t addr, uint8_t map);
int lp5562_chip_en(uint8_t addr, uint8_t enable);

int lp5562_begin(uint8_t addr);