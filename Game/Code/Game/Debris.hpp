#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

class Debris : public Entity
{
public:
	Debris(Game* owner, Vec2 const& startPos, Entity* entity, float sizeScale);
	~Debris() = default;

	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;
	virtual void Die() override;
	Vec2 m_randomDirection;
	Entity* m_entity = nullptr;
private:
	void InitializeLocalVerts();
	void RenderDebris() const;
	void Movement(float deltaSeconds);
private:
	Vertex_PCU m_localVerts[NUM_ASTEROID_VERTS];
};