#include "Engine/UI/Sprite.hpp"

Sprite::Sprite(Canvas* canvas, AABB2 bound, Texture* Sprite, UIElement* parent /*= nullptr*/)
	:Sprite(canvas, bound, Sprite, Rgba8::COLOR_WHITE, parent)
{

}

Sprite::Sprite(Canvas* canvas, AABB2 bound, Texture* Sprite, Rgba8 color /*= Rgba8::COLOR_WHITE*/, UIElement* parent /*= nullptr*/)
{
	SetCanvas(canvas);
	SetBound(bound);
	SetColor(color);
	SetTexture(Sprite);
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

Sprite::~Sprite()
{
	Shutdown();
}

void Sprite::Startup()
{
	if (m_parent)
	{
		SetActive(m_parent->IsActive());
	}
}

void Sprite::Update(float deltaSeconds)
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

void Sprite::Render() const
{
	if (!m_isActive)
	{
		return;
	}

	m_canvas->GetSystemRenderer()->BeginCamera(*m_canvas->GetCamera());

	m_canvas->GetSystemRenderer()->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	m_canvas->GetSystemRenderer()->SetBlendMode(BlendMode::ALPHA);

	m_canvas->GetSystemRenderer()->SetModelConstants(Mat44(), m_color);
	m_canvas->GetSystemRenderer()->BindTexture(m_texture);
	m_canvas->GetSystemRenderer()->DrawVertexBuffer(m_buffer, m_verts.size());

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Render();
	}

	m_canvas->GetSystemRenderer()->EndCamera(*m_canvas->GetCamera());
}

void Sprite::Shutdown()
{
	delete m_buffer;
	m_buffer = nullptr;

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

void Sprite::SetCanvas(Canvas* canvas)
{
	m_canvas = canvas;
	m_canvas->AddElementToCanvas(this);
}

void Sprite::SetBound(AABB2 bound)
{
	m_bound = bound;

	AddVertsForAABB2D(m_verts, m_bound);

	m_buffer = m_canvas->GetSystemRenderer()->CreateVertexBuffer(sizeof(Vertex_PCU) * (unsigned int)m_verts.size());
	m_canvas->GetSystemRenderer()->CopyCPUToGPU(m_verts.data(), (int)m_verts.size() * sizeof(Vertex_PCU), m_buffer);
}

void Sprite::SetTexture(Texture* image)
{
	m_texture = image;
}

void Sprite::SetColor(Rgba8 color)
{
	m_color = color;
}

bool Sprite::IsMouseInBound() const
{
	return m_isMouseInBound;
}

Vec2 Sprite::GetPosition() const
{
	return m_bound.GetCenter();
}

float Sprite::GetWidth() const
{
	return fabsf(m_bound.m_maxs.x - m_bound.m_mins.x);
}

float Sprite::GetHeight() const
{
	return fabsf(m_bound.m_maxs.y - m_bound.m_mins.y);
}
