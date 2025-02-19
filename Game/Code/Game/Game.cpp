#include "Game/Game.hpp"
#include "Game/GameCommon.hpp"
#include "Game/GameExtern.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

Game::Game()
{
	m_worldCamera.SetOrthographicView(Vec2(0, 0), Vec2(WORLD_SIZE_X, WORLD_SIZE_Y));
	m_screenCamera.SetOrthographicView(Vec2(0, 0), Vec2(1600.f, 800.f));
	
}
//..............................
Game::~Game()
{

}
//..............................
void Game::Startup()
{
	InitializeUIVerts();
	InitializePlayerShip();
	SwitchState(ATTRACT_MODE);
	CreateRandomPosListForBackgroundStar();
}
//..............................
void Game::Shutdown()
{
	DeleteAll();
}

//----------------------------------------------------------------------------------------------------------------------------------------
// UPDATE

void Game::Update(float deltaSeconds)
{
	m_secondIntoMode += deltaSeconds;

	if (m_state == LEVEL_MODE || m_state == BOSS_MODE)
	{
		CheckGameOver();
		HandleInput();
		UpdateAllEntities(deltaSeconds);
		CheckCollision();
		CheckLevelCompleted();
		DeleteAllGarbageEntities();
		if (m_state == BOSS_MODE)
		{
			if (m_secondIntoMode > 1.f)
			{
				MusicBoss();
			}
		}
		else
		{
			MusicGame();
		}
	}
	if (m_state == ATTRACT_MODE)
	{
		UpdateAttractMode(deltaSeconds);
		MusicAttract();
	}
	if (m_state == CUT_SCENE_OPEN)
	{
		UpdateOpenCutScene(deltaSeconds);
	}
	if (m_state == CUT_SCENE_END)
	{
		UpdateEndCutScene();
	}
	UpdateCamera(deltaSeconds);
}
//..............................
void Game::UpdateAllEntities(float deltaSeconds)
{
	m_playerShip->Update(deltaSeconds);
	UpdateEntities(m_boss, 1, deltaSeconds);
	UpdateEntities(m_asteroids, MAX_ASTEROIDS, deltaSeconds);
	UpdateEntities(m_bullets, MAX_BULLETS, deltaSeconds);
	UpdateEntities(m_beetles, MAX_BEETLES, deltaSeconds);
	UpdateEntities(m_wasps, MAX_WASPS, deltaSeconds);
	UpdateEntities(m_debrises, MAX_DEBRIS, deltaSeconds);
	UpdateEntities(m_covids, MAX_COVID, deltaSeconds);
	UpdateEntities(m_covidBullets, MAX_BULLETS, deltaSeconds);
}
//..............................
void Game::UpdateEntities(Entity** entityArray, int maxValue, float deltaSeconds)
{
	for (int i = 0; i < maxValue; i++)
	{
		if (entityArray[i] == nullptr)
		{
			continue;
		}
		entityArray[i]->Update(deltaSeconds);
	}
}
//..............................
void Game::UpdateAttractMode(float deltaSeconds)
{
	PlayButtonBlinking();

	if (!g_theApp->m_gameClock->IsPaused())
	{
		if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE) || g_theInput->WasKeyJustPressed('N') || g_theInput->GetController(0).WasButtonJustPressed(XBOX_BUTTON_START))
		{
			SwitchState(CUT_SCENE_OPEN);
		}
	}

	for (int i = 0; i < 6; i++)
	{
		Vec3 movement = Vec3(5.f, 5.f, 0.f) * deltaSeconds;
		m_backgroundStarAttractMode[i].m_position += movement;
	}

}
//..............................
void Game::UpdateCamera(float deltaSeconds)
{
	SetCameraOriginalPosition();

	float shakeX = g_theRNG->RollRandomFloatInRange(-m_screenShakeAmount, m_screenShakeAmount);
	float shakeY = g_theRNG->RollRandomFloatInRange(-m_screenShakeAmount, m_screenShakeAmount);
	m_worldCamera.Translate2D(Vec2(shakeX, shakeY));
	m_screenShakeAmount -= SHAKE_REDUCTION_PER_SEC * deltaSeconds;
	m_screenShakeAmount = Clamp(m_screenShakeAmount, 0.f, MAX_SHAKE);
}
//..............................
void Game::UpdateOpenCutScene(float deltaSeconds)
{
	if (m_secondIntoMode > 0.5f && !g_theApp->m_gameClock->IsPaused())
	{
		if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE) || g_theInput->WasKeyJustPressed('N') || g_theInput->GetController(0).WasButtonJustPressed(XBOX_BUTTON_START))
		{
			SwitchState(LEVEL_MODE);
		}
	}

	if (m_secondIntoMode >= 24.f)
	{
		GameRestart();
		SwitchState(LEVEL_MODE);
	}
	for (int i = 0; i < 6; i++)
	{
		m_backgroundStarAttractMode[i].m_position += Vec3(3.f * deltaSeconds, 3.f * deltaSeconds, 0.f);
	}
}
//..............................
void Game::UpdateEndCutScene()
{
	if (m_secondIntoMode > 0.5f)
	{
		if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE) || g_theInput->WasKeyJustPressed('N') || g_theInput->GetController(0).WasButtonJustPressed(XBOX_BUTTON_START))
		{
			SwitchState(ATTRACT_MODE);
		}
	}
	if (m_secondIntoMode >= 15.f)
	{
		GameRestart();
		SwitchState(ATTRACT_MODE);
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
// RENDER 

void Game::Render() const
{
	g_theRenderer->ClearScreen(Rgba8(0, 0, 0, 255));
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	g_theRenderer->SetDepthStencilMode(DepthMode::DISABLED);
	g_theRenderer->SetModelConstants();

	if (m_state == LEVEL_MODE || m_state == BOSS_MODE)
	{
		g_theRenderer->BeginCamera(m_worldCamera);
		RenderBackGroundWorldCamera();
		RenderAllEntities();

		if (g_debugDraw)
		{
			DebugStarShip();
			DebugBullet();
			DebugAsteroid();
			DebugBeetle();
			DebugWasp();
			DebugDebris();
			DebugCovid();
			DebugCovidBullet();
			DebugBoss();
		}
		g_theRenderer->BeginCamera(m_screenCamera);
		if (m_state == BOSS_MODE)
		{
			RenderBossMode();
		}
		RenderUI();
	}
	if (m_state == ATTRACT_MODE)
	{
		g_theRenderer->BeginCamera(m_screenCamera);
		RenderAttractMode();
	}
	if (m_state == CUT_SCENE_OPEN)
	{
		g_theRenderer->BeginCamera(m_screenCamera);
		RenderOpenCutScene();
	}
	if (m_state == CUT_SCENE_END)
	{
		g_theRenderer->BeginCamera(m_screenCamera);
		RenderEndCutScene();
	}
	g_theRenderer->EndCamera(m_screenCamera);
	g_theRenderer->EndCamera(m_worldCamera);
}
//..............................
void Game::RenderAllEntities() const
{
	if (m_playerShip)
	{
		m_playerShip->Render();
	}
	RenderEntities(m_boss, 1);
	RenderEntities(m_asteroids, MAX_ASTEROIDS);
	RenderEntities(m_bullets, MAX_BULLETS);
	RenderEntities(m_beetles, MAX_BEETLES);
	RenderEntities(m_wasps, MAX_WASPS);
	RenderEntities(m_debrises, MAX_DEBRIS);
	RenderEntities(m_covids, MAX_COVID);
	RenderEntities(m_covidBullets, MAX_BULLETS);
}
//..............................
void Game::RenderEntities(Entity* const* entityArray, int maxValue) const
{
	for (int i = 0; i < maxValue; i++)
	{
		Entity const* entity = entityArray[i];
		if (entity)
		{
			entity->Render();
		}
	}
}
//..............................
void Game::RenderAttractMode() const
{
	RenderBackGroundScreenCamera();

	ThrustFlameRender(m_secondIntoMode);
	// Pink Ship
	DrawPlayerLiveIcon(Vec2(500.f, 400.f), 240.f, 64.f, Rgba8(255, 153, 204, 255));
	// Blue Ship
	DrawPlayerLiveIcon(Vec2(1000.f, 450.f), 240.f, 64.f, Rgba8(102, 153, 204, 255));
	g_theRenderer->DrawVertexArray(3, m_playButtonVerts);

	std::vector<Vertex_PCU> textVerts;
	AddVertsForTextTriangles2D(textVerts, "Press Start / Space to play", Vec2(580.f, 120.f), 25.f, Rgba8(255, 255, 255, 255));
	g_theRenderer->DrawVertexArray((int)textVerts.size(), textVerts.data());


	int pinkDialogue = static_cast<int>(RangeMap(m_secondIntoMode, 0.2f, 4.f, 0.f, 50.f));
	std::vector<Vertex_PCU> pinkDialogueVerts;
	AddVertsForTextTriangles2D(pinkDialogueVerts, "Pink:\n\nOh no! \nI think I'm out of fuel", Vec2(250.f, 620.f), 17.f, Rgba8(255, 120, 129, 255), .56f, false, 0.2f, pinkDialogue);
	g_theRenderer->DrawVertexArray((int)pinkDialogueVerts.size(), pinkDialogueVerts.data());

	int blueDialogue = static_cast<int>(RangeMap(m_secondIntoMode, 3.f, 6.f, 0.f, 50.f));
	std::vector<Vertex_PCU> blueDialogueVerts;
	AddVertsForTextTriangles2D(blueDialogueVerts, "Blue:\n\nThat's not good! \nI'll go ahead and find the\nnearest station", Vec2(1100.f, 320.f), 17.f, Rgba8(100, 120, 255, 255), .56f, false, 0.2f, blueDialogue);
	g_theRenderer->DrawVertexArray((int)blueDialogueVerts.size(), blueDialogueVerts.data());
}
//..............................
void Game::RenderUI() const
{
	for (int liveIndex = 0; liveIndex < static_cast<PlayerShip*>(m_playerShip)->m_extraLives; liveIndex++)
	{
		DrawPlayerLiveIcon(Vec2(32.f + liveIndex * 32.f, 768.f), 90.f, 6.4f, Rgba8(102, 153, 204, 255));
	}
}
//..............................
void Game::DrawPlayerLiveIcon(Vec2 startPos, float orientationDegrees, float scale, Rgba8 color) const
{
	PlayerShip* playerShip = static_cast<PlayerShip*>(m_playerShip);

	Vertex_PCU lifeIcon[NUM_PLAYERSHIP_VERTS];

	for (int i = 0; i < NUM_PLAYERSHIP_VERTS; i++)
	{
		lifeIcon[i] = playerShip->GetLocalVert(i, scale);
		lifeIcon[i].m_color = color;
	}
	TransformVertexArrayXY3D(NUM_PLAYERSHIP_VERTS, lifeIcon, 1.f, orientationDegrees, startPos);

	g_theRenderer->DrawVertexArray(NUM_PLAYERSHIP_VERTS, lifeIcon);
}
//..............................
void Game::RenderBackGroundWorldCamera() const
{
	for (int i = 0; i < 100; i++)
	{
		DrawStarBackground(m_starBackgroundPos[i] + m_playerShip->m_position * 0.1f, Rgba8(255, 255, 255, 255));
	}
	for (int i = 100; i < 200; i++)
	{
		DrawStarBackground(m_starBackgroundPos[i] + m_playerShip->m_position * 0.1f, Rgba8(100, 100, 100, 150));
	}
}
//..............................
void Game::RenderBackGroundScreenCamera() const
{
	for (int i = 0; i < 300; i++)
	{
		DrawStarBackground(m_starBackgroundPosAttractMode[i], Rgba8(255, 255, 255, 255));
	}
	for (int i = 300; i < 600; i++)
	{
		DrawStarBackground(m_starBackgroundPosAttractMode[i], Rgba8(100, 100, 100, 150));
	}
}
//..............................
void Game::DrawStarBackground(Vec2 startPos, Rgba8 color) const
{
	Vertex_PCU backgroundStar[6];
	if (m_state == ATTRACT_MODE || m_state == CUT_SCENE_OPEN)
	{
		for (int i = 0; i < 6; i++)
		{
			backgroundStar[i] = m_backgroundStarAttractMode[i];
			backgroundStar[i].m_color = color;
		}
	}
	else if (m_state == LEVEL_MODE || m_state == BOSS_MODE)
	{
		for (int i = 0; i < 6; i++)
		{
			backgroundStar[i] = m_backgroundStar[i];
			backgroundStar[i].m_color = color;
		}
	}

	TransformVertexArrayXY3D(6, backgroundStar, 1.f, 0, startPos);
	g_theRenderer->DrawVertexArray(6, backgroundStar);
}
//..............................
void Game::ThrustFlameRender(float decreasedThrustByTime) const
{
	// TODO: MAKE IT STICK WITH THE PLAYER'S SHIP INSTEAD OF THIS
	float flameRandomX_1 = g_theRNG->RollRandomFloatInRange(618.f, 622.f);
	float flameRandomY_1 = g_theRNG->RollRandomFloatInRange(595.f, 605.f);

	float flameRandomX_2 = g_theRNG->RollRandomFloatInRange(1128.f, 1132.f);
	float flameRandomY_2 = g_theRNG->RollRandomFloatInRange(655.f, 665.f);

	int flameRandomColorG = g_theRNG->RollRandomIntInRange(240, 255);
	int flameRandomColorB = g_theRNG->RollRandomIntInRange(90, 100);
	int flameRandomColorA = g_theRNG->RollRandomIntInRange(210, 255);

	float clampedFlameX_1 = (flameRandomX_1 > 0) ? flameRandomX_1 - decreasedThrustByTime : 0.f;
	float clampedFlameY_1 = (flameRandomY_1 > 0) ? flameRandomY_1 - decreasedThrustByTime : 0.f;

	Vertex_PCU thurstVerts[6] =
	{

		thurstVerts[2] = Vertex_PCU(515.f, 539.f, 0.f,255,(unsigned char)flameRandomColorG, (unsigned char)flameRandomColorB, (unsigned char)flameRandomColorA),
		thurstVerts[1] = Vertex_PCU(clampedFlameX_1, clampedFlameY_1, 0.f,255, 0, 0, (unsigned char)flameRandomColorA),
		thurstVerts[0] = Vertex_PCU(613.f, 482.f, 0.f,255, (unsigned char)flameRandomColorG, (unsigned char)flameRandomColorB, (unsigned char)flameRandomColorA),

		thurstVerts[5] = Vertex_PCU(1020.f, 585.f, 0.f,255, (unsigned char)flameRandomColorG, (unsigned char)flameRandomColorB, (unsigned char)flameRandomColorA),
		thurstVerts[4] = Vertex_PCU(flameRandomX_2, flameRandomY_2, 0.f,255, 0, 0, (unsigned char)flameRandomColorA),
		thurstVerts[3] = Vertex_PCU(1110.f, 534.f, 0.f,255, (unsigned char)flameRandomColorG, (unsigned char)flameRandomColorB, (unsigned char)flameRandomColorA)
	};

	g_theRenderer->DrawVertexArray(6, thurstVerts);
}
//..............................
void Game::RenderOpenCutScene() const
{
	RenderBackGroundScreenCamera();

	// Pink Ship
	DrawPlayerLiveIcon(Vec2(500.f + m_secondIntoMode * 25.f, 400.f + m_secondIntoMode * 25.f), 240.f, 64.f, Rgba8(255, 153, 204, 255));
	// Blue Ship
	DrawPlayerLiveIcon(Vec2(1000.f - m_secondIntoMode * 25.f, 450.f - m_secondIntoMode * 25.f), 240.f, 64.f, Rgba8(102, 153, 204, 255));

	int pinkDialogue = static_cast<int>(RangeMap(m_secondIntoMode, 3.f, 6.f, 0.f, 50.f));
	std::vector<Vertex_PCU> pinkDialogueVerts;
	AddVertsForTextTriangles2D(pinkDialogueVerts, "Pink:\n\nNoooooo! \nDon't leave me alone here!", Vec2(800.f, 620.f), 17.f, Rgba8(255, 120, 129, 255), .56f, false, 0.2f, pinkDialogue);
	if (m_secondIntoMode < 7.f)
	{
		g_theRenderer->DrawVertexArray((int)pinkDialogueVerts.size(), pinkDialogueVerts.data());
	}

	int blueDialogue = static_cast<int>(RangeMap(m_secondIntoMode, 6.f, 8.f, 0.f, 50.f));
	std::vector<Vertex_PCU> blueDialogueVerts;
	AddVertsForTextTriangles2D(blueDialogueVerts, "Blue:\n\nI'll come back soon \nDon't worry!", Vec2(400.f, 320.f), 17.f, Rgba8(100, 120, 255, 255), .56f, false, 0.2f, blueDialogue);
	if (m_secondIntoMode < 9.f)
	{
		g_theRenderer->DrawVertexArray((int)blueDialogueVerts.size(), blueDialogueVerts.data());
	}

	int instruction = static_cast<int>(RangeMap(m_secondIntoMode, 9.2f, 10.5f, 0.f, 50.f));
	std::vector<Vertex_PCU> instructionVerts;
	AddVertsForTextTriangles2D(instructionVerts, "How To Play Controller\n\nLeft JoyStick to move\nA to shoot\nStart to respawn\n\n\n\n\nHow To Play Keyboard\n\nW to thrust forward\nA to turn left\nD  to turn right\nSpace to shoot\nN to repsawn", Vec2(650.f, 580.f), 20.f, Rgba8(255, 255, 255, 255), .56f, false, 0.2f, instruction);
	g_theRenderer->DrawVertexArray((int)instructionVerts.size(), instructionVerts.data());

	int gogogo = static_cast<int>(RangeMap(m_secondIntoMode, 22.f, 23.f, 0.f, 50.f));
	std::vector<Vertex_PCU> gogogoVerts;
	AddVertsForTextTriangles2D(gogogoVerts, "READY? GO!", Vec2(650.f, 150.f), 40.f, Rgba8(255, 255, 255, 255), .56f, false, 0.2f, gogogo);
	g_theRenderer->DrawVertexArray((int)gogogoVerts.size(), gogogoVerts.data());

	if (m_secondIntoMode > 0.5f)
	{
		std::vector<Vertex_PCU> textVerts;
		AddVertsForTextTriangles2D(textVerts, "Press Start / Space to skip", Vec2(1350.f, 30.f), 10.f, Rgba8(255, 255, 255, 255));
		g_theRenderer->DrawVertexArray((int)textVerts.size(), textVerts.data());
	}
}
//..............................
void Game::RenderEndCutScene() const
{
	int blueDialogue = static_cast<int>(RangeMap(m_secondIntoMode, 0.5f, 4.f, 0.f, 50.f));
	std::vector<Vertex_PCU> blueDialogueVerts;
	AddVertsForTextTriangles2D(blueDialogueVerts, "Blue:\n\n\nI've Won... \n\nBut At What Cost?", Vec2(600.f, 550.f), 35.f, Rgba8(100, 120, 255, 255), .56f, false, 0.2f, blueDialogue);
	if (m_secondIntoMode < 6.f)
	{
		g_theRenderer->DrawVertexArray((int)blueDialogueVerts.size(), blueDialogueVerts.data());
	}

	int theEnd = static_cast<int>(RangeMap(m_secondIntoMode, 6.f, 7.f, 0.f, 50.f));
	std::vector<Vertex_PCU> theEndVerts;
	AddVertsForTextTriangles2D(theEndVerts, "THE END", Vec2(450.f, 280.f), 140.f, Rgba8(255, 0, 0, 255), .56f, false, 0.2f, theEnd);
	if (m_secondIntoMode >= 6.f)
	{
		g_theRenderer->DrawVertexArray((int)theEndVerts.size(), theEndVerts.data());
	}

	if (m_secondIntoMode > 0.5f)
	{
		std::vector<Vertex_PCU> textVerts;
		AddVertsForTextTriangles2D(textVerts, "Press Start / Space to skip", Vec2(1350.f, 30.f), 10.f, Rgba8(255, 255, 255, 255));
		g_theRenderer->DrawVertexArray((int)textVerts.size(), textVerts.data());
	}
}
//..............................
void Game::RenderBossMode() const
{
	int pinkDialogue = static_cast<int>(RangeMap(m_secondIntoMode, 0.2f, 3.f, 0.f, 50.f));
	std::vector<Vertex_PCU> pinkDialogueVerts;
	AddVertsForTextTriangles2D(pinkDialogueVerts, "Pink:\n\n\nYou abandoned me\n\nI can't forgive you\n\nI thought you love meeeeeeeeeeeeeee\n\nDIE, DIE, DIE!!!!!!", Vec2(700.f, 620.f), 17.f, Rgba8(255, 120, 129, 255), .56f, false, 0.2f, pinkDialogue);
	if (m_secondIntoMode < 7.5f)
	{
		g_theRenderer->DrawVertexArray((int)pinkDialogueVerts.size(), pinkDialogueVerts.data());
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
// GAME RESTART

void Game::GameRestart()
{
	Shutdown();
	Startup();
	m_shipDiedDelayTimer = DELAY_SHIP_DEATH_TIMER;
	SwitchState(LEVEL_MODE);
	m_level = -1;
	g_debugDraw = false;
	PlayMusicState(LEVEL_MODE);
}

//----------------------------------------------------------------------------------------------------------------------------------------
// HANDLE INPUT

void Game::HandleInput()
{
	if (g_theInput->WasKeyJustPressed('I'))
	{
		SpawnAsteroid();
	}

	if (g_theInput->WasKeyJustPressed('N') || g_theInput->GetController(0).WasButtonJustPressed(XBOX_BUTTON_START))
	{
		static_cast<PlayerShip*>(m_playerShip)->Respawn();

		for (Entity* beetle : m_beetles)
		{
			if (beetle == nullptr)
			{
				continue;
			}
			static_cast<Beetle*>(beetle)->m_playerShip = m_playerShip;
		}
		for (Entity* wasp : m_wasps)
		{
			if (wasp == nullptr)
			{
				continue;
			}
			static_cast<Wasp*>(wasp)->m_playerShip = m_playerShip;

		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
// LEVEL

bool Game::IsAnyEntityStillAlive(Entity** entity, int entityNum)
{
	for (int i = 0; i < entityNum; i++)
	{
		if (entity[i] == nullptr) continue;
		if (entity[i]->IsAlive()) return true;
	}
	return false;
}
//..............................
void Game::SpawnBossOnLevel()
{
	SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/IntroBoss.wav");
	g_theAudio->StartSound(sound, false, 3.f);
	SwitchState(BOSS_MODE);
	SpawnBoss();
}
//..............................
void Game::SpawnWavesOnLevel(int level, int asteroid, int beetle, int wasp, int covid)
{
	if (level == 0)
	{
		SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/lv1.wav");
		g_theAudio->StartSound(sound);
	}
	if (level == 1)
	{
		SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/lv2.wav");
		g_theAudio->StartSound(sound);
	}
	if (level == 2)
	{
		SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/lv3.wav");
		g_theAudio->StartSound(sound);
	}
	if (level == 3)
	{
		SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/lv4.wav");
		g_theAudio->StartSound(sound);
	}
	for (int i = 0; i < asteroid; i++)
	{
		SpawnAsteroid();
	}
	for (int i = 0; i < beetle; i++)
	{
		SpawnBeetle();
	}
	for (int i = 0; i < wasp; i++)
	{
		SpawnWasp();
	}
	for (int i = 0; i < covid; i++)
	{
		SpawnCovid();
	}
}
//..............................
void Game::CheckLevelCompleted()
{
	if (m_playerShip->m_isDead)
	{
		return;
	}
	if (m_state == BOSS_MODE)
	{
		if (IsAnyEntityStillAlive(m_boss, 1))
		{
			return;
		}
		SwitchState(CUT_SCENE_END);
	}
	if (m_state == LEVEL_MODE)
	{
		if (IsAnyEntityStillAlive(m_asteroids, MAX_ASTEROIDS)
			|| IsAnyEntityStillAlive(m_beetles, MAX_BEETLES)
			|| IsAnyEntityStillAlive(m_wasps, MAX_WASPS)
			|| IsAnyEntityStillAlive(m_covids, MAX_COVID))
		{
			return;
		}
	}
	m_level++;
	if (m_level > 0)
	{
		PlayerShip* ship = dynamic_cast<PlayerShip*>(m_playerShip);
		ship->m_extraLives += 1;
	}

	switch (m_level)
	{
	case 0:
		//TEST BOSS:
		//SpawnBossOnLevel();	
		SpawnWavesOnLevel(0, 2, 1, 0, 0);
		break;
	case 1:
		SpawnWavesOnLevel(1, 4, 2, 1, 0);
		break;
	case 2:
		SpawnWavesOnLevel(2, 6, 3, 2, 1);
		break;
	case 3:
		SpawnWavesOnLevel(3, 8, 4, 3, 2);
		break;
	case 4:
		SpawnBossOnLevel();
		break;
	default:
		break;
	}

}
//..............................
void Game::CheckGameOver()
{
	if (static_cast<PlayerShip*>(m_playerShip)->m_extraLives == -1) {
		m_shipDiedDelayTimer -= 0.025f;
		if (m_shipDiedDelayTimer <= 0)
		{
			SwitchState(ATTRACT_MODE);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
// SPAWN ENTITIES

Asteroid* Game::SpawnAsteroid()
{
	float maxX_SpawnLocation = WORLD_SIZE_X + ASTEROID_COSMETIC_RADIUS * 0.5f;
	float maxY_SpawnLocation = WORLD_SIZE_Y + ASTEROID_COSMETIC_RADIUS * 0.5f;
	float randomX = g_theRNG->RollRandomFloatInRange(0, WORLD_SIZE_X);
	float randomY = g_theRNG->RollRandomFloatInRange(0, WORLD_SIZE_Y);
	float posX = 0.f;
	float posY = 0.f;
	int decider = g_theRNG->RollRandomIntInRange(0, 1);

	if (decider == 0)
	{
		posX = maxX_SpawnLocation;
		posY = randomY;
	}
	else
	{
		posX = randomX;
		posY = maxY_SpawnLocation;
	}

	Asteroid* asteroid = new Asteroid(this, Vec2(posX, posY));
	for (int i = 0; i <= MAX_ASTEROIDS; i++)
	{
		if (i == MAX_ASTEROIDS)
		{
			return nullptr;
		}
		if (m_asteroids[i] == nullptr)
		{
			m_asteroids[i] = asteroid;
			break;
		}
	}
	return asteroid;
}
//..............................
Bullet* Game::SpawnBullet(Vec2 startPos, float fowardDegree)
{
	Bullet* bullet = new Bullet(this, startPos, fowardDegree);
	for (int i = 0; i <= MAX_BULLETS; i++)
	{
		if (i == MAX_BULLETS)
		{
			return nullptr;
		}
		if (m_bullets[i] == nullptr)
		{
			m_bullets[i] = bullet;
			break;
		}
	}
	return bullet;
}
//..............................
Beetle* Game::SpawnBeetle()
{
	float maxX_SpawnLocation = WORLD_SIZE_X + BEETLE_COSMETIC_RADIUS * 0.5f;
	float maxY_SpawnLocation = WORLD_SIZE_Y + BEETLE_COSMETIC_RADIUS * 0.5f;
	float randomX = g_theRNG->RollRandomFloatInRange(0, WORLD_SIZE_X);
	float randomY = g_theRNG->RollRandomFloatInRange(0, WORLD_SIZE_Y);
	float posX = 0.f;
	float posY = 0.f;
	int decider = g_theRNG->RollRandomIntInRange(0, 1);

	if (decider == 0)
	{
		posX = maxX_SpawnLocation;
		posY = randomY;
	}
	else
	{
		posX = randomX;
		posY = maxY_SpawnLocation;
	}

	Beetle* beetle = new Beetle(this, Vec2(posX, posY));
	beetle->m_playerShip = m_playerShip;
	for (int i = 0; i < MAX_BEETLES; i++)
	{
		if (m_beetles[i] == nullptr)
		{
			m_beetles[i] = beetle;
			break;
		}
	}
	return beetle;
}
//..............................
Wasp* Game::SpawnWasp()
{
	float maxX_SpawnLocation = WORLD_SIZE_X + BEETLE_COSMETIC_RADIUS * 0.5f;
	float maxY_SpawnLocation = WORLD_SIZE_Y + BEETLE_COSMETIC_RADIUS * 0.5f;
	float randomX = g_theRNG->RollRandomFloatInRange(0, WORLD_SIZE_X);
	float randomY = g_theRNG->RollRandomFloatInRange(0, WORLD_SIZE_Y);
	float posX = 0.f;
	float posY = 0.f;
	int decider = g_theRNG->RollRandomIntInRange(0, 1);

	if (decider == 0)
	{
		posX = maxX_SpawnLocation;
		posY = randomY;
	}
	else
	{
		posX = randomX;
		posY = maxY_SpawnLocation;
	}

	Wasp* wasp = new Wasp(this, Vec2(posX, posY));
	wasp->m_playerShip = m_playerShip;
	for (int i = 0; i < MAX_WASPS; i++)
	{
		if (m_wasps[i] == nullptr)
		{
			m_wasps[i] = wasp;
			break;
		}
	}
	return wasp;
}
//..............................
Debris* Game::SpawnDebris(Entity* entity, float sizeScale)
{
	Debris* debris = new Debris(this, entity->m_position, entity, sizeScale);
	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		if (m_debrises[i] == nullptr)
		{
			m_debrises[i] = debris;
			break;
		}
	}
	return debris;
}
//..............................
Covid* Game::SpawnCovid()
{
	float maxX_SpawnLocation = WORLD_SIZE_X + COVID_COSMETIC_RADIUS * 0.5f;
	float maxY_SpawnLocation = WORLD_SIZE_Y + COVID_COSMETIC_RADIUS * 0.5f;
	float randomX = g_theRNG->RollRandomFloatInRange(0, WORLD_SIZE_X);
	float randomY = g_theRNG->RollRandomFloatInRange(0, WORLD_SIZE_Y);
	float posX = 0.f;
	float posY = 0.f;
	int decider = g_theRNG->RollRandomIntInRange(0, 1);

	if (decider == 0)
	{
		posX = maxX_SpawnLocation;
		posY = randomY;
	}
	else
	{
		posX = randomX;
		posY = maxY_SpawnLocation;
	}

	Covid* covid = new Covid(this, Vec2(posX, posY));
	for (int i = 0; i <= MAX_COVID; i++)
	{
		if (i == MAX_COVID)
		{
			return nullptr;
		}
		if (m_covids[i] == nullptr)
		{
			m_covids[i] = covid;
			break;
		}
	}
	return covid;
}
//..............................
CovidBullet* Game::SpawnCovidBullet(Vec2 startPos, float fowardDegree, float bulletSpeed)
{
	CovidBullet* covidBullet = new CovidBullet(this, startPos, fowardDegree, bulletSpeed);
	for (int i = 0; i <= MAX_BULLETS; i++)
	{
		if (m_covidBullets[i] == nullptr)
		{
			m_covidBullets[i] = covidBullet;
			break;
		}
	}
	return covidBullet;
}
//..............................
Boss* Game::SpawnBoss()
{
	float maxX_SpawnLocation = 0 - COVID_COSMETIC_RADIUS * 0.5f;
	Boss* boss = new Boss(this, Vec2(maxX_SpawnLocation, g_theRNG->RollRandomFloatInRange(0, WORLD_SIZE_Y)));
	if (m_boss[0] == nullptr)
	{
		m_boss[0] = boss;
	}
	return boss;
}
//..............................
void Game::SpawnDebrisFromDeath(Entity* entity, int minAmount, int maxAmount, float sizeScale)
{
	int amount = g_theRNG->RollRandomIntInRange(minAmount, maxAmount);
	for (int i = 0; i <= amount; i++)
	{
		SpawnDebris(entity, sizeScale);
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
// COLLISION CHECK

void Game::CheckCollision()
{
	CheckCollisionEntities(m_bullets, MAX_BULLETS, m_asteroids, MAX_ASTEROIDS, true);
	CheckCollisionEntities(m_bullets, MAX_BULLETS, m_beetles, MAX_BEETLES, true);
	CheckCollisionEntities(m_bullets, MAX_BULLETS, m_wasps, MAX_WASPS, true);
	CheckCollisionEntities(m_bullets, MAX_BULLETS, m_covids, MAX_COVID, true);
	CheckCollisionEntities(m_bullets, MAX_BULLETS, m_boss, 1, false);

	CheckCollisionPlayerShipVsEntities(m_playerShip, m_asteroids, MAX_ASTEROIDS, false);
	CheckCollisionPlayerShipVsEntities(m_playerShip, m_beetles, MAX_BEETLES, true);
	CheckCollisionPlayerShipVsEntities(m_playerShip, m_wasps, MAX_WASPS, true);
	CheckCollisionPlayerShipVsEntities(m_playerShip, m_covids, MAX_COVID, false);
	CheckCollisionPlayerShipVsEntities(m_playerShip, m_covidBullets, MAX_BULLETS, false);
	CheckCollisionPlayerShipVsEntities(m_playerShip, m_boss, 1, false);
}
//..............................
void Game::CheckCollisionEntities(Entity** entityA, int numA, Entity** entityB, int numB, bool spawnDebris)
{
	for (int i = 0; i < numA; i++)
	{
		if (entityA[i] == nullptr)
		{
			continue;
		}
		for (int j = 0; j < numB; j++)
		{
			if (entityB[j] == nullptr)
			{
				continue;
			}
			CheckCollionEntityVsEntity(*entityA[i], *entityB[j], spawnDebris);
		}
	}
}
//..............................
void Game::CheckCollionEntityVsEntity(Entity& entityA, Entity& entityB, bool spawnDebris)
{
	if (DoEntitiesOverLap(entityA, entityB))
	{
		SoundID sound = g_theAudio->CreateOrGetSound("Data/Audio/Impact.wav");
		g_theAudio->StartSound(sound, false, 0.4f);
		if (!entityA.m_isInvincible)
		{
			m_screenShakeAmount += 0.35f;
			entityA.TakeDamage(1, spawnDebris);
			entityA.m_changeColorTimer = 2.f;
		}
		if (!entityB.m_isInvincible)
		{
			m_screenShakeAmount += 0.35f;
			entityB.TakeDamage(1, spawnDebris);
			entityB.m_changeColorTimer = 2.f;

		}
	}
	if (entityA.m_health <= 0)
	{
		entityA.Die();
	}
	if (entityB.m_health <= 0)
	{
		entityB.Die();
	}
}
//..............................
void Game::CheckCollisionPlayerShipVsEntities(Entity* playerShip, Entity** entity, int num, bool isEntityAutoTargetPlayer)
{
	for (int i = 0; i < num; i++)
	{
		if (entity[i] == nullptr)
		{
			continue;
		}
		CheckCollionPlayerShipVsEntity(*playerShip, *entity[i], isEntityAutoTargetPlayer);
	}
}
//..............................
void Game::CheckCollionPlayerShipVsEntity(Entity& playerShip, Entity& entity, bool isEntityAutoTargetPlayer)
{
	if (DoEntitiesOverLap(playerShip, entity))
	{
		if (!playerShip.m_isInvincible)
		{
			m_screenShakeAmount = MAX_SHAKE;
			playerShip.TakeDamage(1, false);
		}
		if (isEntityAutoTargetPlayer)
		{
			if (!entity.m_isInvincible)
			{
				entity.TakeDamage(1, true);
			}
		}
	}
	if (playerShip.m_health <= 0)
	{
		playerShip.Die();
	}
	if (entity.m_health <= 0)
	{
		entity.Die();
	}
}
//..............................
bool Game::DoEntitiesOverLap(Entity const& a, Entity const& b)
{
	if (a.m_isDead || b.m_isDead)
	{
		return false;
	}
	Vec2 centerA = a.m_position;
	Vec2 centerB = b.m_position;
	float radiusA = a.m_physicsRadius;
	float radiusB = b.m_physicsRadius;
	return DoDiscsOverlap2D(centerA, radiusA, centerB, radiusB);
}

//----------------------------------------------------------------------------------------------------------------------------------------
// DELETE ENTITIES

void Game::DeleteAllGarbageEntities()
{
	DeletGarbageEntities(m_asteroids, MAX_ASTEROIDS);
	DeletGarbageEntities(m_bullets, MAX_BULLETS);
	DeletGarbageEntities(m_beetles, MAX_BEETLES);
	DeletGarbageEntities(m_wasps, MAX_WASPS);
	DeletGarbageEntities(m_debrises, MAX_DEBRIS);
	DeletGarbageEntities(m_covids, MAX_COVID);
	DeletGarbageEntities(m_covidBullets, MAX_BULLETS);
	DeletGarbageEntities(m_boss, 1);
}
//..............................
void Game::DeletGarbageEntities(Entity** entity, int num)
{
	for (int i = 0; i < num; i++)
	{
		if (entity[i] == nullptr)
		{
			continue;
		}
		if (entity[i]->m_isGarbage)
		{
			delete entity[i];
			entity[i] = nullptr;
		}
	}
}
//..............................
void Game::DeleteAll()
{
	delete m_playerShip;
	m_playerShip = nullptr;
	DeleteEntities(m_boss, 1);
	DeleteEntities(m_asteroids, MAX_ASTEROIDS);
	DeleteEntities(m_bullets, MAX_BULLETS);
	DeleteEntities(m_beetles, MAX_BEETLES);
	DeleteEntities(m_wasps, MAX_WASPS);
	DeleteEntities(m_debrises, MAX_DEBRIS);
	DeleteEntities(m_covids, MAX_COVID);
	DeleteEntities(m_covidBullets, MAX_BULLETS);
}
//..............................
void Game::DeleteEntities(Entity** entity, int num)
{
	for (int i = 0; i < num; i++)
	{
		if (entity[i] == nullptr)
		{
			continue;
		}
		delete entity[i];
		entity[i] = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
// DEBUG

void Game::DebugStarShip() const
{
	if (m_playerShip != nullptr)
	{
		float xStart = m_playerShip->m_position.x;
		float yStart = m_playerShip->m_position.y;
		float xEnd = m_playerShip->m_position.x + m_playerShip->m_velocity.x;
		float yEnd = m_playerShip->m_position.y + m_playerShip->m_velocity.y;
		// Velocity Line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xEnd, yEnd), 0.2f, Rgba8(255, 234, 0, 255));

		// Ring
		DrawDebugRing(Vec2(m_playerShip->m_position.x, m_playerShip->m_position.y), m_playerShip->m_physicsRadius, .2f, Rgba8(0, 255, 255, 255));
		DrawDebugRing(Vec2(m_playerShip->m_position.x, m_playerShip->m_position.y), m_playerShip->m_cosmeticRadius, .2f, Rgba8(255, 0, 255, 255));

		// Rotation Line
		Vec2 forward = (Vec2(CosDegrees(m_playerShip->m_orientationDegrees), SinDegrees(m_playerShip->m_orientationDegrees))).GetNormalized() * PLAYER_SHIP_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + forward.x, yStart + forward.y), 0.2f, Rgba8(255, 0, 0, 255));
		Vec2 upward = (forward.GetRotated90Degrees()).GetNormalized() * PLAYER_SHIP_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + upward.x, yStart + upward.y), 0.2f, Rgba8(0, 255, 0, 255));
	}
}
//..............................
void Game::DebugAsteroid() const
{
	for (int i = 0; i < MAX_ASTEROIDS; i++)
	{
		if (m_asteroids[i] == nullptr)
		{
			continue;
		}
		float xStart = m_asteroids[i]->m_position.x;
		float yStart = m_asteroids[i]->m_position.y;
		float xEnd = m_asteroids[i]->m_position.x + static_cast<Asteroid*>(m_asteroids[i])->m_randomDirection.x * 10.f;
		float yEnd = m_asteroids[i]->m_position.y + static_cast<Asteroid*>(m_asteroids[i])->m_randomDirection.y * 10.f;
		// Connect to player line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(m_playerShip->m_position.x, m_playerShip->m_position.y), 0.2f, Rgba8(50, 50, 50, 255));

		// Direction Line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xEnd, yEnd), 0.2f, Rgba8(255, 234, 0, 255));

		// Ring Line
		DrawDebugRing(Vec2(m_asteroids[i]->m_position.x, m_asteroids[i]->m_position.y), m_asteroids[i]->m_physicsRadius, .2f, Rgba8(0, 255, 255, 255));
		DrawDebugRing(Vec2(m_asteroids[i]->m_position.x, m_asteroids[i]->m_position.y), m_asteroids[i]->m_cosmeticRadius, .2f, Rgba8(255, 0, 255, 255));

		// Rotation Line
		Vec2 forward = (Vec2(CosDegrees(m_asteroids[i]->m_orientationDegrees), SinDegrees(m_asteroids[i]->m_orientationDegrees))).GetNormalized() * ASTEROID_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + forward.x, yStart + forward.y), 0.2f, Rgba8(255, 0, 0, 255));
		Vec2 upward = (forward.GetRotated90Degrees()).GetNormalized() * ASTEROID_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + upward.x, yStart + upward.y), 0.2f, Rgba8(0, 255, 0, 255));
	}
}
//..............................
void Game::DebugBullet() const
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (m_bullets[i] == nullptr)
		{
			continue;
		}
		float xStart = m_bullets[i]->m_position.x;
		float yStart = m_bullets[i]->m_position.y;
		float xEnd = m_bullets[i]->m_position.x + 10.f * CosDegrees(m_bullets[i]->m_orientationDegrees);
		float yEnd = m_bullets[i]->m_position.y + 10.f * SinDegrees(m_bullets[i]->m_orientationDegrees);
		// Connect to player line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(m_playerShip->m_position.x, m_playerShip->m_position.y), 0.2f, Rgba8(50, 50, 50, 255));

		// Direction Line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xEnd, yEnd), 0.2f, Rgba8(255, 234, 0, 255));

		// Ring Line
		DrawDebugRing(Vec2(m_bullets[i]->m_position.x, m_bullets[i]->m_position.y), m_bullets[i]->m_physicsRadius, .2f, Rgba8(0, 255, 255, 255));
		DrawDebugRing(Vec2(m_bullets[i]->m_position.x, m_bullets[i]->m_position.y), m_bullets[i]->m_cosmeticRadius, .2f, Rgba8(255, 0, 255, 255));

		// Rotation Line
		Vec2 forward = (Vec2(CosDegrees(m_bullets[i]->m_orientationDegrees), SinDegrees(m_bullets[i]->m_orientationDegrees))).GetNormalized() * BULLET_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + forward.x, yStart + forward.y), 0.2f, Rgba8(255, 0, 0, 255));
		Vec2 upward = (forward.GetRotated90Degrees()).GetNormalized() * BULLET_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + upward.x, yStart + upward.y), 0.2f, Rgba8(0, 255, 0, 255));
	}
}
//..............................
void Game::DebugBeetle() const
{
	for (int i = 0; i < MAX_BEETLES; i++)
	{
		if (m_beetles[i] == nullptr)
		{
			continue;
		}
		float xStart = m_beetles[i]->m_position.x;
		float yStart = m_beetles[i]->m_position.y;
		float xEnd = m_beetles[i]->m_position.x + 10.f * CosDegrees(m_beetles[i]->m_orientationDegrees);
		float yEnd = m_beetles[i]->m_position.y + 10.f * SinDegrees(m_beetles[i]->m_orientationDegrees);
		// Connect to player line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(m_playerShip->m_position.x, m_playerShip->m_position.y), 0.2f, Rgba8(50, 50, 50, 255));

		// Direction Line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xEnd, yEnd), 0.2f, Rgba8(255, 234, 0, 255));

		// Ring Line
		DrawDebugRing(Vec2(m_beetles[i]->m_position.x, m_beetles[i]->m_position.y), m_beetles[i]->m_physicsRadius, .2f, Rgba8(0, 255, 255, 255));
		DrawDebugRing(Vec2(m_beetles[i]->m_position.x, m_beetles[i]->m_position.y), m_beetles[i]->m_cosmeticRadius, .2f, Rgba8(255, 0, 255, 255));

		// Rotation Line
		Vec2 forward = (Vec2(CosDegrees(m_beetles[i]->m_orientationDegrees), SinDegrees(m_beetles[i]->m_orientationDegrees))).GetNormalized() * ASTEROID_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + forward.x, yStart + forward.y), 0.2f, Rgba8(255, 0, 0, 255));
		Vec2 upward = (forward.GetRotated90Degrees()).GetNormalized() * ASTEROID_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + upward.x, yStart + upward.y), 0.2f, Rgba8(0, 255, 0, 255));
	}
}
//..............................
void Game::DebugWasp() const
{
	for (int i = 0; i < MAX_WASPS; i++)
	{
		if (m_wasps[i] == nullptr)
		{
			continue;
		}
		float xStart = m_wasps[i]->m_position.x;
		float yStart = m_wasps[i]->m_position.y;
		float xEnd = m_wasps[i]->m_position.x + 10.f * CosDegrees(m_wasps[i]->m_orientationDegrees);
		float yEnd = m_wasps[i]->m_position.y + 10.f * SinDegrees(m_wasps[i]->m_orientationDegrees);
		// Connect to player line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(m_playerShip->m_position.x, m_playerShip->m_position.y), 0.2f, Rgba8(50, 50, 50, 255));

		// Direction Line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xEnd, yEnd), 0.2f, Rgba8(255, 234, 0, 255));

		// Ring Line
		DrawDebugRing(Vec2(m_wasps[i]->m_position.x, m_wasps[i]->m_position.y), m_wasps[i]->m_physicsRadius, .2f, Rgba8(0, 255, 255, 255));
		DrawDebugRing(Vec2(m_wasps[i]->m_position.x, m_wasps[i]->m_position.y), m_wasps[i]->m_cosmeticRadius, .2f, Rgba8(255, 0, 255, 255));

		// Rotation Line
		Vec2 forward = (Vec2(CosDegrees(m_wasps[i]->m_orientationDegrees), SinDegrees(m_wasps[i]->m_orientationDegrees))).GetNormalized() * ASTEROID_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + forward.x, yStart + forward.y), 0.2f, Rgba8(255, 0, 0, 255));
		Vec2 upward = (forward.GetRotated90Degrees()).GetNormalized() * ASTEROID_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + upward.x, yStart + upward.y), 0.2f, Rgba8(0, 255, 0, 255));
	}
}
//..............................
void Game::DebugDebris() const
{
	for (int i = 0; i < MAX_DEBRIS; i++)
	{
		if (m_debrises[i] == nullptr)
		{
			break;
		}
		float xStart = m_debrises[i]->m_position.x;
		float yStart = m_debrises[i]->m_position.y;
		float xEnd = m_debrises[i]->m_position.x + static_cast<Debris*>(m_debrises[i])->m_randomDirection.x * 10.f;
		float yEnd = m_debrises[i]->m_position.y + static_cast<Debris*>(m_debrises[i])->m_randomDirection.y * 10.f;
		// Connect to player line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(m_playerShip->m_position.x, m_playerShip->m_position.y), 0.2f, Rgba8(50, 50, 50, 255));

		// Direction Line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xEnd, yEnd), 0.2f, Rgba8(255, 234, 0, 255));

		// Ring Line
		DrawDebugRing(Vec2(m_debrises[i]->m_position.x, m_debrises[i]->m_position.y), m_debrises[i]->m_physicsRadius, .2f, Rgba8(0, 255, 255, 255));
		DrawDebugRing(Vec2(m_debrises[i]->m_position.x, m_debrises[i]->m_position.y), m_debrises[i]->m_cosmeticRadius, .2f, Rgba8(255, 0, 255, 255));

		// Rotation Line
		Vec2 forward = (Vec2(CosDegrees(m_debrises[i]->m_orientationDegrees), SinDegrees(m_debrises[i]->m_orientationDegrees))).GetNormalized() * ASTEROID_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + forward.x, yStart + forward.y), 0.2f, Rgba8(255, 0, 0, 255));
		Vec2 upward = (forward.GetRotated90Degrees()).GetNormalized() * ASTEROID_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + upward.x, yStart + upward.y), 0.2f, Rgba8(0, 255, 0, 255));
	}
}
//..............................
void Game::DebugCovid() const
{
	for (int i = 0; i < MAX_ASTEROIDS; i++)
	{
		if (m_covids[i] == nullptr)
		{
			continue;
		}
		float xStart = m_covids[i]->m_position.x;
		float yStart = m_covids[i]->m_position.y;
		float xEnd = m_covids[i]->m_position.x + static_cast<Covid*>(m_covids[i])->m_randomDirection.x * 10.f;
		float yEnd = m_covids[i]->m_position.y + static_cast<Covid*>(m_covids[i])->m_randomDirection.y * 10.f;
		// Connect to player line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(m_playerShip->m_position.x, m_playerShip->m_position.y), 0.2f, Rgba8(50, 50, 50, 255));

		// Direction Line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xEnd, yEnd), 0.2f, Rgba8(255, 234, 0, 255));

		// Ring Line
		DrawDebugRing(Vec2(m_covids[i]->m_position.x, m_covids[i]->m_position.y), m_covids[i]->m_physicsRadius, .2f, Rgba8(0, 255, 255, 255));
		DrawDebugRing(Vec2(m_covids[i]->m_position.x, m_covids[i]->m_position.y), m_covids[i]->m_cosmeticRadius, .2f, Rgba8(255, 0, 255, 255));

		// Rotation Line
		Vec2 forward = (Vec2(CosDegrees(m_covids[i]->m_orientationDegrees), SinDegrees(m_covids[i]->m_orientationDegrees))).GetNormalized() * COVID_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + forward.x, yStart + forward.y), 0.2f, Rgba8(255, 0, 0, 255));
		Vec2 upward = (forward.GetRotated90Degrees()).GetNormalized() * COVID_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + upward.x, yStart + upward.y), 0.2f, Rgba8(0, 255, 0, 255));
	}
}
//..............................
void Game::DebugCovidBullet() const
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (m_covidBullets[i] == nullptr)
		{
			continue;
		}
		float xStart = m_covidBullets[i]->m_position.x;
		float yStart = m_covidBullets[i]->m_position.y;
		float xEnd = m_covidBullets[i]->m_position.x + 10.f * CosDegrees(m_covidBullets[i]->m_orientationDegrees);
		float yEnd = m_covidBullets[i]->m_position.y + 10.f * SinDegrees(m_covidBullets[i]->m_orientationDegrees);
		// Connect to player line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(m_playerShip->m_position.x, m_playerShip->m_position.y), 0.2f, Rgba8(50, 50, 50, 255));

		// Direction Line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xEnd, yEnd), 0.2f, Rgba8(255, 234, 0, 255));

		// Ring Line
		DrawDebugRing(Vec2(m_covidBullets[i]->m_position.x, m_covidBullets[i]->m_position.y), m_covidBullets[i]->m_physicsRadius, .2f, Rgba8(0, 255, 255, 255));
		DrawDebugRing(Vec2(m_covidBullets[i]->m_position.x, m_covidBullets[i]->m_position.y), m_covidBullets[i]->m_cosmeticRadius, .2f, Rgba8(255, 0, 255, 255));

		// Rotation Line
		Vec2 forward = (Vec2(CosDegrees(m_covidBullets[i]->m_orientationDegrees), SinDegrees(m_covidBullets[i]->m_orientationDegrees))).GetNormalized() * COVID_BULLET_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + forward.x, yStart + forward.y), 0.2f, Rgba8(255, 0, 0, 255));
		Vec2 upward = (forward.GetRotated90Degrees()).GetNormalized() * COVID_BULLET_PHYSICS_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + upward.x, yStart + upward.y), 0.2f, Rgba8(0, 255, 0, 255));
	}
}
//..............................
void Game::DebugBoss() const
{
	if (m_boss[0] != nullptr)
	{
		float xStart = m_boss[0]->m_position.x;
		float yStart = m_boss[0]->m_position.y;
		float xEnd = m_boss[0]->m_position.x + 10.f * CosDegrees(m_boss[0]->m_orientationDegrees);
		float yEnd = m_boss[0]->m_position.y + 10.f * SinDegrees(m_boss[0]->m_orientationDegrees);
		// Connect to player line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(m_playerShip->m_position.x, m_playerShip->m_position.y), 0.2f, Rgba8(50, 50, 50, 255));

		// Direction Line
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xEnd, yEnd), 0.2f, Rgba8(255, 234, 0, 255));

		// Ring Line
		DrawDebugRing(Vec2(m_boss[0]->m_position.x, m_boss[0]->m_position.y), m_boss[0]->m_physicsRadius, .2f, Rgba8(0, 255, 255, 255));
		DrawDebugRing(Vec2(m_boss[0]->m_position.x, m_boss[0]->m_position.y), m_boss[0]->m_cosmeticRadius, .2f, Rgba8(255, 0, 255, 255));

		// Rotation Line
		Vec2 forward = (Vec2(CosDegrees(m_boss[0]->m_orientationDegrees), SinDegrees(m_boss[0]->m_orientationDegrees))).GetNormalized() * BOSS_COSMETIC_RADIUS;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + forward.x, yStart + forward.y), 0.2f, Rgba8(255, 0, 0, 255));
		Vec2 upward = (forward.GetRotated90Degrees()).GetNormalized() * 4.f;
		DrawDebugLine(Vec2(xStart, yStart), Vec2(xStart + upward.x, yStart + upward.y), 0.2f, Rgba8(0, 255, 0, 255));
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
//MISC

void Game::InitializePlayerShip()
{
	Vec2 worldCenter(WORLD_SIZE_X * 0.5f, WORLD_SIZE_Y * 0.5f);
	m_playerShip = new PlayerShip(this, worldCenter);
}
//..............................
void Game::SwitchState(GameState state)
{
	m_state = state;
	m_secondIntoMode = 0;
	PlayMusicState(state);
}
//..............................
void Game::InitializeUIVerts()
{
	m_playButtonVerts[0] = Vertex_PCU(520.f, 150.f, 0.f, 0, 255, 0, 255);
	m_playButtonVerts[2] = Vertex_PCU(560.f, 130.f, 0.f, 0, 255, 0, 255);
	m_playButtonVerts[1] = Vertex_PCU(520.f, 110.f, 0.f, 0, 255, 0, 255);

	m_backgroundStar[0] = Vertex_PCU(0.f, 0.2f, 0.f, 255, 255, 255, 255);
	m_backgroundStar[1] = Vertex_PCU(-0.2f, 0.f, 0.f, 255, 255, 255, 255);
	m_backgroundStar[2] = Vertex_PCU(0.f, -0.2f, 0.f, 255, 255, 255, 255);
	m_backgroundStar[3] = Vertex_PCU(0.f, 0.2f, 0.f, 255, 255, 255, 255);
	m_backgroundStar[4] = Vertex_PCU(0.2f, 0.f, 0.f, 255, 255, 255, 255);
	m_backgroundStar[5] = Vertex_PCU(0.f, -0.2f, 0.f, 255, 255, 255, 255);

	m_backgroundStarAttractMode[0] = Vertex_PCU(0.f, 2.f, 0.f, 255, 255, 255, 255);
	m_backgroundStarAttractMode[1] = Vertex_PCU(-2.f, 0.f, 0.f, 255, 255, 255, 255);
	m_backgroundStarAttractMode[2] = Vertex_PCU(0.f, -2.f, 0.f, 255, 255, 255, 255);
	m_backgroundStarAttractMode[3] = Vertex_PCU(0.f, 2.f, 0.f, 255, 255, 255, 255);
	m_backgroundStarAttractMode[4] = Vertex_PCU(2.f, 0.f, 0.f, 255, 255, 255, 255);
	m_backgroundStarAttractMode[5] = Vertex_PCU(0.f, -2.f, 0.f, 255, 255, 255, 255);
}
//..............................
void Game::PlayButtonBlinking()
{
	float maxFadingValue = 0.8f;
	float minFadingValue = 0.4f;

	m_fadingOpacity += 0.008f * (float)m_fadingDir;

	if (m_fadingOpacity >= maxFadingValue)
	{
		m_fadingOpacity = maxFadingValue; //clamp to 1
		m_fadingDir = -1; // minus
	}
	if (m_fadingOpacity <= minFadingValue)
	{
		m_fadingOpacity = minFadingValue; //clamp to 0
		m_fadingDir = 1; // plus
	}

	for (size_t i = 0; i < 3; i++)
	{
		m_playButtonVerts[i].m_color.a = (unsigned char)(255.f * m_fadingOpacity);
	}
}
//..............................
void Game::SetCameraOriginalPosition()
{
	m_worldCamera.SetOrthographicView(Vec2(0, 0), Vec2(WORLD_SIZE_X, WORLD_SIZE_Y));
	m_screenCamera.SetOrthographicView(Vec2(0, 0), Vec2(1600.f, 800.f));
}
//..............................
void Game::CreateRandomPosListForBackgroundStar()
{
	for (int i = 0; i < 600; i++)
	{
		m_starBackgroundPosAttractMode[i] = Vec2(g_theRNG->RollRandomFloatInRange(-1600.f, 1600.f), g_theRNG->RollRandomFloatInRange(-800.f, 800.f));
	}
	for (int i = 0; i < 200; i++)
	{
		m_starBackgroundPos[i] = Vec2(g_theRNG->RollRandomFloatInRange(-5.f, WORLD_SIZE_X), g_theRNG->RollRandomFloatInRange(-5.f, WORLD_SIZE_Y));
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
/// MUSIC

void Game::MusicGame()
{
	if (!m_isGameMusicPlayed)
	{
		StopMusic();
		m_isGameMusicPlayed = true;
		SoundID gameMusic = g_theAudio->CreateOrGetSound("Data/Audio/GameMusic.wav");
		m_gameMusic = g_theAudio->StartSound(gameMusic, true, 1.f);
	}
}
//..............................
void Game::MusicAttract()
{
	if (!m_isAttractMusicPlayed)
	{
		StopMusic();
		m_isAttractMusicPlayed = true;
		SoundID menuMusic = g_theAudio->CreateOrGetSound("Data/Audio/MenuMusic.wav");
		m_menuMusic = g_theAudio->StartSound(menuMusic, true, 1.f);
	}
}//..............................
void Game::MusicBoss()
{
	if (!m_isBossMusicPlayed)
	{
		StopMusic();
		m_isBossMusicPlayed = true;
		SoundID bossMusic = g_theAudio->CreateOrGetSound("Data/Audio/BossMusic.wav");
		m_bossMusic = g_theAudio->StartSound(bossMusic, true, 1.f);
	}
}
//..............................
void Game::StopMusic()
{
	g_theAudio->StopSound(m_gameMusic);
	g_theAudio->StopSound(m_menuMusic);
	g_theAudio->StopSound(m_bossMusic);
}
//...............................
void Game::PlayMusicState(GameState state)
{
	if (state == ATTRACT_MODE)
	{
		m_isAttractMusicPlayed = false;
		m_isGameMusicPlayed = true;
		m_isBossMusicPlayed = true;
	}
	else if (state == LEVEL_MODE)
	{
		m_isGameMusicPlayed = false;
		m_isAttractMusicPlayed = true;
		m_isBossMusicPlayed = true;
	}
	else if (state == BOSS_MODE)
	{
		m_isBossMusicPlayed = false;
		m_isGameMusicPlayed = true;
		m_isAttractMusicPlayed = true;
	}
}
