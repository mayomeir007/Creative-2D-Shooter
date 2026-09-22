#pragma once
#include <random>
#include <vector>
#include "raylib.h"
#include "CArena.hpp"
#include "CPlayer.hpp"
#include "CEnemy.hpp"
#include "CObstacle.hpp"
#include "CProjectile.hpp"
#include "CEffect.hpp"

class CInputState;

// Everything that exists only while playing: arena, player, enemies,
// obstacles, projectiles, effects. Update() runs the 12-step per-frame
// order from GAME_DESIGN.md §5 via the private methods below, in order.
class CWorld
{
public:
  void Init(int screenWidth, int screenHeight);
  void Reset();
  void Update(float dt, const CInputState& input);
  void Draw() const;
  bool PlayerIsDead() const;
  bool AllEnemiesDead() const;
  int EnemiesLeft() const;
  int ConsumeKills();
  const CPlayer& GetPlayer() const;

private:
  void TickTimers(float dt);
  void UpdateSteeringAndInput(float dt, const CInputState& input);
  void ResolveObstacleSliding();
  void ClampToArenaBounds();
  void ResolvePushApart();
  void UpdateFacing(float dt, const CInputState& input);
  void ResolveFiring(const CInputState& input);
  void UpdateProjectiles(float dt);
  void ResolveProjectileCollisions();
  void ReapDeadEnemies();
  void RemoveExpiredProjectiles();
  void RemoveExpiredEffects();

  CArena m_arena{0, 0, 0.0f};
  CPlayer m_player{Vector2{0, 0}};
  std::vector<CEnemy> m_enemies;
  std::vector<CObstacle> m_obstacles;
  std::vector<CProjectile> m_projectiles;
  std::vector<CEffect> m_effects;
  std::mt19937 m_rng;
  int m_pendingKills = 0;
};
