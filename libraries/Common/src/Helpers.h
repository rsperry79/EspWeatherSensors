#include "Arduino.h"
#include <cstdint>

namespace Common
{
	float ConvertCToF(float c);
	bool Sleep(uint ms);
	bool SleepForSeconds(uint sec);
	char* ToCharArray(const String& text);
}
