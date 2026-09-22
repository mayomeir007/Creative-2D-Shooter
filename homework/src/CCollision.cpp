#include "CCollision.hpp"

bool CCollision::CircleVsRect(Vector2 center, float radius, Rectangle rect)
{
  // TODO: Circle-vs-AABB overlap test.
  return false;
}

bool CCollision::SegmentVsRect(Vector2 a, Vector2 b, Rectangle rect)
{
  // TODO: Segment-vs-AABB intersection test.
  return false;
}

bool CCollision::CircleVsCircle(Vector2 a, float ra, Vector2 b, float rb)
{
  // TODO: Circle-vs-circle overlap test.
  return false;
}

Vector2 CCollision::NearestPointOnRect(Vector2 point, Rectangle rect)
{
  // TODO: Closest point on/in the rectangle to `point`.
  return {};
}

Vector2 CCollision::NearestCornerOfRect(Vector2 point, Rectangle rect)
{
  // TODO: Nearest of the rectangle's four corner vertices to `point`.
  return {};
}

float CCollision::RectGap(Rectangle a, Rectangle b)
{
  // TODO: sqrt(hGap^2 + vGap^2) between the two rectangles.
  return 0.0f;
}

void CCollision::ResolveCircleOverlap(Vector2& posA, float radiusA, Vector2& posB, float radiusB)
{
  // TODO: Split any overlap evenly along the line between the two centers.
}
