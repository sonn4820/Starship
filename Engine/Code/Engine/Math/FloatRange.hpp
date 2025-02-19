#pragma once
struct FloatRange
{
public:
	float m_min;
	float m_max;
public:
	FloatRange();
	explicit FloatRange(float min, float max);

	void SetFromText(char const* text);
	bool IsOnRange(float number);
	bool IsOverlappingWith(FloatRange floatRange);
	float GetMedian() const;
	bool		operator==(const FloatRange& compare) const;
	bool		operator!=(const FloatRange& compare) const;
	void		operator=(const FloatRange& copyFrom);

	static const FloatRange ZERO;
	static const FloatRange ONE;
	static const FloatRange ZERO_TO_ONE;
};


