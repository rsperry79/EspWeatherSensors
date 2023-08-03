#include "I2cHelpers.h"
#include "Helpers.h"
#include <Wire.h>

void Common::I2cHelpers::GetI2c()
{
	// F() -- stores the string on the flash and not keep in dynamic memory.
	byte address = 0x00;
	int nDevices = 0;

	SetupI2c();
	Serial.println(F("Scanning for I2C devices..."));
	for (address = 1; address < 127; address++)
	{
		if (CheckI2cAddress(address) == 0)
		{
			nDevices++;
		}
	}

	if (nDevices == 0)
	{
		Serial.println(F("	No I2C device found."));
	}
	else
	{
		Serial.printf("%d I2C device found.", nDevices);
	}

	Serial.println(F("Done searching for I2C.\n")); // extra line break to seperate this in the cautole.
}

byte Common::I2cHelpers::SearchAtAddress(const byte address, const byte secondary)
{
	int idex = 0;

	SetupI2c();

	do
	{
		idex++;
		if (CheckI2cAddress(address) == 0)
		{
			return address;
		}

		if (CheckI2cAddress(secondary) == 0)
		{
			return secondary;
		}
	}
	while (idex < 100);

	return {};
}

void Common::I2cHelpers::SetupI2c()
{
	Serial.println(F("\n\nI2C scanner setting clock to 100hz for maximum compatability."));

	Wire.setClock(100);
	Serial.println(F("I2C scanner is waiting 1 seconds for init..."));
	SleepForSeconds(1);
}

byte Common::I2cHelpers::CheckI2cAddress(const byte& address)
{
	// The i2c_scanner uses the return value of
	// the Write.endTransmisstion to see if
	// a device did acknowledge to the address.
	Serial.print(F("Searching for I2C Device at: 0x"));
	SerialPrintHex(address, false);

	Wire.beginTransmission(address);
	const int error = Wire.endTransmission();

	if (error == 0)
	{
		Serial.print(F("\n\nFound device.\n\n")); // To make this easily visible - extra line breaks.
	}
	else if (error == 1 || error == 2)
	{
		Serial.println(F("	Error."));
	}
	else if (error == 3 || error == 4)
	{
		Serial.println(F("	Timeout."));
	}
	else if (error == 5 || error == 6)
	{
		Serial.println(F("	NACK-No Acknowlegement."));
	}
	else
	{
		Serial.printf("\nI2C returned ErrorCode: %d\n", error); // To make this easily visible - extra line breaks.
	}

	Sleep(100);
	return error;
}

void Common::I2cHelpers::SerialPrintHex(const byte& address, const bool newline)
{
	if (address < 16)
	{
		Serial.print(F("0"));
	}

	Serial.print(address, HEX);

	if (newline == true)
	{
		Serial.print(F("\n"));
	}
}
