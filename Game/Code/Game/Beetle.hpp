#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"
#include "Game/PlayerShip.hpp"
class Beetle: public Entity
{
public:
	Beetle(Game* owner, Vec2 const startPos);
	~Beetle() = default;

	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;

	virtual void Die() override;
	float m_randomStartOrientation;
	Vec2 m_randomDirection;

	Entity* m_playerShip = nullptr;
private:
	void InitializeLocalVerts();
	void RenderBeetle() const;
	void Movement(float deltaSeconds);
	void Animation();
private:
	Vertex_PCU m_localVerts[NUM_BEETLE_VERTS];
	float m_movingHornAmountX = 1.f;
	int m_movingHornDir = 1;
};