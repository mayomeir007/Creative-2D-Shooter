#include "CProjectile.hpp"
#include "Config.hpp"

CProjectile::CProjectile(Vector2 position, Vector2 direction, float speed, int damage, Faction owner)
    : m_position(position),
      m_velocity{direction.x * speed, direction.y * speed},
      m_radius(Config::ProjectileRadius),
      m_damage(damage),
      m_owner(owner)
{
}

void CProjectile::Update(float dt)
{
  // TODO: m_position += m_velocity * dt
}

void CProjectile::Draw() const
{
  // TODO: Fixed color per faction.
}

void CProjectile::Kill()
{
  m_alive = false;
}

bool CProjectile::IsAlive() const
{
  return m_alive;
}

Vector2 CProjectile::Position() const
{
  return m_position;
}

float CProjectile::Radius() const
{
  return m_radius;
}

Faction CProjectile::Owner() const
{
  return m_owner;
}

int CProjectile::Damage() const
{
  return m_damage;
}
