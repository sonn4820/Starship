#include "Game/Asteroid.hpp"
#include "Game/GameExtern.hpp"

Asteroid::Asteroid(Game* owner, Vec2 const& startPos)
	:Entity(owner, startPos)
{
	m_physicsRadius = ASTEROID_PHYSICS_RADIUS;
	m_cosmeticRadius = ASTEROID_COSMETIC_RADIUS;


	m_randomStartOrientation = g_theRNG->RollRandomFloatInRange(0.f, 360.f);
	m_angularVelocity = g_theRNG->RollRandomFloatInRange(-200.f, 200.f);
	m_randomDirection = Vec2(CosDegrees(m_randomStartOrientation), SinDegrees(m_randomStartOrientation));
	m_velocity = m_randomDirection * ASTEROID_SPEED;

	m_health = 3;
	m_entityOriginalColor = Rgba8(100, 100, 100, 255);
	m_entityColor = m_entityOriginalColor;

	InitializeLocalVerts();
}
void Asteroid::Update(float deltaSeconds)
{
	if (m_isDead) 
	{
		return;
	}
	Movement(deltaSeconds);
	WrapAroundTheWorld();
	PopColorWhenGetHit(Rgba8(255, 255, 255, 255));
}

void Asteroid::Render() const
{
	if (m_isDead)
	{
		return;
	}
	RenderAsteroid();
}

void Asteroid::Die()
{
	if (m_isDead)
	{
		return;
	}
	SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/AsteroidDie.wav");
	g_theAudio->StartSound(sound);
	m_isDead = true;
	m_game->SpawnDebrisFromDeath(this, 3, 12, 1.f);
	m_isGarbage = true;
}
void Asteroid::InitializeLocalVerts()
{
	float asteroidRandRadius[NUM_ASTEROID_SIDES] = {};
	for (int i = 0; i < NUM_ASTEROID_SIDES; i++)
	{
		// Get Random radius array with range between the physicsR and comesticR
		asteroidRandRadius[i] = g_theRNG->RollRandomFloatInRange(m_physicsRadius, m_cosmeticRadius);
	}

	constexpr float degreesPerAsteroidSide = 360.f / (float)NUM_ASTEROID_SIDES;
	Vec2 asteroidLocalVertPos[NUM_ASTEROID_SIDES] = {};

	for (int i = 0; i < NUM_ASTEROID_SIDES; i++)
	{
		// Based on the random radius, calculate the x and y of the vert
		float degrees = degreesPerAsteroidSide * (float)i;
		float radius = asteroidRandRadius[i];
		asteroidLocalVertPos[i].x = radius * CosDegrees(degrees);
		asteroidLocalVertPos[i].y = radius * SinDegrees(degrees);
	}
	for (int i = 0; i < NUM_ASTEROID_TRIS; i++)
	{
		// Create a triangle made by 3 verts, which the center will be (0,0) and others will use the vert we calculate above
		int startRadiusIndex = i;
		int endRadiusIndex = (i + 1) % NUM_ASTEROID_SIDES;
		int firstVertIndex = (i * 3);
		int secondVertIndex = (i * 3) + 1;
		int thirdVertIndex = (i * 3) + 2;

		Vec2 secondVertOfs = asteroidLocalVertPos[startRadiusIndex];
		Vec2 thirdVertOfs = asteroidLocalVertPos[endRadiusIndex];

		m_localVerts[firstVertIndex].m_position = Vec3(0.f, 0.f, 0.f);
		m_localVerts[secondVertIndex].m_position = Vec3(secondVertOfs.x, secondVertOfs.y, 0.f);
		m_localVerts[thirdVertIndex].m_position = Vec3(thirdVertOfs.x, thirdVertOfs.y, 0.f);
	}

	for (int i = 0; i < NUM_ASTEROID_VERTS; i++)
	{
		// Set Color
		m_localVerts[i].m_color = m_entityColor;
	}
	m_debrisColor = Rgba8(100, 100, 100, 127);
}

void Asteroid::RenderAsteroid() const
{
	Vertex_PCU tempVertArray[NUM_ASTEROID_VERTS];

	for (int i = 0; i < NUM_ASTEROID_VERTS; i++)
	{
		tempVertArray[i] = m_localVerts[i];
		tempVertArray[i].m_color = m_entityColor;
	}
	TransformVertexArrayXY3D(NUM_ASTEROID_VERTS, tempVertArray, 1.f, m_orientationDegrees, m_position);

	g_theRenderer->DrawVertexArray(NUM_ASTEROID_VERTS, tempVertArray);
}

void Asteroid::Movement(float deltaSeconds)
{
	m_position += (m_velocity * deltaSeconds);
	m_orientationDegrees += (m_angularVelocity * deltaSeconds);
}
