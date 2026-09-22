#include "CSpawnLayout.hpp"
#include "CArena.hpp"

Vector2 CSpawnLayout::PlayerSpawn(const CArena& arena)
{
  // TODO: Bottom-middle, pre-clamped per GAME_DESIGN.md §3.1.
  return {};
}

std::vector<Vector2> CSpawnLayout::EnemySpawns(const CArena& arena, int count)
{
  // TODO: Midpoint-of-each-stretch formula along the top edge + upper halves
  // of the sides, per GAME_DESIGN.md §3.2.
  return {};
}

Color CSpawnLayout::RandomEnemyColor(std::mt19937& rng)
{
  // TODO: Hue rejection-sampled outside [177°, 237°], fixed saturation/brightness.
  return {};
}
