#include "CWeaponSpec.hpp"
#include "Config.hpp"

CWeaponSpec CWeaponSpec::Uzi()
{
  return CWeaponSpec{
      "Uzi",
      Config::UziMagazineSize,
      false,
      Config::UziFireRate,
      Config::ProjectileSpeed,
      Config::DamagePerHit,
      Config::UziReloadTime,
      Config::GunColor,
  };
}

CWeaponSpec CWeaponSpec::Pistol()
{
  return CWeaponSpec{
      "Pistol",
      0,
      true,
      Config::PistolFireRate,
      Config::ProjectileSpeed,
      Config::DamagePerHit,
      0.0f,
      Config::GunColor,
  };
}

CWeaponSpec CWeaponSpec::Revolver()
{
  return CWeaponSpec{
      "Revolver",
      0,
      true,
      Config::RevolverFireRate,
      Config::ProjectileSpeed,
      Config::DamagePerHit,
      0.0f,
      Config::RevolverColor,
  };
}
