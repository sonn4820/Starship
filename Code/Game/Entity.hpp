#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"

class Game;
class Entity
{
public:
	Entity(Game* owner, Vec2 const& startPos);
	virtual ~Entity();

	virtual void Update(float deltaSeconds) = 0;
	virtual void Render() const = 0;

	virtual void Die() = 0;
	//virtual void Die();
	bool IsOffScreen();
	Vec2 GetForwardNormal();
	bool IsAlive();
	void WrapAroundTheWorld();
	void BounceOffTheWorldBoundaries();
	void TakeDamage(int damage, bool spawnDebris);
	void PopColorWhenGetHit(Rgba8 color);
public:
	Game* m_game = nullptr;
	Vec2 m_position;
	Vec2 m_velocity;
	float m_age = 0;
	float m_orientationDegrees = 0.f;
	float m_angularVelocity = 0.f;
	float m_physicsRadius = 5.f;
	float m_cosmeticRadius = 10.f;
	int m_health = 1;
	bool m_isDead = false;
	bool m_isGarbage = false;
	Rgba8 m_debrisColor;
	Rgba8 m_entityColor;
	Rgba8 m_entityOriginalColor;
	float m_changeColorTimer = 0.f;
	bool m_isInvincible = false;
};