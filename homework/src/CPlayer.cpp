#include "CPlayer.hpp"
#include "Config.hpp"
#include "CWeaponSpec.hpp"

CPlayer::CPlayer(Vector2 spawn)
    : CCharacter(spawn, Config::CharacterRadius, BLUE, Config::MaxHealth, Config::MoveSpeed,
                 CWeapon(CWeaponSpec::Uzi())),
      m_spawnPoint(spawn)
{
}

void CPlayer::ApplyMoveInput(Vector2 moveDir, float dt)
{
  // TODO: m_pendingMove = moveDir * m_moveSpeed * dt
}

void CPlayer::AimAt(Vector2 mousePos)
{
  // TODO: Instant facing snap toward the cursor (no turn-rate limit).
}

void CPlayer::ResetToSpawn()
{
  // TODO: SetPosition(m_spawnPoint), full health, rebuild m_weapon fresh.
}

bool CPlayer::StartReload()
{
  // TODO: Delegate to m_weapon.StartReload().
  return false;
}

bool CPlayer::IsReloading() const
{
  // TODO: Delegate to m_weapon.IsReloading().
  return false;
}

int CPlayer::AmmoInMag() const
{
  // TODO: Delegate to m_weapon.Ammo().
  return 0;
}

int CPlayer::MagazineSize() const
{
  // TODO: Delegate to m_weapon.MagazineSize().
  return 0;
}

float CPlayer::ReloadTimeLeft() const
{
  // TODO: Delegate to m_weapon.ReloadLeft().
  return 0.0f;
}

Faction CPlayer::GetFaction() const
{
  return Faction::Player;
}

void CPlayer::Draw() const
{
  // TODO: Base Draw() + m_healthBar.Draw(Position(), HealthFraction()).
}
