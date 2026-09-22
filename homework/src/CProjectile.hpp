#pragma once
#include "raylib.h"
#include "Faction.hpp"

class CProjectile
{
public:
  CProjectile(Vector2 position, Vector2 direction, float speed, int damage, Faction owner);

  void Update(float dt);
  void Draw() const;
  void Kill();
  bool IsAlive() const;
  Vector2 Position() const;
  float Radius() const;
  Faction Owner() const;
  int Damage() const;

private:
  Vector2 m_position;
  Vector2 m_velocity;
  float m_radius;
  int m_damage;
  Faction m_owner;
  bool m_alive = true;
};
