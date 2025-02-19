#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include <string>

class InputSystem;

struct WindowConfig
{
	InputSystem* m_inputSystem = nullptr;
	std::string m_windowTitle = "Untitled App";
	float m_clientAspect = 2.0f;
	bool m_isFullscreen = false;
	IntVec2 m_size = IntVec2(-1, -1);
	IntVec2 m_pos = IntVec2(-1, -1);
};

bool IsMousePresent();

class Window
{
public:
	Window(WindowConfig const& config);

	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	WindowConfig const& GetConfig() const;
	float GetAspect() const;
	void* GetDisplayContext() const;
	//Vec2 GetNormalizedCursorPos() const;

	void* GetHwnd() const;
	IntVec2 GetClientDimensions() const;
	bool GetFileName(std::string& outPath);

	static Window* GetMainWindowInstance();


protected:
	void CreateOSWindow();
	void RunMessagePump();

protected:
	static Window* s_theWindow;

protected:
	WindowConfig	m_config;
	void*			m_hwnd		= nullptr;
	void*			m_dc		= nullptr;
	IntVec2 m_clientDimension;
};