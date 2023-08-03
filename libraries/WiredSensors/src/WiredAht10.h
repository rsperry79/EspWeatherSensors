#ifndef WIRED_AHT10_H
#define WIRED_AHT10_H

#include "AHT1x.h"
#include "SensorHumidity.h"
#include "SensorTemperature.h"
#include "WiredAht10.h"

#pragma once

namespace WiredSensors
{
	class WiredAht10
	{
	public:
		WiredAht10();
		void Begin(SENSOR_ADDRESS address, SENSOR_TYPE sensor_type);
		void SetTemperatureOffset(float offset);
		void Update();

		Common::SensorTemperature* temperature = {};
		Common::SensorHumidity* humidity = {};

	private:
		float getTemperature();
		int getHumidity();

		Aht1X aht10_ = {};
		float offset_ = 0; // offset in degrees C
	};
}
#endif
