#pragma once
#include <random>
#include <vector>
#include "raylib.h"

class CArena;

// Static-only utility. Never instantiated.
class CSpawnLayout
{
public:
  static Vector2 PlayerSpawn(const CArena& arena);
  static std::vector<Vector2> EnemySpawns(const CArena& arena, int count);
  static Color RandomEnemyColor(std::mt19937& rng);
};
