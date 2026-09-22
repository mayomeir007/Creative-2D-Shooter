#include "CWeapon.hpp"

CWeapon::CWeapon(CWeaponSpec spec)
    : m_spec(spec), m_ammo(spec.m_magazineSize)
{
}

void CWeapon::Tick(float dt)
{
  // TODO: Decrement m_cooldownLeft; if reloading, decrement m_reloadLeft and
  // refill + clear the flag at zero.
}

bool CWeapon::CanFire() const
{
  // TODO: !m_isReloading && m_cooldownLeft <= 0 && (m_spec.m_infiniteAmmo || m_ammo >= 1)
  return false;
}

bool CWeapon::ConsumeShot()
{
  // TODO: Decrement ammo (if finite), reset m_cooldownLeft = 1 / m_spec.m_fireRate.
  return false;
}

bool CWeapon::StartReload()
{
  // TODO: No-op if infinite ammo or already reloading; else start the timer.
  return false;
}

bool CWeapon::IsReloading() const
{
  return m_isReloading;
}

int CWeapon::Ammo() const
{
  return m_ammo;
}

int CWeapon::MagazineSize() const
{
  return m_spec.m_magazineSize;
}

float CWeapon::ReloadLeft() const
{
  return m_reloadLeft;
}

const CWeaponSpec& CWeapon::Spec() const
{
  return m_spec;
}
