#include "CHealthBar.hpp"
#include "raymath.h"
#include "Config.hpp"

void CHealthBar::Draw(Vector2 ownerPosition, float healthFraction) const
{
  const Vector2 topLeft{ownerPosition.x + m_offset.x, ownerPosition.y + m_offset.y};

  const Rectangle background{topLeft.x, topLeft.y, m_size.x, m_size.y};
  DrawRectangleRec(background, Config::HealthBarBackgroundColor);

  const Rectangle fill{topLeft.x, topLeft.y, m_size.x * Clamp(healthFraction, 0.0f, 1.0f), m_size.y};
  DrawRectangleRec(fill, Config::HealthBarFillColor);
}
