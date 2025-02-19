#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

class PlayerShip : public Entity
{
public:

	PlayerShip(Game* owner, Vec2 const& startPos);
	~PlayerShip();

	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;
	virtual void Die() override;
	void FireBullet(float deltaSeconds);
	void Respawn();
	Vertex_PCU GetLocalVert(int index, float scale);
	float m_ship_thrust;
	int m_extraLives = PLAYER_SHIP_EXTRA_LIVES;
	
private:
	void Movement(float deltaSeconds);
	void RenderShip() const;
	void RenderFlameThrust() const;
	void InitializeLocalVerts();
	void RandomizeTheFlame();
	void CheckInvincible();
private:
	Vertex_PCU m_localVerts[NUM_PLAYERSHIP_VERTS];
	Vertex_PCU m_thurstVerts[3];
	bool m_isThrusting = false;
	float m_invincibleEffectTimer = 6.f;
	float m_playerShipInvincibleTimer = PLAYER_SHIP_INVINCIBLE_TIMER;
	float m_fireRate = 0.f;
	float m_overfireRate = 0.f;
};