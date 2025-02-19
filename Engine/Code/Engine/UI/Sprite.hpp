#pragma once
#include "Engine/UI/UIElement.hpp"
#include "Engine/UI/Canvas.hpp"

class Sprite : public UIElement
{
public:
	Sprite(Canvas* canvas, AABB2 bound, Texture* image, UIElement* parent = nullptr);
	Sprite(Canvas* canvas, AABB2 bound, Texture* image, Rgba8 color = Rgba8::COLOR_WHITE, UIElement* parent = nullptr);
	virtual ~Sprite();

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

	Texture* m_texture;
	Rgba8 m_color = Rgba8::COLOR_WHITE;

protected:
	Canvas* m_canvas = nullptr;
	AABB2 m_bound;

	bool m_isMouseInBound = false;

	std::vector<Vertex_PCU> m_verts;
	VertexBuffer* m_buffer = nullptr;
};