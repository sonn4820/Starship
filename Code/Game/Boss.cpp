#include "Game/Boss.hpp"
#include "Game/GameExtern.hpp"
#include "Engine/Core/EngineCommon.hpp"

Boss::Boss(Game* owner, Vec2 const startPos)
	: Entity(owner, startPos)
{
	m_physicsRadius = BOSS_OUTSIDE_RING_LENGTH;
	m_cosmeticRadius = BOSS_OUTSIDE_RING_LENGTH + 0.2f;

	m_randomStartOrientation = g_theRNG->RollRandomFloatInRange(0.f, 360.f);
	m_angularVelocity = g_theRNG->RollRandomFloatInRange(-200.f, 200.f);
	m_randomDirection = Vec2(CosDegrees(m_randomStartOrientation), SinDegrees(m_randomStartOrientation));
	m_velocity = m_randomDirection * 25.f;

	m_entityOriginalColor = Rgba8(255, 153, 204, 255);
	m_entityColor = m_entityOriginalColor;
	m_debrisColor = m_entityOriginalColor;

	m_health = BOSS_OUTSIDE_RING_STAGE_HEALTH;

	m_outsideRingColor_in = Rgba8(80, 0, 0, 20);
	m_outsideRingColor_out = Rgba8(255, 0, 0, 180);
	m_insideRingColor_in = Rgba8(0, 0, 80, 20);
	m_insideRingColor_out = Rgba8(0, 0, 255, 180);

	InitializeHeartVerts();
}

void Boss::Update(float deltaSeconds)
{
	if (m_isDead)
	{
		return;
	}
	SwitchState();
	WrapAroundTheWorld();
	Movement(deltaSeconds);

	StateUpdate(deltaSeconds);
}

void Boss::Render() const
{
	if (m_isDead)
	{
		return;
	}
	RenderHeart();
}
void Boss::Die()
{
	if (m_isDead)
	{
		return;
	}
	SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/PlayerDie.wav");
	g_theAudio->StartSound(sound);
	m_isDead = true;
	m_game->SpawnDebrisFromDeath(this, 20, 40, 1.f);
	m_isGarbage = true;
}

void Boss::InitializeHeartVerts()
{
	m_HeartVerts[0] = Vertex_PCU(2.f, 1.f, 0.f, 255, 153, 204, 255);
	m_HeartVerts[1] = Vertex_PCU(0.f, 2.f, 0.f, 255, 153, 204, 255);
	m_HeartVerts[2] = Vertex_PCU(-2.f, 1.f, 0.f, 255, 153, 204, 255);

	m_HeartVerts[3] = Vertex_PCU(0.f, 1.f, 0.f, 255, 153, 204, 255);
	m_HeartVerts[4] = Vertex_PCU(-2.f, 1.f, 0.f, 255, 153, 204, 255);
	m_HeartVerts[5] = Vertex_PCU(-2.f, -1.f, 0.f, 255, 153, 204, 255);

	m_HeartVerts[6] = Vertex_PCU(0.f, 1.f, 0.f, 255, 153, 204, 255);
	m_HeartVerts[7] = Vertex_PCU(0.f, -1.f, 0.f, 255, 153, 204, 255);
	m_HeartVerts[8] = Vertex_PCU(-2.f, -1.f, 0.f, 255, 153, 204, 255);

	m_HeartVerts[9] = Vertex_PCU(0.f, 1.f, 0.f, 255, 153, 204, 255);
	m_HeartVerts[10] = Vertex_PCU(1.f, 0.f, 0.f, 255, 153, 204, 255);
	m_HeartVerts[11] = Vertex_PCU(0.f, -1.f, 0.f, 255, 153, 204, 255);

	m_HeartVerts[12] = Vertex_PCU(2.f, -1.f, 0.f, 255, 153, 204, 255);
	m_HeartVerts[13] = Vertex_PCU(-2.f, -1.f, 0.f, 255, 153, 204, 255);
	m_HeartVerts[14] = Vertex_PCU(0.f, -2.f, 0.f, 255, 153, 204, 255);
}

void Boss::RenderHeart() const
{
	Vertex_PCU tempVertArray[NUM_PLAYERSHIP_VERTS];

	for (int i = 0; i < NUM_PLAYERSHIP_VERTS; i++)
	{
		tempVertArray[i] = m_HeartVerts[i];
		tempVertArray[i].m_color = m_entityColor;
	}
	TransformVertexArrayXY3D(NUM_PLAYERSHIP_VERTS, tempVertArray, 1.f, m_orientationDegrees, m_position);
	if (m_state == OUTSIDE_RING) 
	{
		DrawDebugGlow(m_position, 8.f, m_outsideRingColor_in, m_outsideRingColor_out);
		DrawDebugGlow(m_position, 5.f, m_insideRingColor_in, m_insideRingColor_out);
	}
	if (m_state == INSIDE_RING)
	{
		DrawDebugGlow(m_position, 5.f, m_insideRingColor_in, m_insideRingColor_out);
	}
	g_theRenderer->DrawVertexArray(NUM_PLAYERSHIP_VERTS, tempVertArray);
}
void Boss::Movement(float deltaSeconds)
{
	m_changeDirectionTimer -= 0.05f;
	if (m_changeDirectionTimer < 0.f) 
	{
		m_randomStartOrientation = g_theRNG->RollRandomFloatInRange(0.f, 360.f);
		m_angularVelocity = g_theRNG->RollRandomFloatInRange(-200.f, 200.f);
		m_randomDirection = Vec2(CosDegrees(m_randomStartOrientation), SinDegrees(m_randomStartOrientation));
		m_changeDirectionTimer = m_changeDirectionTime;
	}

	m_position += (m_velocity * deltaSeconds);
	m_orientationDegrees += (m_angularVelocity * deltaSeconds);
}

void Boss::FirstSkill()
{
	if (m_isDead)
	{
		return;
	}

	m_shootingBulletTimer -= 0.05f;
	m_spawningAsteroidTimer -= 0.05f;

	if (m_shootingBulletTimer < 0)
	{
		SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/BossShootP1.wav");
		g_theAudio->StartSound(sound, false, 0.5f);
		float x = m_position.x + .5f * CosDegrees(m_orientationDegrees);
		float y = m_position.y + .5f * SinDegrees(m_orientationDegrees);
		m_game->SpawnCovidBullet(Vec2(x, y), m_orientationDegrees, 12.f);
		m_shootingBulletTimer = 3.f;
	}
	if (m_spawningAsteroidTimer < 0.f)
	{
		m_game->SpawnAsteroid();
		m_spawningAsteroidTimer = 13.f;
	}
}

void Boss::SecondSkill()
{
	if (m_isDead)
	{
		return;
	}

	m_shootingBulletTimer -= 0.05f;

	if (m_shootingBulletTimer < 0.f)
	{
		SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/BossShootP2.wav");
		g_theAudio->StartSound(sound);
		Vec2 shootDir = Vec2::MakeFromPolarDegrees(m_orientationDegrees);
		shootDir.RotateMinus90Degrees();
		for (int i = 0; i < 6; i++)
		{
			m_game->SpawnCovidBullet(m_position, shootDir.GetOrientationDegrees() + 60 * i, 25.f);
		}
		m_shootingBulletTimer = 5.f;
	}
}

void Boss::ThirdSkill()
{
	if (m_isDead)
	{
		return;
	}
	m_spawningEntityTimer -= 0.05f;
	if (m_spawningEntityTimer < 0.f && m_spawnedEntityCounter < m_maxEntityCanSpawn)
	{
		SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/BossShootP3.wav");
		g_theAudio->StartSound(sound);
		m_spawnedEntityCounter++;
		int decider = g_theRNG->RollRandomIntInRange(1, 3);
		switch (decider)
		{
		case 1:
			m_game->SpawnBeetle();
			break;
		case 2:
			m_game->SpawnWasp();
			break;
		case 3:
			m_game->SpawnCovid();
			break;
		}
		m_spawningEntityTimer = 17.f;
	}
}

void Boss::StateUpdate(float deltaSeconds)
{
	UNUSED(deltaSeconds);

	float fractionPerHP = (float)(255 / BOSS_NO_RING_STAGE_HEALTH);

	if (m_state == OUTSIDE_RING)
	{
		m_physicsRadius = BOSS_OUTSIDE_RING_LENGTH;
		m_cosmeticRadius = BOSS_OUTSIDE_RING_LENGTH + 0.2f;
		m_velocity = m_randomDirection * BOSS_OUTSIDE_RING_SPEED;
		m_changeDirectionTime = BOSS_OUTSIDE_RING_RANDOM_MOVING_TIMER;
		ColorDeductionByHealth(m_outsideRingColor_in.a, BOSS_OUTSIDE_RING_STAGE_HEALTH, fractionPerHP);
		ColorDeductionByHealth(m_outsideRingColor_out.a, BOSS_OUTSIDE_RING_STAGE_HEALTH, fractionPerHP);
		FirstSkill();
	}
	if (m_state == INSIDE_RING)
	{
		m_physicsRadius = BOSS_INSIDE_RING_LENGTH;
		m_cosmeticRadius = BOSS_INSIDE_RING_LENGTH + 0.2f;
		m_velocity = m_randomDirection * BOSS_INSIDE_RING_SPEED;
		m_changeDirectionTime = BOSS_INSIDE_RING_RANDOM_MOVING_TIMER;
		ColorDeductionByHealth(m_insideRingColor_in.a, BOSS_INSIDE_RING_STAGE_HEALTH, fractionPerHP);
		ColorDeductionByHealth(m_insideRingColor_out.a, BOSS_INSIDE_RING_STAGE_HEALTH, fractionPerHP);
		SecondSkill();
	}
	if (m_state == NO_RING)
	{
		m_physicsRadius = BOSS_NO_RING_LENGTH;
		m_cosmeticRadius = BOSS_NO_RING_LENGTH + 0.2f;
		m_velocity = m_randomDirection * BOSS_NO_RING_SPEED;
		m_changeDirectionTime = BOSS_NO_RING_RANDOM_MOVING_TIMER;
		PopColorWhenGetHit(Rgba8(255, 255, 255, 255));
		ThirdSkill();
	}
}

void Boss::SwitchState()
{
	if (m_health > BOSS_INSIDE_RING_STAGE_HEALTH)
	{
		m_state = OUTSIDE_RING;

	}
	else if (m_health <= BOSS_INSIDE_RING_STAGE_HEALTH && m_health > BOSS_NO_RING_STAGE_HEALTH)
	{
		m_state = INSIDE_RING;
		if (!m_isOSRSoundPlayed)
		{
			m_isOSRSoundPlayed = true;
			SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/BreakShieldBoss1.wav");
			g_theAudio->StartSound(sound, false, 2.f);
		}

	}
	else if (m_health <= 25)
	{
		m_state = NO_RING;
		if (!m_isISRSoundPlayed)
		{
			m_isISRSoundPlayed = true;
			SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/BreakShieldBoss2.wav");
			g_theAudio->StartSound(sound, false, 2.f);
		}
	}
}

void Boss::ColorDeductionByHealth(unsigned char& valueToChange, int maxHealth, float fraction)
{
	if (valueToChange > 0)
	{
		valueToChange = 255 - (unsigned char)((maxHealth - m_health) * fraction);
	}
}
