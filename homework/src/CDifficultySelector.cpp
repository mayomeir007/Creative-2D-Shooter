#include "CDifficultySelector.hpp"
#include "Config.hpp"
#include "Guide.hpp"

namespace
{
  // Placeholder centered layout, matching COverlayScreen's Start button
  // sizing convention; GAME_DESIGN.md doesn't pin down menu widget layout.
  constexpr float ButtonWidth = 180.0f;
  constexpr float ButtonHeight = 50.0f;
  constexpr float ButtonGap = 20.0f;
  constexpr float RowWidth = ButtonWidth * 4.0f + ButtonGap * 3.0f;
  constexpr float RowLeft = Config::ScreenWidth / 2.0f - RowWidth / 2.0f;
  constexpr float RowY = Config::ScreenHeight / 2.0f - 10.0f;

  Rectangle ButtonRect(int slot)
  {
    return Rectangle{RowLeft + static_cast<float>(slot) * (ButtonWidth + ButtonGap), RowY, ButtonWidth, ButtonHeight};
  }
}

CDifficultySelector::CDifficultySelector()
    : m_easyButton(ButtonRect(0), "Easy"),
      m_mediumButton(ButtonRect(1), "Medium"),
      m_hardButton(ButtonRect(2), "Hard"),
      m_veryHardButton(ButtonRect(3), "Very Hard")
{
}

void CDifficultySelector::Update(Vector2 mousePos, bool clicked)
{
  if (m_easyButton.WasClicked(mousePos, clicked))
  {
    m_selected = Difficulty::Easy;
  }
  else if (m_mediumButton.WasClicked(mousePos, clicked))
  {
    m_selected = Difficulty::Medium;
  }
  else if (m_hardButton.WasClicked(mousePos, clicked))
  {
    m_selected = Difficulty::Hard;
  }
  else if (m_veryHardButton.WasClicked(mousePos, clicked))
  {
    m_selected = Difficulty::VeryHard;
  }
}

void CDifficultySelector::Draw() const
{
  constexpr int labelSize = 24;
  constexpr const char* label = "Difficulty";
  const int labelWidth = HomeworkGuide::MeasureUiText(label, labelSize);
  HomeworkGuide::DrawUiText(label, static_cast<int>(Config::ScreenWidth / 2.0f) - labelWidth / 2, static_cast<int>(RowY) - labelSize - 12, labelSize,
                             HomeworkGuide::SecondaryTextColor);

  m_easyButton.Draw(m_selected == Difficulty::Easy);
  m_mediumButton.Draw(m_selected == Difficulty::Medium);
  m_hardButton.Draw(m_selected == Difficulty::Hard);
  m_veryHardButton.Draw(m_selected == Difficulty::VeryHard);
}

Difficulty CDifficultySelector::Selected() const
{
  return m_selected;
}
