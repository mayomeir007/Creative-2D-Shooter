#include "CSpawnLayout.hpp"
#include "raymath.h"
#include "CArena.hpp"
#include "Config.hpp"

Vector2 CSpawnLayout::PlayerSpawn(const CArena& arena)
{
  // Bottom-middle, already pre-clamped by construction (GAME_DESIGN.md §3.1).
  const Rectangle bounds = arena.Bounds();
  return {bounds.x + bounds.width / 2.0f, bounds.y + bounds.height - Config::CharacterRadius};
}

std::vector<Vector2> CSpawnLayout::EnemySpawns(const CArena& arena, int count)
{
  const Rectangle bounds = arena.Bounds();
  const Vector2 leftMid{bounds.x, bounds.y + bounds.height / 2.0f};
  const Vector2 topLeft{bounds.x, bounds.y};
  const Vector2 topRight{bounds.x + bounds.width, bounds.y};
  const Vector2 rightMid{bounds.x + bounds.width, bounds.y + bounds.height / 2.0f};

  const float seg0 = bounds.height / 2.0f; // leftMid -> topLeft
  const float seg1 = bounds.width;         // topLeft -> topRight
  const float seg2 = bounds.height / 2.0f; // topRight -> rightMid
  const float totalLength = seg0 + seg1 + seg2;

  std::vector<Vector2> spawns;
  spawns.reserve(count);

  for (int i = 0; i < count; ++i)
  {
    const float s = (static_cast<float>(i) + 0.5f) * (totalLength / static_cast<float>(count));

    if (s <= seg0)
    {
      spawns.push_back(Vector2Lerp(leftMid, topLeft, s / seg0));
    }
    else if (s <= seg0 + seg1)
    {
      spawns.push_back(Vector2Lerp(topLeft, topRight, (s - seg0) / seg1));
    }
    else
    {
      spawns.push_back(Vector2Lerp(topRight, rightMid, (s - seg0 - seg1) / seg2));
    }
  }

  return spawns;
}

Color CSpawnLayout::RandomEnemyColor(std::mt19937& rng)
{
  std::uniform_real_distribution<float> hueDist(0.0f, 360.0f);

  float hue;
  do
  {
    hue = hueDist(rng);
  } while (hue >= Config::PlayerHueExcludeMin && hue <= Config::PlayerHueExcludeMax);

  return ColorFromHSV(hue, Config::EnemyColorSaturation, Config::EnemyColorValue);
}
