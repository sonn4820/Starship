#include "Engine/UI/Canvas.hpp"

Canvas::Canvas(UISystem* system, Camera* camera)
	:m_system(system), m_camera(camera)
{
	
}

Canvas::~Canvas()
{
	Shutdown();
}

void Canvas::SetCamera(Camera* camera)
{
	m_camera = camera;
}

Window* Canvas::GetSystemWindow() const
{
	return m_system->GetWindow();
}

Renderer* Canvas::GetSystemRenderer() const
{
	return m_system->GetRenderer();
}

InputSystem* Canvas::GetSystemInputSystem() const
{
	return m_system->GetInputSystem();
}

BitmapFont* Canvas::GetSystemFont() const
{
	return m_system->GetFont();
}

Camera* Canvas::GetCamera() const
{
	return m_camera;
}

void Canvas::AddElementToCanvas(UIElement* element)
{
	for (size_t i = 0; i < m_uiElementsList.size(); i++)
	{
		if (!m_uiElementsList[i])
		{
			m_uiElementsList[i] = element;
			return;
		}
	}
	m_uiElementsList.push_back(element);
}

void Canvas::SetCanvas(Canvas* canvas)
{
	UNUSED(canvas);
	return;
}

void Canvas::Startup()
{
	if (m_parent)
	{
		SetActive(m_parent->IsActive());
	}
}

void Canvas::Update(float deltaSeconds)
{
	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Update(deltaSeconds);
	}
}

void Canvas::Render() const
{
	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Render();
	}
}

void Canvas::Shutdown()
{
	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Shutdown();
		delete m_children[i];
		m_children[i] = nullptr;
	}

	m_children.clear();
}
