#include "CCollision.hpp"
#include <algorithm>
#include <cmath>
#include "raymath.h"

bool CCollision::CircleVsRect(Vector2 center, float radius, Rectangle rect)
{
  return CheckCollisionCircleRec(center, radius, rect);
}

bool CCollision::SegmentVsRect(Vector2 a, Vector2 b, Rectangle rect)
{
  // Liang-Barsky segment-vs-AABB clipping test.
  float t0 = 0.0f;
  float t1 = 1.0f;
  const float dx = b.x - a.x;
  const float dy = b.y - a.y;
  const float p[4] = {-dx, dx, -dy, dy};
  const float q[4] = {a.x - rect.x, rect.x + rect.width - a.x, a.y - rect.y, rect.y + rect.height - a.y};

  for (int i = 0; i < 4; ++i)
  {
    if (p[i] == 0.0f)
    {
      if (q[i] < 0.0f)
      {
        return false;
      }
    }
    else
    {
      const float t = q[i] / p[i];
      if (p[i] < 0.0f)
      {
        if (t > t1) return false;
        if (t > t0) t0 = t;
      }
      else
      {
        if (t < t0) return false;
        if (t < t1) t1 = t;
      }
    }
  }

  return true;
}

bool CCollision::CircleVsCircle(Vector2 a, float ra, Vector2 b, float rb)
{
  return CheckCollisionCircles(a, ra, b, rb);
}

Vector2 CCollision::NearestPointOnRect(Vector2 point, Rectangle rect)
{
  return {
      Clamp(point.x, rect.x, rect.x + rect.width),
      Clamp(point.y, rect.y, rect.y + rect.height),
  };
}

Vector2 CCollision::NearestCornerOfRect(Vector2 point, Rectangle rect)
{
  const float x = (point.x < rect.x + rect.width / 2.0f) ? rect.x : rect.x + rect.width;
  const float y = (point.y < rect.y + rect.height / 2.0f) ? rect.y : rect.y + rect.height;
  return {x, y};
}

float CCollision::RectGap(Rectangle a, Rectangle b)
{
  float hGap = 0.0f;
  if (a.x + a.width < b.x) hGap = b.x - (a.x + a.width);
  else if (b.x + b.width < a.x) hGap = a.x - (b.x + b.width);

  float vGap = 0.0f;
  if (a.y + a.height < b.y) vGap = b.y - (a.y + a.height);
  else if (b.y + b.height < a.y) vGap = a.y - (b.y + b.height);

  return std::sqrt(hGap * hGap + vGap * vGap);
}

void CCollision::ResolveCircleOverlap(Vector2& posA, float radiusA, Vector2& posB, float radiusB)
{
  const Vector2 delta = Vector2Subtract(posB, posA);
  const float dist = Vector2Length(delta);
  const float minDist = radiusA + radiusB;

  if (dist >= minDist || dist <= 0.0001f)
  {
    return;
  }

  const float overlap = minDist - dist;
  const Vector2 normal = Vector2Scale(delta, 1.0f / dist);
  const Vector2 correction = Vector2Scale(normal, overlap * 0.5f);
  posA = Vector2Subtract(posA, correction);
  posB = Vector2Add(posB, correction);
}

void CCollision::ResolveCircleRectOverlap(Vector2& pos, float radius, Rectangle rect)
{
  const Vector2 nearest = NearestPointOnRect(pos, rect);
  const Vector2 delta = Vector2Subtract(pos, nearest);
  const float dist = Vector2Length(delta);

  if (dist > 0.0f)
  {
    // Center is outside the rect; push out along the vector to the nearest
    // edge point if the circle still overlaps it.
    if (dist < radius)
    {
      pos = Vector2Add(nearest, Vector2Scale(delta, radius / dist));
    }
    return;
  }

  // Center is inside (or exactly on the edge of) the rect: push out along
  // whichever side requires the least movement.
  const float left = pos.x - rect.x;
  const float right = (rect.x + rect.width) - pos.x;
  const float top = pos.y - rect.y;
  const float bottom = (rect.y + rect.height) - pos.y;
  const float minPenetration = std::min({left, right, top, bottom});

  if (minPenetration == left) pos.x = rect.x - radius;
  else if (minPenetration == right) pos.x = rect.x + rect.width + radius;
  else if (minPenetration == top) pos.y = rect.y - radius;
  else pos.y = rect.y + rect.height + radius;
}
