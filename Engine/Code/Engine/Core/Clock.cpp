#include "Engine/Core/Clock.hpp"
#include "Engine/Math/MathUtils.hpp"

Clock* Clock::s_theSystemClock = nullptr;

Clock::Clock()
{
	if (m_parent == nullptr)
	{
		m_parent = Clock::s_theSystemClock;
	}
}

Clock::Clock(Clock& parent)
	:m_parent(&parent)
{
	m_parent->AddChild(this);
}

Clock::~Clock()
{
	m_parent = nullptr;
	m_parent->RemoveChild(this);
	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i] = nullptr;
	}
}

void Clock::Reset()
{
	m_lastUpdateTimeInSeconds = 0.0f;
	m_totalSeconds = 0.0f;
	m_deltaSeconds = 0.0f;
	m_frameCount = 0;
	m_timeScale = 1.0f;
	m_isPaused = false;
	m_stepSingleFrame = false;
	m_maxDeltaSeconds = 0.1f;
}

bool Clock::IsPaused() const
{
	return m_isPaused;
}

void Clock::Pause()
{
	m_isPaused = true;
}

void Clock::Unpause()
{
	m_isPaused = false;
	m_stepSingleFrame = false;
}

void Clock::TogglePause()
{
	m_isPaused = !m_isPaused;
}

void Clock::StepSingleFrame()
{
	if (IsPaused())
	{
		Unpause();
	}
	m_stepSingleFrame = true;
}

void Clock::SetTimeScale(float timeScale)
{
	m_timeScale = timeScale;
}

float Clock::GetTimeScale() const
{
	return m_timeScale;
}

float Clock::GetDeltaSeconds() const
{
	return 	m_deltaSeconds;
}

float Clock::GetTotalSeconds() const
{
	return m_totalSeconds;
}

size_t Clock::GetFrameCount() const
{
	return m_frameCount;
}

Clock& Clock::GetSystemClock()
{

	return *s_theSystemClock;
}

void Clock::TickSystemClock()
{
	s_theSystemClock->Tick();
}

void Clock::Tick()
{
	float timeThisFrame = (float)GetCurrentTimeSeconds();

	m_deltaSeconds = timeThisFrame - m_lastUpdateTimeInSeconds;
	m_deltaSeconds = Clamp(m_deltaSeconds, 0.f, m_maxDeltaSeconds);
	m_lastUpdateTimeInSeconds = timeThisFrame;

	Advance(m_deltaSeconds);
}

void Clock::Advance(float deltaTimeSeconds)
{
	m_deltaSeconds = deltaTimeSeconds * m_timeScale;

	if (m_stepSingleFrame)
	{
		Pause();
		m_stepSingleFrame = false;
	}

	if (m_isPaused)
	{
		m_deltaSeconds = 0.0f;
	}

	m_totalSeconds += m_deltaSeconds;
	m_frameCount += 1;

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Advance(m_deltaSeconds);
	}
}

void Clock::AddChild(Clock* childClock)
{
	m_children.push_back(childClock);
}

void Clock::RemoveChild(Clock* childClock)
{
	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] == childClock)
		{
			m_children.erase(m_children.begin() + i);
			break;
		}
	}
}