#include "Aht1X.h"
#include "Wire.h"
#include "Helpers.h"

Aht1X::Aht1X()
{
}

/// <summary>
/// The CTOR. Sets up the default address and type
/// </summary>
/// <param name="address">The I2C address.</param>
/// <param name="sensor_type">The Type of sensor used, ie; AHT10</param>
Aht1X::Aht1X(const SENSOR_ADDRESS address, const SENSOR_TYPE sensor_type)
{
	address_ = address;
	sensor_type_ = sensor_type;
	if (Wire.available() == 0)
	{
		Wire.begin();
	}
}

/// <summary>
/// Initialize I2C & configure the sensor, call this function before
/// doing anything else
/// </summary>
/// <returns></returns>
bool Aht1X::Begin()
{
	Common::Sleep(AHT10_POWER_ON_DELAY); //wait for sensor to initialize 
	SetNormalMode(); //one measurement+sleep mode
	return enableFactoryCalCoefficient(); //load factory calibration co-eff
}

/// <summary>
///  Read temperature as  degrees C
/// </summary>
/// <param name="read_i2c">force to read data to raw_data_buffer_ & error handler</param>
/// <returns>The temperature as a float</returns>
float Aht1X::ReadTemperature(const bool read_i2c)
{
	/*
	- temperature range      -40C..+80C
	- temperature resolution 0.01C
	- temperature accuracy   +/- 0.3C
	*/

	if (static_cast<int>(read_i2c) == AHT10_FORCE_READ_DATA)
	{
		const uint8_t raw = readRawData();
		if (raw == AHT10_ERROR)
		{
			return AHT10_ERROR;
		}
	}

	if (raw_data_buffer_[0] == AHT10_ERROR)
	{
		return AHT10_ERROR; //error handler, collision on I2C bus
	}

	unsigned long temp_data = raw_data_buffer_[3] & 0x0F; //20-bit raw temperature data
	temp_data <<= 8;
	temp_data |= raw_data_buffer_[4];
	temp_data <<= 8;
	temp_data |= raw_data_buffer_[5];
	const float temp = convertTemp(static_cast<float>(temp_data));
	return temp;
}

/// <summary>
///   Read relative humidity, %
/// </summary>
/// <param name="read_i2c">force to read data to raw_data_buffer_ & error handler</param>
/// <returns>the RD as a float</returns>
int Aht1X::ReadHumidity(const bool read_i2c)
{
	/*
	- prolonged exposure for 60 hours at humidity > 80% can lead to a
	  temporary drift of the signal +3%. Sensor slowly returns to the
	  calibrated state at normal operating conditions.
	- relative humidity range      0%..100%
	- relative humidity resolution 0.024%
	- relative humidity accuracy   +/- 2%
	*/
	if (static_cast<int>(read_i2c) == AHT10_FORCE_READ_DATA)
	{
		if (readRawData() == AHT10_ERROR)
		{
			return AHT10_ERROR;
		}
	}

	if (raw_data_buffer_[0] == AHT10_ERROR)
	{
		return AHT10_ERROR; //error handler, collision on I2C bus
	}

	uint32_t h = raw_data_buffer_[1];
	h <<= 8;
	h |= raw_data_buffer_[2];
	h <<= 4;
	h |= raw_data_buffer_[3] >> 4;
	const float humidity = static_cast<float>(h) * 100 / 0x100000;

	//uint32_t rawData = (((uint32_t)raw_data_buffer_[1] << 16) | ((uint16_t)raw_data_buffer_[2] << 8) | (raw_data_buffer_[3])) >> 4; //20-bit raw humidity data

	//float humidity = (float)rawData * 0.000095;

	if (humidity < 0)
	{
		return 0;
	}
	if (humidity > 100)
	{
		return 100;
	}
	return humidity;
}

/// <summary>
///    Restart sensor, without power off
///     - takes ~20ms & all registers are set to default
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool Aht1X::SoftReset()
{
	Wire.beginTransmission(address_);
	Wire.write(AHT10_SOFT_RESET_CMD);

	if (Wire.endTransmission(true) != 0)
	{
		return false; //safety check, make sure sensor reset
	}

	delay(AHT10_SOFT_RESET_DELAY);
	SetNormalMode(); //reinitialize sensor registers after reset
	return enableFactoryCalCoefficient(); //reinitialize sensor registers after reset
}

/// <summary>
/// Set normal measurment mode
///    - one measurement & power down??? no info in datasheet!!!
/// </summary>
/// <returns>If the command was sent correctly</returns>
bool Aht1X::SetNormalMode() const
{
	Wire.beginTransmission(address_);
	Wire.write(AHT10_NORMAL_CMD);
	Wire.write(AHT10_DATA_NOP);
	Wire.write(AHT10_DATA_NOP);

	if (Wire.endTransmission(true) != 0)
	{
		return false; //safety check, make sure transmission complete
	}

	delay(AHT10_CMD_DELAY);

	return true;
}

/// <summary>
///   Set cycle measurment mode - continuous measurement
/// </summary>
/// <returns>If the command was received</returns>
bool Aht1X::SetCycleMode() const
{
	Wire.beginTransmission(address_);

	if (sensor_type_ != AHT20_SENSOR)
	{
		Wire.write(AHT10_INIT_CMD);
	} //set command mode
	else
	{
		Wire.write(AHT20_INIT_CMD);
	}

	Wire.write(AHT10_INIT_CYCLE_MODE | AHT10_INIT_CAL_ENABLE); //0,[0,1],0,[1],0,0,0
	Wire.write(AHT10_DATA_NOP);

	return Wire.endTransmission(true) == 0; //safety check, make sure transmission complete
}

/// <summary>
///  Read status byte from sensor over I2C
/// </summary>
/// <returns></returns>
uint8_t Aht1X::readStatusByte() const
{
	Wire.requestFrom(static_cast<int>(address_), 1);

	if (Wire.available() != 1)
	{
		return AHT10_ERROR; //check rxBuffer & error handler, collision on I2C bus
	}

	/* read byte from "wire.h" rxBuffer */

	return Wire.read();
}

/// <summary>
///     Read Calibration bit from status byte
/// </summary>
/// <param name="read_i2c">force to read status byte</param>
/// <returns>if factory calibration coeff disabled, or the err code./// </returns>
uint8_t Aht1X::getCalibrationBit(const bool read_i2c)
{
	if (static_cast<int>(read_i2c) == AHT10_FORCE_READ_DATA)
	{
		raw_data_buffer_[0] = readStatusByte();
	}

	if (raw_data_buffer_[0] != AHT10_ERROR)
	{
		return bitRead(raw_data_buffer_[0], 3); //get 3-rd bit
	}

	return AHT10_ERROR;
}

/// <summary>
///     Load factory calibration coefficients
/// </summary>
/// <returns></returns>
bool Aht1X::enableFactoryCalCoefficient()
{
	/* load factory calibration coefficient */
	Wire.beginTransmission(address_);
	const byte cmd = sensor_type_ != AHT20_SENSOR ? AHT10_INIT_CMD : AHT20_INIT_CMD;
	Wire.write(cmd);

	Wire.write(AHT10_INIT_CAL_ENABLE); //0,0,0,0,[1],0,0,0
	Wire.write(AHT10_DATA_NOP); //0,0,0,0,0,0,0,0

	if (Wire.endTransmission(true) != 0)
	{
		return false; //safety check, make sure transmission complete
	}
	// wait for the command
	delay(AHT10_CMD_DELAY);

	//return is calibration enabled
	return getCalibrationBit() == 0x01;
}

/// <summary>
///  Read busy bit from status byte
/// </summary>
/// <param name="read_i2c">force to read status byte</param>
/// <returns>if the sensor is busy & in measurement state or the error code.</returns>
uint8_t Aht1X::getBusyBit(const bool read_i2c)
{
	if (static_cast<int>(read_i2c) == AHT10_FORCE_READ_DATA)
	{
		raw_data_buffer_[0] = readStatusByte();
	}

	if (raw_data_buffer_[0] != AHT10_ERROR)
	{
		return bitRead(raw_data_buffer_[0], 7); //get 7-th bit
	}

	return AHT10_ERROR;
}

float Aht1X::convertTemp(const float raw)
{
	float temp = raw / static_cast<float>(1048576);
	temp = temp * 200;
	temp = temp - 50;
	return temp;
}

/// <summary>
/// Read raw measurement data from sensor over I2C
/// </summary>
/// <returns>The raw data.</returns>
uint8_t Aht1X::readRawData()
{
	/* send measurement command */
	Wire.beginTransmission(address_);

	Wire.write(AHT10_START_MEASUREMENT_CMD); //send measurement command
	Wire.write(AHT10_DATA_MEASUREMENT_CMD); //send measurement parameter
	Wire.write(AHT10_DATA_NOP); //send measurement parameter

	if (Wire.endTransmission(true) != 0)
	{
		return AHT10_ERROR; //error handler, collision on I2C bus
	}

	if (getCalibrationBit() != 0x01)
	{
		return AHT10_ERROR; //error handler, calibration coefficient turned off
	}
	if (getBusyBit(AHT10_USE_READ_DATA) != 0x00)
	{
		delay(AHT10_MEASUREMENT_DELAY); //measurement delay
	}

	/* read 6-bytes from sensor */

	Wire.requestFrom(static_cast<int>(address_), 6);

	if (Wire.available() != 6)
	{
		raw_data_buffer_[0] = AHT10_ERROR; //for condition when AHT10_USE_READ_DATA is used
		return AHT10_ERROR; //check rxBuffer & error handler, collision on the i2c bus
	}

	for (unsigned char& i : raw_data_buffer_)
	{
		i = Wire.read();
	}


	return 1U;
}
