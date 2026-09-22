#include "COverlayScreen.hpp"
#include "Config.hpp"
#include "Guide.hpp"

namespace
{
  // Placeholder centered layout; GAME_DESIGN.md never pins down button
  // size/position beyond "a clickable Start button".
  constexpr float ButtonWidth = 200.0f;
  constexpr float ButtonHeight = 60.0f;
}

COverlayScreen::COverlayScreen(std::string title, Color titleColor, bool hasButton, bool showScore)
    : m_title(std::move(title)),
      m_titleColor(titleColor),
      m_hasButton(hasButton),
      m_showScore(showScore),
      m_startButton(Rectangle{Config::ScreenWidth / 2.0f - ButtonWidth / 2.0f,
                               Config::ScreenHeight / 2.0f + 80.0f, ButtonWidth, ButtonHeight},
                     "Start")
{
}

bool COverlayScreen::Update(Vector2 mousePos, bool clicked)
{
  return m_hasButton && m_startButton.WasClicked(mousePos, clicked);
}

void COverlayScreen::Draw(int screenWidth, int screenHeight, int score) const
{
  constexpr int titleSize = 72;
  const int titleWidth = HomeworkGuide::MeasureUiText(m_title.c_str(), titleSize);
  const int titleX = (screenWidth - titleWidth) / 2;
  const int titleY = screenHeight / 2 - 140;
  HomeworkGuide::DrawUiText(m_title.c_str(), titleX, titleY, titleSize, m_titleColor);

  if (m_showScore)
  {
    constexpr int scoreSize = titleSize;
    const char* scoreText = TextFormat("Score: %d", score);
    const int scoreWidth = HomeworkGuide::MeasureUiText(scoreText, scoreSize);
    const int scoreX = (screenWidth - scoreWidth) / 2;
    const int scoreY = titleY + titleSize + 20;
    HomeworkGuide::DrawUiText(scoreText, scoreX, scoreY, scoreSize, HomeworkGuide::PrimaryTextColor);
  }

  if (m_hasButton)
  {
    m_startButton.Draw();
  }
}
