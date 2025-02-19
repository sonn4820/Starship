#include "Engine/UI/Checkbox.hpp"

Checkbox::Checkbox(Canvas* canvas, UIElement* parent /*= nullptr*/)
	:Checkbox(canvas, false, AABB2(), nullptr, nullptr, nullptr, nullptr, Rgba8(255, 255, 255), Rgba8(150, 150, 150), Rgba8(200, 200, 200), Rgba8::COLOR_RED, parent)
{

}

Checkbox::Checkbox(Canvas* canvas, bool isChecked, UIElement* parent /*= nullptr*/)
	:Checkbox(canvas, isChecked, AABB2(), nullptr, nullptr, nullptr, nullptr, Rgba8(255, 255, 255), Rgba8(150, 150, 150), Rgba8(200, 200, 200), Rgba8::COLOR_RED, parent)
{

}

Checkbox::Checkbox(Canvas* canvas, bool isChecked, AABB2 bound, UIElement* parent /*= nullptr*/)
	:Checkbox(canvas, isChecked, bound, nullptr, nullptr, nullptr, nullptr, Rgba8(255, 255, 255), Rgba8(150, 150, 150), Rgba8(200, 200, 200), Rgba8::COLOR_RED, parent)
{

}

Checkbox::Checkbox(Canvas* canvas, bool isChecked, AABB2 bound, Texture* backgroundImageHover, Texture* backgroundImagePressed, Texture* backgroundImageUnhover, Texture* checkImage, UIElement* parent /*= nullptr*/)
	:Checkbox(canvas, isChecked, bound, backgroundImageHover, backgroundImagePressed, backgroundImageUnhover, checkImage, Rgba8(255, 255, 255), Rgba8(150, 150, 150), Rgba8(200, 200, 200), Rgba8::COLOR_RED, parent)
{

}

Checkbox::Checkbox(Canvas* canvas, bool isChecked, AABB2 bound, Rgba8 backgroundColorHover, Rgba8 backgroundColorPressed, Rgba8 backgroundColorUnhover, Rgba8 checkColor, UIElement* parent /*= nullptr*/)
	:Checkbox(canvas, isChecked, bound, nullptr, nullptr, nullptr, nullptr, backgroundColorHover, backgroundColorPressed, backgroundColorUnhover, checkColor, parent)
{

}

Checkbox::Checkbox(Canvas* canvas, bool isChecked, AABB2 bound, Texture* backgroundImageHover, Texture* backgroundImagePressed, Texture* backgroundImageUnhover, Texture* checkImage, Rgba8 backgroundColorHover, Rgba8 backgroundColorPressed, Rgba8 backgroundColorUnhover, Rgba8 checkColor, UIElement* parent)
{
	SetCanvas(canvas);
	SetChecked(isChecked);
	SetBound(bound);
	float offsetX = (bound.m_maxs.x - bound.m_mins.x) * 0.2f;
	float offsetY = (bound.m_maxs.y - bound.m_mins.y) * 0.2f;
	SetCheckBound(AABB2(bound.m_mins.x + offsetX, bound.m_mins.y + offsetY, bound.m_maxs.x - offsetX, bound.m_maxs.y - offsetY));

	SetBackgroundTextureHover(backgroundImageHover);
	SetBackgroundTexturePressed(backgroundImagePressed);
	SetBackgroundTextureUnhover(backgroundImageUnhover);

	SetCheckTexture(checkImage);

	SetBackgroundColorHover(backgroundColorHover);
	SetBackgroundColorPressed(backgroundColorPressed);
	SetBackgroundColorUnhover(backgroundColorUnhover);

	SetCheckColor(checkColor);

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

Checkbox::~Checkbox()
{
	Shutdown();
}

void Checkbox::SetCanvas(Canvas* canvas)
{
	m_canvas = canvas;
	m_canvas->AddElementToCanvas(this);
}

void Checkbox::Startup()
{
	m_backgroundCurrentColor = m_backgroundColorUnhover;
	m_backgroundCurrentTexutre = m_backgroundTextureHover;
	m_checkColor = Rgba8::COLOR_RED;
	if (m_parent)
	{
		SetActive(m_parent->IsActive());
	}
}

void Checkbox::Update(float deltaSeconds)
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
		m_backgroundCurrentColor = m_backgroundColorHover;
		m_backgroundCurrentTexutre = m_backgroundTextureHover;
		m_onHoverEvent.Invoke();

		if (m_canvas->GetSystemInputSystem()->IsKeyDown(KEYCODE_LEFT_MOUSE))
		{
			m_backgroundCurrentColor = m_backgroundColorPressed;
			m_backgroundCurrentTexutre = m_backgroundTexturePressed;
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
				ToggleChecked();
			}
		}
	}
	else
	{
		SetFocus(false);
		m_backgroundCurrentColor = m_backgroundColorUnhover;
		m_backgroundCurrentTexutre = m_backgroundTextureUnhover;
		m_onUnhoverEvent.Invoke();
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Update(deltaSeconds);
	}
}

void Checkbox::Render() const
{
	if (!m_isActive)
	{
		return;
	}

	m_canvas->GetSystemRenderer()->BeginCamera(*m_canvas->GetCamera());

	m_canvas->GetSystemRenderer()->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	m_canvas->GetSystemRenderer()->SetBlendMode(BlendMode::ALPHA);
	
	m_canvas->GetSystemRenderer()->SetModelConstants(Mat44(), m_backgroundCurrentColor);
	m_canvas->GetSystemRenderer()->BindTexture(m_backgroundCurrentTexutre);
	m_canvas->GetSystemRenderer()->DrawVertexBuffer(m_backgroundBuffer, m_backgroundVerts.size());

	if (IsChecked())
	{
		m_canvas->GetSystemRenderer()->SetModelConstants(Mat44(), m_checkColor);
		m_canvas->GetSystemRenderer()->BindTexture(m_checkTexture);
		m_canvas->GetSystemRenderer()->DrawVertexBuffer(m_checkBuffer, m_checkVerts.size());
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Render();
	}

	m_canvas->GetSystemRenderer()->EndCamera(*m_canvas->GetCamera());
}

void Checkbox::Shutdown()
{
	delete m_checkBuffer;
	m_checkBuffer = nullptr;

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

void Checkbox::ToggleChecked()
{
	m_isChecked = !m_isChecked;
}

void Checkbox::SetChecked(bool isChecked)
{
	m_isChecked = isChecked;
}

bool Checkbox::IsChecked() const
{
	return m_isChecked;
}

void Checkbox::SetBound(AABB2 bound)
{
	m_bound = bound;
	AddVertsForAABB2D(m_backgroundVerts, m_bound);
	m_backgroundBuffer = m_canvas->GetSystemRenderer()->CreateVertexBuffer(sizeof(Vertex_PCU) * (unsigned int)m_backgroundVerts.size());
	m_canvas->GetSystemRenderer()->CopyCPUToGPU(m_backgroundVerts.data(), (unsigned int)m_backgroundVerts.size() * sizeof(Vertex_PCU), m_backgroundBuffer);
}

void Checkbox::SetCheckBound(AABB2 bound)
{
	m_checkBound = bound;
	AddVertsForAABB2D(m_checkVerts, m_checkBound);
	m_checkBuffer = m_canvas->GetSystemRenderer()->CreateVertexBuffer(sizeof(Vertex_PCU) * (unsigned int)m_checkVerts.size());
	m_canvas->GetSystemRenderer()->CopyCPUToGPU(m_checkVerts.data(), (unsigned int)m_checkVerts.size() * sizeof(Vertex_PCU), m_checkBuffer);
}

void Checkbox::SetBackgroundTextureHover(Texture* image)
{
	m_backgroundTextureHover = image;
}

void Checkbox::SetBackgroundTexturePressed(Texture* image)
{
	m_backgroundTexturePressed = image;
}

void Checkbox::SetBackgroundTextureUnhover(Texture* image)
{
	m_backgroundTextureUnhover= image;
}

void Checkbox::SetCheckTexture(Texture* image)
{
	m_checkTexture = image;
}

void Checkbox::SetBackgroundColorHover(Rgba8 color)
{
	m_backgroundColorHover = color;
}

void Checkbox::SetBackgroundColorPressed(Rgba8 color)
{
	m_backgroundColorPressed = color;
}

void Checkbox::SetBackgroundColorUnhover(Rgba8 color)
{
	m_backgroundColorUnhover = color;
}

void Checkbox::SetCheckColor(Rgba8 color)
{
	m_checkColor = color;
}

void Checkbox::OnClickEvent(UICallbackFunctionPointer const& callback)
{
	m_onClickEvent.AddListener(callback);
}

void Checkbox::OnHoverEvent(UICallbackFunctionPointer const& callback)
{
	m_onHoverEvent.AddListener(callback);
}

void Checkbox::OnUnhoverEvent(UICallbackFunctionPointer const& callback)
{
	m_onUnhoverEvent.AddListener(callback);
}

void Checkbox::OnPressedEvent(UICallbackFunctionPointer const& callback)
{
	m_onPressedEvent.AddListener(callback);
}
