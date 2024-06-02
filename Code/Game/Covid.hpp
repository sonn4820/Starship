#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"
class Covid: public Entity
{
public:
	Covid(Game* owner, Vec2 const startPos);
	~Covid() = default;

	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;
	virtual void Die() override;
	float m_randomStartOrientation;
	Vec2 m_randomDirection;
private:
	void InitializeLocalBodyVerts();
	void InitializeLocalSpikeVerts();
	void RenderCovidBody() const;
	void RenderCovidSpike() const;
	void Movement(float deltaSeconds);
	void Animation();
	void FireCovidBullet();
private:
	Vertex_PCU m_localBodyVerts[NUM_COVID_VERTS];
	Vertex_PCU m_localSpikeVerts[12];
	float m_movingAmount = 1.f;
	int m_movingDir = 1;
	float m_shootingBulletTimer = 2.f;
};