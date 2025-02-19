#include "Engine/UI/Button.hpp"

Button::Button(Canvas* canvas, bool hasBorder, Rgba8 borderColor, UIElement* parent)
	:Button(canvas, AABB2(), TextSetting(), nullptr, nullptr, nullptr, Rgba8(255, 255, 255), Rgba8(150, 150, 150), Rgba8(200, 200, 200), hasBorder, borderColor, parent)
{

}

Button::Button(Canvas* canvas,AABB2 bound, bool hasBorder, Rgba8 borderColor, UIElement* parent)
	:Button(canvas, bound, TextSetting(), nullptr, nullptr, nullptr, Rgba8(255, 255, 255), Rgba8(150, 150, 150), Rgba8(200, 200, 200), hasBorder, borderColor, parent)
{

}

Button::Button(Canvas* canvas, AABB2 bound, TextSetting textSetting, bool hasBorder, Rgba8 borderColor, UIElement* parent)
	:Button(canvas, bound, textSetting, nullptr, nullptr, nullptr, Rgba8(255, 255, 255), Rgba8(150, 150, 150), Rgba8(200, 200, 200), hasBorder, borderColor, parent)
{

}

Button::Button(Canvas* canvas,AABB2 bound, TextSetting textSetting, Texture* hover, Texture* pressed, Texture* unhover, bool hasBorder, Rgba8 borderColor, UIElement* parent)
	:Button(canvas, bound, textSetting, hover, pressed, unhover, Rgba8(255, 255, 255), Rgba8(150, 150, 150), Rgba8(200, 200, 200), hasBorder, borderColor, parent)
{

}

Button::Button(Canvas* canvas, AABB2 bound, TextSetting textSetting, Rgba8 hoverColor, Rgba8 pressedColor, Rgba8 unhoverColor, bool hasBorder, Rgba8 borderColor, UIElement* parent)
	:Button(canvas, bound, textSetting, nullptr, nullptr, nullptr, hoverColor, pressedColor, unhoverColor, hasBorder, borderColor, parent)
{

}

Button::Button(Canvas* canvas, AABB2 bound, TextSetting textSetting, Texture* hover, Texture* pressed, Texture* unhover, Rgba8 hoverColor, Rgba8 pressedColor, Rgba8 unhoverColor, bool hasBorder, Rgba8 borderColor, UIElement* parent)
{
	m_hasBorder = hasBorder;
	m_borderColor = borderColor;

	SetCanvas(canvas);
	SetBound(bound);
	SetText(textSetting);

	SetTextureHover(hover);
	SetTextureHover(pressed);
	SetTextureHover(unhover);

	SetColorHover(hoverColor);
	SetColorPressed(pressedColor);
	SetColorUnhover(unhoverColor);

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

Button::~Button()
{
	Shutdown();
}

void Button::Startup()
{
	m_currentColor = m_colorUnhover;
	m_currentTexture = m_textureUnhover;
	if (m_parent)
	{
		SetActive(m_parent->IsActive());
	}
}

void Button::Update(float deltaSeconds)
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

	if (m_bound.IsPointInside(mousePos))
	{
		m_currentColor = m_colorHover;
		m_currentTextColor = m_textColorHover;
		m_currentTexture = m_textureHover;
		m_onHoverEvent.Invoke();

		if (m_canvas->GetSystemInputSystem()->IsKeyDown(KEYCODE_LEFT_MOUSE))
		{
			m_currentColor = m_colorPressed;
			m_currentTexture = m_texturePressed;
			m_currentTextColor = m_textColorPressed;
			m_onPressedEvent.Invoke();
			if (!IsOtherElementHavingFocus(m_canvas))
			{
				SetFocus(true);
			}
		}
		else
		{
			if (m_canvas->GetSystemInputSystem()->WasKeyJustReleased(KEYCODE_LEFT_MOUSE))
			{
				SetFocus(false);
				m_onClickEvent.Invoke();
			}
		}
	}
	else
	{
		SetFocus(false);
		m_currentColor = m_colorUnhover;
		m_currentTexture = m_textureUnhover;
		m_currentTextColor = m_textColorUnhover;
		m_onUnhoverEvent.Invoke();
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Update(deltaSeconds);
	}
}

void Button::Render() const
{
	if (!m_isActive)
	{
		return;
	}

	m_canvas->GetSystemRenderer()->BeginCamera(*m_canvas->GetCamera());

	m_canvas->GetSystemRenderer()->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	m_canvas->GetSystemRenderer()->SetBlendMode(BlendMode::ALPHA);

	if (m_buttonBuffer)
	{
		m_canvas->GetSystemRenderer()->SetModelConstants(Mat44(), m_currentColor);
		m_canvas->GetSystemRenderer()->BindTexture(m_currentTexture);
		m_canvas->GetSystemRenderer()->DrawVertexBuffer(m_buttonBuffer, m_buttonVerts.size());
	}

	if (m_textBuffer)
	{
		m_canvas->GetSystemRenderer()->SetModelConstants(Mat44(), m_currentTextColor);
		m_canvas->GetSystemRenderer()->BindTexture(&m_canvas->GetSystemFont()->GetTexture());
		m_canvas->GetSystemRenderer()->DrawVertexBuffer(m_textBuffer, m_textVerts.size());
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Render();
	}

	m_canvas->GetSystemRenderer()->EndCamera(*m_canvas->GetCamera());
}

void Button::Shutdown()
{
	delete m_textBuffer;
	m_textBuffer = nullptr;

	delete m_buttonBuffer;
	m_buttonBuffer = nullptr;

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

void Button::SetCanvas(Canvas* canvas)
{
	m_canvas = canvas;
	m_canvas->AddElementToCanvas(this);
}

void Button::SetBound(AABB2 bound)
{
	m_bound = bound;
	if (m_hasBorder)
	{
		AddVertsForAABB2D(m_buttonVerts, m_bound, m_borderColor);
		AddVertsForAABB2D(m_buttonVerts, m_bound.AddOffSet(5.f));
	}
	else
	{
		AddVertsForAABB2D(m_buttonVerts, m_bound);
	}
	
	m_buttonBuffer = m_canvas->GetSystemRenderer()->CreateVertexBuffer(sizeof(Vertex_PCU) * (int)m_buttonVerts.size());
	m_canvas->GetSystemRenderer()->CopyCPUToGPU(m_buttonVerts.data(), (int)m_buttonVerts.size() * sizeof(Vertex_PCU), m_buttonBuffer);
}

void Button::SetText(TextSetting textSetting)
{
	m_textSetting = textSetting;
	m_canvas->GetSystemFont()->AddVertsForTextInBox2D(m_textVerts, m_bound, m_textSetting.m_height, m_textSetting.m_text, m_textSetting.m_color, m_textSetting.m_aspect, m_textSetting.m_alignment);
	if (!m_textVerts.empty())
	{
		m_textBuffer = m_canvas->GetSystemRenderer()->CreateVertexBuffer(sizeof(Vertex_PCU) * (int)m_textVerts.size());
		m_canvas->GetSystemRenderer()->CopyCPUToGPU(m_textVerts.data(), (int)m_textVerts.size() * sizeof(Vertex_PCU), m_textBuffer);
	}
}

void Button::SetTextureHover(Texture* image)
{
	m_textureHover = image;
}

void Button::SetTexturePressed(Texture* image)
{
	m_texturePressed = image;
}

void Button::SetTextureUnhover(Texture* image)
{
	m_textureUnhover = image;
}

void Button::SetColorHover(Rgba8 color)
{
	m_colorHover = color;
}

void Button::SetColorPressed(Rgba8 color)
{
	m_colorPressed = color;
}

void Button::SetColorUnhover(Rgba8 color)
{
	m_colorUnhover = color;
}

void Button::SetTextColorHover(Rgba8 color)
{
	m_textColorHover = color;
}

void Button::SetTextColorPressed(Rgba8 color)
{
	m_textColorPressed = color;
}

void Button::SetTextColorUnhover(Rgba8 color)
{
	m_textColorUnhover = color;
}

void Button::OnClickEvent(UICallbackFunctionPointer const& callback)
{
	m_onClickEvent.AddListener(callback);
}

void Button::OnHoverEvent(UICallbackFunctionPointer const& callback)
{
	m_onHoverEvent.AddListener(callback);
}

void Button::OnUnhoverEvent(UICallbackFunctionPointer const& callback)
{
	m_onUnhoverEvent.AddListener(callback);
}

void Button::OnPressedEvent(UICallbackFunctionPointer const& callback)
{
	m_onPressedEvent.AddListener(callback);
}
