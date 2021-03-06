/**
 * @file  cmdSensors.h
 * @author Gustavo Díaz H - g.hernan.diaz@ing.uchile.cl
 * @author Carlos Gonzalez C - carlgonz@uchile.cl
 * @date 2019
 * @copyright GNU GPL v3
 *
 * This header have definitions of commands related to the PlantSat payload
 * sensors
 */

#ifndef _CMD_SENS_H
#define _CMD_SENS_H

#include <stdint.h>
#include <unistd.h>

#include "mcp9808.h"
#include "hdc1010.h"
#include "veml6070.h"
#include "apds9250.h"
#include "bmp3_interface.h"
#include "scd30.h"

#include "config.h"

#include "os/os.h"
#include "osDelay.h"

#include "repoCommand.h"
#include "taskSensors.h"


void cmd_sensors_init(void);

/**
 * Control sensors state machine task
 * @param fmt "%u %u %d"
 * @param params <action> <step> <nsamples>
 * action: Next action ACT_PAUSE= 0, ACT_START=1, ACT_STAND_BY=2,
 * step: Seconds between sates update (or samples)
 * nsamples: Number of samples to take before go to ACT_STAND_BY. Use -1
 *           to run for ever.
 * @param nparams 3
 * @code
 *
 * //Start machine, every 2 seconds, 10 samples max.
 * set_state 1 2 10
 * //Start machine, every 1 seconds, for ever
 * set_state 1 1 -1
 * //Stpo machine [keep 2 seconds, 10 samples]
 * set_state 2 2 10
 *
 * @return
 */
int set_state(char *fmt, char *params, int nparams);

/**
 * MCP9808 Temperature Sensor
 */
int mcp_init(char *fmt, char *params, int nparams);
int mcp_read_temp(char *fmt, char *params, int nparams);
int mcp_get_res(char *fmt, char *params, int nparams);
int mcp_set_res(char *fmt, char *params, int nparams);

/**
 * HDC1010 Humidity Sensor
 */
int hdc_init(char *fmt, char *params, int nparams);
int hdc_read(char *fmt, char *params, int nparams);

/**
 * VEML6070 UV Sensor
 */
int veml_init(char *fmt, char *params, int nparams);
int veml_get(char *fmt, char *params, int nparams);

/**
 * APDS9250 RGB and IR Sensor
 */
/**
 * Light sensor read mode
 */
 typedef enum veml_type
 {
     veml_als, ///< ALS mode
     veml_rgs, ///< RGB mode
     veml_all  ///< ALS and RGB mode
 } vmel_type_t;

 /**
  * Light sensor data
  */
typedef struct {
    uint32_t r;
    uint32_t g;
    uint32_t b;
    uint32_t ir;
    uint32_t als;
} apds9250_sensor_data_t;

/**
 * Init light sensor data
 * @param fmt ""
 * @param params ""
 * @param nparams 0
 * @return CMD_OK or CMD_FAIL
 */
int apds_init(char *fmt, char *params, int nparams);

/**
 * Read light sensor data as ALS, RGB or both.
 *
 * @param fmt Parameters format: %d or empty
 * @param params Number or parameters 1 or 0
 * @param nparams Parameters as string "0" | "1" | "2" | ""
 *      0: read mode ALS (ALS and IR)
 *      1: read mode RGB
 *      2: read mode ALS and RGB
 *     "": default to 2, ALS and RGB
 * @return CMD_OK
 */
int apds_get(char *fmt, char *params, int nparams);

/**
 * BMP388 Pressure Sensor
 */
int bmp_init(char *fmt, char *params, int nparams);
int bmp_get(char *fmt, char *params, int nparams);

/**
 * SCD30 C02 Sensor
 */
int scd_init(char *fmt, char *params, int nparams);
int scd_get(char *fmt, char *params, int nparams);

#endif /* _CMD_SENS_H */
