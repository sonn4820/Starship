#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "ThirdParty/SquirrelNoise/SmoothNoise.hpp"
#include "ThirdParty/SquirrelNoise/RawNoise.hpp"
#include <math.h>
#include <cstdlib> 
#include <time.h> 

int RandomNumberGenerator::RollRandomIntLessThan(int maxNotInclusive)
{
	unsigned int randomInt =  Get1dNoiseUint(m_position++, m_seed);
	return randomInt % maxNotInclusive;
}

int RandomNumberGenerator::RollRandomIntInRange(int minInclusive, int maxInclusive)
{
	unsigned int randomInt = Get1dNoiseUint(m_position++, m_seed);
	return minInclusive + randomInt % (maxInclusive - minInclusive + 1);
}

unsigned int RandomNumberGenerator::RollRandomUnsignedIntInRange(unsigned int minInclusive, unsigned int maxInclusive)
{
	unsigned int randomInt = Get1dNoiseUint(m_position++, m_seed);
	return minInclusive + randomInt % (maxInclusive - minInclusive + 1);
}

float RandomNumberGenerator::RollRandomFloatZeroToOne()
{
	unsigned int randomInt = Get1dNoiseUint(m_position++, m_seed);
	return (float)randomInt / (float)UINT_MAX;
}

float RandomNumberGenerator::RollRandomFloatMinusOneToOne() 
{
	return RollRandomFloatInRange(-1.f, 1.f);
}
float RandomNumberGenerator::RollRandomFloatInRange(float minInclusive, float maxInclusive)
{
 	unsigned int randomInt = Get1dNoiseUint(m_position++, m_seed);
	float percentage = (float)randomInt / (float)UINT_MAX;
	return minInclusive + percentage * (maxInclusive - minInclusive);
}

float RandomNumberGenerator::RollRandomFloatInRange(FloatRange range)
{
	return RollRandomFloatInRange(range.m_min, range.m_max);
}

bool RandomNumberGenerator::RollRandomChance(float rate)
{
	if (rate > 1.f) 
	{
		return true;
	}
	return RandomNumberGenerator::RollRandomFloatZeroToOne() < rate;
}

int RandomNumberGenerator::RollRandomSign()
{
	int sign = RollRandomIntInRange(-1, 1);
	return sign / abs(sign);
}

void RandomNumberGenerator::SetSeed(int seed)
{
	m_seed = seed;
}
