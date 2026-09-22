#include "CEnemy.hpp"
#include <cmath>
#include "raymath.h"
#include "Config.hpp"
#include "CObstacle.hpp"
#include "CWeaponSpec.hpp"

namespace
{
  CWeapon MakeEnemyWeapon(Difficulty difficulty)
  {
    // Easy enemies carry a slower revolver; Medium/Hard use the baseline pistol.
    return (difficulty == Difficulty::Easy) ? CWeapon(CWeaponSpec::Revolver()) : CWeapon(CWeaponSpec::Pistol());
  }
}

CEnemy::CEnemy(Vector2 spawn, Color color, Difficulty difficulty)
    : CCharacter(spawn, Config::CharacterRadius, color, Config::MaxHealth, Config::MoveSpeed,
                 MakeEnemyWeapon(difficulty)),
      m_turnRate(Config::EnemyTurnRateDegPerSec),
      m_aimTolerance(Config::EnemyAimToleranceDeg),
      m_difficulty(difficulty)
{
  m_facingRad = PI / 2.0f; // straight down, per GAME_DESIGN.md §3.2
}

void CEnemy::ChooseSteering(Vector2 playerPos, const std::vector<CObstacle>& obstacles, float dt)
{
  Vector2 target = playerPos;

  if (!HasLineOfSight(playerPos, obstacles))
  {
    for (const CObstacle& obstacle : obstacles)
    {
      if (obstacle.BlocksSegment(m_position, playerPos))
      {
        target = obstacle.NearestCorner(m_position);
        break;
      }
    }
  }

  const Vector2 toTarget = Vector2Subtract(target, m_position);
  Vector2 direction{0, 0};
  if (Vector2LengthSqr(toTarget) > 0.0f)
  {
    direction = Vector2Normalize(toTarget);
  }

  m_pendingMove = Vector2Scale(direction, m_moveSpeed * dt);
}

bool CEnemy::HasLineOfSight(Vector2 playerPos, const std::vector<CObstacle>& obstacles) const
{
  for (const CObstacle& obstacle : obstacles)
  {
    if (obstacle.BlocksSegment(m_position, playerPos))
    {
      return false;
    }
  }
  return true;
}

bool CEnemy::IsAimedAtPlayer(Vector2 playerPos) const
{
  const Vector2 toPlayer = Vector2Subtract(playerPos, m_position);
  if (Vector2LengthSqr(toPlayer) <= 0.0f)
  {
    return true;
  }

  const float angleDiff = Vector2Angle(FacingDir(), Vector2Normalize(toPlayer));
  return std::fabs(angleDiff) * RAD2DEG <= m_aimTolerance;
}

Faction CEnemy::GetFaction() const
{
  return Faction::Enemy;
}

void CEnemy::UpdateFacing(float dt, Vector2 playerPos)
{
  const Vector2 toPlayer = Vector2Subtract(playerPos, m_position);
  if (Vector2LengthSqr(toPlayer) <= 0.0f)
  {
    return;
  }

  const Vector2 targetDir = Vector2Normalize(toPlayer);
  const float angleDiff = Vector2Angle(FacingDir(), targetDir);
  const float maxStep = (m_turnRate * DEG2RAD) * dt;
  const float step = Clamp(angleDiff, -maxStep, maxStep);
  SetFacingRad(m_facingRad + step);
}
