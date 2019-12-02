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

#include "config.h"

#include "os/os.h"
#include "osDelay.h"

#include "repoCommand.h"

void cmd_sensors_init(void);

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

#endif /* _CMD_SENS_H */
