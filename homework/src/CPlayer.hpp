#pragma once
#include "CCharacter.hpp"
#include "CHealthBar.hpp"

class CPlayer : public CCharacter
{
public:
  CPlayer(Vector2 spawn);

  void ApplyMoveInput(Vector2 moveDir, float dt);
  void AimAt(Vector2 mousePos);
  void ResetToSpawn();
  bool StartReload();
  bool IsReloading() const;
  int AmmoInMag() const;
  int MagazineSize() const;
  float ReloadTimeLeft() const;
  Faction GetFaction() const override;
  void Draw() const override;

private:
  Vector2 m_spawnPoint;
  CHealthBar m_healthBar;
};
