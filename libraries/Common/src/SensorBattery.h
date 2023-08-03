#ifndef SENSOR_BATTERY_H
#define SENSOR_BATTERY_H

#pragma once

namespace Common
{
	struct SensorBattery
	{
	public:
		explicit SensorBattery(bool flag);
		virtual bool GetLowBattery();
		virtual bool UpdateBattery(bool flag);

	private:
		bool* hasLowBattery;
		bool updateBatteryFlag(bool flag);
	};
}
#endif
