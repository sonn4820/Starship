#include "Engine/UI/Slider.hpp"

Slider::Slider(Canvas* canvas, UIElement* parent /*= nullptr*/)
	:Slider(canvas, 0, 100.f, AABB2(), Vec2(15, 15), nullptr, nullptr, Rgba8::COLOR_WHITE, Rgba8::COLOR_WHITE, Rgba8::COLOR_WHITE, Rgba8(255, 255, 255), Rgba8(150, 150, 150), Rgba8(200, 200, 200), parent)
{

}

Slider::Slider(Canvas* canvas, float minValue, float maxValue, AABB2 backgroundBound, Vec2 knobSize, UIElement* parent /*= nullptr*/)
	:Slider(canvas, minValue, maxValue, backgroundBound, knobSize, nullptr, nullptr, Rgba8::COLOR_WHITE, Rgba8::COLOR_WHITE, Rgba8::COLOR_WHITE, Rgba8(255, 255, 255), Rgba8(150, 150, 150), Rgba8(200, 200, 200), parent)
{

}

Slider::Slider(Canvas* canvas, float minValue, float maxValue, AABB2 backgroundBound, Vec2 knobSize, Texture* backgroundImage, Texture* knobImage, UIElement* parent /*= nullptr*/)
	:Slider(canvas, minValue, maxValue, backgroundBound, knobSize, backgroundImage, knobImage, Rgba8::COLOR_WHITE, Rgba8::COLOR_WHITE, Rgba8::COLOR_WHITE, Rgba8(255, 255, 255), Rgba8(150, 150, 150), Rgba8(200, 200, 200), parent)
{

}

Slider::Slider(Canvas* canvas, float minValue, float maxValue, AABB2 backgroundBound, Vec2 knobSize, Rgba8 backgroundColorHover, Rgba8 backgroundColorPressed, Rgba8 backgroundColorUnhover, Rgba8 knobColorHover, Rgba8 knobColorPressed, Rgba8 knobColorUnhover, UIElement* parent /*= nullptr*/)
	:Slider(canvas, minValue, maxValue, backgroundBound, knobSize, nullptr, nullptr, backgroundColorHover, backgroundColorPressed, backgroundColorUnhover, knobColorHover, knobColorPressed, knobColorUnhover, parent)
{

}

Slider::Slider(Canvas* canvas, float minValue, float maxValue, AABB2 backgroundBound, Vec2 knobSize, Texture* backgroundImage, Texture* knobImage, Rgba8 backgroundColorHover, Rgba8 backgroundColorPressed, Rgba8 backgroundColorUnhover, Rgba8 knobColorHover, Rgba8 knobColorPressed, Rgba8 knobColorUnhover, UIElement* parent /*= nullptr*/)
{
	SetCanvas(canvas);
	SetMinValue(minValue);
	SetMaxValue(maxValue);
	SetBackgroundBound(backgroundBound);
	SetKnobSize(knobSize);
	SetBackgroundImage(backgroundImage);
	SetKnobImage(knobImage);
	SetBackgroundColor(backgroundColorHover, backgroundColorPressed, backgroundColorUnhover);
	SetKnobColor(knobColorHover, knobColorPressed, knobColorUnhover);
	SetValue(0);

	if (parent)
	{
		parent->AddChild(this);
	}
	else
	{
		canvas->AddChild(this);
	}

	Startup();
}

Slider::~Slider()
{
	Shutdown();
}

void Slider::Startup()
{
	m_backgroundCurrentColor = m_backgroundColorUnhover;
	m_knobCurrentColor = m_knobColorUnhover;

	if (m_parent)
	{
		SetActive(m_parent->IsActive());
	}
}

void Slider::Update(float deltaSeconds)
{
	if (!m_isActive || !m_interactive)
	{
		return;
	}

	m_delayTimer -= deltaSeconds;
	if (m_delayTimer > 0.f)
	{
		return;
	}

	Vec2 mouseNormalizedPos = m_canvas->GetSystemInputSystem()->GetCursorNormalizedPosition();
	Vec2 mousePos = mouseNormalizedPos * m_canvas->GetCamera()->GetOrthographicTopRight();
	AABB2 knobBound = AABB2(m_knobPosition.x - m_knobSize.x * 1.5f, m_knobPosition.y - m_knobSize.y * 1.5f, m_knobPosition.x + m_knobSize.x * 1.5f, m_knobPosition.y + m_knobSize.y * 1.5f);

	if (knobBound.IsPointInside(mousePos))
	{
		m_backgroundCurrentColor = m_backgroundColorHover;
		m_knobCurrentColor = m_knobColorHover;
		m_onHoverEvent.Invoke();

		if (!IsOtherElementHavingFocus(m_canvas))
		{
			m_hasClicked = m_canvas->GetSystemInputSystem()->IsKeyDown(KEYCODE_LEFT_MOUSE);
		}
	}
	else
	{
		m_backgroundCurrentColor = m_backgroundColorUnhover;
		m_knobCurrentColor = m_knobColorUnhover;
		m_onUnhoverEvent.Invoke();
		if (!m_canvas->GetSystemInputSystem()->IsKeyDown(KEYCODE_LEFT_MOUSE))
		{
			m_hasClicked = false;
			SetFocus(false);
		}
	}

	if (m_hasClicked)
	{
		SetFocus(true);
		m_backgroundCurrentColor = m_backgroundColorPressed;
		m_knobCurrentColor = m_knobColorPressed;

		UpdateKnobPostion();
		m_onChangingValueEvent.Invoke();
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Update(deltaSeconds);
	}
}

void Slider::Render() const
{
	if (!m_isActive)
	{
		return;
	}

	m_canvas->GetSystemRenderer()->BeginCamera(*m_canvas->GetCamera());

	m_canvas->GetSystemRenderer()->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	m_canvas->GetSystemRenderer()->SetBlendMode(BlendMode::ALPHA);

	m_canvas->GetSystemRenderer()->SetModelConstants(Mat44(), m_backgroundCurrentColor);
	m_canvas->GetSystemRenderer()->BindTexture(m_backgroundTexutre);
	m_canvas->GetSystemRenderer()->DrawVertexBuffer(m_backgroundBuffer, m_backgroundVerts.size());

	std::vector<Vertex_PCU> knobVerts;
	AABB2 knobBound = AABB2(m_knobPosition.x - m_knobSize.x, m_knobPosition.y - m_knobSize.y, m_knobPosition.x + m_knobSize.x, m_knobPosition.y + m_knobSize.y);
	AddVertsForAABB2D(knobVerts, knobBound);
	m_canvas->GetSystemRenderer()->SetModelConstants(Mat44(), m_knobCurrentColor);
	m_canvas->GetSystemRenderer()->BindTexture(m_knobTexutre);
	m_canvas->GetSystemRenderer()->DrawVertexArray(knobVerts.size(), knobVerts.data());

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Render();
	}

	m_canvas->GetSystemRenderer()->EndCamera(*m_canvas->GetCamera());
}

void Slider::Shutdown()
{
	delete m_backgroundBuffer;
	m_backgroundBuffer = nullptr;

	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children[i])
		{
			m_children[i]->Shutdown();
			delete m_children[i];
		}

		m_children[i] = nullptr;
	}

	m_children.clear();
}

void Slider::SetCanvas(Canvas* canvas)
{
	m_canvas = canvas;
	m_canvas->AddElementToCanvas(this);
}

void Slider::SetBackgroundBound(AABB2 backgroundBound)
{
	m_backgroundBound = backgroundBound;
	AddVertsForAABB2D(m_backgroundVerts, m_backgroundBound);
	m_backgroundBuffer = m_canvas->GetSystemRenderer()->CreateVertexBuffer(sizeof(Vertex_PCU) * (int)m_backgroundVerts.size());
	m_canvas->GetSystemRenderer()->CopyCPUToGPU(m_backgroundVerts.data(), (int)m_backgroundVerts.size() * sizeof(Vertex_PCU), m_backgroundBuffer);
}

void Slider::SetKnobSize(Vec2 knobSize)
{
	m_knobSize = knobSize;
}

void Slider::SetBackgroundImage(Texture* image)
{
	m_backgroundTexutre = image;
}

void Slider::SetKnobImage(Texture* image)
{
	m_knobTexutre = image;
}

void Slider::SetBackgroundColor(Rgba8 hover, Rgba8 pressed, Rgba8 unhover)
{
	m_backgroundColorHover = hover;
	m_backgroundColorPressed = pressed;
	m_backgroundColorUnhover = unhover;
}

void Slider::SetKnobColor(Rgba8 hover, Rgba8 pressed, Rgba8 unhover)
{
	m_knobColorHover = hover;
	m_knobColorPressed = pressed;
	m_knobColorUnhover = unhover;
}

void Slider::UpdateKnobPostion()
{
	float cursorXPosOnScreen = m_canvas->GetSystemInputSystem()->GetCursorNormalizedPosition().x * m_canvas->GetCamera()->GetOrthographicTopRight().x;

	if (m_orientation == HORIZONTAL)
	{
		float xPos = Clamp(cursorXPosOnScreen, m_backgroundBound.m_mins.x, m_backgroundBound.m_maxs.x);
		SetValueNormalized(RangeMapClamped(xPos, m_backgroundBound.m_mins.x, m_backgroundBound.m_maxs.x, 0.f, 1.f));
		m_knobPosition = Vec2(xPos, m_backgroundBound.GetCenter().y);
	}

	if (m_orientation == VERTICAL)
	{
		float yPos = Clamp(cursorXPosOnScreen, m_backgroundBound.m_mins.y, m_backgroundBound.m_maxs.y);
		SetValueNormalized(RangeMapClamped(yPos, m_backgroundBound.m_mins.y, m_backgroundBound.m_maxs.y, 0.f, 1.f));
		m_knobPosition = Vec2(m_backgroundBound.GetCenter().x, yPos);
	}

}

void Slider::SetValue(float value)
{
	SetValueNormalized(RangeMapClamped(value, m_minValue, m_maxValue, 0.f, 1.f));
}

void Slider::SetValueNormalized(float normalizedValue)
{
	m_valueNormalized = normalizedValue;

	if (m_orientation == HORIZONTAL)
	{
		float cursorXPosOnScreen = Interpolate(m_backgroundBound.m_mins.x, m_backgroundBound.m_maxs.x, m_valueNormalized);
		float xPos = Clamp(cursorXPosOnScreen, m_backgroundBound.m_mins.x, m_backgroundBound.m_maxs.x);
		m_knobPosition = Vec2(xPos, m_backgroundBound.GetCenter().y);
	}

	if (m_orientation == VERTICAL)
	{
		float cursorYPosOnScreen = Interpolate(m_backgroundBound.m_mins.y, m_backgroundBound.m_maxs.y, m_valueNormalized);
		float yPos = Clamp(cursorYPosOnScreen, m_backgroundBound.m_mins.y, m_backgroundBound.m_maxs.y);
		m_knobPosition = Vec2(m_backgroundBound.GetCenter().x, yPos);
	}
}

void Slider::SetMinValue(float value)
{
	m_minValue = value;
}

void Slider::SetMaxValue(float value)
{
	m_maxValue = value;
}

void Slider::SetOrientation(SliderOrientation orientation)
{
	m_orientation = orientation;
}

float Slider::GetValue() const
{
	return m_valueNormalized * m_maxValue;
}

float Slider::GetValueNormalized() const
{
	return m_valueNormalized;
}

float Slider::GetMinValue() const
{
	return m_minValue;
}

float Slider::GetMaxValue() const
{
	return m_maxValue;
}

void Slider::OnHoverEvent(UICallbackFunctionPointer const& callback)
{
	m_onHoverEvent.AddListener(callback);
}

void Slider::OnUnhoverEvent(UICallbackFunctionPointer const& callback)
{
	m_onUnhoverEvent.AddListener(callback);
}

void Slider::OnChangingValueEvent(UICallbackFunctionPointer const& callback)
{
	m_onChangingValueEvent.AddListener(callback);
}
