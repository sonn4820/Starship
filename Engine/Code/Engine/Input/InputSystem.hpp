#pragma once
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Input/XboxController.hpp"
#include "Engine/Input/KeyButtonState.hpp"
#include "Engine/Core/NamedStrings.hpp"

typedef NamedStrings EventArgs;

extern unsigned char const KEYCODE_F1;
extern unsigned char const KEYCODE_F2;
extern unsigned char const KEYCODE_F3;
extern unsigned char const KEYCODE_F4;
extern unsigned char const KEYCODE_F5;
extern unsigned char const KEYCODE_F6;
extern unsigned char const KEYCODE_F7;
extern unsigned char const KEYCODE_F8;
extern unsigned char const KEYCODE_F9;
extern unsigned char const KEYCODE_F10;
extern unsigned char const KEYCODE_F11;
extern unsigned char const KEYCODE_SPACE;
extern unsigned char const KEYCODE_SHIFT;
extern unsigned char const KEYCODE_CTRL;
extern unsigned char const KEYCODE_ALT;
extern unsigned char const KEYCODE_ESCAPE;
extern unsigned char const KEYCODE_ENTER;
extern unsigned char const KEYCODE_UPARROW;
extern unsigned char const KEYCODE_DOWNARROW;
extern unsigned char const KEYCODE_LEFTARROW;
extern unsigned char const KEYCODE_RIGHTARROW;
extern unsigned char const KEYCODE_LEFT_MOUSE;
extern unsigned char const KEYCODE_RIGHT_MOUSE;
extern unsigned char const KEYCODE_TIDLE;
extern unsigned char const KEYCODE_LEFTBRACKET;
extern unsigned char const KEYCODE_RIGHTBRACKET;
extern const unsigned char KEYCODE_BACKSPACE;
extern const unsigned char KEYCODE_INSERT;
extern const unsigned char KEYCODE_HOME;
extern const unsigned char KEYCODE_DELETE;
extern const unsigned char KEYCODE_END;
extern const unsigned char KEYCODE_COMMA;
extern const unsigned char KEYCODE_PERIOD;
extern const unsigned char MOUSE_WHEEL;

constexpr int NUM_KEYCODES = 256;
constexpr int NUM_XBOX_CONTROLLER = 4;


struct InputConfig
{

};
struct CursorState
{
	IntVec2 m_cursorClientDelta;
	IntVec2 m_cursorClientPosition;

	bool m_hiddenMode = false;
	bool m_relativeMode = false;
};
class InputSystem
{
public:
	InputSystem(InputConfig	config);
	~InputSystem();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();
	bool WasKeyJustPressed(unsigned char keyCode);
	bool WasKeyJustReleased(unsigned char keyCode);
	bool IsKeyDown(unsigned char keyCode);
	bool HandleKeyPressed(unsigned char keyCode);
	bool HandleKeyReleased(unsigned char keyCode);
	bool WasMouseWheelScrolledUp();
	bool WasMouseWheelScrolledDown();
	void ResetCursorMode();
	XboxController const& GetController(int controllerID);

	static bool Event_KeyPressed(EventArgs& args);
	static bool Event_KeyReleased(EventArgs& args);
	static bool Event_MouseScroll(EventArgs& args);
	
	void SetCursorMode(bool hiddenMode, bool relativeMode);
	Vec2 GetCursorClientDelta() const;
	Vec2 GetCursorNormalizedPosition() const;
	int	 GetMouseScrollValue() const;
	void ResetCursorPosition();

protected:
	InputConfig					m_config;
	KeyButtonState				m_keyStates[NUM_KEYCODES];
	XboxController				m_controllers[NUM_XBOX_CONTROLLER];
	CursorState					m_cursorState;
	int							m_mouseScrollValue = 0;
};

extern InputSystem* g_theInput;


