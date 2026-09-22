#pragma once
#include <string>
#include "raylib.h"

// Immutable per-weapon stats. Kept separate from CWeapon (the live, per-owner
// instance) so a future weapon-pickup feature just swaps which spec a
// CWeapon wraps.
struct CWeaponSpec
{
  std::string m_name;
  int m_magazineSize = 0;
  bool m_infiniteAmmo = false;
  float m_fireRate = 0.0f;
  float m_projectileSpeed = 0.0f;
  int m_damagePerHit = 0;
  float m_reloadTime = 0.0f;
  Color m_gunColor{40, 44, 52, 255};

  static CWeaponSpec Uzi();
  static CWeaponSpec Pistol();
  static CWeaponSpec Revolver();
};
