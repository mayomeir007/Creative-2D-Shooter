#include "CHud.hpp"
#include "CPlayer.hpp"
#include "Guide.hpp"
#include "Config.hpp"

void CHud::Draw(const CPlayer& player, int score, int enemiesLeft) const
{
  DrawStats(player, score, enemiesLeft);
  DrawControlsLegend();
  DrawFps();
}

void CHud::DrawStats(const CPlayer& player, int score, int enemiesLeft) const
{
  constexpr int fontSize = 26;
  constexpr int lineHeight = 32;
  constexpr int rightMargin = 32;
  constexpr int bottomMargin = 32;
  constexpr int rowCount = 5;
  constexpr Color ReloadColor{255, 176, 59, 255};

  const int blockTop = Config::ScreenHeight - bottomMargin - rowCount * lineHeight;

  const auto drawRightAligned = [&](int row, const char* text, Color color)
  {
    const int y = blockTop + row * lineHeight;
    const int width = HomeworkGuide::MeasureUiText(text, fontSize);
    HomeworkGuide::DrawUiText(text, Config::ScreenWidth - rightMargin - width, y, fontSize, color);
  };

  if (player.IsReloading())
  {
    drawRightAligned(0, TextFormat("Reloading %.1fs", player.ReloadTimeLeft()), ReloadColor);
  }
  drawRightAligned(1, TextFormat("Score: %d", score), HomeworkGuide::PrimaryTextColor);
  drawRightAligned(2, TextFormat("Enemies left: %d", enemiesLeft), HomeworkGuide::PrimaryTextColor);
  drawRightAligned(3, TextFormat("Ammo: %d/%d", player.AmmoInMag(), player.MagazineSize()), HomeworkGuide::PrimaryTextColor);
  drawRightAligned(4, TextFormat("Health: %d", player.Health()), HomeworkGuide::PrimaryTextColor);
}

void CHud::DrawControlsLegend() const
{
  constexpr int fontSize = 22;
  constexpr int leftMargin = 32;
  constexpr int bottomMargin = 32;
  const char* text = "WSAD move | Mouse aim | Space/LMB shoot | R reload | P pause";
  HomeworkGuide::DrawUiText(text, leftMargin, Config::ScreenHeight - bottomMargin - fontSize, fontSize,
                             HomeworkGuide::SecondaryTextColor);
}

void CHud::DrawFps() const
{
  HomeworkGuide::DrawFpsMeter(Config::ScreenWidth);
}
