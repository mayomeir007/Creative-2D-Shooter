#pragma once
#include "Difficulty.hpp"

class CPlayer;

// No persistent data members — everything drawn is derived from what's
// passed into Draw() plus raylib's own per-frame state.
class CHud
{
public:
  void Draw(const CPlayer& player, int score, int enemiesLeft, Difficulty difficulty) const;

private:
  void DrawStats(const CPlayer& player, int score, int enemiesLeft) const;
  void DrawControlsLegend() const;
  void DrawFps() const;
  void DrawDifficulty(Difficulty difficulty) const;
};
