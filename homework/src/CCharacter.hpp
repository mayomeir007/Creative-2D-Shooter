#pragma once
#include <optional>
#include <vector>
#include "raylib.h"
#include "Faction.hpp"
#include "CWeapon.hpp"
#include "CProjectile.hpp"

class CArena;
class CObstacle;

// Abstract base shared by CPlayer and CEnemy: a circle with health, a facing
// direction, and a weapon. Movement input handling and AI decision-making
// are genuinely different between the two subclasses and belong in them.
class CCharacter
{
public:
  virtual ~CCharacter() = default;

  virtual Faction GetFaction() const = 0;

  Vector2 Position() const;
  float Radius() const;
  Color GetColor() const;
  int Health() const;
  int MaxHealth() const;
  float HealthFraction() const;
  float FacingRad() const;
  Vector2 FacingDir() const;
  bool IsDead() const;

  void Tick(float dt);
  void SetPosition(Vector2 p);
  virtual void Draw() const;
  void TakeDamage(int amount);
  void MoveWithSlide(const std::vector<CObstacle>& obstacles);
  void ClampToArena(const CArena& arena);
  std::optional<CProjectile> TryFire(bool wantsToFire);

protected:
  CCharacter(Vector2 position, float radius, Color color, int maxHealth, float moveSpeed, CWeapon weapon);

  void SetFacingRad(float radians);

  Vector2 m_position;
  float m_radius;
  Color m_color;
  int m_health;
  int m_maxHealth;
  float m_facingRad = 0.0f;
  float m_moveSpeed;
  CWeapon m_weapon;
  Vector2 m_pendingMove{0, 0};
};
