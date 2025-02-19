#include "Engine/Core/EngineCommon.hpp"

NamedStrings g_gameConfigBlackboard;

unsigned char FromFloatToUnsignedChar(float value)
{
	return static_cast<unsigned char>(value);
}

float FromUnsignedCharToFloat(unsigned char value)
{
	return  static_cast<float>(value);
}
