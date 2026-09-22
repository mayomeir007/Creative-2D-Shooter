#include "CWorld.hpp"
#include "CInputState.hpp"

void CWorld::Init(int screenWidth, int screenHeight)
{
  // TODO: m_arena = CArena(screenWidth, screenHeight, Config::ArenaMargin);
  // m_player = CPlayer(CSpawnLayout::PlayerSpawn(m_arena)); seed m_rng.
}

void CWorld::Reset()
{
  // TODO: m_player.ResetToSpawn(); rebuild enemies/obstacles; clear
  // projectiles/effects; m_pendingKills = 0.
}

void CWorld::Update(float dt, const CInputState& input)
{
  // TODO: Run the private steps below in order (see OOP_DESIGN.md §12).
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
  // TODO: Arena outline, obstacles, enemies, player, projectiles, effects,
  // in back-to-front order.
}

bool CWorld::PlayerIsDead() const
{
  // TODO: m_player.IsDead()
  return false;
}

bool CWorld::AllEnemiesDead() const
{
  // TODO: m_enemies.empty()
  return false;
}

int CWorld::EnemiesLeft() const
{
  // TODO: m_enemies.size()
  return 0;
}

int CWorld::ConsumeKills()
{
  // TODO: Return m_pendingKills and reset it to 0.
  return 0;
}

const CPlayer& CWorld::GetPlayer() const
{
  return m_player;
}

void CWorld::TickTimers(float dt)
{
  // TODO: Tick(dt) on the player and on every enemy.
}

void CWorld::UpdateSteeringAndInput(float dt, const CInputState& input)
{
  // TODO: Reload/ApplyMoveInput for the player; ChooseSteering for every enemy.
}

void CWorld::ResolveObstacleSliding()
{
  // TODO: MoveWithSlide(m_obstacles) for the player, then for each enemy.
}

void CWorld::ClampToArenaBounds()
{
  // TODO: ClampToArena(m_arena) for the player and every enemy.
}

void CWorld::ResolvePushApart()
{
  // TODO: Every player-vs-enemy and enemy-vs-enemy pair: ResolveCircleOverlap
  // + write back via SetPosition + re-clamp to the arena.
}

void CWorld::UpdateFacing(float dt, const CInputState& input)
{
  // TODO: m_player.AimAt(input.m_mousePos); each enemy's UpdateFacing(dt, playerPos).
}

void CWorld::ResolveFiring(const CInputState& input)
{
  // TODO: Player fires on input.m_fireHeld; each enemy fires on
  // HasLineOfSight + IsAimedAtPlayer + cooldown. Append to m_projectiles.
}

void CWorld::UpdateProjectiles(float dt)
{
  // TODO: Update(dt) on every projectile.
}

void CWorld::ResolveProjectileCollisions()
{
  // TODO: Projectile vs obstacle/arena/circles; damage; push CEffect::Hit/Death;
  // defer removal to ReapDeadEnemies.
}

void CWorld::ReapDeadEnemies()
{
  // TODO: Remove dead enemies from m_enemies; increment m_pendingKills once
  // per removed enemy.
}

void CWorld::RemoveExpiredProjectiles()
{
  // TODO: Sweep projectiles with IsAlive() == false.
}

void CWorld::RemoveExpiredEffects()
{
  // TODO: Sweep effects with IsExpired() == true.
}
