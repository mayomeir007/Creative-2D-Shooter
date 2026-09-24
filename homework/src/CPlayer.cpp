#include "CPlayer.hpp"
#include <cmath>
#include "raymath.h"
#include "Config.hpp"
#include "CWeaponSpec.hpp"

CPlayer::CPlayer(Vector2 spawn)
    : CCharacter(spawn, Config::CharacterRadius, Config::PlayerColor, Config::PlayerMaxHealth, Config::PlayerMoveSpeed,
                 CWeapon(CWeaponSpec::Uzi())),
      m_spawnPoint(spawn)
{
}

void CPlayer::ApplyMoveInput(Vector2 moveDir, float dt)
{
  m_pendingMove = Vector2Scale(moveDir, m_moveSpeed * dt);
}

void CPlayer::AimAt(Vector2 mousePos)
{
  const Vector2 toMouse = Vector2Subtract(mousePos, m_position);
  if (Vector2LengthSqr(toMouse) > 0.0f)
  {
    SetFacingRad(std::atan2(toMouse.y, toMouse.x));
  }
}

void CPlayer::ResetToSpawn()
{
  SetPosition(m_spawnPoint);
  m_health = m_maxHealth;
  m_weapon = CWeapon(CWeaponSpec::Uzi());
}

bool CPlayer::StartReload()
{
  return m_weapon.StartReload();
}

bool CPlayer::IsReloading() const
{
  return m_weapon.IsReloading();
}

int CPlayer::AmmoInMag() const
{
  return m_weapon.Ammo();
}

int CPlayer::MagazineSize() const
{
  return m_weapon.MagazineSize();
}

float CPlayer::ReloadTimeLeft() const
{
  return m_weapon.ReloadLeft();
}

Faction CPlayer::GetFaction() const
{
  return Faction::Player;
}

void CPlayer::Draw() const
{
  CCharacter::Draw();
  m_healthBar.Draw(Position(), HealthFraction());
}
