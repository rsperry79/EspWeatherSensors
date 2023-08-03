#include "SensorHumidity.h"

Common::SensorHumidity::SensorHumidity(const int rh) : last_humidity_(rh)
{
}

/// <summary>
/// Updates the sensor data if a change has occurred past the threshold.
/// </summary>
/// <returns>True if an update had occurred.</returns>
bool Common::SensorHumidity::UpdateHumidity(const int rh)
{
	return updateHumidity(rh);
}

/// <summary>
/// Gets the relative humidity
/// </summary>
/// <returns>the humidity as a whole number</returns>
int Common::SensorHumidity::GetHumidity()
{
	return last_humidity_;
}


/// <summary>
/// A private method to update the humidity.
/// </summary>
/// <param name="rh">The relative humidity.</param>
bool Common::SensorHumidity::updateHumidity(const int rh)
{
	bool has_change = false;
	if (last_humidity_ != rh && rh >= 0 && rh <= 100)
	{
		last_humidity_ = rh;
		has_change = true;
	}

	return has_change;
}
