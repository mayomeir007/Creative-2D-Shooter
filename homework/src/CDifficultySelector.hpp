#pragma once
#include "raylib.h"
#include "CButton.hpp"
#include "Difficulty.hpp"

// Main-menu-only widget: lets the player pick a difficulty before starting a
// game. The choice is locked in for that game — Playing/Paused/GameOver/Win
// never show it; changing difficulty means returning to the main menu (Esc)
// and picking again before the next Start.
class CDifficultySelector
{
public:
  CDifficultySelector();

  void Update(Vector2 mousePos, bool clicked);
  void Draw() const;
  Difficulty Selected() const;

private:
  CButton m_easyButton;
  CButton m_mediumButton;
  CButton m_hardButton;
  CButton m_veryHardButton;
  Difficulty m_selected = Difficulty::Easy;
};
