#pragma once
#include "raylib.h"

// Static-only geometry utility. Never instantiated.
class CCollision
{
public:
  static bool CircleVsRect(Vector2 center, float radius, Rectangle rect);
  static bool SegmentVsRect(Vector2 a, Vector2 b, Rectangle rect);
  static bool CircleVsCircle(Vector2 a, float ra, Vector2 b, float rb);
  static Vector2 NearestPointOnRect(Vector2 point, Rectangle rect);
  static Vector2 NearestCornerOfRect(Vector2 point, Rectangle rect);
  static float RectGap(Rectangle a, Rectangle b);
  static void ResolveCircleOverlap(Vector2& posA, float radiusA, Vector2& posB, float radiusB);
  static void ResolveCircleRectOverlap(Vector2& pos, float radius, Rectangle rect);
};
