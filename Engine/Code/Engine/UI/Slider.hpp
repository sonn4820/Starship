#pragma once
#include "Engine/UI/UIElement.hpp"
#include "Engine/UI/Canvas.hpp"

enum SliderOrientation
{
	HORIZONTAL,
	VERTICAL
};
class Slider : public UIElement
{
public:

	Slider(Canvas* canvas, UIElement* parent = nullptr);
	Slider(Canvas* canvas, float minValue, float maxValue, AABB2 backgroundBound, Vec2 knobSize, UIElement* parent = nullptr);
	Slider(Canvas* canvas, float minValue, float maxValue, AABB2 backgroundBound, Vec2 knobSize, Texture* backgroundImage, Texture* knobImage, UIElement* parent = nullptr);
	Slider(Canvas* canvas, float minValue, float maxValue, AABB2 backgroundBound, Vec2 knobSize, Rgba8 backgroundColorHover, Rgba8 backgroundColorPressed, Rgba8 backgroundColorUnhover, Rgba8 knobColorHover, Rgba8 knobColorPressed, Rgba8 knobColorUnhover, UIElement* parent = nullptr);
	Slider(Canvas* canvas, float minValue, float maxValue, AABB2 backgroundBound, Vec2 knobSize, Texture* backgroundImage, Texture* knobImage, Rgba8 backgroundColorHover, Rgba8 backgroundColorPressed, Rgba8 backgroundColorUnhover, Rgba8 knobColorHover, Rgba8 knobColorPressed, Rgba8 knobColorUnhover, UIElement* parent = nullptr);
	virtual ~Slider();

	void Startup() override;
	void Update(float deltaSeconds) override;
	void Render() const override;
	void Shutdown() override;

	void SetCanvas(Canvas* canvas) override;
	void SetBackgroundBound(AABB2 backgroundBound);
	void SetKnobSize(Vec2 knobSize);
	void SetBackgroundImage(Texture* image);
	void SetKnobImage(Texture* image);
	void SetBackgroundColor(Rgba8 hover, Rgba8 pressed, Rgba8 unhover);
	void SetKnobColor(Rgba8 hover, Rgba8 pressed, Rgba8 unhover);

	void UpdateKnobPostion();
	void SetValue(float value);
	void SetValueNormalized(float normalizedValue);
	void SetMinValue(float value);
	void SetMaxValue(float value);
	void SetOrientation(SliderOrientation orientation);

	float GetValue() const;
	float GetValueNormalized() const;
	float GetMinValue() const;
	float GetMaxValue() const;

	void OnHoverEvent(UICallbackFunctionPointer const& callback);
	void OnUnhoverEvent(UICallbackFunctionPointer const& callback);
	void OnChangingValueEvent(UICallbackFunctionPointer const& callback);

public:
	Texture* m_backgroundTexutre = nullptr;
	Texture* m_knobTexutre = nullptr;

	Rgba8 m_backgroundColorHover;
	Rgba8 m_backgroundColorUnhover;
	Rgba8 m_backgroundColorPressed;

	Rgba8 m_knobColorHover;
	Rgba8 m_knobColorUnhover;
	Rgba8 m_knobColorPressed;
protected:

	Canvas* m_canvas = nullptr;

	UIEvent m_onHoverEvent;
	UIEvent m_onUnhoverEvent;
	UIEvent m_onChangingValueEvent;

	std::vector<Vertex_PCU> m_backgroundVerts;
	VertexBuffer* m_backgroundBuffer = nullptr;

	AABB2 m_backgroundBound;
	Vec2 m_knobSize;

	float m_valueNormalized = 0.f;

	bool m_hasClicked = false;

	SliderOrientation m_orientation = HORIZONTAL;

private:
	Rgba8 m_backgroundCurrentColor;
	Rgba8 m_knobCurrentColor;
	float m_minValue = 0.f;
	float m_maxValue = 100.f;
	Vec2 m_knobPosition;
};

