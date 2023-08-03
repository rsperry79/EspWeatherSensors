#include "WiredAht10.h"

#include "Helpers.h"
#include "SensorTemperature.h"


WiredSensors::WiredAht10::WiredAht10()
{
}


void WiredSensors::WiredAht10::Begin(SENSOR_ADDRESS address, SENSOR_TYPE sensor_type)
{
	aht10_ = Aht1X(address, sensor_type);
	while (!aht10_.Begin())
	{
		Serial.println(F("AHT10 not connected or fail to load calibration coefficient"));
		//(F()) save string to flash & keeps dynamic memory free
		Common::Sleep(5000);
	}

	const float temp = getTemperature();
	Serial.printf("Temperature: %f\n", temp);

	const int hum = getHumidity();
	Serial.printf("Humidity: %d\n", hum);

	this->temperature = new Common::SensorTemperature(temp);
	this->humidity = new Common::SensorHumidity(hum);
}

void WiredSensors::WiredAht10::SetTemperatureOffset(const float offset)
{
	offset_ = offset;
}

void WiredSensors::WiredAht10::Update()
{
	humidity->UpdateHumidity(getHumidity());
	temperature->UpdateTemperature(getTemperature());
}

float WiredSensors::WiredAht10::getTemperature()
{
	const float temp = aht10_.ReadTemperature();
	return temp + offset_;
}

int WiredSensors::WiredAht10::getHumidity()
{
	const int hum = aht10_.ReadHumidity();
	return hum;
}
