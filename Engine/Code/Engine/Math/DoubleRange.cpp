#include "Engine/Math/DoubleRange.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

DoubleRange::DoubleRange()
	:m_min(0.f), m_max(0.f)
{
}

DoubleRange::DoubleRange(double min, double max)
	: m_min(min), m_max(max)
{
}

void DoubleRange::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, '~');

	if (strings.size() != 2)
	{
		ERROR_AND_DIE("INPUT WRONG FORMAT DoubleRange");
	}
	const char* minChar = strings[0].c_str();
	const char* maxChar = strings[1].c_str();

	m_min = static_cast<double>(atof(minChar));
	m_max = static_cast<double>(atof(maxChar));
}

bool DoubleRange::IsOnRange(double number)
{
	return number >= m_min && number <= m_max;
}

bool DoubleRange::IsOverlappingWith(DoubleRange DoubleRange)
{
	return m_min <= DoubleRange.m_max && m_max >= DoubleRange.m_min;
}

double DoubleRange::GetMedian() const
{
	return (m_min + m_max) * 0.5f;
}

bool DoubleRange::operator==(const DoubleRange& compare) const
{
	return m_max == compare.m_max && m_min == compare.m_min;
}

bool DoubleRange::operator!=(const DoubleRange& compare) const
{
	return m_max != compare.m_max || m_min != compare.m_min;
}

void DoubleRange::operator=(const DoubleRange& copyFrom)
{
	m_max = copyFrom.m_max;
	m_min = copyFrom.m_min;
}
const DoubleRange DoubleRange::ZERO = DoubleRange();
const DoubleRange DoubleRange::ONE = DoubleRange(1.f, 1.f);
const DoubleRange DoubleRange::ZERO_TO_ONE = DoubleRange(0.f, 1.f);