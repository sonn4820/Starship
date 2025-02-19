#pragma once
#include "Engine/UI/UIElement.hpp"
#include "Engine/UI/Canvas.hpp"

class Checkbox : public UIElement
{
public:

	Checkbox(Canvas* canvas, UIElement* parent = nullptr);
	Checkbox(Canvas* canvas, bool isChecked, UIElement* parent = nullptr);
	Checkbox(Canvas* canvas, bool isChecked, AABB2 bound, UIElement* parent = nullptr);
	Checkbox(Canvas* canvas, bool isChecked, AABB2 bound, Texture* backgroundImageHover, Texture* backgroundImagePressed, Texture* backgroundImageUnhover, Texture* checkImage, UIElement* parent = nullptr);
	Checkbox(Canvas* canvas, bool isChecked, AABB2 bound, Rgba8 backgroundColorHover, Rgba8 backgroundColorPressed, Rgba8 backgroundColorUnhover, Rgba8 checkColor, UIElement* parent = nullptr);
	Checkbox(Canvas* canvas, bool isChecked, AABB2 bound, Texture* backgroundImageHover, Texture* backgroundImagePressed, Texture* backgroundImageUnhover, Texture* checkImage, Rgba8 backgroundColorHover, Rgba8 backgroundColorPressed, Rgba8 backgroundColorUnhover, Rgba8 checkColor, UIElement* parent = nullptr);
	virtual ~Checkbox();

	void SetCanvas(Canvas* canvas) override;
	void Startup() override;
	void Update(float deltaSeconds) override;
	void Render() const override;
	void Shutdown() override;

	void ToggleChecked();
	void SetChecked(bool isChecked);
	bool IsChecked() const;

	void SetBound(AABB2 bound);
	void SetCheckBound(AABB2 bound);
	void SetBackgroundTextureHover(Texture* image);
	void SetBackgroundTexturePressed(Texture* image);
	void SetBackgroundTextureUnhover(Texture* image);
	void SetCheckTexture(Texture* image);

	void SetBackgroundColorHover(Rgba8 color);
	void SetBackgroundColorPressed(Rgba8 color);
	void SetBackgroundColorUnhover(Rgba8 color);

	void SetCheckColor(Rgba8 color);

	void OnClickEvent(UICallbackFunctionPointer const& callback);
	void OnHoverEvent(UICallbackFunctionPointer const& callback);
	void OnUnhoverEvent(UICallbackFunctionPointer const& callback);
	void OnPressedEvent(UICallbackFunctionPointer const& callback);

public:
	Texture* m_checkTexture = nullptr;
	Rgba8 m_checkColor;

	Texture* m_backgroundTextureHover = nullptr;
	Texture* m_backgroundTexturePressed = nullptr;
	Texture* m_backgroundTextureUnhover = nullptr;

	Rgba8 m_backgroundColorHover;
	Rgba8 m_backgroundColorUnhover;
	Rgba8 m_backgroundColorPressed;

protected:
	Canvas* m_canvas = nullptr;

	UIEvent m_onClickEvent;
	UIEvent m_onHoverEvent;
	UIEvent m_onUnhoverEvent;
	UIEvent m_onPressedEvent;

	std::vector<Vertex_PCU> m_checkVerts;
	VertexBuffer* m_checkBuffer = nullptr;

	std::vector<Vertex_PCU> m_backgroundVerts;
	VertexBuffer* m_backgroundBuffer = nullptr;

	AABB2 m_bound;
	AABB2 m_checkBound;


private:
	bool m_isChecked = false;
	Rgba8 m_backgroundCurrentColor;
	Texture* m_backgroundCurrentTexutre = nullptr;
};