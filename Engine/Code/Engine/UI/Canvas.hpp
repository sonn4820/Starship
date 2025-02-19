#pragma once
#include "Engine/UI/UISystem.hpp"
#include "Engine/UI/UIElement.hpp"

class Canvas : public UIElement
{
public:
	Canvas(UISystem* system, Camera* camera);
	~Canvas();

	void Startup() override;
	void Update(float deltaSeconds) override;
	void Render() const override;
	void Shutdown() override;

	void SetCamera(Camera* camera);

	Window* GetSystemWindow() const;
	Renderer* GetSystemRenderer() const;
	InputSystem* GetSystemInputSystem() const;
	BitmapFont* GetSystemFont() const;
	Camera* GetCamera() const;

	void AddElementToCanvas(UIElement* element);


	std::vector<UIElement*> m_uiElementsList;

protected:
	Camera* m_camera = nullptr;
	UISystem* m_system = nullptr;

private:
	void SetCanvas(Canvas* canvas) override; // DON"T USE IT
};