#ifndef SENSOR_TEMPERATURE_H
#define SENSOR_TEMPERATURE_H

#pragma once

namespace Common
{
	struct SensorTemperature
	{
	public:
		explicit SensorTemperature(float temperature);
		virtual bool UpdateTemperature(float temperature);
		virtual float& GetTemperature();

	private:
		float* last_temperature_ = nullptr;
		bool updateTemperature(float temp);
	};
}
#endif
