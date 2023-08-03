#include <HomeSpan.h>
#include <Helpers.h>
#include <WiredHomeSpanAht1X.h>
#include <WiredAht10.h>
#include <Wire.h>
#include <HomeSpan.h>

#include "GlobalConfig.h"

//#include "Helpers.h"
#include "WiredHomeSpanAht1X.h"

WiredHomeSpan::WiredHomeSpanAht1X aht10;

int Count = 0;

void setup()
{
	init();
	setup_home_span();
	aht10 = *new WiredHomeSpan::WiredHomeSpanAht1X();
	aht10.Begin("name", AHT10_ADDRESS_0X38, AHT10_SENSOR);

	//ReceiverUpdate::ReceiverUpdateSetup(DATA_PIN);
	homeSpan.enableAutoStartAP();
	homeSpan.autoPoll(8192, 0, 1);
}

void loop()
{

	aht10.Update();
	//ReceiverUpdate::ReceiverUpdate();
//	homeSpan.poll();
}

void init()
{
	Wire.begin();
	Serial.begin(SERIAL_BAUD);
//	Wire.setClock(100000);
//	Common::SleepForSeconds(1);
}

void setup_home_span()
{
	// Default AP Configuration
	homeSpan.setApSSID(SSID);
	homeSpan.setApPassword(SETUP_PASSWORD);
	homeSpan.enableAutoStartAP();

	// Default Options
	homeSpan.setPairingCode(PAIRING_CODE);

	// Logging Options
	homeSpan.setLogLevel(LOG_LEVEL);
	homeSpan.enableWebLog(LOG_LENGTH, TIME_SERVER, TZ, LOG_URI);

	// Control
	homeSpan.setStatusPin(LED_BUILTIN_PIN);

	// start the bridge
	homeSpan.begin(Category::Bridges, BRIDGE_NAME);

	new SpanAccessory();
	new Service::AccessoryInformation();
	new Characteristic::Identify();
}
