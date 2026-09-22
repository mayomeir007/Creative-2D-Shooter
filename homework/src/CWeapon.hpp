#pragma once
#include "CWeaponSpec.hpp"

// The live, per-owner instance: ammo, cooldown, reload state.
class CWeapon
{
public:
  explicit CWeapon(CWeaponSpec spec);

  void Tick(float dt);
  bool CanFire() const;
  bool ConsumeShot();
  bool StartReload();
  bool IsReloading() const;
  int Ammo() const;
  int MagazineSize() const;
  float ReloadLeft() const;
  const CWeaponSpec& Spec() const;

private:
  CWeaponSpec m_spec;
  int m_ammo;
  float m_cooldownLeft = 0.0f;
  float m_reloadLeft = 0.0f;
  bool m_isReloading = false;
};
