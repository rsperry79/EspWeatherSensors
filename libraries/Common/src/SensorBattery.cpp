#include "SensorBattery.h"

Common::SensorBattery::SensorBattery(bool flag)
{
	hasLowBattery = &flag;
}

/// <summary>
/// Gets the low battery flag.
/// </summary>
/// <returns>a bool representing if the battery is low (true) or if it has a normal charge (false).</returns>
bool Common::SensorBattery::GetLowBattery()
{
	return hasLowBattery != nullptr;
}

bool Common::SensorBattery::UpdateBattery(const bool flag)
{
	return updateBatteryFlag(flag);
}

/// <summary>       
/// A private method to set the battery flag.
/// </summary>
/// <param name="flag">The bool representing the battery status.</param>
bool Common::SensorBattery::updateBatteryFlag(const bool flag)
{
	bool has_change = false;
	if (flag != *hasLowBattery)
	{
		hasLowBattery = new bool(flag);
		has_change = true;
	}

	return has_change;
}
