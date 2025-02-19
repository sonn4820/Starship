#include "Engine/UI/Panel.hpp"

Panel::Panel(Canvas* canvas, bool hasBorder, Rgba8 borderColor, UIElement* parent /*= nullptr*/)
	:Panel(canvas, AABB2(), Rgba8::COLOR_WHITE, nullptr, hasBorder, borderColor, parent)
{

}

Panel::Panel(Canvas* canvas, AABB2 bound, bool hasBorder, Rgba8 borderColor, UIElement* parent /*= nullptr*/)
	:Panel(canvas, bound, Rgba8::COLOR_WHITE, nullptr, hasBorder, borderColor, parent)
{

}

Panel::Panel(Canvas* canvas, AABB2 bound, Rgba8 color, bool hasBorder, Rgba8 borderColor, UIElement* parent /*= nullptr*/)
	:Panel(canvas, bound, color, nullptr, hasBorder, borderColor, parent)
{

}

Panel::Panel(Canvas* canvas, AABB2 bound, Rgba8 color, Texture* image, bool hasBorder, Rgba8 borderColor, UIElement* parent /*= nullptr*/)
{
	m_hasBorder = hasBorder;
	m_borderColor = borderColor;

	SetCanvas(canvas);
	SetBound(bound);
	SetColor(color);
	SetTexture(image);
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

Panel::~Panel()
{
	Shutdown();
}

void Panel::Startup()
{
	if (m_parent)
	{
		SetActive(m_parent->IsActive());
	}
}

void Panel::Update(float deltaSeconds)
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
	m_isMouseInBound = m_bound.IsPointInside(mousePos);

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Update(deltaSeconds);
	}
}

void Panel::Render() const
{
	if (!m_isActive)
	{
		return;
	}

	m_canvas->GetSystemRenderer()->BeginCamera(*m_canvas->GetCamera());

	m_canvas->GetSystemRenderer()->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	m_canvas->GetSystemRenderer()->SetBlendMode(BlendMode::ALPHA);

	if (m_hasBorder)
	{
		m_canvas->GetSystemRenderer()->SetModelConstants(Mat44(), m_borderColor);
		m_canvas->GetSystemRenderer()->BindTexture(nullptr);
		m_canvas->GetSystemRenderer()->DrawVertexBuffer(m_borderBuffer, m_borderVerts.size());
	}

	m_canvas->GetSystemRenderer()->SetModelConstants(Mat44(), m_color);
	m_canvas->GetSystemRenderer()->BindTexture(m_texture);
	m_canvas->GetSystemRenderer()->DrawVertexBuffer(m_panelBuffer, m_panelVerts.size());

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Render();
	}

	m_canvas->GetSystemRenderer()->EndCamera(*m_canvas->GetCamera());
}

void Panel::Shutdown()
{
	delete m_panelBuffer;
	m_panelBuffer = nullptr;

	delete m_borderBuffer;
	m_borderBuffer = nullptr;

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

void Panel::SetCanvas(Canvas* canvas)
{
	m_canvas = canvas;
	m_canvas->AddElementToCanvas(this);
}

void Panel::SetBound(AABB2 bound)
{
	m_bound = bound;

	if (m_hasBorder)
	{
		AddVertsForAABB2D(m_borderVerts, m_bound, m_borderColor);
		AddVertsForAABB2D(m_panelVerts, m_bound.AddOffSet(5.f));
	}
	else
	{
		AddVertsForAABB2D(m_panelVerts, m_bound);
	}

	m_panelBuffer = m_canvas->GetSystemRenderer()->CreateVertexBuffer(sizeof(Vertex_PCU) * (unsigned int)m_panelVerts.size());
	m_canvas->GetSystemRenderer()->CopyCPUToGPU(m_panelVerts.data(), (int)m_panelVerts.size() * sizeof(Vertex_PCU), m_panelBuffer);

	if (m_hasBorder)
	{
		m_borderBuffer = m_canvas->GetSystemRenderer()->CreateVertexBuffer(sizeof(Vertex_PCU) * (unsigned int)m_borderVerts.size());
		m_canvas->GetSystemRenderer()->CopyCPUToGPU(m_borderVerts.data(), (int)m_borderVerts.size() * sizeof(Vertex_PCU), m_borderBuffer);
	}
}

void Panel::SetTexture(Texture* image)
{
	m_texture = image;
}

void Panel::SetColor(Rgba8 color)
{
	m_color = color;
}

bool Panel::IsMouseInBound() const
{
	return m_isMouseInBound;
}

Vec2 Panel::GetPosition() const
{
	return m_bound.GetCenter();
}

float Panel::GetWidth() const
{
	return fabsf(m_bound.m_maxs.x - m_bound.m_mins.x);
}

float Panel::GetHeight() const
{
	return fabsf(m_bound.m_maxs.y - m_bound.m_mins.y);
}
