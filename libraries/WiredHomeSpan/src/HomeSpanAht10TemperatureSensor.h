#ifndef WIRED_SENSOR_TEMPERATURE_H
#define WIRED_SENSOR_TEMPERATURE_H

#include "HomeSpan.h"
#include "SensorTemperature.h"

#pragma once

struct HomeSpanAht10TemperatureSensor : Service::TemperatureSensor
{
private:
	const char* name_ = {};
	Common::SensorTemperature* temperature_;
	SpanCharacteristic* home_kit_temp_;

	void updateTemperature(double temp) const
	{
		const double f_rounded = round(temperature_->GetTemperature() * 10) / 10.0;
		home_kit_temp_->setVal(f_rounded);
	}

public:
	HomeSpanAht10TemperatureSensor(const char* name, Common::SensorTemperature& temperature)
	{
		name_ = name;
		temperature_ = &temperature;
		const float temp = temperature.GetTemperature();
		home_kit_temp_ = new Characteristic::CurrentTemperature(static_cast<double>(temp));
		home_kit_temp_->setRange(-50, 100);
	}

	void loop() override
	{
		if (home_kit_temp_->timeVal() > 5000)
		{
			// check time elapsed since last update and proceed only if greater than 5 seconds
			const double temp = (double)temperature_->GetTemperature();
			updateTemperature(temp);
			WEBLOG("%s Update %f", name_, temp);
		}
	}
};
#endif
