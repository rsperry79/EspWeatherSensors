#ifndef AHT1_X_H
#define AHT1_X_H

#include "Arduino.h"

#define AHT10_FORCE_READ_DATA      true  //force to read data
#define AHT10_USE_READ_DATA        false //force to use data from previous read

enum
{
	//no info in data-sheet!!! my guess it is DAC resolution, saw someone send 0x00 instead
	AHT10_ERROR = 0xFF,
	//returns 255, if communication error is occurred

	AHT10_DATA_NOP = 0x00,
	//no info in data-sheet!!!
	AHT10_MEASUREMENT_DELAY = 80,
	//at least 75 milliseconds
	AHT10_POWER_ON_DELAY = 40,
	//at least 20..40 milliseconds
	AHT10_CMD_DELAY = 350,
	//at least 300 milliseconds, no info in data-sheet!!!
	AHT10_SOFT_RESET_DELAY = 20 //less than 20 milliseconds
};

enum
{
	AHT10_INIT_NORMAL_MODE = 0x00,
	//enable normal mode
	AHT10_INIT_CYCLE_MODE = 0x20,
	//enable cycle mode
	AHT10_INIT_CMD_MODE = 0x40,
	//enable command mode
	AHT10_INIT_CAL_ENABLE = 0x08,
	//load factory calibration coeff
	AHT10_DATA_MEASUREMENT_CMD = 0x33,
	AHT10_INIT_CMD = 0xE1,
	//initialization command for AHT10/AHT15
	AHT20_INIT_CMD = 0xBE,
	//initialization command for AHT20
	AHT10_START_MEASUREMENT_CMD = 0xAC,
	//start measurement command
	AHT10_NORMAL_CMD = 0xA8,
	//normal cycle mode command, no info in data-sheet!!!
	AHT10_SOFT_RESET_CMD = 0xBA //soft reset command
};

typedef enum sensor_type
{
	AHT10_SENSOR = 0x00,
	AHT15_SENSOR = 0x01,
	AHT20_SENSOR = 0x02
} SENSOR_TYPE;

typedef enum sensor_address
{
	AHT10_ADDRESS_0X38 = 0x38,
	//chip I2C address no.1 for AHT10/AHT15/AHT20, address pin connected to GND
	AHT10_ADDRESS_0X39 = 0x39 //chip I2C address no.2 for AHT10 only, address pin connected to Vcc
} SENSOR_ADDRESS;

using SENSOR_CMD = unsigned char;

class Aht1X
{
public:
	Aht1X();
	Aht1X(SENSOR_ADDRESS address, SENSOR_TYPE sensor_type);
	bool Begin();
	float ReadTemperature(bool read_i2c = AHT10_FORCE_READ_DATA);
	int ReadHumidity(bool read_i2c = AHT10_FORCE_READ_DATA);
	bool SoftReset();
	bool SetNormalMode() const;
	bool SetCycleMode() const;

private:
	uint8_t address_ = {};
	SENSOR_CMD e_sensor_calibrate_cmd_[3] = {0xE1, 0x08, 0x00};
	SENSOR_CMD e_sensor_normal_cmd_[3] = {0xA8, 0x00, 0x00};
	SENSOR_CMD e_sensor_measure_cmd_[3] = {0xAC, 0x33, 0x00};
	SENSOR_CMD e_sensor_reset_cmd_ = 0xBA;

	SENSOR_TYPE sensor_type_ = AHT10_SENSOR;
	uint8_t raw_data_buffer_[6] = {AHT10_ERROR, 0, 0, 0, 0, 0};

	static float convertTemp(float raw);
	uint8_t readRawData();
	uint8_t readStatusByte() const;
	uint8_t getCalibrationBit(bool read_i2c = AHT10_FORCE_READ_DATA);
	bool enableFactoryCalCoefficient();
	uint8_t getBusyBit(bool read_i2c = AHT10_FORCE_READ_DATA);
};
#endif
