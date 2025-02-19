#pragma once
#include "Engine/UI/UIElement.hpp"
#include "Engine/UI/Canvas.hpp"

struct Button : public UIElement
{
	Button(Canvas* canvas, bool hasBorder = true, Rgba8 borderColor = Rgba8::COLOR_BLACK, UIElement* parent = nullptr);
	Button(Canvas* canvas, AABB2 bound, bool hasBorder = true, Rgba8 borderColor = Rgba8::COLOR_BLACK, UIElement* parent = nullptr);
	Button(Canvas* canvas, AABB2 bound, TextSetting textSetting, bool hasBorder = true, Rgba8 borderColor = Rgba8::COLOR_BLACK, UIElement* parent = nullptr);
	Button(Canvas* canvas, AABB2 bound, TextSetting textSetting, Rgba8 hoverColor, Rgba8 pressedColor, Rgba8 unhoverColor, bool hasBorder = true, Rgba8 borderColor = Rgba8::COLOR_BLACK, UIElement* parent = nullptr);
	Button(Canvas* canvas, AABB2 bound, TextSetting textSetting, Texture* hover, Texture* pressed, Texture* unhover, bool hasBorder = true, Rgba8 borderColor = Rgba8::COLOR_BLACK, UIElement* parent = nullptr);
	Button(Canvas* canvas, AABB2 bound, TextSetting textSetting, Texture* hover, Texture* pressed, Texture* unhover, Rgba8 hoverColor, Rgba8 pressedColor, Rgba8 unhoverColor, bool hasBorder = true, Rgba8 borderColor = Rgba8::COLOR_BLACK, UIElement* parent = nullptr);
	virtual ~Button();

	void Startup() override;
	void Update(float deltaSeconds) override;
	void Render() const override;
	void Shutdown() override;

	void SetCanvas(Canvas* canvas) override;
	void SetBound(AABB2 bound);
	void SetText(TextSetting textSetting);

	void SetTextureHover(Texture* image);
	void SetTexturePressed(Texture* image);
	void SetTextureUnhover(Texture* image);

	void SetColorHover(Rgba8 color);
	void SetColorPressed(Rgba8 color);
	void SetColorUnhover(Rgba8 color);

	void SetTextColorHover(Rgba8 color);
	void SetTextColorPressed(Rgba8 color);
	void SetTextColorUnhover(Rgba8 color);

	void OnClickEvent(UICallbackFunctionPointer const& callback);
	void OnHoverEvent(UICallbackFunctionPointer const& callback);
	void OnUnhoverEvent(UICallbackFunctionPointer const& callback);
	void OnPressedEvent(UICallbackFunctionPointer const& callback);

public:
	bool m_hasBorder = false;
	Rgba8 m_borderColor = Rgba8(255, 255, 255);

	Texture* m_textureHover = nullptr;
	Texture* m_texturePressed = nullptr;
	Texture* m_textureUnhover = nullptr;

	Rgba8 m_colorUnhover = Rgba8(200, 200, 200);
	Rgba8 m_colorHover = Rgba8(255, 255, 255);
	Rgba8 m_colorPressed = Rgba8(150, 150, 150);

	Rgba8 m_textColorUnhover = Rgba8(20, 20, 20);
	Rgba8 m_textColorHover = Rgba8(0, 0, 0);
	Rgba8 m_textColorPressed = Rgba8(255, 255, 255);

protected:
	Canvas* m_canvas = nullptr;

	UIEvent m_onClickEvent;
	UIEvent m_onHoverEvent;
	UIEvent m_onUnhoverEvent;
	UIEvent m_onPressedEvent;

	std::vector<Vertex_PCU> m_textVerts;
	VertexBuffer* m_textBuffer = nullptr;

	std::vector<Vertex_PCU> m_buttonVerts;
	VertexBuffer* m_buttonBuffer = nullptr;

	AABB2 m_bound;
	TextSetting m_textSetting;

private:
	Texture* m_currentTexture = nullptr;
	Rgba8 m_currentColor;
	Rgba8 m_currentTextColor;
};