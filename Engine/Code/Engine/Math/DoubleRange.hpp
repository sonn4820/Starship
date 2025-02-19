#pragma once
struct DoubleRange
{
public:
	double m_min;
	double m_max;
public:
	DoubleRange();
	explicit DoubleRange(double min, double max);

	void SetFromText(char const* text);
	bool IsOnRange(double number);
	bool IsOverlappingWith(DoubleRange doubleRange);
	double GetMedian() const;
	bool		operator==(const DoubleRange& compare) const;
	bool		operator!=(const DoubleRange& compare) const;
	void		operator=(const DoubleRange& copyFrom);

	static const DoubleRange ZERO;
	static const DoubleRange ONE;
	static const DoubleRange ZERO_TO_ONE;
};


