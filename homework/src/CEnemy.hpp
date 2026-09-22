#pragma once
#include <vector>
#include "CCharacter.hpp"
#include "Difficulty.hpp"

class CObstacle;

class CEnemy : public CCharacter
{
public:
  CEnemy(Vector2 spawn, Color color, Difficulty difficulty);

  void ChooseSteering(Vector2 playerPos, const std::vector<CObstacle>& obstacles, float dt);
  bool HasLineOfSight(Vector2 playerPos, const std::vector<CObstacle>& obstacles) const;
  bool IsAimedAtPlayer(Vector2 playerPos) const;
  Faction GetFaction() const override;
  void UpdateFacing(float dt, Vector2 playerPos);

private:
  // Hard-only: once blocked, walks the blocking obstacle's corners in order
  // (instead of always re-seeking the single nearest corner) until line of
  // sight to the player reopens.
  Vector2 ChooseHardCornerTarget(Vector2 playerPos, const std::vector<CObstacle>& obstacles);

  float m_turnRate;
  float m_aimTolerance;
  Difficulty m_difficulty;
  int m_activeObstacleIndex = -1;
  int m_activeCornerIndex = -1;
};
