#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"
#include "Game/PlayerShip.hpp"

enum BossState
{
	OUTSIDE_RING, 
	INSIDE_RING, 
	NO_RING
};
class Boss : public Entity
{
public:
	Boss(Game* owner, Vec2 const startPos);
	~Boss() = default;

	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;
	virtual void Die() override;
	float m_randomStartOrientation;
	Vec2 m_randomDirection;
private:
	void InitializeHeartVerts();
	void RenderHeart() const;
	void Movement(float deltaSeconds);
	void FirstSkill();
	void SecondSkill();
	void ThirdSkill();
	void StateUpdate(float deltaSeconds);
	void SwitchState();
	void ColorDeductionByHealth(unsigned char& valueToChange, int maxHealth, float fraction);

private:

	Entity* m_playerShip = nullptr;
	BossState m_state;
	Vertex_PCU m_HeartVerts[NUM_PLAYERSHIP_VERTS];

	float m_shootingBulletTimer = 0.f;
	float m_spawningAsteroidTimer = 0.f;
	float m_spawningEntityTimer = 0.f;
	int m_maxEntityCanSpawn = 10;
	int m_spawnedEntityCounter = 0;

	float m_changeDirectionTimer;
	float m_changeDirectionTime;

	Rgba8 m_outsideRingColor_in;
	Rgba8 m_outsideRingColor_out;
	Rgba8 m_insideRingColor_in;
	Rgba8 m_insideRingColor_out;

	bool m_isOSRSoundPlayed = false;
	bool m_isISRSoundPlayed = false;
	bool m_isNRSoundPlayed = false;
};