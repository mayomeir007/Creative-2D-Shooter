#include "CArena.hpp"
#include "raymath.h"
#include "Config.hpp"

CArena::CArena(int screenWidth, int screenHeight, float margin)
    : m_bounds{margin, margin, static_cast<float>(screenWidth) - 2 * margin,
               static_cast<float>(screenHeight) - 2 * margin},
      m_margin(margin)
{
}

Rectangle CArena::Bounds() const
{
  return m_bounds;
}

Vector2 CArena::ClampCircle(Vector2 pos, float radius) const
{
  return Vector2Clamp(pos, {m_bounds.x + radius, m_bounds.y + radius},
                       {m_bounds.x + m_bounds.width - radius, m_bounds.y + m_bounds.height - radius});
}

bool CArena::CircleExitsBounds(Vector2 pos, float radius) const
{
  return !Vector2Equals(ClampCircle(pos, radius), pos);
}

void CArena::Draw() const
{
  DrawRectangleLinesEx(m_bounds, 2.0f, Config::ArenaOutlineColor);
}
