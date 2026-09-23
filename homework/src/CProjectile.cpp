#include "CProjectile.hpp"
#include "raymath.h"
#include "Config.hpp"

CProjectile::CProjectile(Vector2 position, Vector2 direction, float speed, int damage, Faction owner, Color color)
    : m_position(position),
      m_velocity{direction.x * speed, direction.y * speed},
      m_radius(Config::ProjectileRadius),
      m_damage(damage),
      m_owner(owner),
      m_color(color)
{
}

void CProjectile::Update(float dt)
{
  m_position = Vector2Add(m_position, Vector2Scale(m_velocity, dt));
}

void CProjectile::Draw() const
{
  DrawCircleV(m_position, m_radius, m_color);
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
