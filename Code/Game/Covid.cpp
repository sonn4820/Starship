#include "Game/Covid.hpp"
#include "Game/GameExtern.hpp"

Covid::Covid(Game* owner, Vec2 const startPos)
	: Entity(owner, startPos)
{
	m_physicsRadius = COVID_PHYSICS_RADIUS;
	m_cosmeticRadius = COVID_COSMETIC_RADIUS;


	m_randomStartOrientation = g_theRNG->RollRandomFloatInRange(0.f, 360.f);
	m_angularVelocity = g_theRNG->RollRandomFloatInRange(-200.f, 200.f);
	m_randomDirection = Vec2(CosDegrees(m_randomStartOrientation), SinDegrees(m_randomStartOrientation));
	m_velocity = m_randomDirection * COVID_SPEED;

	m_health = 6;
	m_entityOriginalColor = Rgba8(125, 0, 0, 255);
	m_entityColor = m_entityOriginalColor;

	InitializeLocalBodyVerts();
	InitializeLocalSpikeVerts();
}

void Covid::Update(float deltaSeconds)
{
	if (m_isDead)
	{
		return;
	}
	FireCovidBullet();
	Animation();
	Movement(deltaSeconds);
	WrapAroundTheWorld();
	PopColorWhenGetHit(Rgba8(255, 255, 255, 255));
}

void Covid::Render() const
{
	if (m_isDead)
	{
		return;
	}
	RenderCovidBody();
	RenderCovidSpike();
}
void Covid::Die()
{
	if (m_isDead)
	{
		return;
	}
	SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/CovidDie.wav");
	g_theAudio->StartSound(sound);
	m_isDead = true;
	m_game->SpawnDebrisFromDeath(this, 3, 12, 1.f);
	m_isGarbage = true;
}
void Covid::InitializeLocalBodyVerts()
{
	float asteroidRandRadius[NUM_COVID_SIDES] = {};
	for (int i = 0; i < NUM_COVID_SIDES; i++)
	{
		asteroidRandRadius[i] = g_theRNG->RollRandomFloatInRange(m_physicsRadius, m_cosmeticRadius);
	}

	constexpr float degreesPerAsteroidSide = 360.f / (float)NUM_COVID_SIDES;
	Vec2 asteroidLocalVertPos[NUM_COVID_SIDES] = {};

	for (int i = 0; i < NUM_COVID_SIDES; i++)
	{
		float degrees = degreesPerAsteroidSide * (float)i;
		float radius = asteroidRandRadius[i];
		asteroidLocalVertPos[i].x = radius * CosDegrees(degrees);
		asteroidLocalVertPos[i].y = radius * SinDegrees(degrees);
	}
	for (int i = 0; i < NUM_COVID_TRIS; i++)
	{
		int startRadiusIndex = i;
		int endRadiusIndex = (i + 1) % NUM_COVID_SIDES;
		int firstVertIndex = (i * 3);
		int secondVertIndex = (i * 3) + 1;
		int thirdVertIndex = (i * 3) + 2;

		Vec2 secondVertOfs = asteroidLocalVertPos[startRadiusIndex];
		Vec2 thirdVertOfs = asteroidLocalVertPos[endRadiusIndex];

		m_localBodyVerts[firstVertIndex].m_position = Vec3(0.f, 0.f, 0.f);
		m_localBodyVerts[secondVertIndex].m_position = Vec3(secondVertOfs.x, secondVertOfs.y, 0.f);
		m_localBodyVerts[thirdVertIndex].m_position = Vec3(thirdVertOfs.x, thirdVertOfs.y, 0.f);
	}

	for (int i = 0; i < NUM_COVID_VERTS; i++)
	{
		// Set Color
		m_localBodyVerts[i].m_color = m_entityColor;
	}
	m_debrisColor = Rgba8(125, 0, 0, 127);
}

void Covid::InitializeLocalSpikeVerts()
{
	m_localSpikeVerts[0]  = Vertex_PCU(2.6f, 2.6f, 0.f, 255, 0, 0, 255);
	m_localSpikeVerts[1]  = Vertex_PCU(0.8f, 1.8f, 0.f, 255, 0, 0, 255);
	m_localSpikeVerts[2]  = Vertex_PCU(1.8f, 0.8f, 0.f, 255, 0, 0, 255);
	m_localSpikeVerts[3] = Vertex_PCU(-2.6f, -2.6f, 0.f, 255, 0, 0, 255);
	m_localSpikeVerts[4] = Vertex_PCU(-0.8f, -1.8f, 0.f, 255, 0, 0, 255);
	m_localSpikeVerts[5] = Vertex_PCU(-1.8f, - 0.8f, 0.f, 255, 0, 0, 255);
	m_localSpikeVerts[6]  = Vertex_PCU(2.6f, -2.6f, 0.f, 255, 0, 0, 255);
	m_localSpikeVerts[7]  = Vertex_PCU(1.8f, -0.8f, 0.f, 255, 0, 0, 255);
	m_localSpikeVerts[8]  = Vertex_PCU(0.8f, -1.8f, 0.f, 255, 0, 0, 255);
	m_localSpikeVerts[9] =	Vertex_PCU(-2.6f, 2.6f, 0.f, 255, 0, 0, 255);
	m_localSpikeVerts[10] = Vertex_PCU(-1.8f, 0.8f, 0.f, 255, 0, 0, 255);
	m_localSpikeVerts[11] = Vertex_PCU(-0.8f, 1.8f, 0.f, 255, 0, 0, 255);
}

void Covid::RenderCovidBody() const
{
	Vertex_PCU tempVertArray[NUM_COVID_VERTS];

	for (int i = 0; i < NUM_COVID_VERTS; i++)
	{
		tempVertArray[i] = m_localBodyVerts[i];
		tempVertArray[i].m_color = m_entityColor;
	}
	TransformVertexArrayXY3D(NUM_COVID_VERTS, tempVertArray, 1.f, m_orientationDegrees, m_position);

	DrawDebugGlow(m_position, 3.f, Rgba8(40, 0, 0, 20), Rgba8(255, 0, 0, 120));

	g_theRenderer->DrawVertexArray(NUM_COVID_VERTS, tempVertArray);
}
void Covid::RenderCovidSpike() const
{
	Vertex_PCU tempVertArray[12];

	for (int i = 0; i < 12; i++)
	{
		tempVertArray[i] = m_localSpikeVerts[i];
	}
	TransformVertexArrayXY3D(12, tempVertArray, 1.f, m_orientationDegrees, m_position);

	g_theRenderer->DrawVertexArray(12, tempVertArray);
}

void Covid::Movement(float deltaSeconds)
{
	m_position += (m_velocity * deltaSeconds);
	m_orientationDegrees += (m_angularVelocity * deltaSeconds);
}

void Covid::Animation()
{
	float maxValue = 2.6f;
	float minValue = 0.f;

	m_movingAmount += 0.05f * (float)m_movingDir;

	if (m_movingAmount >= maxValue)
	{
		m_movingAmount = maxValue;
		m_movingDir = -1;
	}
	if (m_movingAmount <= minValue)
	{
		m_movingAmount = minValue;
		m_movingDir = 1;
	}
	m_localSpikeVerts[0] = Vertex_PCU(m_movingAmount, m_movingAmount, 0.f, 85, 0, 0, 255);
	m_localSpikeVerts[3] = Vertex_PCU(-m_movingAmount, -m_movingAmount, 0.f, 85, 0, 0, 255);
	m_localSpikeVerts[6] = Vertex_PCU(m_movingAmount, -m_movingAmount, 0.f, 85, 0, 0, 255);
	m_localSpikeVerts[9] = Vertex_PCU(-m_movingAmount, m_movingAmount, 0.f, 85, 0, 0, 255);
}

void Covid::FireCovidBullet()
{
	if (m_isDead)
	{
		return;
	}
	m_shootingBulletTimer -= 0.05f;
	if (m_shootingBulletTimer < 0) 
	{
		float x = m_position.x + .5f * CosDegrees(m_orientationDegrees);
		float y = m_position.y + .5f * SinDegrees(m_orientationDegrees);
		m_game->SpawnCovidBullet(Vec2(x, y), m_orientationDegrees, COVID_BULLET_SPEED);
		m_shootingBulletTimer = 2.f;
	}
}