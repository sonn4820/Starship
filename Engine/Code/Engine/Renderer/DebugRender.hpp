#pragma once
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/DevConsole.hpp"

enum class DebugRenderMode
{
	ALWAYS,
	USE_DEPTH,
	XRAY,
};
struct DebugRenderConfig
{
	Renderer* m_renderer = nullptr;
	std::string m_fontName = "SquirrelFixedFont";
};

//Set Up
void DebugRenderSystemStartUp(const DebugRenderConfig& config);
void DebugRenderSystemShutdown();

//Control
void DebugRenderSetVisible();
void DebugRenderSetHidden();
void DebugRenderClear();

//Output
void DebugRenderBeginFrame();
void DebugRenderWorld(const Camera& camera);
void DebugRenderScreen(const Camera& camera);
void DebugRenderEndFrame();

//Geometry
void DebugAddWorldPoint(const Vec3& pos, float radius, float duration, const Rgba8& startColor = Rgba8::COLOR_WHITE, const Rgba8& endColor = Rgba8::COLOR_WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldBasis(const Mat44& transform = Mat44(), float duration = 0, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldLine(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& startColor = Rgba8::COLOR_WHITE, const Rgba8& endColor = Rgba8::COLOR_WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireCylinder(const Vec3& base, const Vec3& top, float radius, float duration, const Rgba8& startColor = Rgba8::COLOR_WHITE, const Rgba8& endColor = Rgba8::COLOR_WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireSphere(const Vec3& center, float radius, float duration, const Rgba8& startColor = Rgba8::COLOR_WHITE, const Rgba8& endColor = Rgba8::COLOR_WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldArrow(const Vec3& start, const Vec3& end, float radius = 0.025f, float duration = 0.f, const Rgba8& startColor = Rgba8::COLOR_WHITE, const Rgba8& endColor = Rgba8::COLOR_WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireArrow(const Vec3& start, const Vec3& end, float radius = 0.025f, float duration = 0.f, const Rgba8& startColor = Rgba8::COLOR_WHITE, const Rgba8& endColor = Rgba8::COLOR_WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldText(const std::string& text, const Mat44& transform, float textHeight, const Vec2& alignment, float duration, const Rgba8& startColor = Rgba8::COLOR_WHITE, const Rgba8& endColor = Rgba8::COLOR_WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldBillboardText(const std::string& text, const Vec3& origin, float textHeight, const Vec2& alignment, float duration, const Rgba8& startColor = Rgba8::COLOR_WHITE, const Rgba8& endColor = Rgba8::COLOR_WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);


void DebugAddScreenText(const std::string& text, const Vec2& position, float size, const Rgba8& startColor = Rgba8::COLOR_WHITE, const Rgba8& endColor = Rgba8::COLOR_WHITE, float duration = 0.f, const Vec2& alignment = Vec2(0.5f, 0.5f));
void DebugAddMessage(const std::string& text, float duration = 0.f, const Rgba8& startColor = Rgba8::COLOR_WHITE, const Rgba8& endColor = Rgba8::COLOR_WHITE);

// Console commands
bool Command_DebugRenderClear(EventArgs& args);
bool Command_DebugRenderToggle(EventArgs& args);