#pragma once
#include <string>
#include "raylib.h"

class CButton
{
public:
  CButton(Rectangle rect, std::string label);

  bool WasClicked(Vector2 mousePos, bool clicked) const;
  void Draw(bool selected = false) const;

private:
  bool IsHovered(Vector2 mousePos) const;

  Rectangle m_rect;
  std::string m_label;
};
