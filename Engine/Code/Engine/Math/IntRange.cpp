#include "Engine/Math/IntRange.hpp"

IntRange::IntRange()
	:m_min(0), m_max(0)
{
}

IntRange::IntRange(int min, int max)
	: m_min(min), m_max(max)
{
}

bool IntRange::IsOnRange(int number)
{
	return number >= m_min && number <= m_max;
}

bool IntRange::IsOverlappingWith(IntRange floatRange)
{
	return floatRange.m_min >= m_min && floatRange.m_max <= m_max;
}

bool IntRange::operator==(const IntRange& compare) const
{
	return m_max == compare.m_max && m_min == compare.m_min;
}

bool IntRange::operator!=(const IntRange& compare) const
{
	return m_max != compare.m_max || m_min != compare.m_min;
}

void IntRange::operator=(const IntRange& copyFrom)
{
	m_max = copyFrom.m_max;
	m_min = copyFrom.m_min;
}
const IntRange IntRange::ZERO = IntRange();
const IntRange IntRange::ONE = IntRange(1, 1);
const IntRange IntRange::ZERO_TO_ONE = IntRange(0, 1);
