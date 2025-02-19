#pragma once

#include "Game/Asteroid.hpp"
#include "Game/Bullet.hpp"
#include "Game/PlayerShip.hpp"
#include "Game/Beetle.hpp"
#include "Game/Wasp.hpp"
#include "Game/Debris.hpp"
#include "Game/Covid.hpp"
#include "Game/CovidBullet.hpp"
#include "Game/Boss.hpp"
#include "Engine/Renderer/Camera.hpp"

enum GameState 
{
	ATTRACT_MODE,
	CUT_SCENE_OPEN,
	LEVEL_MODE,
	BOSS_MODE,
	CUT_SCENE_END
};
class Game
{
public:
	Game();
	~Game();

	void Startup();
	void Update(float deltaSeconds);
	void Render() const;
	void Shutdown();

	// VARIABLES
	int m_level = -1;
	bool m_isGameMusicPlayed = false;
	bool m_isAttractMusicPlayed = false;
	bool m_isBossMusicPlayed = false;
	Camera m_worldCamera;
	Camera m_screenCamera;
	GameState m_state = ATTRACT_MODE;

	// ENTITIES
	Entity* m_playerShip = nullptr;
	Entity* m_boss[1] =	{};
	Entity* m_asteroids[MAX_ASTEROIDS] = {};
	Entity* m_bullets[MAX_BULLETS] = {};
	Entity* m_beetles[MAX_BEETLES] = {};
	Entity* m_wasps[MAX_WASPS] = {};
	Entity* m_debrises[MAX_DEBRIS] = {};
	Entity* m_covids[MAX_COVID] = {};
	Entity* m_covidBullets[MAX_BULLETS] = {};

	// SPAWN ENTITIES
	Asteroid* SpawnAsteroid();
	Beetle* SpawnBeetle();
	Wasp* SpawnWasp();
	Bullet* SpawnBullet(Vec2 startPos, float fowardDegree);
	Debris* SpawnDebris(Entity* entity, float sizeScale);
	Covid* SpawnCovid();
	CovidBullet* SpawnCovidBullet(Vec2 startPos, float fowardDegree, float bulletSpeed);
	Boss* SpawnBoss();
	void SpawnDebrisFromDeath(Entity* entity, int minAmount, int maxAmount, float sizeScale);

	// MUSIC
	void PlayMusicState(GameState state);

	// STATE
	void SwitchState(GameState state);

	// GAME RESTART
	void GameRestart();

private:
	// VARIABLES
	float m_screenShakeAmount = 0.0f;
	float m_secondIntoMode = 0.f;
	float m_shipDiedDelayTimer = DELAY_SHIP_DEATH_TIMER;
	float m_fadingOpacity = 1.f;

	int m_fadingDir = -1;

	bool m_IsBackgroundDrawn = false;
	bool m_isFading = true;

	Vec2 m_starBackgroundPos[200] = {};
	Vec2 m_starBackgroundPosAttractMode[600] = {};

	Vertex_PCU m_playButtonVerts[3] = {};
	Vertex_PCU m_backgroundStarAttractMode[6] = {};
	Vertex_PCU m_backgroundStar[6] = {};

	SoundPlaybackID m_gameMusic = 0;
	SoundPlaybackID m_menuMusic = 0;
	SoundPlaybackID m_bossMusic = 0;

	// UPDATE
	void UpdateAllEntities(float deltaSeconds);
	void UpdateEntities(Entity** entityArray, int maxValue, float deltaSeconds);
	void UpdateCamera(float deltaSeconds);
	void UpdateAttractMode(float deltaSeconds);
	void UpdateOpenCutScene(float deltaSeconds);
	void UpdateEndCutScene();
	void CheckGameOver();
	void HandleInput();

	// RENDER
	void RenderAllEntities() const;
	void RenderEntities(Entity* const* entityArray, int maxValue) const;
	void RenderUI() const;
	void RenderBackGroundWorldCamera() const;
	void RenderBackGroundScreenCamera() const;
	void DrawPlayerLiveIcon(Vec2 startPos, float orientationDegrees, float scale, Rgba8 color) const;
	void DrawStarBackground(Vec2 startPos, Rgba8 color) const;
	void RenderAttractMode() const;
	void ThrustFlameRender(float decreasedThrustByTime) const;
	void RenderOpenCutScene() const;
	void RenderEndCutScene() const;
	void RenderBossMode() const;

	// WAVE
	void CheckLevelCompleted();
	bool IsAnyEntityStillAlive(Entity** entity, int entityNum);
	void SpawnWavesOnLevel(int level, int asteroid, int beetle, int wasp, int covid);
	void SpawnBossOnLevel();

	// COLLISION
	void CheckCollision();
	void CheckCollisionPlayerShipVsEntities(Entity* playerShip, Entity** entity, int num, bool isEntityAutoTargetPlayer);
	void CheckCollionPlayerShipVsEntity(Entity& playerShip, Entity& entity, bool isEntityAutoTargetPlayer);
	void CheckCollisionEntities(Entity** entityA, int numA, Entity** entityB, int numB, bool spawnDebris);
	void CheckCollionEntityVsEntity(Entity& entityA, Entity& entityB, bool spawnDebris);
	bool DoEntitiesOverLap(Entity const& a, Entity const& b);

	// DELETE
	void DeleteAllGarbageEntities();
	void DeletGarbageEntities(Entity** entity, int num);
	void DeleteAll();
	void DeleteEntities(Entity** entity, int num);

	// DEBUG
	void DebugStarShip() const;
	void DebugAsteroid() const;
	void DebugBullet() const;
	void DebugBeetle() const;
	void DebugWasp() const;
	void DebugDebris() const;
	void DebugCovid() const;
	void DebugCovidBullet() const;
	void DebugBoss() const;

	// MISC
	void InitializeUIVerts();
	void InitializePlayerShip();
	void PlayButtonBlinking();
	void SetCameraOriginalPosition();
	void CreateRandomPosListForBackgroundStar();
	void MusicGame();
	void MusicAttract();
	void MusicBoss();
	void StopMusic();
};