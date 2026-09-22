#include "CWorld.hpp"
#include <algorithm>
#include <optional>
#include <random>
#include "CInputState.hpp"
#include "CCollision.hpp"
#include "CObstacleGenerator.hpp"
#include "CSpawnLayout.hpp"
#include "Config.hpp"

void CWorld::Init(int screenWidth, int screenHeight)
{
  m_arena = CArena(screenWidth, screenHeight, Config::ArenaMargin);
  m_player = CPlayer(CSpawnLayout::PlayerSpawn(m_arena));
  m_rng.seed(std::random_device{}());
}

void CWorld::Reset(Difficulty difficulty)
{
  m_difficulty = difficulty;
  m_player.ResetToSpawn();

  const std::vector<Vector2> enemySpawns = CSpawnLayout::EnemySpawns(m_arena, Config::EnemyCount);
  m_enemies.clear();
  m_enemies.reserve(enemySpawns.size());
  for (const Vector2& spawn : enemySpawns)
  {
    m_enemies.emplace_back(spawn, CSpawnLayout::RandomEnemyColor(m_rng), m_difficulty);
  }

  std::vector<Vector2> clearancePoints = enemySpawns;
  clearancePoints.push_back(m_player.Position());
  m_obstacles = CObstacleGenerator::Generate(Config::ObstacleCount, m_arena, clearancePoints, m_rng);

  m_projectiles.clear();
  m_effects.clear();
  m_pendingKills = 0;
}

void CWorld::Update(float dt, const CInputState& input)
{
  TickTimers(dt);
  UpdateSteeringAndInput(dt, input);
  ResolveObstacleSliding();
  ClampToArenaBounds();
  ResolvePushApart();
  UpdateFacing(dt, input);
  ResolveFiring(input);
  UpdateProjectiles(dt);
  ResolveProjectileCollisions();
  RemoveExpiredProjectiles();
  RemoveExpiredEffects();
}

void CWorld::Draw() const
{
  m_arena.Draw();

  for (const CObstacle& obstacle : m_obstacles)
  {
    obstacle.Draw();
  }
  for (const CEnemy& enemy : m_enemies)
  {
    enemy.Draw();
  }
  m_player.Draw();
  for (const CProjectile& projectile : m_projectiles)
  {
    projectile.Draw();
  }
  for (const CEffect& effect : m_effects)
  {
    effect.Draw();
  }
}

bool CWorld::PlayerIsDead() const
{
  return m_player.IsDead();
}

bool CWorld::AllEnemiesDead() const
{
  return m_enemies.empty();
}

int CWorld::EnemiesLeft() const
{
  return static_cast<int>(m_enemies.size());
}

int CWorld::ConsumeKills()
{
  const int kills = m_pendingKills;
  m_pendingKills = 0;
  return kills;
}

const CPlayer& CWorld::GetPlayer() const
{
  return m_player;
}

Difficulty CWorld::GetDifficulty() const
{
  return m_difficulty;
}

void CWorld::TickTimers(float dt)
{
  m_player.Tick(dt);
  for (CEnemy& enemy : m_enemies)
  {
    enemy.Tick(dt);
  }
}

void CWorld::UpdateSteeringAndInput(float dt, const CInputState& input)
{
  if (input.m_reloadPressed)
  {
    m_player.StartReload();
  }
  m_player.ApplyMoveInput(input.m_moveDir, dt);

  for (CEnemy& enemy : m_enemies)
  {
    enemy.ChooseSteering(m_player.Position(), m_obstacles, dt);
  }
}

void CWorld::ResolveObstacleSliding()
{
  m_player.MoveWithSlide(m_obstacles);
  for (CEnemy& enemy : m_enemies)
  {
    enemy.MoveWithSlide(m_obstacles);
  }
}

void CWorld::ClampToArenaBounds()
{
  m_player.ClampToArena(m_arena);
  for (CEnemy& enemy : m_enemies)
  {
    enemy.ClampToArena(m_arena);
  }
}

void CWorld::ResolvePushApart()
{
  for (CEnemy& enemy : m_enemies)
  {
    Vector2 playerPos = m_player.Position();
    Vector2 enemyPos = enemy.Position();
    CCollision::ResolveCircleOverlap(playerPos, m_player.Radius(), enemyPos, enemy.Radius());
    m_player.SetPosition(playerPos);
    enemy.SetPosition(enemyPos);
  }

  for (std::size_t i = 0; i < m_enemies.size(); ++i)
  {
    for (std::size_t j = i + 1; j < m_enemies.size(); ++j)
    {
      Vector2 posA = m_enemies[i].Position();
      Vector2 posB = m_enemies[j].Position();
      CCollision::ResolveCircleOverlap(posA, m_enemies[i].Radius(), posB, m_enemies[j].Radius());
      m_enemies[i].SetPosition(posA);
      m_enemies[j].SetPosition(posB);
    }
  }

  // The circle-vs-circle push above can shove a character into an obstacle
  // it wasn't touching before (e.g. one character pushed against a wall by
  // another). Push back out so nobody ends up stuck inside geometry.
  m_player.PushOutOfObstacles(m_obstacles);
  for (CEnemy& enemy : m_enemies)
  {
    enemy.PushOutOfObstacles(m_obstacles);
  }

  ClampToArenaBounds();
}

void CWorld::UpdateFacing(float dt, const CInputState& input)
{
  m_player.AimAt(input.m_mousePos);
  for (CEnemy& enemy : m_enemies)
  {
    enemy.UpdateFacing(dt, m_player.Position());
  }
}

void CWorld::ResolveFiring(const CInputState& input)
{
  if (std::optional<CProjectile> shot = m_player.TryFire(input.m_fireHeld))
  {
    m_projectiles.push_back(*shot);
  }

  for (CEnemy& enemy : m_enemies)
  {
    const bool wantsToFire =
        enemy.HasLineOfSight(m_player.Position(), m_obstacles) && enemy.IsAimedAtPlayer(m_player.Position());
    if (std::optional<CProjectile> shot = enemy.TryFire(wantsToFire))
    {
      m_projectiles.push_back(*shot);
    }
  }
}

void CWorld::UpdateProjectiles(float dt)
{
  for (CProjectile& projectile : m_projectiles)
  {
    projectile.Update(dt);
  }
}

void CWorld::ResolveProjectileCollisions()
{
  for (CProjectile& projectile : m_projectiles)
  {
    if (!projectile.IsAlive())
    {
      continue;
    }

    const Vector2 pos = projectile.Position();
    const float radius = projectile.Radius();

    if (m_arena.CircleExitsBounds(pos, radius))
    {
      projectile.Kill();
      continue;
    }

    bool blockedByObstacle = false;
    for (const CObstacle& obstacle : m_obstacles)
    {
      if (obstacle.BlocksCircle(pos, radius))
      {
        blockedByObstacle = true;
        break;
      }
    }
    if (blockedByObstacle)
    {
      projectile.Kill();
      continue;
    }

    // Enemy shots can damage the player; player shots never do.
    if (projectile.Owner() == Faction::Enemy &&
        CCollision::CircleVsCircle(pos, radius, m_player.Position(), m_player.Radius()))
    {
      if constexpr (!Config::DebugPlayerInvincible)
      {
        m_player.TakeDamage(projectile.Damage());
      }
      m_effects.push_back(CEffect::Hit(pos, m_player.GetColor()));
      if (m_player.IsDead())
      {
        m_effects.push_back(CEffect::Death(m_player.Position(), m_player.Radius(), m_player.GetColor()));
      }
      projectile.Kill();
      continue;
    }

    // Any projectile (player or enemy-owned) can damage an enemy — enemy
    // friendly fire is intentional (GAME_DESIGN.md §3.3).
    for (CEnemy& enemy : m_enemies)
    {
      if (CCollision::CircleVsCircle(pos, radius, enemy.Position(), enemy.Radius()))
      {
        enemy.TakeDamage(projectile.Damage());
        m_effects.push_back(CEffect::Hit(pos, enemy.GetColor()));
        if (enemy.IsDead())
        {
          m_effects.push_back(CEffect::Death(enemy.Position(), enemy.Radius(), enemy.GetColor()));
        }
        projectile.Kill();
        break;
      }
    }
  }

  ReapDeadEnemies();
}

void CWorld::ReapDeadEnemies()
{
  const std::size_t before = m_enemies.size();
  m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(), [](const CEnemy& enemy) { return enemy.IsDead(); }),
                   m_enemies.end());
  m_pendingKills += static_cast<int>(before - m_enemies.size());
}

void CWorld::RemoveExpiredProjectiles()
{
  m_projectiles.erase(
      std::remove_if(m_projectiles.begin(), m_projectiles.end(), [](const CProjectile& p) { return !p.IsAlive(); }),
      m_projectiles.end());
}

void CWorld::RemoveExpiredEffects()
{
  // OOP_DESIGN.md's per-step method list has no dedicated "tick effects"
  // step, so effect aging is folded into this bookkeeping pass using the
  // current frame's delta time (matches the dt CWorld::Update was called
  // with, since both ultimately come from GetFrameTime() for this frame).
  const float dt = GetFrameTime();
  for (CEffect& effect : m_effects)
  {
    effect.Update(dt);
  }

  m_effects.erase(std::remove_if(m_effects.begin(), m_effects.end(), [](const CEffect& effect) { return effect.IsExpired(); }),
                   m_effects.end());
}
