#include "Engine/UI/Text.hpp"

Text::Text(Canvas* canvas, UIElement* parent /*= nullptr*/)
	:Text(canvas, Vec2(), TextSetting(), parent)
{

}

Text::Text(Canvas* canvas, Vec2 position, TextSetting text, UIElement* parent /*= nullptr*/)
{
	SetCanvas(canvas);
	SetPosition(position);
	SetTextSetting(text);

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

Text::~Text()
{
	Shutdown();
}

void Text::Startup()
{
	if (m_parent)
	{
		SetActive(m_parent->IsActive());
	}
}

void Text::Update(float deltaSeconds)
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
}

void Text::Render() const
{
	if (!m_isActive)
	{
		return;
	}

	m_canvas->GetSystemRenderer()->BeginCamera(*m_canvas->GetCamera());

	m_canvas->GetSystemRenderer()->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	m_canvas->GetSystemRenderer()->SetBlendMode(BlendMode::ALPHA);
	
	m_canvas->GetSystemRenderer()->SetModelConstants(Mat44::CreateTranslation2D(m_position), m_textSetting.m_color);

	m_canvas->GetSystemRenderer()->BindTexture(&m_canvas->GetSystemFont()->GetTexture());
	m_canvas->GetSystemRenderer()->DrawVertexArray((int)m_textVerts.size(), m_textVerts.data());

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Render();
	}

	m_canvas->GetSystemRenderer()->EndCamera(*m_canvas->GetCamera());
}

void Text::Shutdown()
{
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

void Text::SetCanvas(Canvas* canvas)
{
	m_canvas = canvas;
	m_canvas->AddElementToCanvas(this);
}

void Text::SetPosition(Vec2 pos)
{
	m_position = pos;
}

void Text::SetTextSetting(TextSetting textSetting)
{
	m_textSetting = textSetting;
	m_canvas->GetSystemFont()->AddVertsForText2D(m_textVerts, Vec2(), m_textSetting.m_height, m_textSetting.m_text, m_textSetting.m_color, m_textSetting.m_aspect, m_textSetting.m_lineOffset, m_textSetting.m_alignment.x);
	//RotateText(m_textSetting.m_orientation);
}

void Text::RotateText(float angle)
{
	float aspect = 1 / Window::GetMainWindowInstance()->GetAspect();
	float width = m_canvas->GetSystemFont()->GetTextWidth(m_textSetting.m_height, m_textSetting.m_text, m_textSetting.m_aspect);
	Vec2 pos = m_position - Vec2(width, m_textSetting.m_height) * aspect * m_textSetting.m_alignment;
	TransformVertexArrayXY3D((int)m_textVerts.size(), m_textVerts.data(), 1.f, angle, pos);
}

void Text::SetTextOffSet(Vec2 translate)
{
	float aspect = 1 / Window::GetMainWindowInstance()->GetAspect();
	TransformVertexArrayXY3D((int)m_textVerts.size(), m_textVerts.data(), 1.f, 0.f, translate * aspect);
}

Vec2 Text::GetPosition() const
{
	return m_position;
}

void Text::SetText(std::string text)
{
	m_textSetting.m_text = text;
	m_textVerts.clear();
	SetTextSetting(m_textSetting);
}
