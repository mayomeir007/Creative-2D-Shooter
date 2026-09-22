#pragma once
#include "raylib.h"

// A per-frame snapshot of input, sampled once and passed down by value/const-ref
// so nothing else reads raylib input functions directly.
class CInputState
{
public:
  static CInputState Sample();

  Vector2 m_moveDir{0, 0};
  Vector2 m_mousePos{0, 0};
  bool m_fireHeld = false;
  bool m_reloadPressed = false;
  bool m_pausePressed = false;
  bool m_escPressed = false;
  bool m_clickPressed = false;
};
