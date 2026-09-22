#pragma once
#include <string>
#include "raylib.h"
#include "CButton.hpp"

// Shared by MainMenu/Paused/GameOver/Win — they differ only in title
// text/color and whether a button is present (Paused has none).
class COverlayScreen
{
public:
  COverlayScreen(std::string title, Color titleColor, bool hasButton);

  bool Update(Vector2 mousePos, bool clicked);
  void Draw(int screenWidth, int screenHeight) const;

private:
  std::string m_title;
  Color m_titleColor;
  bool m_hasButton;
  CButton m_startButton;
};
