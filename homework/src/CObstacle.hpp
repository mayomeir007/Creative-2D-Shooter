#pragma once
#include "raylib.h"

class CObstacle
{
public:
  explicit CObstacle(Rectangle rect);

  Rectangle Rect() const;
  bool BlocksCircle(Vector2 center, float radius) const;
  bool BlocksSegment(Vector2 a, Vector2 b) const;
  Vector2 NearestCorner(Vector2 from) const;
  float DistanceTo(Vector2 point) const;
  float GapTo(const CObstacle& other) const;
  float HalfThickness() const;
  void Draw() const;

private:
  Rectangle m_rect;
};
