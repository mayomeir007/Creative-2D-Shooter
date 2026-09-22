#include "CEffect.hpp"
#include "raymath.h"
#include "Config.hpp"

CEffect::CEffect(Vector2 position, float radius, Color color, float duration, bool isDeath)
    : m_position(position), m_radius(radius), m_color(color), m_duration(duration), m_isDeath(isDeath)
{
}

CEffect CEffect::Hit(Vector2 position, Color color)
{
  return CEffect(position, 0.0f, color, Config::HitEffectDuration, false);
}

CEffect CEffect::Death(Vector2 position, float radius, Color color)
{
  return CEffect(position, radius, color, Config::DeathEffectDuration, true);
}

void CEffect::Update(float dt)
{
  m_age += dt;
}

bool CEffect::IsExpired() const
{
  return m_age >= m_duration;
}

void CEffect::Draw() const
{
  const float t = m_duration > 0.0f ? Clamp(m_age / m_duration, 0.0f, 1.0f) : 1.0f;

  if (m_isDeath)
  {
    const float radius = m_radius * (1.0f - t);
    if (radius > 0.0f)
    {
      DrawCircleV(m_position, radius, m_color);
    }
  }
  else
  {
    Color flashColor = m_color;
    flashColor.a = static_cast<unsigned char>(255.0f * (1.0f - t));
    DrawCircleV(m_position, Config::CharacterRadius * 0.5f, flashColor);
  }
}
