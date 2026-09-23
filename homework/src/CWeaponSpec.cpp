#include "CWeaponSpec.hpp"
#include "Config.hpp"

CWeaponSpec CWeaponSpec::Uzi()
{
  return CWeaponSpec{
      "Uzi",
      Config::UziMagazineSize,
      false,
      Config::UziFireRate,
      Config::UziProjectileSpeed,
      Config::DamagePerHit,
      Config::UziReloadTime,
      Config::GunColor,
      Config::UziProjectileColor,
  };
}

CWeaponSpec CWeaponSpec::Pistol()
{
  return CWeaponSpec{
      "Pistol",
      0,
      true,
      Config::PistolFireRate,
      Config::PistolProjectileSpeed,
      Config::DamagePerHit,
      0.0f,
      Config::GunColor,
      Config::PistolProjectileColor,
  };
}

CWeaponSpec CWeaponSpec::Revolver()
{
  return CWeaponSpec{
      "Revolver",
      0,
      true,
      Config::RevolverFireRate,
      Config::RevolverProjectileSpeed,
      Config::DamagePerHit,
      0.0f,
      Config::RevolverColor,
      Config::RevolverProjectileColor,
  };
}
