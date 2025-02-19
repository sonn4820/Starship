#include "Game/PlayerShip.hpp"
#include "Game/GameExtern.hpp"
#include "Engine/Core/Vertex_PCU.hpp"

PlayerShip::PlayerShip(Game* owner, Vec2 const& startPos)
	:Entity(owner, startPos)
{
	m_physicsRadius = PLAYER_SHIP_PHYSICS_RADIUS;
	m_cosmeticRadius = PLAYER_SHIP_COSMETIC_RADIUS;

	m_entityOriginalColor = Rgba8(102, 153, 204, 255);
	m_entityColor = m_entityOriginalColor;

	InitializeLocalVerts();
}
PlayerShip::~PlayerShip()
{

}
void PlayerShip::Update(float deltaSeconds)
{
	if (m_isDead)
	{
		return;
	}

	Movement(deltaSeconds);
	BounceOffTheWorldBoundaries();
	FireBullet(deltaSeconds);
	RandomizeTheFlame();
	CheckInvincible();
}
void PlayerShip::Render() const
{
	if (m_isDead)
	{
		return;
	}

	RenderShip();
	RenderFlameThrust();
}
void PlayerShip::Die()
{
	if (m_isDead)
	{
		return;
	}
	m_isDead = true;
	SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/PlayerDie.wav");
	g_theAudio->StartSound(sound);
	m_game->SpawnDebrisFromDeath(this, 5, 30, 1.f);
	m_extraLives--;
}

void PlayerShip::FireBullet(float deltaSeconds)
{
	if (g_theApp->m_gameClock->IsPaused())
	{
		return;
	}
	m_fireRate -= deltaSeconds;
	if (g_theInput->IsKeyDown(KEYCODE_SPACE) || g_theInput->GetController(0).IsButtonDown(XBOX_BUTTON_A))
	{
		if (m_isDead)
		{
			return;
		}
		if (m_fireRate < 0.f)
		{
			float x = m_position.x + .5f * CosDegrees(m_orientationDegrees);
			float y = m_position.y + .5f * SinDegrees(m_orientationDegrees);
			float firingDir = m_orientationDegrees + g_theRNG->RollRandomFloatInRange(-2.f, 2.f);
			if (m_overfireRate > 30.f && m_overfireRate <= 60.f)
			{
				firingDir += g_theRNG->RollRandomFloatInRange(-7.f, 7.f);
			}
			else if (m_overfireRate > 60.f)
			{
				firingDir += g_theRNG->RollRandomFloatInRange(-m_overfireRate * 0.2f, m_overfireRate * 0.2f);
			}
			m_game->SpawnBullet(Vec2(x, y), firingDir);
			SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/BulletShoot.wav");
			g_theAudio->StartSound(sound, false, 0.5f);
			m_fireRate = 0.75f;
			m_overfireRate += 5.f;
			if (m_overfireRate >= 153) 
			{
				m_overfireRate = 153;
			}
		}
	}
	else
	{
		m_overfireRate -= 0.75f;
	}
	if (m_overfireRate > 30.f)
	{
		unsigned char red = 102;
		unsigned char green = 153;
		unsigned char blue = 204;
		red = (unsigned char)((float)red + m_overfireRate);
		green = (unsigned char)((float)green - m_overfireRate);
		blue = (unsigned char)((float)blue - m_overfireRate);
		m_localVerts[9].m_color = Rgba8(red, green, blue, 255);
		m_localVerts[10].m_color = Rgba8(red, green, blue, 255);
		m_localVerts[11].m_color = Rgba8(red, green, blue, 255);
	}
	else
	{
		m_localVerts[9].m_color = m_entityOriginalColor;
		m_localVerts[10].m_color = m_entityOriginalColor;
		m_localVerts[11].m_color = m_entityOriginalColor;
	}

}

void PlayerShip::Respawn()
{
	if (IsAlive()) return;
	if (m_extraLives == -1) return;
	SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/PlayerRespawn.wav");
	g_theAudio->StartSound(sound);
	m_playerShipInvincibleTimer = PLAYER_SHIP_INVINCIBLE_TIMER;
	Vec2 worldCenter(WORLD_SIZE_X * 0.5f, WORLD_SIZE_Y * 0.5f);
	m_position = worldCenter;
	m_velocity = Vec2(0.f, 0.f);
	m_orientationDegrees = 0;
	m_health = 1;
	m_isDead = false;
	m_overfireRate = 0.f;
}

Vertex_PCU PlayerShip::GetLocalVert(int index, float scale)
{
	Vertex_PCU vertex = m_localVerts[index];
	vertex.m_position *= scale;
	return vertex;
}

void PlayerShip::Movement(float deltaSeconds)
{
	Vec2 forward = Vec2(CosDegrees(m_orientationDegrees), SinDegrees(m_orientationDegrees));

	// Controller
	if (g_theInput->GetController(0).GetLeftStick().GetMagnitude() > 0)
	{
		float joyStickOrientation = g_theInput->GetController(0).GetLeftStick().GetOrientationDegrees();
		m_orientationDegrees = joyStickOrientation;
	}

	m_ship_thrust = g_theInput->GetController(0).GetLeftStick().GetMagnitude() * PLAYER_SHIP_ACCELERATION;

	// Keyboard	
	if (g_theInput->IsKeyDown('A'))
	{
		m_orientationDegrees += deltaSeconds * PLAYER_SHIP_TURN_SPEED;
	}
	if (g_theInput->IsKeyDown('D'))
	{
		m_orientationDegrees -= deltaSeconds * PLAYER_SHIP_TURN_SPEED;
	}
	if (g_theInput->IsKeyDown('W'))
	{
		m_ship_thrust = PLAYER_SHIP_ACCELERATION;
	}

	m_velocity += (forward * m_ship_thrust * deltaSeconds);
	m_position += (m_velocity * deltaSeconds);
}

void PlayerShip::RenderShip() const
{
	Vertex_PCU tempVertArray[NUM_PLAYERSHIP_VERTS];

	for (int i = 0; i < NUM_PLAYERSHIP_VERTS; i++)
	{
		tempVertArray[i] = m_localVerts[i];
		if (m_isInvincible) 
		{
			tempVertArray[i].m_color = m_entityColor;
		}
	}

	TransformVertexArrayXY3D(NUM_PLAYERSHIP_VERTS, tempVertArray, 1.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(NUM_PLAYERSHIP_VERTS, tempVertArray);
}

void PlayerShip::RenderFlameThrust() const
{
	Vertex_PCU tempFlameVertArray[3];

	for (int i = 0; i < 3; i++)
	{
		tempFlameVertArray[i] = m_thurstVerts[i];
	}

	TransformVertexArrayXY3D(3, tempFlameVertArray, 1.f, m_orientationDegrees, m_position);

	g_theRenderer->DrawVertexArray(3, tempFlameVertArray);
}

void PlayerShip::InitializeLocalVerts()
{
	m_localVerts[0] = Vertex_PCU(2.f, 1.f, 0.f, 102, 153, 204, 255);
	m_localVerts[1] = Vertex_PCU(0.f, 2.f, 0.f, 102, 153, 204, 255);
	m_localVerts[2] = Vertex_PCU(-2.f, 1.f, 0.f, 102, 153, 204, 255);

	m_localVerts[3] = Vertex_PCU(0.f, 1.f, 0.f, 102, 153, 204, 255);
	m_localVerts[4] = Vertex_PCU(-2.f, 1.f, 0.f, 102, 153, 204, 255);
	m_localVerts[5] = Vertex_PCU(-2.f, -1.f, 0.f, 102, 153, 204, 255);

	m_localVerts[6] = Vertex_PCU(-2.f, -1.f, 0.f, 102, 153, 204, 255);
	m_localVerts[7] = Vertex_PCU(0.f, -1.f, 0.f, 102, 153, 204, 255);
	m_localVerts[8] = Vertex_PCU(0.f, 1.f, 0.f, 102, 153, 204, 255);

	m_localVerts[9] = Vertex_PCU(0.f, -1.f, 0.f, 102, 153, 204, 255);
	m_localVerts[10] = Vertex_PCU(1.f, 0.f, 0.f, 102, 153, 204, 255);
	m_localVerts[11] = Vertex_PCU(0.f, 1.f, 0.f, 102, 153, 204, 255);

	m_localVerts[12] = Vertex_PCU(2.f, -1.f, 0.f, 102, 153, 204, 255);
	m_localVerts[13] = Vertex_PCU(-2.f, -1.f, 0.f, 102, 153, 204, 255);
	m_localVerts[14] = Vertex_PCU(0.f, -2.f, 0.f, 102, 153, 204, 255);

	m_debrisColor = Rgba8(102, 153, 204, 127);
}

void PlayerShip::RandomizeTheFlame()
{
	float flameRandomLength = g_theRNG->RollRandomFloatInRange(-6.f, -4.f) * RangeMapClamped(m_ship_thrust, 0.f, PLAYER_SHIP_ACCELERATION, 0.2f, 1.f);
	float flameRandomColorG = g_theRNG->RollRandomIntInRange(100, 255) * RangeMapClamped(m_ship_thrust, 0.f, PLAYER_SHIP_ACCELERATION, 0.f, 1.f);
	float flameRandomColorB = g_theRNG->RollRandomIntInRange(0, 100) * RangeMapClamped(m_ship_thrust, 0.f, PLAYER_SHIP_ACCELERATION, 0.f, 1.f);
	float flameRandomColorA = g_theRNG->RollRandomIntInRange(170, 255) * RangeMapClamped(m_ship_thrust, 0.f, PLAYER_SHIP_ACCELERATION, 0.f, 1.f);
	m_thurstVerts[0] = Vertex_PCU(-2.f, 0.8f, 0.f, 255, (unsigned char)flameRandomColorG, (unsigned char)flameRandomColorB, (unsigned char)flameRandomColorA);
	m_thurstVerts[1] = Vertex_PCU(flameRandomLength, 0.f, 0.f, 255, 0, 0, (unsigned char)flameRandomColorA);
	m_thurstVerts[2] = Vertex_PCU(-2.f, -0.8f, 0.f, 255, (unsigned char)flameRandomColorG, (unsigned char)flameRandomColorB, (unsigned char)flameRandomColorA);
}

void PlayerShip::CheckInvincible()
{
	m_playerShipInvincibleTimer -= 0.02f;
	if (m_playerShipInvincibleTimer <= 0.f)
	{
		m_isInvincible = false;
	}
	else
	{
		m_isInvincible = true;
	}

	if (m_isInvincible)
	{
		m_invincibleEffectTimer -= 0.5f;
		if (m_invincibleEffectTimer <= 3.f)
		{
			m_entityColor = Rgba8(102, 153, 204, 100);
			if (m_invincibleEffectTimer <= 0.f)
			{
				m_invincibleEffectTimer = 6.f;
			}
		}
		else
		{
			m_entityColor = m_entityOriginalColor;
		}

	}
	else
	{
		m_entityColor = m_entityOriginalColor;
	}
}

