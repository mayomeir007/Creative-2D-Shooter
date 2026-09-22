#include "CHud.hpp"
#include "CPlayer.hpp"

void CHud::Draw(const CPlayer& player, int score, int enemiesLeft) const
{
  // TODO: Orchestrate DrawStats / DrawControlsLegend / DrawFps.
}

void CHud::DrawStats(const CPlayer& player, int score, int enemiesLeft) const
{
  // TODO: Bottom-right stack (§6): reload line (reserve its slot even when
  // blank), score, enemies left, ammo, health.
}

void CHud::DrawControlsLegend() const
{
  // TODO: Bottom-left single line.
}

void CHud::DrawFps() const
{
  // TODO: Top-right, matches starter overlay style.
}
