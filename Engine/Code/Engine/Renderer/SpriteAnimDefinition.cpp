#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Math/MathUtils.hpp"

SpriteAnimDefinition::SpriteAnimDefinition(const SpriteSheet& sheet, int startSpriteIndex, int endSpriteIndex, float durationSeconds, SpriteAnimPlaybackType playbackType)
	:m_spriteSheet(sheet), m_startSpriteIndex(startSpriteIndex), m_endSpriteIndex(endSpriteIndex), m_durationSeconds(durationSeconds), m_playbackType(playbackType)
{
}

const SpriteDefinition& SpriteAnimDefinition::GetSpriteDefAtTime(float seconds) const
{
	if (m_durationSeconds <= 0)
	{
		return m_spriteSheet.GetSpriteDef(m_startSpriteIndex);
	}
	float normalizedTime = seconds / m_durationSeconds;
	int spriteOffset = 0;
	if (m_playbackType == SpriteAnimPlaybackType::ONCE)
	{
		if (normalizedTime > 1.f)
		{
			spriteOffset = m_endSpriteIndex - m_startSpriteIndex;
		}
	}
	else if (m_playbackType == SpriteAnimPlaybackType::LOOP)
	{
		normalizedTime = normalizedTime - static_cast<float>(RoundDownToInt(normalizedTime));
	}
	else if (m_playbackType == SpriteAnimPlaybackType::PINGPONG)
	{
		int loopNum = RoundDownToInt(normalizedTime);
		float normalizedTimeInLoop = normalizedTime - static_cast<float>(RoundDownToInt(normalizedTime));
		if (loopNum % 2 == 1)
		{
			normalizedTime = 1.f - normalizedTimeInLoop;
		}
		else
		{
			normalizedTime = normalizedTimeInLoop;
		}

	}
	spriteOffset = (int)Interpolate(static_cast<float>(0), static_cast<float>(m_endSpriteIndex - m_startSpriteIndex + 1), normalizedTime);
	int spriteResult = m_startSpriteIndex + spriteOffset;
	if (spriteResult > m_endSpriteIndex) spriteResult = m_endSpriteIndex;
	return m_spriteSheet.GetSpriteDef(spriteResult);
}
