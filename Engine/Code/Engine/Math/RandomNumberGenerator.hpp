#pragma once


struct FloatRange;

class RandomNumberGenerator
{
public:
	RandomNumberGenerator(unsigned int seed = 0) { m_seed = seed; m_position = 0; }
	int RollRandomIntLessThan(int maxNotInclusive);
	int RollRandomIntInRange(int minInclusive, int maxInclusive);
	unsigned int RollRandomUnsignedIntInRange(unsigned int minInclusive, unsigned int maxInclusive);
	float RollRandomFloatZeroToOne();
	float RollRandomFloatMinusOneToOne();
	float RollRandomFloatInRange(float minInclusive, float maxInclusive);
	float RollRandomFloatInRange(FloatRange range);
	bool RollRandomChance(float rate);
	int RollRandomSign();

	void SetSeed(int seed);

public:
	unsigned int m_seed = 0;
	int m_position = 0;
};


