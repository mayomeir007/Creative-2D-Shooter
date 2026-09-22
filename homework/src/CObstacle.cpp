#include "CObstacle.hpp"
#include "CCollision.hpp"

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
  // TODO: CCollision::CircleVsRect(center, radius, m_rect)
  return false;
}

bool CObstacle::BlocksSegment(Vector2 a, Vector2 b) const
{
  // TODO: CCollision::SegmentVsRect(a, b, m_rect)
  return false;
}

Vector2 CObstacle::NearestCorner(Vector2 from) const
{
  // TODO: CCollision::NearestCornerOfRect(from, m_rect)
  return {};
}

float CObstacle::DistanceTo(Vector2 point) const
{
  // TODO: length(point - CCollision::NearestPointOnRect(point, m_rect))
  return 0.0f;
}

float CObstacle::GapTo(const CObstacle& other) const
{
  // TODO: CCollision::RectGap(m_rect, other.m_rect)
  return 0.0f;
}

float CObstacle::HalfThickness() const
{
  // TODO: min(width, height) / 2
  return 0.0f;
}

void CObstacle::Draw() const
{
  // TODO: Filled rect + lighter outline.
}
