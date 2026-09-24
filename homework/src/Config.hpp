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
  constexpr float PlayerMoveSpeed = 260.0f;
  constexpr float EnemyMoveSpeed = 260.0f;
  constexpr int MaxHealth = 6;
  constexpr float EnemyTurnRateDegPerSec = 180.0f;
  constexpr float EnemyAimToleranceDeg = 5.0f;

  // Hard-difficulty corner-following steering. An enemy's own collision
  // circle can never actually reach a corner's exact point — it stops at a
  // tangent distance of ~CharacterRadius first, and once tangent there, any
  // further move (toward going around the corner) gets rejected by the
  // axis-separated slide too, freezing it in place. So the enemy instead
  // aims for a point offset outward from the corner by CornerClearance
  // (along the diagonal from the obstacle's center) — genuine free space to
  // route through. CornerReachDistance is how close (px) to that offset
  // point counts as "reached" before advancing to the next corner.
  constexpr float CornerClearance = CharacterRadius + 8.0f;
  constexpr float CornerReachDistance = 12.0f;

  // Projectiles
  constexpr float ProjectileRadius = 5.0f;
  constexpr int DamagePerHit = 1;

  // Weapons
  constexpr int UziMagazineSize = 50;
  constexpr float UziFireRate = 8.0f;
  constexpr float UziReloadTime = 3.0f;
  constexpr float UziProjectileSpeed = 900.0f;
  constexpr float PistolFireRate = 4.0f;
  constexpr float PistolProjectileSpeed = UziProjectileSpeed * 0.8f;

  // Easy-difficulty enemy weapon: a slower single-action sidearm, fire rate
  // and projectile speed are both fixed ratios of the player's Uzi.
  constexpr float RevolverFireRate = UziFireRate * 0.25f;
  constexpr float RevolverProjectileSpeed = UziProjectileSpeed * 0.6f;
  constexpr Color RevolverColor{150, 150, 158, 255}; // gray

  // Population
  constexpr int EnemyCount = 5;
  constexpr int ObstacleCount = 8;
  constexpr float ObstacleMinSize = 40.0f;
  constexpr float ObstacleMaxSize = 260.0f;
  constexpr float ObstacleClearanceFromSpawn = 150.0f;

  // A gap narrower than the player's diameter isn't a corridor at all — the
  // player is blocked flush against the obstacle's face well before reaching
  // the wall, same as any ordinary obstacle edge, so it's left alone. Only a
  // gap from the diameter up to a small buffer above it (for the
  // axis-separated slide's imprecision at that width) is wide enough to
  // partially enter but too narrow to reliably pass through — that's the
  // only range rejected during generation.
  constexpr float StickingGapMin = CharacterRadius * 2.0f; // player's diameter
  constexpr float StickingGapMax = StickingGapMin + 32.0f;

  // Scoring
  constexpr int ScorePerKill = 1;

  // Debug/testing only — set back to false before shipping. When true, the
  // player takes no damage from enemy projectiles (GameOver becomes
  // unreachable via combat).
  constexpr bool DebugPlayerInvincible = false;

  // Seek-cover AI toggle. When true, an enemy that takes damage searches a
  // disk of radius r around its current position for the nearest point whose
  // line of sight to the player is obstructed, then walks there before
  // resuming its normal difficulty-driven behavior. r grows as the enemy's
  // remaining health drops: 2r = d * (MaxHealth - health) / (MaxHealth - 1),
  // where d is the arena's diagonal — a barely-hurt enemy only checks a
  // small pocket nearby, a nearly-dead one searches out to the full
  // diagonal.
  constexpr bool EnableSeekCoverAI = true;
  constexpr int SeekCoverRadialSteps = 10;
  constexpr int SeekCoverAngularSteps = 24;
  constexpr float SeekCoverReachDistance = 12.0f;

  // Enemy color randomization (GAME_DESIGN.md §3.2): hue excluded near the
  // player's blue (~207°) so no enemy can be mistaken for the player.
  constexpr float EnemyColorSaturation = 0.55f;
  constexpr float EnemyColorValue = 1.0f;
  constexpr float PlayerHueExcludeMin = 177.0f;
  constexpr float PlayerHueExcludeMax = 237.0f;

  // UI — this document's own additions; not pinned down by GAME_DESIGN.md §7
  // (OOP_DESIGN.md §1), so these are placeholder layout/color values.
  constexpr Vector2 HealthBarOffset{-32.0f, -48.0f};
  constexpr Vector2 HealthBarSize{64.0f, 8.0f};
  constexpr float HitEffectDuration = 0.15f;
  constexpr float DeathEffectDuration = 0.3f;

  constexpr Color PlayerColor{77, 171, 247, 255}; // GAME_DESIGN.md §3.1
  constexpr Color UziProjectileColor{120, 215, 255, 255};
  constexpr Color PistolProjectileColor{255, 120, 120, 255};
  constexpr Color RevolverProjectileColor{255, 176, 176, 255}; // PistolProjectileColor, tinted closer to white
  constexpr Color GunColor{40, 44, 52, 255};
  constexpr Color EyeColor{255, 255, 255, 255};
  constexpr Color PupilColor{40, 44, 52, 255};
  constexpr float GunLength = 26.0f;
  constexpr float GunWidth = 10.0f;
  constexpr float EyeRadius = 4.0f;
  constexpr float EyeSpacing = 16.0f;

  constexpr Color HealthBarBackgroundColor{40, 44, 52, 220};
  constexpr Color HealthBarFillColor{102, 209, 122, 255};

  constexpr Color ObstacleFillColor{58, 66, 78, 255};
  constexpr Color ObstacleOutlineColor{92, 104, 120, 255};

  constexpr Color ArenaOutlineColor{92, 104, 120, 255};

  constexpr Color ButtonColor{58, 66, 78, 255};
  constexpr Color ButtonOutlineColor{120, 215, 255, 255};
  constexpr Color ButtonSelectedColor{40, 90, 120, 255};
}
