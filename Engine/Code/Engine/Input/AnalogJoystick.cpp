#include "Engine/Input/AnalogJoystick.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec2.hpp"

Vec2 AnalogJoystick::GetPosition() const
{
    return m_correctedPosition;
}

float AnalogJoystick::GetMagnitude() const
{
    return m_correctedPosition.GetLength();
}

float AnalogJoystick::GetOrientationDegrees() const
{
    return m_correctedPosition.GetOrientationDegrees();
}

Vec2 AnalogJoystick::GetRawUncorrectedPosition() const
{
    return m_rawPosition;
}

float AnalogJoystick::GetInnerDeadZoneFraction() const
{
    return m_innerDeadZoneFraction;
}

float AnalogJoystick::GetOuterDeadZoneFraction() const
{
    return m_outerDeadZoneFraction;
}

void AnalogJoystick::Reset()
{
	m_innerDeadZoneFraction = 0.00f;
	m_outerDeadZoneFraction = 1.00f;
    UpdatePosition(0.f, 0.f);
}

void AnalogJoystick::SetDeadZoneThresholds(float normalizedInnerDeadZoneThreshold, float normalizedOuterDeadZoneThreshold)
{
    m_innerDeadZoneFraction = normalizedInnerDeadZoneThreshold;
    m_outerDeadZoneFraction = normalizedOuterDeadZoneThreshold;
}

void AnalogJoystick::UpdatePosition(float rawNormalizedX, float rawNormalizedY)
{
    m_rawPosition = Vec2(rawNormalizedX, rawNormalizedY);
    float uncorrectedPositionLength = m_rawPosition.GetLength();
    float uncorrectedPositionDegrees = m_rawPosition.GetOrientationDegrees();

    float correctedPositionLength = RangeMapClamped(uncorrectedPositionLength, m_innerDeadZoneFraction, m_outerDeadZoneFraction, 0.f, 1.f);
    m_correctedPosition.SetPolarDegrees(uncorrectedPositionDegrees, correctedPositionLength);
}
