/*
  This is a library written for the SCD30
  SparkFun sells these at its website: www.sparkfun.com
  Do you like this library? Help support SparkFun. Buy a board!
  https://www.sparkfun.com/products/14751

  Written by Nathan Seidle @ SparkFun Electronics, May 22nd, 2018

  The SCD30 measures CO2 with accuracy of +/- 30ppm.

  This library handles the initialization of the SCD30 and outputs
  CO2 levels, relative humidty, and temperature.

  https://github.com/sparkfun/SparkFun_SCD30_Arduino_Library

  Development environment specifics:
  Arduino IDE 1.8.5

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#ifndef __SCD30_H
#define __SCD30_H

#include "i2c.h"

//The default I2C address for the SCD30 is 0x61.
#define SCD30_ADDRESS 0x61

//Available commands

#define COMMAND_CONTINUOUS_MEASUREMENT 0x0010
#define COMMAND_SET_MEASUREMENT_INTERVAL 0x4600
#define COMMAND_GET_DATA_READY 0x0202
#define COMMAND_READ_MEASUREMENT 0x0300
#define COMMAND_AUTOMATIC_SELF_CALIBRATION 0x5306
#define COMMAND_SET_FORCED_RECALIBRATION_FACTOR 0x5204
#define COMMAND_SET_TEMPERATURE_OFFSET 0x5403
#define COMMAND_SET_ALTITUDE_COMPENSATION 0x5102

int scd30_begin(void);

int scd30_beginMeasuring(uint16_t pressureOffset);
//int scd30_beginMeasuring(void);

float scd30_getCO2(void);
float scd30_getHumidity(void);
float scd30_getTemperature(void);

void scd30_setMeasurementInterval(uint16_t interval);
void scd30_setAmbientPressure(uint16_t pressure_mbar);
void scd30_setAltitudeCompensation(uint16_t altitude);
void scd30_setAutoSelfCalibration(int enable);
void scd30_setForcedRecalibrationFactor(uint16_t concentration);
void scd30_setTemperatureOffset(float tempOffset);

int scd30_dataAvailable();
int scd30_readMeasurement();

int scd30_sendCommand(uint16_t command, uint16_t arguments);
//int scd30_sendCommand(uint16_t command);

uint16_t scd30_readRegister(uint16_t registerAddress);

uint8_t scd30_computeCRC8(uint8_t data[], uint8_t len);

//Variables
//Global main datums
int scd30_found;
float scd30_co2;
float scd30_temperature;
float scd30_humidity;

//These track the staleness of the current data
//This allows us to avoid calling readMeasurement() every time individual datums are requested
int scd30_co2HasBeenReported;
int scd30_humidityHasBeenReported;
int scd30_temperatureHasBeenReported;

#endif //__SCD30_H