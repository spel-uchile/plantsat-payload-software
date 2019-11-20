/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2019, Gustavo Dia Huenupan, g.hernan.diaz@ing.uchile.cl
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

#include "mcp9808.h"

int mcp9808_init(void)
{
    printf("mcp9808 Init");

    if (read16(MCP9808_REG_MANUF_ID) != 0x0054)
        return 0;
    if (read16(MCP9808_REG_DEVICE_ID) != 0x0400)
        return 0;

    write16(MCP9808_REG_CONFIG, 0x0);
    return 1;
}

float mcp9808_read_temp_c(void)
{
    float temp = 0;
    uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

    if (t != 0xFFFF)
    {
        temp = t & 0x0FFF;
        temp /= 16.0;
        if (t & 0x1000)
            temp -= 256;
    }
    else
    {
        printf("Error Sampling Temp");
        return 0;
    }
    printf("Sampled Temp: %.4f", temp);

    return temp;
}

float mcp9808_read_temp_f(void)
{
    float temp = 0;
    uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

    if (t != 0xFFFF)
    {
        temp = t & 0x0FFF;
        temp /= 16.0;
        if (t & 0x1000)
            temp -= 256;

        temp = temp * 9.0 / 5.0 + 32;
    }
    else
    {
        printf("Error Sampling Temp");
        return 0;
    }
    printf("Sampled Temp: %.4f", temp);

    return temp;
}

void mcp9808_shutdown_wake(uint8_t sw)
{
    uint16_t conf_shutdown;
    uint16_t conf_register = read16(MCP9808_REG_CONFIG);
    int ec;

    if (sw == 1)
    {
        conf_shutdown = conf_register | MCP9808_REG_CONFIG_SHUTDOWN;
        ec = write16(MCP9808_REG_CONFIG, conf_shutdown);
        printf("mcp9808 Temp shutdown result: %d", ec);
    }

    if (sw == 0)
    {
        conf_shutdown = conf_register & ~MCP9808_REG_CONFIG_SHUTDOWN;
        ec = write16(MCP9808_REG_CONFIG, conf_shutdown);
        printf("mcp9808 Temp wakeup result: %d", ec);
    }
}

int mcp9808_shutdown(void)
{
    mcp9808_shutdown_wake(1);
    return 1;
}

int mcp9808_wake(void) {
    mcp9808_shutdown_wake(0);
    usleep(250e3);
    return 1;
}

int mcp9808_get_resolution(void)
{
    uint8_t res = read8(MCP9808_REG_RESOLUTION);
    if (res > 3)
        return 0;
    return (int)res;
}

int mcp9808_set_resolution(uint8_t value)
{
    printf("Setting mcp9808 Temp Resolution: %d", value);
    int ec = write8(MCP9808_REG_RESOLUTION, value & 0x03);
    return ec ? 1:0;
}
