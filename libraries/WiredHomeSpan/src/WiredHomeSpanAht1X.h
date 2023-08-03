#ifndef WIRED_HOME_SPAN_AHT1X_H
#define WIRED_HOME_SPAN_AHT1X_H


#include "AHT1x.h"
#include "WiredAht10.h"


#pragma once

namespace WiredHomeSpan
{
	class WiredHomeSpanAht1X
	{
	public:
		WiredHomeSpanAht1X();
		void Begin(char* name, const SENSOR_ADDRESS address, const SENSOR_TYPE sensor_type);
		void Update();

	private:
		WiredSensors::WiredAht10 aht10_ = *new WiredSensors::WiredAht10();
	};
}
#endif
