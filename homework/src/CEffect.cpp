#include "CEffect.hpp"
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
  // TODO: Advance m_age.
}

bool CEffect::IsExpired() const
{
  // TODO: m_age >= m_duration
  return false;
}

void CEffect::Draw() const
{
  // TODO: Flash (Hit) or shrinking circle (Death), based on m_isDeath.
}
