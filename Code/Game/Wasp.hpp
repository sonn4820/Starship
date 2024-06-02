#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"
#include "Game/PlayerShip.hpp"
class Wasp: public Entity
{
public:
	Wasp(Game* owner, Vec2 const startPos);
	~Wasp() = default;

	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;
	virtual void Die() override;
	float m_randomStartOrientation;
	Vec2 m_randomDirection;

	Entity* m_playerShip = nullptr;
private:
	void InitializeLocalVerts();
	void RenderWasp() const;
	void Movement(float deltaSeconds);
	void Animation();
private:
	Vertex_PCU m_localVerts[NUM_WASP_VERTS];
	float m_movingWingAmountY = 1.f;
	int m_movingWingDir = 1;
};