#include "Engine/UI/UIElement.hpp"
#include "Engine/UI/Canvas.hpp"

UIElement::UIElement()
{

}

bool UIElement::AddChild(UIElement* child)
{
	m_children.push_back(child);
	child->m_parent = this;

	return true;
}

bool UIElement::RemoveChild(UIElement* child)
{
	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] == child)
		{
			m_children.erase(m_children.begin() + i);
			return true;
		}
	}
	return false;
}

void UIElement::SetParent(UIElement* parent)
{
	m_parent = parent;
	if (m_parent)
	{
		SetActive(m_parent->IsActive());
	}
}

void UIElement::SetActive(bool isActive)
{
	m_isActive = isActive;
	m_delayTimer = DELAY_TIME;

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->SetActive(isActive);
	}
}

void UIElement::SetFocus(bool focus)
{
	m_hasFocus = focus;
}

bool UIElement::IsActive() const
{
	return m_isActive;
}

bool UIElement::HasFocus() const
{
	return m_hasFocus;
}

bool UIElement::IsOtherElementHavingFocus(Canvas* canvas) const
{
	for (size_t i = 0; i < canvas->m_uiElementsList.size(); i++)
	{
		if (canvas->m_uiElementsList[i]->HasFocus() && canvas->m_uiElementsList[i] != this)
		{
			return true;
		}
	}

	return false;
}
