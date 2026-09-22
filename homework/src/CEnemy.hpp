#pragma once
#include <vector>
#include "CCharacter.hpp"

class CObstacle;

class CEnemy : public CCharacter
{
public:
  CEnemy(Vector2 spawn, Color color);

  void ChooseSteering(Vector2 playerPos, const std::vector<CObstacle>& obstacles, float dt);
  bool HasLineOfSight(Vector2 playerPos, const std::vector<CObstacle>& obstacles) const;
  bool IsAimedAtPlayer(Vector2 playerPos) const;
  Faction GetFaction() const override;
  void UpdateFacing(float dt, Vector2 playerPos);

private:
  float m_turnRate;
  float m_aimTolerance;
};
