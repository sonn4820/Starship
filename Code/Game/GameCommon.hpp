#pragma once
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/Time.hpp"
#include "Game/SimpleTriangleFont.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Renderer/Camera.hpp"
#include <math.h>


constexpr int MAX_LEVEL = 5;

constexpr float MAX_SHAKE = 10.f;
constexpr float SHAKE_REDUCTION_PER_SEC = 7.f;

constexpr int NUM_STARTING_ASTEROIDS = 6;
constexpr int MAX_ASTEROIDS = 12;
constexpr int MAX_BULLETS = 60;
constexpr int MAX_BEETLES = 5;
constexpr int MAX_WASPS = 4;
constexpr int MAX_COVID = 6;
constexpr int MAX_DEBRIS = 80;

constexpr float WORLD_SIZE_X = 200.f;
constexpr float WORLD_SIZE_Y = 100.f;
constexpr float WORLD_CENTER_X = WORLD_SIZE_X / 2.f;
constexpr float WORLD_CENTER_Y = WORLD_SIZE_Y / 2.f;
constexpr float DELAY_SHIP_DEATH_TIMER = 3.5f;

constexpr float ASTEROID_SPEED = 10.f;
constexpr int NUM_ASTEROID_SIDES = 16;
constexpr int NUM_ASTEROID_TRIS = NUM_ASTEROID_SIDES;
constexpr int NUM_ASTEROID_VERTS = 3* NUM_ASTEROID_TRIS;
constexpr float ASTEROID_PHYSICS_RADIUS = 1.6f;
constexpr float ASTEROID_COSMETIC_RADIUS = 2.0f;

constexpr float DEBRIS_SPEED = 15.f;
constexpr float DEBRIS_FADE_SPEED = 20.f;
constexpr int NUM_DEBRIS_SIDES = 16;
constexpr int NUM_DEBRIS_TRIS = NUM_DEBRIS_SIDES;
constexpr int NUM_DEBRIS_VERTS = 3 * NUM_DEBRIS_TRIS;

constexpr int NUM_BULLET_VERTS = 6;
constexpr float BULLET_LIFETIME_SECONDS = 2.0f;
constexpr float BULLET_SPEED = 50.f;
constexpr float BULLET_PHYSICS_RADIUS = 0.5f;
constexpr float BULLET_COSMETIC_RADIUS = 2.0f;

constexpr float COVID_BULLET_SPEED = 14.f;
constexpr int NUM_COVID_BULLET_SIDES = 24;
constexpr int NUM_COVID_BULLET_TRIS = NUM_COVID_BULLET_SIDES;
constexpr int NUM_COVID_BULLET_VERTS = 3 * NUM_COVID_BULLET_TRIS;
constexpr float COVID_BULLET_PHYSICS_RADIUS = 1.7f;
constexpr float COVID_BULLET_COSMETIC_RADIUS = 2.0f;

constexpr int NUM_PLAYERSHIP_VERTS = 15;
constexpr float PLAYER_SHIP_ACCELERATION = 40.f;
constexpr float PLAYER_SHIP_TURN_SPEED = 300.f;
constexpr float PLAYER_SHIP_PHYSICS_RADIUS = 1.75f;
constexpr float PLAYER_SHIP_COSMETIC_RADIUS = 2.25f;
constexpr int PLAYER_SHIP_EXTRA_LIVES = 5;
constexpr float PLAYER_SHIP_INVINCIBLE_TIMER = 2.f;

constexpr int NUM_BEETLE_VERTS = 18;
constexpr float BEETLE_PHYSICS_RADIUS = 1.7f;
constexpr float BEETLE_COSMETIC_RADIUS = 2.2f;
constexpr float BEETLE_SPEED = 1000.f;

constexpr int NUM_WASP_VERTS = 24;
constexpr float WASP_PHYSICS_RADIUS = 1.9f;
constexpr float WASP_COSMETIC_RADIUS = 2.3f;
constexpr float WASP_SPEED = 7.f;

constexpr float COVID_SPEED = 4.f;
constexpr int NUM_COVID_SIDES = 27;
constexpr int NUM_COVID_TRIS = NUM_COVID_SIDES;
constexpr int NUM_COVID_VERTS = 3 * NUM_COVID_TRIS;
constexpr float COVID_PHYSICS_RADIUS = 1.85f;
constexpr float COVID_COSMETIC_RADIUS = 1.9f;

constexpr int BOSS_LEVEL = 4;
constexpr int BOSS_OUTSIDE_RING_STAGE_HEALTH = 54;
constexpr int BOSS_INSIDE_RING_STAGE_HEALTH = (int)(BOSS_OUTSIDE_RING_STAGE_HEALTH * 0.667f);
constexpr int BOSS_NO_RING_STAGE_HEALTH = (int)(BOSS_OUTSIDE_RING_STAGE_HEALTH * 0.334f);
constexpr float BOSS_OUTSIDE_RING_LENGTH = 8.f;
constexpr float BOSS_INSIDE_RING_LENGTH = 5.f;
constexpr float BOSS_NO_RING_LENGTH = 2.f;
constexpr float BOSS_OUTSIDE_RING_SPEED = 25.f;
constexpr float BOSS_INSIDE_RING_SPEED = 20.f;
constexpr float BOSS_NO_RING_SPEED = 15.f;
constexpr float BOSS_OUTSIDE_RING_RANDOM_MOVING_TIMER = 13.f;
constexpr float BOSS_INSIDE_RING_RANDOM_MOVING_TIMER = 10.f;
constexpr float BOSS_NO_RING_RANDOM_MOVING_TIMER = 7.f;
constexpr float BOSS_COSMETIC_RADIUS = 8.f;

void DrawDebugRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color);
void DrawDebugLine(Vec2 const& startPos, Vec2 const& endPos, float thickness, Rgba8 const& color);
void DrawDebugGlow(Vec2 const& center, float radius, Rgba8 const& colorInside, Rgba8 const& colorOutside);