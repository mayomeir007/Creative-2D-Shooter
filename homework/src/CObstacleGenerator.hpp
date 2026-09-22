#pragma once
#include <random>
#include <vector>
#include "raylib.h"
#include "CObstacle.hpp"

class CArena;

// Static-only utility. Never instantiated.
class CObstacleGenerator
{
public:
  static std::vector<CObstacle> Generate(int count, const CArena& arena,
                                          const std::vector<Vector2>& clearancePoints,
                                          std::mt19937& rng);
};
