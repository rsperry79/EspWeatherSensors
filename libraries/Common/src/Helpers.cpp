#include "Helpers.h"

float Common::ConvertCToF(const float c)
{
	const float temp = static_cast<float>(1.8) * c;
	return temp + 32;
}

bool Common::Sleep(const uint ms)
{
	const ulong start = millis();
	ulong current = 0;
	do
	{
		current = millis();
	}
	while (current - start < ms);

	return true;
}

bool Common::SleepForSeconds(const uint sec)
{
	Sleep(sec * 1000);
	return false;
}

char* Common::ToCharArray(const String& text)
{
	const int len = text.length();
	char buf[len];
	text.toCharArray(buf, len);
	return buf;
}
