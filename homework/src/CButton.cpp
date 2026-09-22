#include "CButton.hpp"

CButton::CButton(Rectangle rect, std::string label)
    : m_rect(rect), m_label(std::move(label))
{
}

bool CButton::WasClicked(Vector2 mousePos, bool clicked) const
{
  // TODO: IsHovered(mousePos) && clicked
  return false;
}

void CButton::Draw() const
{
  // TODO: Draw the button rect + label.
}

bool CButton::IsHovered(Vector2 mousePos) const
{
  // TODO: Point-in-rect test against m_rect.
  return false;
}
