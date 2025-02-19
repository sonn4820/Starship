#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

class Asteroid : public Entity
{
public:
	Asteroid(Game* owner, Vec2 const& startPos);
	~Asteroid() = default;

	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;
	virtual void Die() override;
	float m_randomStartOrientation;
	Vec2 m_randomDirection;
private:
	void InitializeLocalVerts();
	void RenderAsteroid() const;
	void Movement(float deltaSeconds);

private:
	Vertex_PCU m_localVerts[NUM_ASTEROID_VERTS];
};