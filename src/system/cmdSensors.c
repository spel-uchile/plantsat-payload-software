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

void cmd_sensors_init(void)
{
    /* MCP9808 Temperature sensor commands */
    cmd_add("temp_init", mcp_init, "", 0);
    cmd_add("temp_get", mcp_read_temp, "", 0);

    cmd_add("hum_init", hdc_init, "%i", 1);
    cmd_add("hum_get", hdc_read, "", 0);

    cmd_add("uv_init", veml_init, "", 0);
    cmd_add("uv_get", veml_get, "", 0);

    cmd_add("rbg_init", apds_init, "", 0);
    cmd_add("rgb_get", apds_get, "%d", 1);

    cmd_add("pres_init", bmp_init, "", 0);
    cmd_add("pres_get", bmp_get, "", 0);
}

int mcp_init(char *fmt, char *params, int nparams)
{
    int rc = mcp9808_init();
    if(!rc)
        return CMD_FAIL;

    rc = mcp9808_set_resolution(mcp9808_res_00625);
    if(!rc)
        return CMD_FAIL;

    return CMD_OK;
}

int mcp_read_temp(char *fmt, char *params, int nparams) {
    mcp9808_wake();
    float temp = mcp9808_read_temp_c();
    mcp9808_shutdown();

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

/**
 * HDC1010 Humidity Sensor
 */
int hdc_init(char *fmt, char *params, int nparams)
{
    int i2c_addr;
    if(params == NULL || sscanf(params, fmt, &i2c_addr) != nparams)
        i2c_addr = HDC1000_I2CADDR;

    LOGI(tag, "Init HDC1010 with addr: %#x...", (uint8_t)i2c_addr);
    int rc = hdc1010_begin((uint8_t)i2c_addr);
    return rc ? CMD_OK : CMD_FAIL;
}

int hdc_read(char *fmt, char *params, int nparams)
{
    float temp, hum;
    temp = hdc1010_readTemperature();
    hum = hdc1010_readHumidity();

    LOGI(tag, "HDC1010:  %.4f °C, %.4f %%.", temp, hum);
    return CMD_OK;
}

/**
 * VEML6070 UV Sensor
 */
int veml_init(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Init VEML6070. Int. time: %d", VEML6070_1_T);
    veml6070_begin(VEML6070_1_T);
    return CMD_OK;
}

int veml_get(char *fmt, char *params, int nparams)
{
    int32_t uv = veml6070_readUV();
    if(uv == -1)
    {
        LOGE(tag, "Error reading VEML6070");
        return CMD_FAIL;
    }
    else
    {
        LOGI(tag, "VEML6070 UV: %d", uv);
        return CMD_OK;
    }
}

/**
 * APDS9250 Light Sensor
 */
int apds_init(char *fmt, char *params, int nparams)
{
    int rc = apds9250_begin();
    return rc ? CMD_OK : CMD_FAIL;
}

int apds_get(char *fmt, char *params, int nparams)
{
    int mode;
    apds9250_sensor_data_t data;

    if(params == NULL || sscanf(params, fmt, &mode) != nparams)
        mode = veml_all;

    LOGI(tag, "Reading light sensors channel: %d", mode);
    if(mode == veml_als || mode == veml_all)
    {
        apds9250_setModeALS();
        osDelay(500);
        data.als = apds9250_getRawALSData();
        data.ir = apds9250_getRawIRData();
        LOGI(tag, "Light Sensor ALS: %d, IR: %d", data.als, data.ir);
    }
    else if (mode == veml_rgs || mode == veml_all)
    {
        apds9250_setModeRGB();
        osDelay(500);
        data.r = apds9250_getRawRedData();
        data.g = apds9250_getRawGreenData();
        data.b = apds9250_getRawBlueData();
        LOGI(tag, "Red, Green, Blue: (%d, %d, %d)", data.r, data.g, data.b);
    }

    return CMD_OK;
}

/**
 * BMP388 Pressure Sensor
 */
int bmp_init(char *fmt, char *params, int nparams)
{
    int rc = bmp3_begin(0x76);
    if(!rc)
        return CMD_FAIL;

    bmp3_setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp3_setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp3_setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    //bmp3_setOutputDataRate(BMP3_ODR_50_HZ);

    return CMD_OK;
}

int bmp_get(char *fmt, char *params, int nparams)
{
    bmp3_data_t data;
    double alt;

    int rc = bmp3_performReading2(&data);
    if(rc)
    {
        alt = bmp3_readAltitude2(&data);
        LOGI(tag, "T°: %f °C, Press: %f hPa, Alt: %f m.", data.temperature, data.pressure, alt)
    }
    else
        return CMD_FAIL;
}