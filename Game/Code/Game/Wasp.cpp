#include "Game/Wasp.hpp"
#include "Game/GameExtern.hpp"

Wasp::Wasp(Game* owner, Vec2 const startPos)
	: Entity(owner, startPos)
{
	m_physicsRadius = BEETLE_PHYSICS_RADIUS;
	m_cosmeticRadius = BEETLE_COSMETIC_RADIUS;

	m_health = 3;
	m_entityOriginalColor = Rgba8(255, 255, 0, 255);
	m_entityColor = m_entityOriginalColor;

	InitializeLocalVerts();
}

void Wasp::Update(float deltaSeconds)
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

void Wasp::Render() const
{
	if (m_isDead)
	{
		return;
	}
	RenderWasp();
}
void Wasp::Die()
{
	if (m_isDead)
	{
		return;
	}
	SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/WaspDie.wav");
	g_theAudio->StartSound(sound);
	m_isDead = true;
	m_game->SpawnDebrisFromDeath(this, 3, 12, 1.f);
	m_isGarbage = true;
}

void Wasp::RenderWasp() const
{
	Vertex_PCU tempVertArray[NUM_WASP_VERTS];

	for (int i = 0; i < NUM_WASP_VERTS; i++)
	{
		tempVertArray[i] = m_localVerts[i];
		tempVertArray[i].m_color = m_entityColor;
	}
	TransformVertexArrayXY3D(NUM_WASP_VERTS, tempVertArray, 1.f, m_orientationDegrees, m_position);

	g_theRenderer->DrawVertexArray(NUM_WASP_VERTS, tempVertArray);
}

void Wasp::Movement(float deltaSeconds)
{

	Vec2 targetPos = m_playerShip->m_position;
	Vec2 toTarget = (targetPos - m_position).GetNormalized();

	if (!m_playerShip->m_isDead)
	{
		m_orientationDegrees = toTarget.GetOrientationDegrees();
	}

	Vec2 forward = Vec2(CosDegrees(m_orientationDegrees), SinDegrees(m_orientationDegrees));

	m_velocity += forward * WASP_SPEED * deltaSeconds;
	m_position += m_velocity.GetClamped(30.f) * deltaSeconds;
}

void Wasp::Animation()
{
	float maxValue = 3.5f;
	float minValue = 1.5f;

	m_movingWingAmountY += 0.05f * (float)m_movingWingDir;

	if (m_movingWingAmountY >= maxValue)
	{
		m_movingWingAmountY = maxValue;
		m_movingWingDir = -1;
	}
	if (m_movingWingAmountY <= minValue)
	{
		m_movingWingAmountY = minValue;
		m_movingWingDir = 1;
	}
	m_localVerts[19] = Vertex_PCU(0.f, m_movingWingAmountY, 0.f, 255, 255, 0, 255);
	m_localVerts[22] = Vertex_PCU(0.f, -m_movingWingAmountY, 0.f, 255, 255, 0, 255);
}

void Wasp::InitializeLocalVerts()
{
	//Middle Body
	m_localVerts[0] = Vertex_PCU(-1.f, 0.f, 0.f, 255, 255, 0, 255);
	m_localVerts[1] = Vertex_PCU(1.f, 0.f, 0.f, 255, 255, 0, 255);
	m_localVerts[2] = Vertex_PCU(0.f, 0.5f, 0.f, 255, 255, 0, 255);

	m_localVerts[5] = Vertex_PCU(-1.f, 0.f, 0.f, 255, 255, 0, 255);
	m_localVerts[4] = Vertex_PCU(1.f, 0.f, 0.f, 255, 255, 0, 255);
	m_localVerts[3] = Vertex_PCU(0.f, -0.5f, 0.f, 255, 255, 0, 255);

	//Head
	m_localVerts[6] = Vertex_PCU(-1.75f, 0.f, 0.f, 255, 255, 0, 255);
	m_localVerts[7] = Vertex_PCU(-1.f, 0.f, 0.f, 255, 255, 0, 255);
	m_localVerts[8] = Vertex_PCU(-1.5f, 0.25f, 0.f, 255, 255, 0, 255);

	m_localVerts[11] = Vertex_PCU(-1.75f, 0.f, 0.f, 255, 255, 0, 255);
	m_localVerts[10] = Vertex_PCU(-1.f, 0.f, 0.f, 255, 255, 0, 255);
	m_localVerts[9] = Vertex_PCU(-1.5f, -0.25f, 0.f, 255, 255, 0, 255);

	//Lower Body
	m_localVerts[12] = Vertex_PCU(1.f, 0.f, 0.f, 255, 255, 0, 255);
	m_localVerts[13] = Vertex_PCU(3.f, 0.f, 0.f, 242, 140, 40, 255);
	m_localVerts[14] = Vertex_PCU(1.5f, 0.5f, 0.f, 255, 255, 0, 255);

	m_localVerts[17] = Vertex_PCU(1.f, 0.f, 0.f, 255, 255, 0, 255);
	m_localVerts[16] = Vertex_PCU(3.f, 0.f, 0.f, 242, 140, 40, 255);
	m_localVerts[15] = Vertex_PCU(1.5f, -0.5f, 0.f, 255, 255, 0, 255);

	//Right Wing
	m_localVerts[18] = Vertex_PCU(0.f, 0.5f, 0.f, 255, 255, 0, 255);
	m_localVerts[19] = Vertex_PCU(0.f, 1.5f, 0.f, 255, 255, 0, 255);
	m_localVerts[20] = Vertex_PCU(-0.45f, 0.75f, 0.f, 255, 255, 0, 255);

	//Left Wing
	m_localVerts[23] = Vertex_PCU(0.f, -0.5f, 0.f, 255, 255, 0, 255);
	m_localVerts[22] = Vertex_PCU(0.f, -1.5f, 0.f, 255, 255, 0, 255);
	m_localVerts[21] = Vertex_PCU(-0.45f, -0.75f, 0.f, 255, 255, 0, 255);

	m_debrisColor = Rgba8(255, 255, 0, 127);
}
