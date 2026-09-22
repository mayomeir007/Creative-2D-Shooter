#pragma once
#include "raylib.h"

// Minimal hit/death placeholder (GAME_DESIGN.md §9: "brief flash/scale-down").
class CEffect
{
public:
  static CEffect Hit(Vector2 position, Color color);
  static CEffect Death(Vector2 position, float radius, Color color);

  void Update(float dt);
  bool IsExpired() const;
  void Draw() const;

private:
  CEffect(Vector2 position, float radius, Color color, float duration, bool isDeath);

  Vector2 m_position;
  float m_radius;
  Color m_color;
  float m_age = 0.0f;
  float m_duration;
  bool m_isDeath;
};
