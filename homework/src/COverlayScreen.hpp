#pragma once
#include <string>
#include "raylib.h"
#include "CButton.hpp"

// Shared by MainMenu/Paused/GameOver/Win — they differ only in title
// text/color, whether a button is present (Paused has none), and whether
// a score report is shown (GameOver/Win only).
class COverlayScreen
{
public:
  COverlayScreen(std::string title, Color titleColor, bool hasButton, bool showScore);

  bool Update(Vector2 mousePos, bool clicked);
  void Draw(int screenWidth, int screenHeight, int score) const;

private:
  std::string m_title;
  Color m_titleColor;
  bool m_hasButton;
  bool m_showScore;
  CButton m_startButton;
};
