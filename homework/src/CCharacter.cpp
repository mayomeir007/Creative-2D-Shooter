#include "CCharacter.hpp"

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
  // TODO: Health() / (float)MaxHealth()
  return 0.0f;
}

float CCharacter::FacingRad() const
{
  return m_facingRad;
}

Vector2 CCharacter::FacingDir() const
{
  // TODO: Unit vector from m_facingRad.
  return {};
}

bool CCharacter::IsDead() const
{
  // TODO: m_health <= 0
  return false;
}

void CCharacter::Tick(float dt)
{
  // TODO: Forward to m_weapon.Tick(dt).
}

void CCharacter::SetPosition(Vector2 p)
{
  m_position = p;
}

void CCharacter::Draw() const
{
  // TODO: Body + gun rectangle toward facing + two eye dots.
}

void CCharacter::TakeDamage(int amount)
{
  // TODO: Subtract, clamp m_health to [0, m_maxHealth] immediately.
}

void CCharacter::MoveWithSlide(const std::vector<CObstacle>& obstacles)
{
  // TODO: Axis-separated slide using m_pendingMove, then reset it to {0, 0}.
}

void CCharacter::ClampToArena(const CArena& arena)
{
  // TODO: m_position = arena.ClampCircle(m_position, m_radius)
}

void CCharacter::SetFacingRad(float radians)
{
  // TODO: Normalize the angle and store it in m_facingRad.
}

std::optional<CProjectile> CCharacter::TryFire(bool wantsToFire)
{
  // TODO: If wantsToFire && m_weapon.CanFire(), consume the shot and spawn a
  // projectile at m_position + FacingDir() * m_radius; else return empty.
  return std::nullopt;
}
