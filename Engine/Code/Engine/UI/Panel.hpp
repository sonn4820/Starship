#pragma once
#include "Engine/UI/UIElement.hpp"
#include "Engine/UI/Canvas.hpp"

class Panel : public UIElement
{
public:
	Panel(Canvas* canvas, bool hasBorder = true, Rgba8 borderColor = Rgba8::COLOR_BLACK, UIElement* parent = nullptr);
	Panel(Canvas* canvas, AABB2 bound, bool hasBorder = true, Rgba8 borderColor = Rgba8::COLOR_BLACK, UIElement* parent = nullptr);
	Panel(Canvas* canvas, AABB2 bound, Rgba8 color, bool hasBorder = true, Rgba8 borderColor = Rgba8::COLOR_BLACK, UIElement* parent = nullptr);
	Panel(Canvas* canvas, AABB2 bound, Rgba8 color, Texture* image, bool hasBorder = true, Rgba8 borderColor = Rgba8::COLOR_BLACK, UIElement* parent = nullptr);
	virtual ~Panel();

	void Startup() override;
	void Update(float deltaSeconds) override;
	void Render() const override;
	void Shutdown() override;

	void SetCanvas(Canvas* canvas) override;
	void SetBound(AABB2 bound);
	void SetTexture(Texture* image);
	void SetColor(Rgba8 color);

	bool IsMouseInBound() const;
	Vec2 GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;

public:
	bool m_hasBorder = false;
	Rgba8 m_borderColor = Rgba8(255, 255, 255);
	Texture* m_texture;
	Rgba8 m_color = Rgba8(200, 200, 200, 150);

protected:
	Canvas* m_canvas = nullptr;
	AABB2 m_bound;

	bool m_isMouseInBound = false;

	std::vector<Vertex_PCU> m_panelVerts;
	VertexBuffer* m_panelBuffer = nullptr;

	std::vector<Vertex_PCU> m_borderVerts;
	VertexBuffer* m_borderBuffer = nullptr;
};