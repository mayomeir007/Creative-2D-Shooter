#include "CButton.hpp"
#include "Guide.hpp"
#include "Config.hpp"

CButton::CButton(Rectangle rect, std::string label)
    : m_rect(rect), m_label(std::move(label))
{
}

bool CButton::WasClicked(Vector2 mousePos, bool clicked) const
{
  return IsHovered(mousePos) && clicked;
}

void CButton::Draw() const
{
  DrawRectangleRec(m_rect, Config::ButtonColor);
  DrawRectangleLinesEx(m_rect, 2.0f, Config::ButtonOutlineColor);

  constexpr int fontSize = 28;
  const int textWidth = HomeworkGuide::MeasureUiText(m_label.c_str(), fontSize);
  const int textX = static_cast<int>(m_rect.x + m_rect.width / 2.0f) - textWidth / 2;
  const int textY = static_cast<int>(m_rect.y + m_rect.height / 2.0f) - fontSize / 2;
  HomeworkGuide::DrawUiText(m_label.c_str(), textX, textY, fontSize, HomeworkGuide::PrimaryTextColor);
}

bool CButton::IsHovered(Vector2 mousePos) const
{
  return CheckCollisionPointRec(mousePos, m_rect);
}
