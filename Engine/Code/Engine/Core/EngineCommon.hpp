#pragma once
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/NamedStrings.hpp"

#define UNUSED(x) (void)(x);
#pragma warning (disable : 26812)

extern NamedStrings g_gameConfigBlackboard;
unsigned char FromFloatToUnsignedChar(float value);
float FromUnsignedCharToFloat(unsigned char value);