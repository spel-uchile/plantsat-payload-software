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

#include "scd30.h"

/* Especially during obtaining the ACK BIT after a byte sent the SCD30 is using clock stretching  (but NOT only there)!
 * The need for clock stretching is described in the Sensirion_CO2_Sensors_SCD30_Interface_Description.pdf
 *
 * The default clock stretch (maximum wait time) on the ESP8266-library (2.4.2) is 230us which is set during _i2cPort->begin();
 * In the current implementation of the ESP8266 I2C driver there is NO error message when this time expired, while
 * the clock stretch is still happening, causing uncontrolled behaviour of the hardware combination.
 *
 * To set ClockStretchlimit() a check for ESP8266 boards has been added in the driver.
 *
 * With setting to 20000, we set a max timeout of 20mS (> 20x the maximum measured) basically disabling the time-out
 * and now wait for clock stretch to be controlled by the client.
 */

int scd30_begin(void)
{
    //_i2cPort->setClockStretchLimit(200000);

    scd30_found = 0;
    scd30_co2 = 0;
    scd30_temperature = 0;
    scd30_humidity = 0;

    scd30_co2HasBeenReported = 1;
    scd30_humidityHasBeenReported = 1;
    scd30_temperatureHasBeenReported = 1;

    //Check for device to respond correctly
    if(scd30_beginMeasuring(0) == 1) //Start continuous measurements
    {
        scd30_setMeasurementInterval(2); //2 seconds between measurements
        scd30_setAutoSelfCalibration(1); //Enable auto-self-calibration
        return 1;
    }

    return 0; //Something went wrong
}

//Returns the latest available CO2 level
//If the current level has already been reported, trigger a new read
float scd30_getCO2(void)
{
    if (scd30_co2HasBeenReported == 1) //Trigger a new read
        scd30_readMeasurement(); //Pull in new co2, humidity, and temp into global vars

    scd30_co2HasBeenReported = 1;

    return scd30_co2; //Cut off decimal as co2 is 0 to 10,000
}

//Returns the latest available humidity
//If the current level has already been reported, trigger a new read
float scd30_getHumidity(void)
{
    if (scd30_humidityHasBeenReported == 1) //Trigger a new read
        scd30_readMeasurement(); //Pull in new co2, humidity, and temp into global vars

    scd30_humidityHasBeenReported = 1;

    return scd30_humidity;
}

//Returns the latest available temperature
//If the current level has already been reported, trigger a new read
float scd30_getTemperature(void)
{
    if (scd30_temperatureHasBeenReported == 1) //Trigger a new read
        scd30_readMeasurement(); //Pull in new co2, humidity, and temp into global vars

    scd30_temperatureHasBeenReported = 1;

    return scd30_temperature;
}

//Enables or disables the ASC
void scd30_setAutoSelfCalibration(int enable)
{
    if (enable)
        scd30_sendCommand(COMMAND_AUTOMATIC_SELF_CALIBRATION, 1); //Activate continuous ASC
    else
        scd30_sendCommand(COMMAND_AUTOMATIC_SELF_CALIBRATION, 0); //Deactivate continuous ASC
}

//Set the forced recalibration factor. See 1.3.7.
//The reference CO2 concentration has to be within the range 400 ppm ≤ cref(CO2) ≤ 2000 ppm.
void scd30_setForcedRecalibrationFactor(uint16_t concentration)
{
    if(concentration < 400 || concentration > 2000) return; //Error check.
    scd30_sendCommand(COMMAND_SET_FORCED_RECALIBRATION_FACTOR, concentration);
}

//Set the temperature offset. See 1.3.8.
void scd30_setTemperatureOffset(float tempOffset)
{
    int16_t tickOffset = tempOffset * 100;
    scd30_sendCommand(COMMAND_SET_TEMPERATURE_OFFSET, tickOffset);
}

//Set the altitude compenstation. See 1.3.9.
void scd30_setAltitudeCompensation(uint16_t altitude)
{
    scd30_sendCommand(COMMAND_SET_ALTITUDE_COMPENSATION, altitude);
}

//Set the pressure compenstation. This is passed during measurement startup.
//mbar can be 700 to 1200
void scd30_setAmbientPressure(uint16_t pressure_mbar)
{
    if(pressure_mbar < 700 || pressure_mbar > 1200) pressure_mbar = 0; //Error check
    scd30_sendCommand(COMMAND_CONTINUOUS_MEASUREMENT, pressure_mbar);
}

//Begins continuous measurements
//Continuous measurement status is saved in non-volatile memory. When the sensor
//is powered down while continuous measurement mode is active SCD30 will measure
//continuously after repowering without sending the measurement command.
//Returns true if successful
int scd30_beginMeasuring(uint16_t pressureOffset)
{
    return(scd30_sendCommand(COMMAND_CONTINUOUS_MEASUREMENT, pressureOffset));
}

//Overload - no pressureOffset
//int scd30_beginMeasuring(void)
//{
//    return(scd30_beginMeasuring(0));
//}

//Sets interval between measurements
//2 seconds to 1800 seconds (30 minutes)
void scd30_setMeasurementInterval(uint16_t interval)
{
    scd30_sendCommand(COMMAND_SET_MEASUREMENT_INTERVAL, interval);
}

//Returns true when data is available
int scd30_dataAvailable()
{
    uint16_t response = scd30_readRegister(COMMAND_GET_DATA_READY);

    if (response == 1) return 1;
    return 0;
}

//Get 18 bytes from SCD30
//Updates global variables with floats
//Returns true if success
int scd30_readMeasurement()
{
    //Verify we have data from the sensor
    if (scd30_dataAvailable() == 0)
        return 0;

    uint32_t tempCO2 = 0;
    uint32_t tempHumidity = 0;
    uint32_t tempTemperature = 0;

    uint8_t reg_tx[1] = {COMMAND_READ_MEASUREMENT & 0xFF};
    i2c_write_n(SCD30_ADDRESS, COMMAND_READ_MEASUREMENT >> 8, reg_tx, 1);

    uint8_t reg_rx[18] = {0};
    i2c_read_from_n(SCD30_ADDRESS, reg_rx, 18);

    tempCO2 = reg_rx[0];
    tempCO2 <<= 8;
    tempCO2 |= reg_rx[1];
    tempCO2 <<= 8;
    tempCO2 |= reg_rx[3];
    tempCO2 <<= 8;
    tempCO2 |= reg_rx[4];

    tempTemperature = reg_rx[6];
    tempTemperature <<= 8;
    tempTemperature |= reg_rx[7];
    tempTemperature <<= 8;
    tempTemperature |= reg_rx[9];
    tempTemperature <<= 8;
    tempTemperature |= reg_rx[10];

    tempHumidity = reg_rx[12];
    tempHumidity <<= 8;
    tempHumidity |= reg_rx[13];
    tempHumidity <<= 8;
    tempHumidity |= reg_rx[15];
    tempHumidity <<= 8;
    tempHumidity |= reg_rx[16];

    //Now copy the uint32s into their associated floats
    memcpy(&scd30_co2, &tempCO2, sizeof(scd30_co2));
    memcpy(&scd30_temperature, &tempTemperature, sizeof(scd30_temperature));
    memcpy(&scd30_humidity, &tempHumidity, sizeof(scd30_humidity));

    //Mark our global variables as fresh
    scd30_co2HasBeenReported = 0;
    scd30_humidityHasBeenReported = 0;
    scd30_temperatureHasBeenReported = 0;

    return 1; //Success! New data available in globals.
}

//Gets two bytes from SCD30
uint16_t scd30_readRegister(uint16_t registerAddress)
{
    uint8_t reg_tx[1] = {registerAddress & 0xFF};
    i2c_write_n(SCD30_ADDRESS, registerAddress >> 8, reg_tx, 1);

    uint8_t reg_rx[2] = {0};
    i2c_read_from_n(SCD30_ADDRESS, reg_rx, 2);

    return ((uint16_t)reg_rx[0] << 8 | reg_rx[1]);
}

//Sends a command along with arguments and CRC
int scd30_sendCommand(uint16_t command, uint16_t arguments)
{
    uint8_t data[2];
    data[0] = arguments >> 8;
    data[1] = arguments & 0xFF;
    uint8_t crc = scd30_computeCRC8(data, 2); //Calc CRC on the arguments only, not the command

    uint8_t reg_tx[4] = {command & 0xFF, arguments >> 8, arguments & 0xFF, crc};
    i2c_write_n(SCD30_ADDRESS, command >> 8, reg_tx, 4);

    return 1;//Todo check for success
}

//Sends just a command, no arguments, no CRC
//int scd30_sendCommand(uint16_t command)
//{
//    uint8_t reg_tx[1] = {command & 0xFF};
//    i2c_write_n(SCD30_ADDRESS, command >> 8, reg_tx, 1);
//
//    return 1;//Todo check for success
//}

//Given an array and a number of bytes, this calculate CRC8 for those bytes
//CRC is only calc'd on the data portion (two bytes) of the four bytes being sent
//From: http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html
//Tested with: http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
//x^8+x^5+x^4+1 = 0x31
uint8_t scd30_computeCRC8(uint8_t data[], uint8_t len)
{
    uint8_t crc = 0xFF; //Init with 0xFF

    for (uint8_t x = 0 ; x < len ; x++)
    {
        crc ^= data[x]; // XOR-in the next input byte

        for (uint8_t i = 0 ; i < 8 ; i++)
        {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ 0x31);
            else
                crc <<= 1;
        }
    }

    return crc; //No output reflection
}