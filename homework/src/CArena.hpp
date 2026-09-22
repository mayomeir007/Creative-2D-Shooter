#pragma once
#include "raylib.h"

class CArena
{
public:
  CArena(int screenWidth, int screenHeight, float margin);

  Rectangle Bounds() const;
  Vector2 ClampCircle(Vector2 pos, float radius) const;
  bool CircleExitsBounds(Vector2 pos, float radius) const;
  void Draw() const;

private:
  Rectangle m_bounds;
  float m_margin;
};
