#include "Engine/Core/Timer.hpp"
#include "Engine/Core/Clock.hpp"

Timer::Timer(float period, const Clock* clock)
	:m_period(period), m_clock(clock)
{
}

void Timer::Start()
{
	m_startTime = m_clock->GetTotalSeconds();
}

void Timer::Stop()
{
	m_startTime = 0.f;
}

float Timer::GetElapsedTime() const
{
	if (IsStopped())
	{
		return 0.f;
	}
	else
	{
		return static_cast<float>(m_clock->GetTotalSeconds() - m_startTime);
	}
}

float Timer::GetElapsedFraction() const
{
	return GetElapsedTime() / m_period;
}

bool Timer::IsStopped() const
{
	return m_startTime == 0.f;
}

bool Timer::HasPeriodElapsed() const
{
	return !IsStopped() && GetElapsedFraction() > 1.f;
}

bool Timer::DecrementPeriodIfElapsed()
{
	if (HasPeriodElapsed())
	{
		m_startTime += m_period;
		return true;
	}

	return false;
}
