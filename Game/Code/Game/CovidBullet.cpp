#include "Game/CovidBullet.hpp"
#include "Game/GameExtern.hpp"

CovidBullet::CovidBullet(Game* owner, Vec2 const startPos, float fowardDegree, float bulletSpeed)
	:Entity(owner, startPos)
{
	m_physicsRadius = COVID_BULLET_PHYSICS_RADIUS;
	m_cosmeticRadius = COVID_BULLET_COSMETIC_RADIUS;

	m_orientationDegrees = fowardDegree;
	m_velocity = Vec2(CosDegrees(m_orientationDegrees), SinDegrees(m_orientationDegrees)) * bulletSpeed;

	m_entityOriginalColor = Rgba8(184, 32, 61, 255);
	m_entityColor = m_entityOriginalColor;

	m_health = 1;

	InitializeLocalVerts();
}

void CovidBullet::Update(float deltaSeconds)
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

void CovidBullet::Render() const
{
	if (m_isDead)
	{
		return;
	}
	RenderBullet();
}
void CovidBullet::Die()
{
	if (m_isDead)
	{
		return;
	};
	m_isDead = true;
 	m_game->SpawnDebrisFromDeath(this, 4, 6, 0.25f);
	m_isGarbage = true;
}

void CovidBullet::InitializeLocalVerts()
{
	float asteroidRandRadius[NUM_COVID_BULLET_SIDES] = {};
	for (int i = 0; i < NUM_COVID_BULLET_SIDES; i++)
	{
		// Get Random radius array with range between the physicsR and comesticR
		asteroidRandRadius[i] = g_theRNG->RollRandomFloatInRange(0.5f, 0.6f);
	}

	constexpr float degreesPerAsteroidSide = 360.f / (float)NUM_COVID_BULLET_SIDES;
	Vec2 asteroidLocalVertPos[NUM_COVID_BULLET_SIDES] = {};

	for (int i = 0; i < NUM_COVID_BULLET_SIDES; i++)
	{
		// Based on the random radius, calculate the x and y of the vert
		float degrees = degreesPerAsteroidSide * (float)i;
		float radius = asteroidRandRadius[i];
		asteroidLocalVertPos[i].x = radius * CosDegrees(degrees);
		asteroidLocalVertPos[i].y = radius * SinDegrees(degrees);
	}
	for (int i = 0; i < NUM_COVID_BULLET_TRIS; i++)
	{
		// Create a triangle made by 3 verts, which the center will be (0,0) and others will use the vert we calculate above
		int startRadiusIndex = i;
		int endRadiusIndex = (i + 1) % NUM_COVID_BULLET_SIDES;
		int firstVertIndex = (i * 3);
		int secondVertIndex = (i * 3) + 1;
		int thirdVertIndex = (i * 3) + 2;

		Vec2 secondVertOfs = asteroidLocalVertPos[startRadiusIndex];
		Vec2 thirdVertOfs = asteroidLocalVertPos[endRadiusIndex];

		m_localVerts[firstVertIndex].m_position = Vec3(0.f, 0.f, 0.f);
		m_localVerts[secondVertIndex].m_position = Vec3(secondVertOfs.x, secondVertOfs.y, 0.f);
		m_localVerts[thirdVertIndex].m_position = Vec3(thirdVertOfs.x, thirdVertOfs.y, 0.f);
	}

	for (int i = 0; i < NUM_COVID_BULLET_VERTS; i++)
	{
		// Set Color
		m_localVerts[i].m_color = m_entityColor;
	}
	m_debrisColor = Rgba8(120, 0, 0, 120);
}

void CovidBullet::RenderBullet() const
{
	Vertex_PCU tempVertArray[NUM_COVID_BULLET_VERTS];

	for (int i = 0; i < NUM_COVID_BULLET_VERTS; i++)
	{
		tempVertArray[i] = m_localVerts[i];
	}
	TransformVertexArrayXY3D(NUM_COVID_BULLET_VERTS, tempVertArray, 1.f, m_orientationDegrees, m_position);

	DrawDebugGlow(m_position, 2.f, Rgba8(191, 17, 104, 40), Rgba8(184, 32, 61, 170));

	g_theRenderer->DrawVertexArray(NUM_COVID_BULLET_VERTS, tempVertArray);
}
