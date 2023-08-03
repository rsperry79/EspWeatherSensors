#ifndef I2C_HELPERS_H
#define I2C_HELPERS_H
#include "Arduino.h"


#pragma once
namespace Common
{
	class I2cHelpers
	{
	public:
		static void GetI2c();
		static byte CheckI2cAddress(const byte& address);
		static byte SearchAtAddress(byte address, byte secondary);
		static void SetupI2c();
		static void SerialPrintHex(const byte& address, const bool newline);

	private:
	};
};

#endif
