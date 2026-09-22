#include "CEnemy.hpp"
#include "Config.hpp"
#include "CWeaponSpec.hpp"

CEnemy::CEnemy(Vector2 spawn, Color color)
    : CCharacter(spawn, Config::CharacterRadius, color, Config::MaxHealth, Config::MoveSpeed,
                 CWeapon(CWeaponSpec::Pistol())),
      m_turnRate(Config::EnemyTurnRateDegPerSec),
      m_aimTolerance(Config::EnemyAimToleranceDeg)
{
  m_facingRad = PI / 2.0f; // straight down, per GAME_DESIGN.md §3.2
}

void CEnemy::ChooseSteering(Vector2 playerPos, const std::vector<CObstacle>& obstacles, float dt)
{
  // TODO: LOS-gated steer toward the player, else toward the nearest corner
  // of the blocking obstacle; write m_pendingMove = direction * m_moveSpeed * dt.
}

bool CEnemy::HasLineOfSight(Vector2 playerPos, const std::vector<CObstacle>& obstacles) const
{
  // TODO: Segment-vs-every-obstacle test, freshly computed on every call.
  return false;
}

bool CEnemy::IsAimedAtPlayer(Vector2 playerPos) const
{
  // TODO: True if FacingDir() is within m_aimTolerance of the direction to playerPos.
  return false;
}

Faction CEnemy::GetFaction() const
{
  return Faction::Enemy;
}

void CEnemy::UpdateFacing(float dt, Vector2 playerPos)
{
  // TODO: Rotate toward the player by at most m_turnRate * dt, shorter
  // angular path, no overshoot.
}
