#pragma once
struct IntRange
{
public:
	int m_min;
	int m_max;
public:
	IntRange();
	explicit IntRange(int min, int max);

	bool IsOnRange(int number);
	bool IsOverlappingWith(IntRange floatRange);
	bool		operator==(const IntRange& compare) const;
	bool		operator!=(const IntRange& compare) const;
	void		operator=(const IntRange& copyFrom);

	static const IntRange ZERO;
	static const IntRange ONE;
	static const IntRange ZERO_TO_ONE;
};