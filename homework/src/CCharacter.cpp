#include "CCharacter.hpp"
#include <algorithm>
#include <cmath>
#include "raymath.h"
#include "CArena.hpp"
#include "CCollision.hpp"
#include "CObstacle.hpp"
#include "Config.hpp"

CCharacter::CCharacter(Vector2 position, float radius, Color color, int maxHealth, float moveSpeed, CWeapon weapon)
    : m_position(position),
      m_radius(radius),
      m_color(color),
      m_health(maxHealth),
      m_maxHealth(maxHealth),
      m_moveSpeed(moveSpeed),
      m_weapon(weapon)
{
}

Vector2 CCharacter::Position() const
{
  return m_position;
}

float CCharacter::Radius() const
{
  return m_radius;
}

Color CCharacter::GetColor() const
{
  return m_color;
}

int CCharacter::Health() const
{
  return m_health;
}

int CCharacter::MaxHealth() const
{
  return m_maxHealth;
}

float CCharacter::HealthFraction() const
{
  return static_cast<float>(m_health) / static_cast<float>(m_maxHealth);
}

float CCharacter::FacingRad() const
{
  return m_facingRad;
}

Vector2 CCharacter::FacingDir() const
{
  return {std::cos(m_facingRad), std::sin(m_facingRad)};
}

bool CCharacter::IsDead() const
{
  return m_health <= 0;
}

void CCharacter::Tick(float dt)
{
  m_weapon.Tick(dt);
}

void CCharacter::SetPosition(Vector2 p)
{
  m_position = p;
}

void CCharacter::Draw() const
{
  DrawCircleV(m_position, m_radius, m_color);

  const Rectangle gunRect{m_position.x, m_position.y, Config::GunLength, Config::GunWidth};
  const Vector2 gunOrigin{-m_radius, Config::GunWidth / 2.0f};
  DrawRectanglePro(gunRect, gunOrigin, m_facingRad * RAD2DEG, Config::GunColor);

  const Vector2 facing = FacingDir();
  const Vector2 eyeCenter = Vector2Add(m_position, Vector2Scale(facing, m_radius * 0.5f));
  const Vector2 perp{-facing.y, facing.x};
  const Vector2 eyeOffset = Vector2Scale(perp, Config::EyeSpacing * 0.5f);
  DrawCircleV(Vector2Add(eyeCenter, eyeOffset), Config::EyeRadius, Config::EyeColor);
  DrawCircleV(Vector2Subtract(eyeCenter, eyeOffset), Config::EyeRadius, Config::EyeColor);
}

void CCharacter::TakeDamage(int amount)
{
  m_health = std::clamp(m_health - amount, 0, m_maxHealth);
}

void CCharacter::MoveWithSlide(const std::vector<CObstacle>& obstacles)
{
  const auto blocked = [&](Vector2 pos)
  {
    for (const CObstacle& obstacle : obstacles)
    {
      if (obstacle.BlocksCircle(pos, m_radius))
      {
        return true;
      }
    }
    return false;
  };

  Vector2 candidate{m_position.x + m_pendingMove.x, m_position.y};
  if (!blocked(candidate))
  {
    m_position.x = candidate.x;
  }

  candidate = {m_position.x, m_position.y + m_pendingMove.y};
  if (!blocked(candidate))
  {
    m_position.y = candidate.y;
  }

  m_pendingMove = {0, 0};
}

void CCharacter::PushOutOfObstacles(const std::vector<CObstacle>& obstacles)
{
  for (const CObstacle& obstacle : obstacles)
  {
    if (obstacle.BlocksCircle(m_position, m_radius))
    {
      CCollision::ResolveCircleRectOverlap(m_position, m_radius, obstacle.Rect());
    }
  }
}

void CCharacter::ClampToArena(const CArena& arena)
{
  m_position = arena.ClampCircle(m_position, m_radius);
}

void CCharacter::SetFacingRad(float radians)
{
  m_facingRad = Wrap(radians, -PI, PI);
}

std::optional<CProjectile> CCharacter::TryFire(bool wantsToFire)
{
  if (!wantsToFire || !m_weapon.CanFire())
  {
    return std::nullopt;
  }

  const Vector2 facing = FacingDir();
  const Vector2 spawnPos = Vector2Add(m_position, Vector2Scale(facing, m_radius));
  const CWeaponSpec& spec = m_weapon.Spec();
  m_weapon.ConsumeShot();

  return CProjectile(spawnPos, facing, spec.m_projectileSpeed, spec.m_damagePerHit, GetFaction());
}
