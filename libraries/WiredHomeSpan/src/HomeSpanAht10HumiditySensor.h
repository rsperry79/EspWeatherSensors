#ifndef WIRED_HUMIDITY_SENSOR_H
#define WIRED_HUMIDITY_SENSOR_H

#include "HomeSpan.h"
#include "SensorHumidity.h"

#pragma once

struct HomeSpanAht10HumiditySensor : Service::HumiditySensor
{
private:
	const char* name_ = {};
	SpanCharacteristic* home_kit_humidity_;
	Common::SensorHumidity* humidity_ = {};

public:
	HomeSpanAht10HumiditySensor(const char* name, Common::SensorHumidity& humidity)
	{
		name_ = name;
		humidity_ = &humidity;
		const int hum = humidity_->GetHumidity();
		home_kit_humidity_ = new Characteristic::CurrentRelativeHumidity(static_cast<double>(hum));
		home_kit_humidity_->setRange(0, 100);
	}

	void loop() override
	{
		if (home_kit_humidity_->timeVal() > 5000)
		{
			home_kit_humidity_->setVal(humidity_->GetHumidity());
			const int hum = humidity_->GetHumidity();
			WEBLOG("%s Update %f", name_, hum);
		}
	}
};
#endif
