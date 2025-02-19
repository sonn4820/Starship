#include "Engine/Math/FloatRange.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

FloatRange::FloatRange()
	:m_min(0.f), m_max(0.f)
{
}

FloatRange::FloatRange(float min, float max)
	: m_min(min), m_max(max)
{
}

void FloatRange::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, '~');

	if (strings.size() != 2)
	{
		ERROR_AND_DIE("INPUT WRONG FORMAT FLOATRANGE");
	}
	const char* minChar = strings[0].c_str();
	const char* maxChar = strings[1].c_str();

	m_min = static_cast<float>(atof(minChar));
	m_max = static_cast<float>(atof(maxChar));
}

bool FloatRange::IsOnRange(float number)
{
	return number >= m_min && number <= m_max;
}

bool FloatRange::IsOverlappingWith(FloatRange floatRange)
{
	return m_min <= floatRange.m_max && m_max >= floatRange.m_min;
}

float FloatRange::GetMedian() const
{
	return (m_min + m_max) * 0.5f;
}

bool FloatRange::operator==(const FloatRange& compare) const
{
	return m_max == compare.m_max && m_min == compare.m_min;
}

bool FloatRange::operator!=(const FloatRange& compare) const
{
	return m_max != compare.m_max || m_min != compare.m_min;
}

void FloatRange::operator=(const FloatRange& copyFrom)
{
	m_max = copyFrom.m_max;
	m_min = copyFrom.m_min;
}
const FloatRange FloatRange::ZERO = FloatRange();
const FloatRange FloatRange::ONE = FloatRange(1.f, 1.f);
const FloatRange FloatRange::ZERO_TO_ONE = FloatRange(0.f, 1.f);