#pragma once
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/DebugRender.hpp"
#include <functional>

constexpr float DELAY_TIME = 0.02f;
using UICallbackFunctionPointer = std::function<void()>;
class UIElement;

struct UICallbackFunction
{
	UICallbackFunction(UICallbackFunctionPointer callback, size_t id);
	size_t m_callbackID;
	UICallbackFunctionPointer m_callbackPtr;

	bool operator ==(UICallbackFunction const& compare) { return m_callbackID == compare.m_callbackID; }
};

struct UIConfig
{
	Window* m_window =nullptr;
	Renderer* m_renderer = nullptr;
	InputSystem* m_inputSystem = nullptr;
	BitmapFont* m_font = nullptr;
};

class UISystem
{
public:
	UISystem(UIConfig config);

	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	void SetCamera(Camera* camera);
	int AddFont(BitmapFont* font);

	Window* GetWindow() const;
	Renderer* GetRenderer() const;
	InputSystem* GetInputSystem() const;
	Camera* GetCamera() const;
	BitmapFont* GetFont(int index = 0) const;

private:
	Window* m_window = nullptr;
	Renderer* m_renderer = nullptr; 
	InputSystem* m_inputSystem = nullptr;
	std::vector<BitmapFont*> m_fonts;
};

class UIEvent
{
public:
	size_t AddListener(UICallbackFunctionPointer const& callback);
	void RemoveListener(size_t callbackID);
	void Invoke();

public:
	std::vector<UICallbackFunction*> m_callbackList;
};

struct TextSetting
{
	TextSetting() = default;
	TextSetting(std::string text, float height = 20.f, Rgba8 color = Rgba8::COLOR_BLACK, float orientation = 0, Vec2 alignment = Vec2(0.5f, 0.5f), float lineOffset =0.f, float aspect = 0.5f);
	std::string m_text;
	float m_height = 20.f;
	float m_aspect = 0.5f;
	Rgba8 m_color = Rgba8::COLOR_BLACK;
	Vec2 m_alignment = Vec2(0.5f, 0.5f);
	float m_orientation = 0.f;
	float m_lineOffset = 0.f;

	float GetWidth() const;
	void SetHeight(float height);
};