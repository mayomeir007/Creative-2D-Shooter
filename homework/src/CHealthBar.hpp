#pragma once
#include "raylib.h"
#include "Config.hpp"

// Small, reusable rendering component. Only CPlayer ever owns one.
class CHealthBar
{
public:
  void Draw(Vector2 ownerPosition, float healthFraction) const;

private:
  Vector2 m_offset = Config::HealthBarOffset;
  Vector2 m_size = Config::HealthBarSize;
};
