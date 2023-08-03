#ifndef SENSOR_HUMIDITY_H
#define SENSOR_HUMIDITY_H

#pragma once
namespace Common
{
	struct SensorHumidity
	{
	public:
		explicit SensorHumidity(int rh);
		virtual bool UpdateHumidity(int rh);
		virtual int GetHumidity();

	private:
		int last_humidity_;
		bool updateHumidity(int rh);
	};
}
#endif
