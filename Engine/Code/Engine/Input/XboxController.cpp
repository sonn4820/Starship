#include "Engine/Input/XboxController.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <Windows.h>
#include <Xinput.h>
#pragma comment( lib, "xinput9_1_0" ) 

XboxController::XboxController()
{

}

XboxController::~XboxController()
{
}

bool XboxController::IsConnected() const
{
	return m_isConnected;
}

int XboxController::GetControllerID() const
{
	return m_id;
}

AnalogJoystick const& XboxController::GetLeftStick() const
{
	return m_leftStick;
}

AnalogJoystick const& XboxController::GetRightStick() const
{
	return m_rightStick;
}

float XboxController::GetLeftTrigger() const
{
	return m_leftTrigger;
}

float XboxController::GetRightTrigger() const
{
	return m_rightTrigger;
}

KeyButtonState const& XboxController::GetButton(XboxButtonID buttonID) const
{
	return m_button[buttonID];
}

bool XboxController::IsButtonDown(XboxButtonID buttonID) const
{
	return GetButton(buttonID).m_isPressed;
}

bool XboxController::WasButtonJustPressed(XboxButtonID buttonID) const
{
	if (!GetButton(buttonID).m_wasPressedLastFrame && GetButton(buttonID).m_isPressed) return true;
	else return false;
}

bool XboxController::WasButtonJustReleased(XboxButtonID buttonID) const
{
	if (GetButton(buttonID).m_wasPressedLastFrame && !GetButton(buttonID).m_isPressed) return true;
	else return false;
}

void XboxController::Update()
{
	XINPUT_STATE xboxControllerState = {};
	DWORD errorStatus = XInputGetState(m_id, &xboxControllerState);
	if (errorStatus != ERROR_SUCCESS) 
	{
		Reset();
		m_isConnected = false;
		return;
	}
	m_isConnected = true;

	XINPUT_GAMEPAD const& state = xboxControllerState.Gamepad;

	UpdateJoystick(m_rightStick, state.sThumbRX, state.sThumbRY);
	UpdateJoystick(m_leftStick, state.sThumbLX, state.sThumbLY);

	UpdateTrigger(m_rightTrigger, state.bRightTrigger);
	UpdateTrigger(m_leftTrigger, state.bLeftTrigger);

	UpdateButton(XBOX_BUTTON_A, state.wButtons, XINPUT_GAMEPAD_A);
	UpdateButton(XBOX_BUTTON_B, state.wButtons, XINPUT_GAMEPAD_B);
	UpdateButton(XBOX_BUTTON_X, state.wButtons, XINPUT_GAMEPAD_X);
	UpdateButton(XBOX_BUTTON_Y, state.wButtons, XINPUT_GAMEPAD_Y);
	UpdateButton(XBOX_BUTTON_START, state.wButtons, XINPUT_GAMEPAD_START);
	UpdateButton(XBOX_BUTTON_BACK, state.wButtons, XINPUT_GAMEPAD_BACK);
	UpdateButton(XBOX_BUTTON_SHOULDER_LEFT, state.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER);
	UpdateButton(XBOX_BUTTON_SHOULDER_RIGHT, state.wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	UpdateButton(XBOX_BUTTON_THUMB_RIGHT, state.wButtons, XINPUT_GAMEPAD_RIGHT_THUMB);
	UpdateButton(XBOX_BUTTON_THUMB_LEFT, state.wButtons, XINPUT_GAMEPAD_LEFT_THUMB);
	UpdateButton(XBOX_BUTTON_DPAD_UP, state.wButtons, XINPUT_GAMEPAD_DPAD_UP);
	UpdateButton(XBOX_BUTTON_DPAD_DOWN, state.wButtons, XINPUT_GAMEPAD_DPAD_DOWN);
	UpdateButton(XBOX_BUTTON_DPAD_RIGHT, state.wButtons, XINPUT_GAMEPAD_DPAD_RIGHT);
	UpdateButton(XBOX_BUTTON_DPAD_LEFT, state.wButtons, XINPUT_GAMEPAD_DPAD_LEFT);
}

void XboxController::Reset()
{
	m_leftStick.Reset();
	m_rightStick.Reset();
	m_leftTrigger = 0;
	m_rightTrigger = 0;
	for (int i = 0; i < NUM; i++)
	{
		m_button[i].m_isPressed = false;
		m_button[i].m_wasPressedLastFrame = false;
	}
}

void XboxController::UpdateJoystick(AnalogJoystick& out_joystick, short rawX, short rawY)
{
	float rawNormalizedX = RangeMapClamped((float)(rawX), -32768.f, 32767.f, -1.f, 1.f);
	float rawNormalizedY = RangeMapClamped((float)(rawY), -32768.f, 32767.f, -1.f, 1.f);
	out_joystick.UpdatePosition(rawNormalizedX, rawNormalizedY);
}

void XboxController::UpdateTrigger(float& out_triggerValue, unsigned char rawValue)
{
	out_triggerValue = RangeMapClamped((float)(rawValue), 5.f, 250.f, 0.f, 1.f);
}

void XboxController::UpdateButton(XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonFlag)
{
	KeyButtonState& button = m_button[buttonID];
	button.m_wasPressedLastFrame = button.m_isPressed;
	button.m_isPressed = (buttonFlags & buttonFlag) == buttonFlag;
}
