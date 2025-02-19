#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#include<Windows.h>

const unsigned char KEYCODE_F1 = VK_F1;
const unsigned char KEYCODE_F2 = VK_F2;
const unsigned char KEYCODE_F3 = VK_F3;
const unsigned char KEYCODE_F4 = VK_F4;
const unsigned char KEYCODE_F5 = VK_F5;
const unsigned char KEYCODE_F6 = VK_F6;
const unsigned char KEYCODE_F7 = VK_F7;
const unsigned char KEYCODE_F8 = VK_F8;
const unsigned char KEYCODE_F9 = VK_F9;
const unsigned char KEYCODE_F10 = VK_F10;
const unsigned char KEYCODE_F11 = VK_F11;
const unsigned char KEYCODE_SPACE = VK_SPACE;
const unsigned char KEYCODE_SHIFT = VK_SHIFT;
const unsigned char KEYCODE_CTRL = VK_CONTROL;
const unsigned char KEYCODE_ALT = VK_MENU;
const unsigned char KEYCODE_ESCAPE = VK_ESCAPE;
const unsigned char KEYCODE_ENTER = VK_RETURN;
const unsigned char KEYCODE_UPARROW = VK_UP;
const unsigned char KEYCODE_DOWNARROW = VK_DOWN;
const unsigned char KEYCODE_LEFTARROW = VK_LEFT;
const unsigned char KEYCODE_RIGHTARROW = VK_RIGHT;
const unsigned char KEYCODE_LEFT_MOUSE = VK_LBUTTON;
const unsigned char KEYCODE_RIGHT_MOUSE = VK_RBUTTON;
const unsigned char KEYCODE_TIDLE = 0xC0;
const unsigned char KEYCODE_LEFTBRACKET = 0xDB;
const unsigned char KEYCODE_RIGHTBRACKET = 0xDD;
const unsigned char KEYCODE_BACKSPACE = VK_BACK;
const unsigned char KEYCODE_INSERT = VK_INSERT;
const unsigned char KEYCODE_HOME = VK_HOME;
const unsigned char KEYCODE_DELETE = VK_DELETE;
const unsigned char KEYCODE_END = VK_END;
const unsigned char KEYCODE_COMMA = VK_OEM_COMMA;
const unsigned char KEYCODE_PERIOD = VK_OEM_PERIOD;
const unsigned char MOUSE_WHEEL = WH_MOUSE;



InputSystem* g_theInput = nullptr;

InputSystem::InputSystem(InputConfig config)
	:m_config(config)
{
	for (int controllerIndex = 0; controllerIndex < NUM_XBOX_CONTROLLER; controllerIndex++)
	{
		m_controllers[controllerIndex].m_id = controllerIndex;
		m_controllers[controllerIndex].m_leftStick.SetDeadZoneThresholds(0.3f, 0.95f);
		m_controllers[controllerIndex].m_rightStick.SetDeadZoneThresholds(0.3f, 0.95f);
	}
}

InputSystem::~InputSystem()
{
}

void InputSystem::Startup()
{
	g_theEventSystem->SubscribeEventCallbackFunction("KeyPressed", InputSystem::Event_KeyPressed);
	g_theEventSystem->SubscribeEventCallbackFunction("KeyReleased", InputSystem::Event_KeyReleased);
	g_theEventSystem->SubscribeEventCallbackFunction("MouseScroll", InputSystem::Event_MouseScroll);
}

void InputSystem::Shutdown()
{
}

void InputSystem::BeginFrame()
{
	for (int controllerIndex = 0; controllerIndex < NUM_XBOX_CONTROLLER; controllerIndex++)
	{
		m_controllers[controllerIndex].Update();
	}

	if (m_cursorState.m_hiddenMode)
	{
		while (ShowCursor(FALSE) >= 0);
	}
	else
	{
		while (ShowCursor(TRUE) < 0);
	}

	IntVec2 lastCursorPos = m_cursorState.m_cursorClientPosition;

	HWND windowHandle = HWND(Window::GetMainWindowInstance()->GetHwnd());

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(windowHandle, &pt);
	m_cursorState.m_cursorClientPosition.x = pt.x;
	m_cursorState.m_cursorClientPosition.y = pt.y;

	if (m_cursorState.m_relativeMode)
	{
		m_cursorState.m_cursorClientDelta.x = m_cursorState.m_cursorClientPosition.x - lastCursorPos.x;
		m_cursorState.m_cursorClientDelta.y = m_cursorState.m_cursorClientPosition.y - lastCursorPos.y;

		POINT ptCenter;
		ptCenter.x = Window::GetMainWindowInstance()->GetClientDimensions().x / 2;
		ptCenter.y = Window::GetMainWindowInstance()->GetClientDimensions().y / 2;
		ClientToScreen(windowHandle, &ptCenter);
		SetCursorPos(ptCenter.x, ptCenter.y);
		GetCursorPos(&pt);
		ScreenToClient(windowHandle, &pt);
		m_cursorState.m_cursorClientPosition.x = pt.x;
		m_cursorState.m_cursorClientPosition.y = pt.y;
	}
	else
	{
		m_cursorState.m_cursorClientDelta.x = m_cursorState.m_cursorClientPosition.x - lastCursorPos.x;
		m_cursorState.m_cursorClientDelta.y = m_cursorState.m_cursorClientPosition.y - lastCursorPos.y;
	}

	if (!GetFocus())
	{
		m_cursorState.m_hiddenMode = false;
	}
}
void InputSystem::EndFrame()
{
	for (int i = 0; i < 256; i++)
	{
		m_keyStates[i].m_wasPressedLastFrame = m_keyStates[i].m_isPressed;
	}
	for (int controllerIndex = 0; controllerIndex < NUM_XBOX_CONTROLLER; controllerIndex++)
	{
		for (int i = 0; i < 14; i++)
		{
			m_controllers[controllerIndex].m_button[i].m_wasPressedLastFrame = m_controllers[controllerIndex].m_button[i].m_isPressed;
		}
	}
	m_mouseScrollValue = 0;
}
bool InputSystem::WasKeyJustPressed(unsigned char keyCode)
{
	if (!m_keyStates[keyCode].m_wasPressedLastFrame && m_keyStates[keyCode].m_isPressed) return true;
	return false;
}

bool InputSystem::WasKeyJustReleased(unsigned char keyCode)
{
	if (m_keyStates[keyCode].m_wasPressedLastFrame && !m_keyStates[keyCode].m_isPressed) return true;
	return false;
}

bool InputSystem::IsKeyDown(unsigned char keyCode)
{
	return m_keyStates[keyCode].m_isPressed;
}

bool InputSystem::HandleKeyPressed(unsigned char keyCode)
{
	m_keyStates[keyCode].m_isPressed = true;
	return m_keyStates[keyCode].m_isPressed;
}

bool InputSystem::HandleKeyReleased(unsigned char keyCode)
{
	m_keyStates[keyCode].m_isPressed = false;
	return m_keyStates[keyCode].m_isPressed;
}

bool InputSystem::WasMouseWheelScrolledUp()
{
	return m_mouseScrollValue > 0;
}

bool InputSystem::WasMouseWheelScrolledDown()
{

	return m_mouseScrollValue < 0;
}

void InputSystem::ResetCursorMode()
{
	if (m_cursorState.m_hiddenMode)
	{
		while (ShowCursor(false) >= 0);
	}
	else
	{
		while (ShowCursor(true) < 0);
	}
}

XboxController const& InputSystem::GetController(int controllerID)
{
	return m_controllers[controllerID];
}

bool InputSystem::Event_KeyPressed(EventArgs& args)
{
	if (g_theDevConsole->IsOpen())
	{
		return false;
	}
	if (!g_theInput)
	{
		return false;
	}
	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	g_theInput->HandleKeyPressed(keyCode);
	return true;
}
bool InputSystem::Event_KeyReleased(EventArgs& args)
{
	if (!g_theInput)
	{
		return false;
	}
	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	g_theInput->HandleKeyReleased(keyCode);
	return true;
}

bool InputSystem::Event_MouseScroll(EventArgs& args)
{
	g_theInput->m_mouseScrollValue = (int)args.GetValue("MouseWheel", 0);
	return true;
}

void InputSystem::SetCursorMode(bool hiddenMode, bool relativeMode)
{
	m_cursorState.m_hiddenMode = hiddenMode;
	m_cursorState.m_relativeMode = relativeMode;
}

Vec2 InputSystem::GetCursorClientDelta() const
{
	float x = (float)m_cursorState.m_cursorClientDelta.x;
	float y = (float)m_cursorState.m_cursorClientDelta.y;
	return Vec2(x, y);
}

Vec2 InputSystem::GetCursorNormalizedPosition() const
{
	HWND windowHandle = HWND(Window::GetMainWindowInstance()->GetHwnd());
	POINT cursorCoords;
	RECT clientRect;
	::GetCursorPos(&cursorCoords);
	::ScreenToClient(windowHandle, &cursorCoords);
	::GetClientRect(windowHandle, &clientRect);
	float cursorX = float(cursorCoords.x) / float(clientRect.right);
	float cursorY = float(cursorCoords.y) / float(clientRect.bottom);
	return Vec2(cursorX, 1.f - cursorY);
}

int InputSystem::GetMouseScrollValue() const
{
	return m_mouseScrollValue;
}

void InputSystem::ResetCursorPosition()
{
	IntVec2 lastCursorPos = m_cursorState.m_cursorClientPosition;

	HWND windowHandle = HWND(Window::GetMainWindowInstance()->GetHwnd());

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(windowHandle, &pt);
	m_cursorState.m_cursorClientPosition.x = pt.x;
	m_cursorState.m_cursorClientPosition.y = pt.y;

	m_cursorState.m_cursorClientDelta.x = m_cursorState.m_cursorClientPosition.x - lastCursorPos.x;
	m_cursorState.m_cursorClientDelta.y = m_cursorState.m_cursorClientPosition.y - lastCursorPos.y;

	POINT ptCenter;
	ptCenter.x = Window::GetMainWindowInstance()->GetClientDimensions().x / 2;
	ptCenter.y = Window::GetMainWindowInstance()->GetClientDimensions().y / 2;
	ClientToScreen(windowHandle, &ptCenter);
	SetCursorPos(ptCenter.x, ptCenter.y);
	GetCursorPos(&pt);
	ScreenToClient(windowHandle, &pt);
	m_cursorState.m_cursorClientPosition.x = pt.x;
	m_cursorState.m_cursorClientPosition.y = pt.y;
}
