#include "WiredHomeSpanAht1X.h"
#include "HomeSpanAht10HumiditySensor.h"
#include "HomeSpanAht10TemperatureSensor.h"

WiredHomeSpan::WiredHomeSpanAht1X::WiredHomeSpanAht1X()
{
	Serial.println("Aht1x init");
}


void WiredHomeSpan::WiredHomeSpanAht1X::Begin(char* name, const SENSOR_ADDRESS address, const SENSOR_TYPE sensor_type)
{
	aht10_.Begin(address, sensor_type);


	new SpanAccessory();
	new Service::AccessoryInformation();
	new Characteristic::Identify();
	new Characteristic::Name("Aht10 Humidity");
	new HomeSpanAht10HumiditySensor("Aht10 Humidity", *aht10_.humidity);

	new SpanAccessory();
	new Service::AccessoryInformation();
	new Characteristic::Identify();
	new Characteristic::Name("Aht10 Temperature");
	new HomeSpanAht10TemperatureSensor("Aht10 Temperature", *aht10_.temperature);
}

void WiredHomeSpan::WiredHomeSpanAht1X::Update()
{
	aht10_.Update();
}
