/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2018, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
 *      Copyright 2018, Tomas Opazo Toro, tomas.opazo.t@gmail.com
 *      Copyright 2018, Matias Ramirez Martinez, nicoram.mt@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cmdSensors.h"

static const char* tag = "cmdSens";

void cmd_mcp9808_init(void)
{
    cmd_add("mcp_init", mcp_init, "", 0);
    cmd_add("mcp_get_temp", mcp_read_temp, "", 0);
    cmd_add("mcp_get_res", mcp_get_res, "", 0);
    cmd_add("mcp_set_res", mcp_set_res, "%d", 1);
}

int mcp_init(char *fmt, char *params, int nparams)
{
    int rc = mcp9808_init();
    return rc ? CMD_OK : CMD_FAIL;
}

int mcp_read_temp(char *fmt, char *params, int nparams) {
    float temp = mcp9808_read_temp_c();

    /* TODO: Save temp as telemetry data */
    LOGI(tag, "TEMP: %0.4f", temp)

    return temp != 0 ? CMD_OK : CMD_FAIL;
}

int mcp_get_res(char *fmt, char *params, int nparams) {
    int res = mcp9808_get_resolution();
    LOGI(tag, "RES: %d", res)

    return res ? CMD_OK : CMD_FAIL;
}

int mcp_set_res(char *fmt, char *params, int nparams) {
    int value;

    if(params == NULL || sscanf(params, fmt, &value) != nparams)
        return CMD_ERROR;

    LOGI(tag, "Setting mcp9808 Temp Resolution: %d", value);
    int ec = mcp9808_set_resolution((uint8_t)value);

    return ec ? CMD_OK : CMD_FAIL;
}
