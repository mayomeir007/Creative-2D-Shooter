#pragma once
#include <optional>
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

  // Called when this enemy takes damage. Hard/VeryHard only (and only when
  // Config::EnableSeekCoverAI is set): searches for the nearest point within
  // the seek-cover disk (radius set by current health) whose line of sight
  // to the player is blocked, and if one exists, has the enemy head there
  // before resuming its usual steering.
  void SeekCover(Vector2 playerPos, const std::vector<CObstacle>& obstacles, Rectangle arenaBounds);

private:
  // VeryHard-only: once blocked, walks the blocking obstacle's corners in
  // order (instead of always re-seeking the single nearest corner) until
  // line of sight to the player reopens.
  Vector2 ChooseCornerWalkTarget(Vector2 playerPos, const std::vector<CObstacle>& obstacles);

  std::optional<Vector2> FindCoverPoint(Vector2 playerPos, const std::vector<CObstacle>& obstacles,
                                         Rectangle arenaBounds, float searchRadius) const;

  float m_turnRate;
  float m_aimTolerance;
  Difficulty m_difficulty;
  int m_activeObstacleIndex = -1;
  int m_activeCornerIndex = -1;
  bool m_seekingCover = false;
  Vector2 m_coverTarget{0, 0};
};
