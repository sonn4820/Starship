#include "Game/Bullet.hpp"
#include "Game/GameExtern.hpp"

Bullet::Bullet(Game* owner, Vec2 const startPos, float fowardDegree)
	:Entity(owner, startPos)
{
	m_physicsRadius = BULLET_PHYSICS_RADIUS;
	m_cosmeticRadius = BULLET_COSMETIC_RADIUS;

	m_orientationDegrees = fowardDegree;
	m_velocity = Vec2(CosDegrees(m_orientationDegrees), SinDegrees(m_orientationDegrees)) * BULLET_SPEED;

	m_health = 1;

	InitializeLocalVerts();
}

void Bullet::Update(float deltaSeconds)
{
	if (m_isDead)
	{
		return;
	}
	if (IsOffScreen())
	{
		m_isDead = true;
		m_isGarbage = true;
	}
	m_position += (m_velocity * deltaSeconds);
}

void Bullet::Render() const
{
	if (m_isDead)
	{
		return;
	}
	RenderBullet();
}
void Bullet::Die()
{
	if (m_isDead)
	{
		return;
	};
	m_isDead = true;
 	m_game->SpawnDebrisFromDeath(this, 1, 3, 0.25f);
	m_isGarbage = true;
}

void Bullet::InitializeLocalVerts()
{
	//Head
	m_localVerts[0] = Vertex_PCU(0.f, -.5f, 0.f, 255, 255, 0, 255);
	m_localVerts[1] = Vertex_PCU(.5f, 0.f, 0.f, 255, 255, 255, 255);
	m_localVerts[2] = Vertex_PCU(0.f, .5f, 0.f, 255, 255, 0, 255);
	//Tail
	m_localVerts[3] = Vertex_PCU(0.f, .5f, 0.f, 255, 0, 0, 255);
	m_localVerts[4] = Vertex_PCU(-2.f, 0.f, 0.f, 255, 0, 0, 0);
	m_localVerts[5] = Vertex_PCU(0.f, -.5f, 0.f, 255, 0, 0, 255);

	m_debrisColor = Rgba8(255, 0, 0, 127);
}

void Bullet::RenderBullet() const
{
	Vertex_PCU tempVertArray[NUM_BULLET_VERTS];

	for (int i = 0; i < NUM_BULLET_VERTS; i++)
	{
		tempVertArray[i] = m_localVerts[i];
	}
	TransformVertexArrayXY3D(NUM_BULLET_VERTS, tempVertArray, 1.f, m_orientationDegrees, m_position);

	DrawDebugGlow(m_position, 3.f, Rgba8(255, 0, 0, 120), Rgba8(120, 0, 0, 20));
	g_theRenderer->DrawVertexArray(NUM_BULLET_VERTS, tempVertArray);
}
