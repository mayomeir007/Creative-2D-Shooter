#include "CWeapon.hpp"

CWeapon::CWeapon(CWeaponSpec spec)
    : m_spec(spec), m_ammo(spec.m_magazineSize)
{
}

void CWeapon::Tick(float dt)
{
  if (m_cooldownLeft > 0.0f)
  {
    m_cooldownLeft -= dt;
  }

  if (m_isReloading)
  {
    m_reloadLeft -= dt;
    if (m_reloadLeft <= 0.0f)
    {
      m_ammo = m_spec.m_magazineSize;
      m_isReloading = false;
      m_reloadLeft = 0.0f;
    }
  }
}

bool CWeapon::CanFire() const
{
  return !m_isReloading && m_cooldownLeft <= 0.0f && (m_spec.m_infiniteAmmo || m_ammo >= 1);
}

bool CWeapon::ConsumeShot()
{
  if (!CanFire())
  {
    return false;
  }

  if (!m_spec.m_infiniteAmmo)
  {
    --m_ammo;
  }
  m_cooldownLeft = 1.0f / m_spec.m_fireRate;
  return true;
}

bool CWeapon::StartReload()
{
  if (m_spec.m_infiniteAmmo || m_isReloading)
  {
    return false;
  }

  m_isReloading = true;
  m_reloadLeft = m_spec.m_reloadTime;
  return true;
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
