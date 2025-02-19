#pragma once

#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"

struct Rgba8
{
public:
	unsigned char r = 255;
	unsigned char g = 255;
	unsigned char b = 255;
	unsigned char a = 255;

	static Rgba8 COLOR_BLACK;
	static Rgba8 COLOR_WHITE;
	static Rgba8 DARK;
	static Rgba8 LIGHT;
	static Rgba8 COLOR_GRAY;
	static Rgba8 COLOR_DARK_GRAY;
	static Rgba8 COLOR_DARK_X2_GRAY;
	static Rgba8 COLOR_TRANSPARENT;
	static Rgba8 COLOR_RED;
	static Rgba8 COLOR_DARK_RED;
	static Rgba8 COLOR_PINK;
	static Rgba8 COLOR_GREEN;
	static Rgba8 COLOR_DARK_GREEN;
	static Rgba8 COLOR_BLUE;
	static Rgba8 COLOR_YELLOW;
	static Rgba8 COLOR_DARK_YELLOW;
	static Rgba8 COLOR_MAGNETA;
	static Rgba8 COLOR_CYAN;
	static Rgba8 COLOR_ORANGE;
	static Rgba8 COLOR_VIOLET;
	static Rgba8 COLOR_INDIGO;
	static Rgba8 COLOR_LIGHT_BLUE;
	static Rgba8 COLOR_BRIGHT_BLUE;
	static Rgba8 COLOR_DARK_BLUE;
	static Rgba8 COLOR_LIGHT_GRAY;
	static Rgba8 COLOR_RAGDOLL_CONSTRAINT;
	static Rgba8 COLOR_RAGDOLL_NODE;
	

	Rgba8() = default;
	Rgba8(unsigned char rByte, unsigned char gByte, unsigned char bByte, unsigned char aByte = 255);
	void SetFromText(char const* text);
	void GetAsFloats(float* colorAsFloats) const;
	bool operator==(const Rgba8& compare) const;
	void operator=(const Rgba8 set);
	void operator*=(const Rgba8 rgba);
	void operator*=(const float scale);

	static Rgba8 Create_FromFloat(float r, float g, float b, float a = 1.f);
	static Rgba8 Create_FromVec3(Vec3 vec3, unsigned char a = 255);
	static Rgba8 Create_ExchangeFromVec3(Vec3 vec3, unsigned char a = 255);
	static Rgba8 Create_FromVec4(Vec4 vec4);
	static Rgba8 Create_WithFloatMultiplier(Rgba8 original, float rM, float gM, float bM, float aM = 1.f);

	static Rgba8 GetDarkerColor(Rgba8 color, float percentageZeroToOne);
	static Rgba8 GetRandomColor(bool randomAlpha = false, int offsetSeed = 0);
};