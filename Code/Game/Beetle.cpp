#include "Game/Beetle.hpp"
#include "Game/GameExtern.hpp"

Beetle::Beetle(Game* owner, Vec2 const startPos)
	: Entity(owner, startPos)
{
	m_physicsRadius = BEETLE_PHYSICS_RADIUS;
	m_cosmeticRadius = BEETLE_COSMETIC_RADIUS;


	m_health = 2;
	m_entityOriginalColor = Rgba8(0, 255, 0, 255);
	m_entityColor = m_entityOriginalColor;

	InitializeLocalVerts();
}

void Beetle::Update(float deltaSeconds)
{
	if (m_isDead)
	{
		return;
	}

	Animation();
	Movement(deltaSeconds);
	WrapAroundTheWorld();
	PopColorWhenGetHit(Rgba8(255, 255, 255, 255));
}

void Beetle::Render() const
{
	if (m_isDead)
	{
		return;
	}
	RenderBeetle();
}
void Beetle::Die()
{
	if (m_isDead)
	{
		return;
	}
	SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/BeetleDie.wav");
	g_theAudio->StartSound(sound);
	m_isDead = true;
	m_game->SpawnDebrisFromDeath(this, 3, 12, 1.f);
	m_isGarbage = true;
}

void Beetle::InitializeLocalVerts()
{
	// Head
	m_localVerts[0] = Vertex_PCU(-1.f, 0.f, 0.f, 0, 255, 0, 255);
	m_localVerts[1] = Vertex_PCU(1.f, 0.f, 0.f, 0, 255, 0, 255);
	m_localVerts[2] = Vertex_PCU(0.f, 1.f, 0.f, 0, 255, 0, 255);

	m_localVerts[3] = Vertex_PCU(0.f, -1.f, 0.f, 0, 255, 0, 255);
	m_localVerts[4] = Vertex_PCU(1.f, 0.f, 0.f, 0, 255, 0, 255);
	m_localVerts[5] = Vertex_PCU(-1.f, 0.f, 0.f, 0, 255, 0, 255);

	//Body
	m_localVerts[6] = Vertex_PCU(-2.5f, 1.5f, 0.f, 0, 255, 0, 255);
	m_localVerts[7] = Vertex_PCU(-4.f, 0.f, 0.f, 0, 255, 0, 255);
	m_localVerts[8] = Vertex_PCU(-1.f, 0.f, 0.f, 0, 255, 0, 255);

	m_localVerts[9] = Vertex_PCU(-1.f, 0.f, 0.f, 0, 255, 0, 255);
	m_localVerts[10] = Vertex_PCU(-4.f, 0.f, 0.f, 0, 255, 0, 255);
	m_localVerts[11] = Vertex_PCU(-2.5f, -1.5f, 0.f, 0, 255, 0, 255);

	//Horn
	m_localVerts[12] = Vertex_PCU(0.5f, 0.5f, 0.f, 0, 255, 0, 255);
	m_localVerts[13] = Vertex_PCU(2.5f, 0.5f, 0.f, 0, 255, 0, 255);
	m_localVerts[14] = Vertex_PCU(1.5f, 1.0f, 0.f, 0, 255, 0, 255);

	m_localVerts[15] = Vertex_PCU(1.5f, -1.0f, 0.f, 0, 255, 0, 255);
	m_localVerts[16] = Vertex_PCU(2.5f, -0.5f, 0.f, 0, 255, 0, 255);
	m_localVerts[17] = Vertex_PCU(0.5f, -0.5f, 0.f, 0, 255, 0, 255);

	m_debrisColor = Rgba8(0, 255, 0, 127);
}

void Beetle::RenderBeetle() const
{
	Vertex_PCU tempVertArray[NUM_BEETLE_VERTS];

	for (int i = 0; i < NUM_BEETLE_VERTS; i++)
	{
		tempVertArray[i] = m_localVerts[i];
		tempVertArray[i].m_color = m_entityColor;
	}
	TransformVertexArrayXY3D(NUM_BEETLE_VERTS, tempVertArray, 1.f, m_orientationDegrees, m_position);

	g_theRenderer->DrawVertexArray(NUM_BEETLE_VERTS, tempVertArray);
}

void Beetle::Movement(float deltaSeconds)
{
	Vec2 targetPos = m_playerShip->m_position;
	Vec2 toTarget = (targetPos - m_position).GetNormalized();

	if (!m_playerShip->m_isDead)
	{
		m_orientationDegrees = toTarget.GetOrientationDegrees();
	}

	Vec2 forward = Vec2(CosDegrees(m_orientationDegrees), SinDegrees(m_orientationDegrees));

	m_velocity = forward * BEETLE_SPEED * deltaSeconds;
	m_position += m_velocity * deltaSeconds;
}

void Beetle::Animation()
{
	float maxValue = 3.5f;
	float minValue = 1.5f;

	m_movingHornAmountX += 0.05f * (float)m_movingHornDir;

	if (m_movingHornAmountX >= maxValue)
	{
		m_movingHornAmountX = maxValue;
		m_movingHornDir = -1;
	}
	if (m_movingHornAmountX <= minValue)
	{
		m_movingHornAmountX = minValue;
		m_movingHornDir = 1;
	}
	m_localVerts[13] = Vertex_PCU(m_movingHornAmountX, 0.5f, 0.f, 0, 255, 0, 255);
	m_localVerts[16] = Vertex_PCU(m_movingHornAmountX, -0.5f, 0.f, 0, 255, 0, 255);
}