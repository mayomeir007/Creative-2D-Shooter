#pragma once
#include "raylib.h"

// All tunable numbers live here so no class hardcodes a magic number.
// Values are from GAME_DESIGN.md §7 unless noted otherwise.
namespace Config
{
  // Window (mirrors main.cpp)
  constexpr int ScreenWidth = 1920;
  constexpr int ScreenHeight = 1080;
  constexpr int TargetFps = 60;

  // Arena
  constexpr float ArenaMargin = 60.0f;

  // Characters
  constexpr float CharacterRadius = 32.0f;
  constexpr float MoveSpeed = 260.0f;
  constexpr int MaxHealth = 6;
  constexpr float EnemyTurnRateDegPerSec = 180.0f;
  constexpr float EnemyAimToleranceDeg = 5.0f;

  // Projectiles
  constexpr float ProjectileRadius = 5.0f;
  constexpr float ProjectileSpeed = 900.0f;
  constexpr int DamagePerHit = 1;

  // Weapons
  constexpr int UziMagazineSize = 50;
  constexpr float UziFireRate = 8.0f;
  constexpr float UziReloadTime = 3.0f;
  constexpr float PistolFireRate = 4.0f;

  // Population
  constexpr int EnemyCount = 5;
  constexpr int ObstacleCount = 8;
  constexpr float ObstacleMinSize = 40.0f;
  constexpr float ObstacleMaxSize = 260.0f;
  constexpr float ObstacleClearanceFromSpawn = 150.0f;

  // Scoring
  constexpr int ScorePerKill = 1;

  // UI — this document's own additions; not pinned down by GAME_DESIGN.md §7
  // (OOP_DESIGN.md §1), so these are placeholder layout values.
  constexpr Vector2 HealthBarOffset{-32.0f, -48.0f};
  constexpr Vector2 HealthBarSize{64.0f, 8.0f};
  constexpr float HitEffectDuration = 0.15f;
  constexpr float DeathEffectDuration = 0.3f;
}
