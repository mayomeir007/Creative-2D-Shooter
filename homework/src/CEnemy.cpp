#include "CEnemy.hpp"
#include <cmath>
#include <limits>
#include "raymath.h"
#include "Config.hpp"
#include "CObstacle.hpp"
#include "CWeaponSpec.hpp"

namespace
{
  CWeapon MakeEnemyWeapon(Difficulty difficulty)
  {
    // Easy enemies carry a slower revolver; every other difficulty uses the baseline pistol.
    return (difficulty == Difficulty::Easy) ? CWeapon(CWeaponSpec::Revolver()) : CWeapon(CWeaponSpec::Pistol());
  }

  int FindBlockingObstacleIndex(Vector2 from, Vector2 to, const std::vector<CObstacle>& obstacles)
  {
    for (std::size_t i = 0; i < obstacles.size(); ++i)
    {
      if (obstacles[i].BlocksSegment(from, to))
      {
        return static_cast<int>(i);
      }
    }
    return -1;
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

  if (m_seekingCover && Vector2Distance(m_position, m_coverTarget) <= Config::SeekCoverReachDistance)
  {
    m_seekingCover = false;
  }

  if (m_seekingCover)
  {
    target = m_coverTarget;
  }
  else if (HasLineOfSight(playerPos, obstacles))
  {
    m_activeObstacleIndex = -1;
    m_activeCornerIndex = -1;
  }
  else if (m_difficulty == Difficulty::VeryHard)
  {
    target = ChooseCornerWalkTarget(playerPos, obstacles);
  }
  else
  {
    const int blockingIndex = FindBlockingObstacleIndex(m_position, playerPos, obstacles);
    if (blockingIndex >= 0)
    {
      target = obstacles[blockingIndex].NearestCorner(m_position);
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

Vector2 CEnemy::ChooseCornerWalkTarget(Vector2 playerPos, const std::vector<CObstacle>& obstacles)
{
  const int blockingIndex = FindBlockingObstacleIndex(m_position, playerPos, obstacles);
  if (blockingIndex < 0)
  {
    return playerPos; // HasLineOfSight already said blocked; stay safe if that ever disagrees.
  }

  if (blockingIndex != m_activeObstacleIndex)
  {
    // Newly blocked, or a different obstacle is now in the way — restart the
    // walk from whichever corner of it is nearest right now.
    m_activeObstacleIndex = blockingIndex;
    m_activeCornerIndex = obstacles[blockingIndex].NearestCornerIndex(m_position);
  }
  else if (Vector2Distance(m_position, obstacles[m_activeObstacleIndex].CornerApproachPoint(
                                            m_activeCornerIndex, Config::CornerClearance)) <=
           Config::CornerReachDistance)
  {
    // Reached this corner and the player is still obstructed (we're still in
    // this branch) — move on to the next corner around the same obstacle.
    m_activeCornerIndex = (m_activeCornerIndex + 1) % 4;
  }

  return obstacles[m_activeObstacleIndex].CornerApproachPoint(m_activeCornerIndex, Config::CornerClearance);
}

void CEnemy::SeekCover(Vector2 playerPos, const std::vector<CObstacle>& obstacles, Rectangle arenaBounds)
{
  if (!Config::EnableSeekCoverAI || (m_difficulty != Difficulty::Hard && m_difficulty != Difficulty::VeryHard))
  {
    return; // seek-cover AI is a Hard/VeryHard behavior.
  }

  if (m_maxHealth <= 1)
  {
    return; // formula divides by (MaxHealth - 1); nothing to scale with a single hit point.
  }

  // 2r = d * (MaxHealth - health) / (MaxHealth - 1), so r is half of that.
  const float diagonal = std::sqrt(arenaBounds.width * arenaBounds.width + arenaBounds.height * arenaBounds.height);
  const float healthLost = static_cast<float>(m_maxHealth - m_health);
  const float searchRadius = diagonal * healthLost / static_cast<float>(2 * (m_maxHealth - 1));

  if (const std::optional<Vector2> cover = FindCoverPoint(playerPos, obstacles, arenaBounds, searchRadius))
  {
    m_seekingCover = true;
    m_coverTarget = *cover;
  }
}

std::optional<Vector2> CEnemy::FindCoverPoint(Vector2 playerPos, const std::vector<CObstacle>& obstacles,
                                               Rectangle arenaBounds, float searchRadius) const
{
  std::optional<Vector2> best;
  float bestDistSqr = std::numeric_limits<float>::max();

  for (int ring = 1; ring <= Config::SeekCoverRadialSteps; ++ring)
  {
    const float ringRadius = searchRadius * static_cast<float>(ring) / static_cast<float>(Config::SeekCoverRadialSteps);

    for (int step = 0; step < Config::SeekCoverAngularSteps; ++step)
    {
      const float angle = (2.0f * PI * static_cast<float>(step)) / static_cast<float>(Config::SeekCoverAngularSteps);
      const Vector2 candidate{m_position.x + ringRadius * std::cos(angle), m_position.y + ringRadius * std::sin(angle)};

      if (candidate.x - m_radius < arenaBounds.x || candidate.x + m_radius > arenaBounds.x + arenaBounds.width ||
          candidate.y - m_radius < arenaBounds.y || candidate.y + m_radius > arenaBounds.y + arenaBounds.height)
      {
        continue; // outside the arena
      }

      bool reachable = true;
      bool obstructed = false;
      for (const CObstacle& obstacle : obstacles)
      {
        if (obstacle.BlocksCircle(candidate, m_radius))
        {
          reachable = false;
          break;
        }
        if (obstacle.BlocksSegment(candidate, playerPos))
        {
          obstructed = true;
        }
      }
      if (!reachable || !obstructed)
      {
        continue;
      }

      const float distSqr = Vector2DistanceSqr(m_position, candidate);
      if (distSqr < bestDistSqr)
      {
        bestDistSqr = distSqr;
        best = candidate;
      }
    }
  }

  return best;
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
