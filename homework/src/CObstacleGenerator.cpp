#include "CObstacleGenerator.hpp"
#include "CArena.hpp"

std::vector<CObstacle> CObstacleGenerator::Generate(int count, const CArena& arena,
                                                      const std::vector<Vector2>& clearancePoints,
                                                      std::mt19937& rng)
{
  // TODO: Rejection-sampling loop per GAME_DESIGN.md §3.4, capped at ~200
  // attempts per obstacle; skip an obstacle that can't find a valid spot.
  return {};
}
