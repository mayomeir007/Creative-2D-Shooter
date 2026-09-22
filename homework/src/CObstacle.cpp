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
