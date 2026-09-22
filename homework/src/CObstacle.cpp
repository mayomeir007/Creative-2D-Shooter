#include "CObstacle.hpp"
#include <algorithm>
#include "raymath.h"
#include "CCollision.hpp"
#include "Config.hpp"

CObstacle::CObstacle(Rectangle rect)
    : m_rect(rect)
{
}

Rectangle CObstacle::Rect() const
{
  return m_rect;
}

bool CObstacle::BlocksCircle(Vector2 center, float radius) const
{
  return CCollision::CircleVsRect(center, radius, m_rect);
}

bool CObstacle::BlocksSegment(Vector2 a, Vector2 b) const
{
  return CCollision::SegmentVsRect(a, b, m_rect);
}

Vector2 CObstacle::NearestCorner(Vector2 from) const
{
  return CCollision::NearestCornerOfRect(from, m_rect);
}

Vector2 CObstacle::CornerAt(int index) const
{
  switch (index & 3)
  {
    case 0: return {m_rect.x, m_rect.y};
    case 1: return {m_rect.x + m_rect.width, m_rect.y};
    case 2: return {m_rect.x + m_rect.width, m_rect.y + m_rect.height};
    default: return {m_rect.x, m_rect.y + m_rect.height};
  }
}

int CObstacle::NearestCornerIndex(Vector2 from) const
{
  int best = 0;
  float bestDistSqr = Vector2DistanceSqr(from, CornerAt(0));
  for (int i = 1; i < 4; ++i)
  {
    const float distSqr = Vector2DistanceSqr(from, CornerAt(i));
    if (distSqr < bestDistSqr)
    {
      bestDistSqr = distSqr;
      best = i;
    }
  }
  return best;
}

Vector2 CObstacle::CornerApproachPoint(int index, float clearance) const
{
  // A circle can never actually reach a corner's exact point — its own
  // radius stops it at a tangent distance first. Offset the target outward
  // from the corner by `clearance` along EACH axis independently (not along
  // the diagonal to the rect's center) so it's guaranteed clear of both
  // edges meeting at that corner, regardless of the rect's aspect ratio —
  // a diagonal-to-center offset goes shallow on a long, narrow obstacle and
  // barely clears the short edge, putting the target right back in a
  // tangent-lock against that edge instead of the corner point.
  switch (index & 3)
  {
    case 0: return {m_rect.x - clearance, m_rect.y - clearance};                                // top-left
    case 1: return {m_rect.x + m_rect.width + clearance, m_rect.y - clearance};                 // top-right
    case 2: return {m_rect.x + m_rect.width + clearance, m_rect.y + m_rect.height + clearance}; // bottom-right
    default: return {m_rect.x - clearance, m_rect.y + m_rect.height + clearance};               // bottom-left
  }
}

float CObstacle::DistanceTo(Vector2 point) const
{
  return Vector2Distance(point, CCollision::NearestPointOnRect(point, m_rect));
}

float CObstacle::GapTo(const CObstacle& other) const
{
  return CCollision::RectGap(m_rect, other.m_rect);
}

float CObstacle::HalfThickness() const
{
  return std::min(m_rect.width, m_rect.height) / 2.0f;
}

void CObstacle::Draw() const
{
  DrawRectangleRec(m_rect, Config::ObstacleFillColor);
  DrawRectangleLinesEx(m_rect, 2.0f, Config::ObstacleOutlineColor);
}
