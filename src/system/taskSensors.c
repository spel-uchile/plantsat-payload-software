/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2019, Carlos Gonzalez Cortes, carlgonz@uchile.cl
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

#include "taskSensors.h"

static const char *tag = "Sensors";

void taskSensors(void *param)
{
    LOGI(tag, "Started");
    portTick xLastWakeTime = osTaskGetTickCount();

    int i;
    cmd_t *cmd_init;
    cmd_t *cmd_get;
    char *init_cmds[] = {"temp_init", "hum_init", "uv_init", "rgb_init", "pres_init"};
    char *get_cmds[] = {"temp_get", "hum_get", "uv_get", "rgb_get", "pres_get"};

    for(i=0; i<5; i++)
    {
        cmd_init = cmd_get_str(init_cmds[i]);
        cmd_send(cmd_init);
    }

    while(1)
    {
        osTaskDelayUntil(&xLastWakeTime, 5000); //Suspend task
        for(i=0; i<5; i++)
        {
            cmd_get = cmd_get_str(get_cmds[i]);
            cmd_send(cmd_get);
        }
    }
}