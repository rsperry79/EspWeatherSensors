#include "SensorTemperature.h"

Common::SensorTemperature::SensorTemperature(const float temperature) : last_temperature_(new float(temperature))
{
}

// return true if change threshold has been exceeded
bool Common::SensorTemperature::UpdateTemperature(const float temperature)
{
	return updateTemperature(temperature);
}

bool Common::SensorTemperature::updateTemperature(const float temp)
{
	bool has_change = false;
	if (*last_temperature_ != temp)
	{
		last_temperature_ = new float(temp);
		has_change = true;
	}

	return has_change;
}

float& Common::SensorTemperature::GetTemperature()
{
	return *last_temperature_;
}
