#include "CInputState.hpp"
#include "raymath.h"

CInputState CInputState::Sample()
{
  CInputState state;

  Vector2 dir{0, 0};
  if (IsKeyDown(KEY_W)) dir.y -= 1.0f;
  if (IsKeyDown(KEY_S)) dir.y += 1.0f;
  if (IsKeyDown(KEY_A)) dir.x -= 1.0f;
  if (IsKeyDown(KEY_D)) dir.x += 1.0f;
  if (Vector2LengthSqr(dir) > 0.0f)
  {
    dir = Vector2Normalize(dir);
  }
  state.m_moveDir = dir;

  state.m_mousePos = GetMousePosition();
  state.m_fireHeld = IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsKeyDown(KEY_SPACE);
  state.m_reloadPressed = IsKeyPressed(KEY_R);
  state.m_pausePressed = IsKeyPressed(KEY_P);
  state.m_escPressed = IsKeyPressed(KEY_ESCAPE);
  state.m_clickPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

  return state;
}
