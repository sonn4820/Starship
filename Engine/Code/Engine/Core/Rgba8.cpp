#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <ctime>

Rgba8 Rgba8::COLOR_BLACK = Rgba8(0, 0, 0);
Rgba8 Rgba8::COLOR_WHITE = Rgba8(255, 255, 255);
Rgba8 Rgba8::DARK = Rgba8(0, 0, 0, 100);
Rgba8 Rgba8::LIGHT = Rgba8(255, 255, 255, 100);
Rgba8 Rgba8::COLOR_GRAY = Rgba8(127, 127, 127);
Rgba8 Rgba8::COLOR_DARK_GRAY = Rgba8(60, 60, 60);
Rgba8 Rgba8::COLOR_DARK_X2_GRAY = Rgba8(30, 30, 30);
Rgba8 Rgba8::COLOR_TRANSPARENT = Rgba8(0, 0, 0, 0);

Rgba8 Rgba8::COLOR_RED = Rgba8(255, 0, 0);
Rgba8 Rgba8::COLOR_DARK_RED = Rgba8(127, 0, 0);
Rgba8 Rgba8::COLOR_PINK = Rgba8(255, 192, 203);
Rgba8 Rgba8::COLOR_GREEN = Rgba8(0, 255, 0);
Rgba8 Rgba8::COLOR_DARK_GREEN = Rgba8(0, 127, 0);
Rgba8 Rgba8::COLOR_BLUE = Rgba8(0, 0, 255);
Rgba8 Rgba8::COLOR_YELLOW = Rgba8(255, 255, 0);
Rgba8 Rgba8::COLOR_DARK_YELLOW = Rgba8(127, 127, 0);
Rgba8 Rgba8::COLOR_MAGNETA = Rgba8(255, 0, 255);
Rgba8 Rgba8::COLOR_CYAN = Rgba8(0, 255, 255);

Rgba8 Rgba8::COLOR_ORANGE = Rgba8(255, 127, 0);
Rgba8 Rgba8::COLOR_VIOLET = Rgba8(148, 0, 211);
Rgba8 Rgba8::COLOR_INDIGO = Rgba8(76, 0, 130);
Rgba8 Rgba8::COLOR_LIGHT_BLUE = Rgba8(73, 216, 230);
Rgba8 Rgba8::COLOR_BRIGHT_BLUE = Rgba8(0, 150, 255);
Rgba8 Rgba8::COLOR_DARK_BLUE = Rgba8(0, 20, 120);
Rgba8 Rgba8::COLOR_LIGHT_GRAY = Rgba8(170, 170, 170);
Rgba8 Rgba8::COLOR_RAGDOLL_CONSTRAINT = Rgba8(0, 136, 159);
Rgba8 Rgba8::COLOR_RAGDOLL_NODE = Rgba8(102, 184, 197);

Rgba8::Rgba8(unsigned char rByte, unsigned char gByte, unsigned char bByte, unsigned char aByte)
	:r(rByte), g(gByte), b(bByte), a(aByte)
{

}

void Rgba8::SetFromText(char const* text)
{

	Strings strings = SplitStringOnDelimiter(text, ',');

	if (strings.size() < 3 || strings.size() > 4)
	{
		ERROR_AND_DIE("INPUT WRONG FORMAT RGB8");
	}

	if (strings.size() == 3)
	{
		const char* rChar = strings[0].c_str();
		const char* gChar = strings[1].c_str();
		const char* bChar = strings[2].c_str();

		r = static_cast<unsigned char>(atoi(rChar));
		g = static_cast<unsigned char>(atoi(gChar));
		b = static_cast<unsigned char>(atoi(bChar));
	}
	if (strings.size() == 4)
	{
		const char* rChar = strings[0].c_str();
		const char* gChar = strings[1].c_str();
		const char* bChar = strings[2].c_str();
		const char* aChar = strings[3].c_str();

		r = static_cast<unsigned char>(atoi(rChar));
		g = static_cast<unsigned char>(atoi(gChar));
		b = static_cast<unsigned char>(atoi(bChar));
		a = static_cast<unsigned char>(atoi(aChar));
	}

}

void Rgba8::GetAsFloats(float* colorAsFloats) const
{
	colorAsFloats[0] = RangeMap(r, 0.f, 255.f, 0.f, 1.f);
	colorAsFloats[1] = RangeMap(g, 0.f, 255.f, 0.f, 1.f);
	colorAsFloats[2] = RangeMap(b, 0.f, 255.f, 0.f, 1.f);
	colorAsFloats[3] = RangeMap(a, 0.f, 255.f, 0.f, 1.f);
}

void Rgba8::operator*=(const Rgba8 rgba)
{
	float ogByte[4];
	float mulByte[4];

	GetAsFloats(ogByte);
	rgba.GetAsFloats(mulByte);

	float newR = ogByte[0] * mulByte[0];
	float newG = ogByte[1] * mulByte[1];
	float newB = ogByte[2] * mulByte[2];
	float newA = ogByte[3] * mulByte[3];

	Rgba8 result = Rgba8::Create_FromFloat(newR, newG, newB, newA);
	*this = result;
}

void Rgba8::operator*=(const float scale)
{
	float ogByte[4];
	GetAsFloats(ogByte);

	float newR = ogByte[0] * scale;
	float newG = ogByte[1] * scale;
	float newB = ogByte[2] * scale;
	float newA = ogByte[3] * scale;

	Rgba8 result = Rgba8::Create_FromFloat(newR, newG, newB, newA);
	*this = result;
}

Rgba8 Rgba8::Create_FromFloat(float r, float g, float b, float a /*= 1.f*/)
{
	Rgba8 result;
	result.r = DenormalizeByte(r);
	result.g = DenormalizeByte(g);
	result.b = DenormalizeByte(b);
	result.a = DenormalizeByte(a);

	return result;
}

Rgba8 Rgba8::Create_FromVec3(Vec3 vec3, unsigned char a /*= 1.f*/)
{
	Rgba8 result;
	result.r = DenormalizeByte(vec3.x);
	result.g = DenormalizeByte(vec3.y);
	result.b = DenormalizeByte(vec3.z);
	result.a = a;

	return result;
}

Rgba8 Rgba8::Create_ExchangeFromVec3(Vec3 vec3, unsigned char a /*= 1.f*/)
{
	Rgba8 result;
	result.r = (unsigned char)vec3.x;
	result.g = (unsigned char)vec3.y;
	result.b = (unsigned char)vec3.z;
	result.a = a;

	return result;
}

Rgba8 Rgba8::Create_FromVec4(Vec4 vec4)
{
	Rgba8 result;
	result.r = DenormalizeByte(vec4.x);
	result.g = DenormalizeByte(vec4.y);
	result.b = DenormalizeByte(vec4.z);
	result.a = DenormalizeByte(vec4.w);

	return result;
}

Rgba8 Rgba8::Create_WithFloatMultiplier(Rgba8 original, float rM, float gM, float bM, float aM /*= 1.f*/)
{
	original.r = DenormalizeByte(rM);
	original.g = DenormalizeByte(gM);
	original.b = DenormalizeByte(bM);
	original.a = DenormalizeByte(aM);

	return original;
}

Rgba8 Rgba8::GetDarkerColor(Rgba8 color, float percentageZeroToOne)
{
	float ogByte[4];
	color.GetAsFloats(ogByte);

	float newR = ogByte[0] * percentageZeroToOne;
	float newG = ogByte[1] * percentageZeroToOne;
	float newB = ogByte[2] * percentageZeroToOne;

	return Rgba8::Create_FromFloat(newR, newG, newB, color.a);
}

Rgba8 Rgba8::GetRandomColor(bool randomAlpha, int offsetSeed)
{
	srand((int)time(nullptr));
	auto rng = RandomNumberGenerator(rand() + offsetSeed);

	float newR = rng.RollRandomFloatZeroToOne();
	float newG = rng.RollRandomFloatZeroToOne();
	float newB = rng.RollRandomFloatZeroToOne();
	float newA = 1.f;
	if(randomAlpha) newA = rng.RollRandomFloatZeroToOne();

	return Rgba8::Create_FromFloat(newR, newG, newB, newA);
}

void Rgba8::operator=(const Rgba8 set)
{
	r = set.r;
	g = set.g;
	b = set.b;
	a = set.a;
}

bool Rgba8::operator==(const Rgba8& compare) const
{
	return r == compare.r
		&& g == compare.g
		&& b == compare.b;
}

