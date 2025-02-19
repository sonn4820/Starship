#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"
class Bullet: public Entity
{
public:
	Bullet(Game* owner, Vec2 const startPos, float fowardDegree);
	~Bullet() = default;

	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;
	virtual void Die() override;
	float m_randomStartOrientation;
	Vec2 m_randomDirection;
private:
	void InitializeLocalVerts();
	void RenderBullet() const;
private:
	Vertex_PCU m_localVerts[NUM_BULLET_VERTS];

};