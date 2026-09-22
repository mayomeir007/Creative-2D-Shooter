#include "COverlayScreen.hpp"
#include "Config.hpp"

namespace
{
  // Placeholder centered layout; GAME_DESIGN.md never pins down button
  // size/position beyond "a clickable Start button".
  constexpr float ButtonWidth = 200.0f;
  constexpr float ButtonHeight = 60.0f;
}

COverlayScreen::COverlayScreen(std::string title, Color titleColor, bool hasButton)
    : m_title(std::move(title)),
      m_titleColor(titleColor),
      m_hasButton(hasButton),
      m_startButton(Rectangle{Config::ScreenWidth / 2.0f - ButtonWidth / 2.0f,
                               Config::ScreenHeight / 2.0f + 80.0f, ButtonWidth, ButtonHeight},
                     "Start")
{
}

bool COverlayScreen::Update(Vector2 mousePos, bool clicked)
{
  // TODO: True iff the button exists, is hovered, and was clicked this frame.
  return false;
}

void COverlayScreen::Draw(int screenWidth, int screenHeight) const
{
  // TODO: Centered title (+ button if present) over whatever backdrop was
  // already rendered.
}
