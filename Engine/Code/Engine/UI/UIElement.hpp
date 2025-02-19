#pragma once
#include "Engine/UI/UISystem.hpp"

class Canvas;

class UIElement
{
public:
	UIElement();
	virtual ~UIElement() = default;

	virtual void SetCanvas(Canvas* canvas) = 0;
	virtual void Startup() = 0;
	virtual void Update(float deltaSeconds) = 0;
	virtual void Render() const = 0;
	virtual void Shutdown() = 0;

	bool AddChild(UIElement* child);
	bool RemoveChild(UIElement* child);
	void SetParent(UIElement* parent);

	void SetActive(bool isActive);
	void SetFocus(bool focus);
	bool IsActive() const;
	bool HasFocus() const;

	bool IsOtherElementHavingFocus(Canvas* canvas) const;

protected:
	Canvas* m_canvas = nullptr;
	UIElement* m_parent = nullptr;
	std::vector< UIElement*> m_children;

	bool m_interactive = true;
	bool m_isActive = true;
	bool m_hasFocus = false;
	float m_delayTimer = DELAY_TIME;
};