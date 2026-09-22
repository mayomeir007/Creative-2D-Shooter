#include "CArena.hpp"

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
  // TODO: Clamp pos to [M+r, size-M-r] on each axis.
  return pos;
}

bool CArena::CircleExitsBounds(Vector2 pos, float radius) const
{
  // TODO: !Vector2Equals(ClampCircle(pos, radius), pos)
  return false;
}

void CArena::Draw() const
{
  // TODO: Draw a thin border outline at m_bounds.
}
