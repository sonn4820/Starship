#include "Game/Entity.hpp"
#include "Game/GameExtern.hpp"


Entity::Entity(Game* owner, Vec2 const& startPos) 
	: m_game(owner), m_position (startPos)
{

}
Entity::~Entity() {

}

bool Entity::IsOffScreen()
{
	if (m_position.x - m_cosmeticRadius > WORLD_SIZE_X || m_position.x + m_cosmeticRadius < 0 || m_position.y - m_cosmeticRadius > WORLD_SIZE_Y || m_position.y + m_cosmeticRadius < 0)
	{
		m_isGarbage = true;
		return true;
	}
	return false;
}

Vec2 Entity::GetForwardNormal()
{
	return Vec2(CosDegrees(m_orientationDegrees), SinDegrees(m_orientationDegrees));
}

bool Entity::IsAlive()
{
	return !m_isDead;
}

void Entity::Die()
{

}

void Entity::WrapAroundTheWorld()
{
	if (m_position.x > WORLD_SIZE_X + m_cosmeticRadius)
	{
		m_position = Vec2(-m_cosmeticRadius * 0.5f, m_position.y);
	}
	if (m_position.x < -m_cosmeticRadius)
	{
		m_position = Vec2(WORLD_SIZE_X + m_cosmeticRadius * 0.5f, m_position.y);
	}

	if (m_position.y > WORLD_SIZE_Y + m_cosmeticRadius)
	{
		m_position = Vec2(m_position.x, -m_cosmeticRadius * 0.5f);
	}
	if (m_position.y < -m_cosmeticRadius)
	{
		m_position = Vec2(m_position.x, WORLD_SIZE_Y + m_position.y * 0.5f);
	}
}

void Entity::BounceOffTheWorldBoundaries()
{
	if (m_position.x >= WORLD_SIZE_X - m_cosmeticRadius)
	{
		Vec2 normalVec = Vec2(200, 100) - Vec2(200, 0);
		Vec2 normalizedVec = (normalVec.GetRotatedMinus90Degrees()).GetNormalized();
		m_velocity = m_velocity.GetReflected(normalizedVec);
	}
	if (m_position.x <= 0 + m_cosmeticRadius)
	{
		Vec2 normalVec = Vec2(0, 100) - Vec2(0, 0);
		Vec2 normalizedVec = (normalVec.GetRotated90Degrees()).GetNormalized();
		m_velocity = m_velocity.GetReflected(normalizedVec);
	}
	if (m_position.y >= WORLD_SIZE_Y - m_cosmeticRadius)
	{
		Vec2 normalVec = Vec2(200, 100) - Vec2(0, 100);
		Vec2 normalizedVec = (normalVec.GetRotatedMinus90Degrees()).GetNormalized();
		m_velocity = m_velocity.GetReflected(normalizedVec);
	}
	if (m_position.y <= 0 + m_cosmeticRadius)
	{
		Vec2 normalVec = Vec2(200, 0) - Vec2(0, 0);
		Vec2 normalizedVec = (normalVec.GetRotatedMinus90Degrees()).GetNormalized();
		m_velocity = m_velocity.GetReflected(normalizedVec);
	}
}

void Entity::TakeDamage(int damage, bool spawnDebris)
{
	m_health -= damage;
	if (spawnDebris) 
	{
		m_game->SpawnDebrisFromDeath(this, 4, 7, .6f);
	}
}

void Entity::PopColorWhenGetHit(Rgba8 color)
{
	if (m_health > 0)
	{
		m_changeColorTimer -= 0.35f;
		if (m_changeColorTimer < 0)
		{
			m_entityColor = m_entityOriginalColor;
		}
		else
		{
			m_entityColor = color;
		}
	}
}
