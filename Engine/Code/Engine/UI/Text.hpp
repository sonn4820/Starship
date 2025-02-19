#pragma once
#include "Engine/UI/UIElement.hpp"
#include "Engine/UI/Canvas.hpp"

class Text : public UIElement
{
public:
	Text(Canvas* canvas, UIElement* parent = nullptr);
	Text(Canvas* canvas, Vec2 position,	TextSetting text, UIElement* parent = nullptr);
	~Text();

	void Startup() override;
	void Update(float deltaSeconds) override;
	void Render() const override;
	void Shutdown() override;

	void SetCanvas(Canvas* canvas) override;
	void SetPosition(Vec2 pos);
	void SetTextSetting(TextSetting text);

	void RotateText(float angle);
	void SetTextOffSet(Vec2 translate);

	Vec2 GetPosition() const;

	void SetText(std::string text);

protected:
	Canvas* m_canvas = nullptr;
	Vec2 m_position;
	TextSetting m_textSetting;

	std::vector<Vertex_PCU> m_textVerts;
};