#include "Game/Debris.hpp"
#include "Game/GameExtern.hpp"

Debris::Debris(Game* owner, Vec2 const& startPos, Entity* entity, float sizeScale)
	:Entity(owner, startPos)
{
	m_entity = entity;
	m_physicsRadius = m_entity->m_physicsRadius * g_theRNG->RollRandomFloatInRange(0.1f, 0.3f) * sizeScale;
	m_cosmeticRadius = m_entity->m_cosmeticRadius * g_theRNG->RollRandomFloatInRange(0.4f, 0.8f) * sizeScale;
	InitializeLocalVerts();

	m_angularVelocity = g_theRNG->RollRandomFloatInRange(-200.f, 200.f);
	m_randomDirection = Vec2(CosDegrees(g_theRNG->RollRandomFloatInRange(0.f, 360.f)), SinDegrees(g_theRNG->RollRandomFloatInRange(0.f, 360.f))) + entity->m_velocity.GetNormalized();
	m_velocity = m_randomDirection * DEBRIS_SPEED;
}

void Debris::Update(float deltaSeconds)
{
	if (m_isDead)
	{
		return;
	}

	if (IsOffScreen() || m_age >= 2)
	{
		Die();
	}

	Movement(deltaSeconds);	
}
void Debris::Render() const
{
	if (m_isDead)
	{
		return;
	}
	RenderDebris();
}

void Debris::Die()
{
	if (m_isDead)
	{
		return;
	}
	m_isDead = true;
	m_isGarbage = true;
}

void Debris::InitializeLocalVerts()
{
	float asteroidRandRadius[NUM_DEBRIS_SIDES] = {};
	for (int i = 0; i < NUM_DEBRIS_SIDES; i++)
	{
		// Get Random radius array with range between the physicsR and comesticR
		asteroidRandRadius[i] = g_theRNG->RollRandomFloatInRange(m_physicsRadius, m_cosmeticRadius);
	}

	constexpr float degreesPerAsteroidSide = 360.f / (float)NUM_DEBRIS_SIDES;
	Vec2 asteroidLocalVertPos[NUM_DEBRIS_SIDES] = {};

	for (int i = 0; i < NUM_DEBRIS_SIDES; i++)
	{
		// Based on the random radius, calculate the x and y of the vert
		float degrees = degreesPerAsteroidSide * (float)i;
		float radius = asteroidRandRadius[i];
		asteroidLocalVertPos[i].x = radius * CosDegrees(degrees);
		asteroidLocalVertPos[i].y = radius * SinDegrees(degrees);
	}

	for (int i = 0; i < NUM_DEBRIS_TRIS; i++)
	{
		// Create a triangle made by 3 verts, which the center will be (0,0) and others will use the vert we calculate above
		int startRadiusIndex = i;
		int endRadiusIndex = (i + 1) % NUM_DEBRIS_SIDES;
		int firstVertIndex = (i * 3);
		int secondVertIndex = (i * 3) + 1;
		int thirdVertIndex = (i * 3) + 2;

		Vec2 secondVertOfs = asteroidLocalVertPos[startRadiusIndex];
		Vec2 thirdVertOfs = asteroidLocalVertPos[endRadiusIndex];

		m_localVerts[firstVertIndex].m_position = Vec3(0.f, 0.f, 0.f);
		m_localVerts[secondVertIndex].m_position = Vec3(secondVertOfs.x, secondVertOfs.y, 0.f);
		m_localVerts[thirdVertIndex].m_position = Vec3(thirdVertOfs.x, thirdVertOfs.y, 0.f);
	}

	m_debrisColor = m_entity->m_debrisColor;

	for (int i = 0; i < NUM_DEBRIS_VERTS; i++)
	{
		// Set Color
		m_localVerts[i].m_color = m_debrisColor;
	}
}

void Debris::RenderDebris() const
{
	Vertex_PCU tempVertArray[NUM_DEBRIS_VERTS];

	for (int i = 0; i < NUM_DEBRIS_VERTS; i++)
	{
		tempVertArray[i] = m_localVerts[i];
		tempVertArray[i].m_color = m_debrisColor;
	}
	TransformVertexArrayXY3D(NUM_DEBRIS_VERTS, tempVertArray, 1.f, m_orientationDegrees, m_position);

	g_theRenderer->DrawVertexArray(NUM_DEBRIS_VERTS, tempVertArray);
}

void Debris::Movement(float deltaSeconds)
{
	m_age += deltaSeconds;

	m_position += (m_velocity * deltaSeconds);
	m_orientationDegrees += (m_angularVelocity * deltaSeconds);

	//Color Fading
	float fadingRate = (float)m_debrisColor.a - DEBRIS_FADE_SPEED * deltaSeconds;
	m_debrisColor = Rgba8(m_debrisColor.r, m_debrisColor.g, m_debrisColor.b, (unsigned char)fadingRate);
}
