#pragma once

#include "raylib.h"

#include <sstream>
#include <string>

namespace HomeworkGuide
{
inline Font GuideFont = GetFontDefault();
inline bool GuideFontLoaded = false;
inline bool GuideFontLoadAttempted = false;

constexpr Color PrimaryTextColor{225, 232, 242, 255};
constexpr Color SecondaryTextColor{166, 178, 196, 255};
constexpr Color AccentColor{120, 215, 255, 255};
constexpr Color CodeTextColor{255, 214, 122, 255};

inline void InitializeFont()
{
  GuideFontLoadAttempted = true;
  Font loaded = LoadFontEx("C:/Windows/Fonts/segoeui.ttf", 64, nullptr, 0);
  if (loaded.texture.id != 0)
  {
    GuideFont = loaded;
    GuideFontLoaded = true;
  }
}

inline void ShutdownFont()
{
  if (GuideFontLoaded)
  {
    UnloadFont(GuideFont);
    GuideFontLoaded = false;
    GuideFont = GetFontDefault();
  }
}

inline void DrawUiText(char const* text, int x, int y, int size, Color color)
{
  const float scale = static_cast<float>(size) / static_cast<float>(GuideFont.baseSize);
  const Vector2 basePos{static_cast<float>(x), static_cast<float>(y)};
  const float fontSize = static_cast<float>(size);

  // Render a tiny multi-pass stroke to emulate a bolder weight.
  DrawTextEx(GuideFont, text, {basePos.x + 0.8f, basePos.y}, fontSize, scale, color);
  DrawTextEx(GuideFont, text, {basePos.x, basePos.y + 0.8f}, fontSize, scale, color);
  DrawTextEx(GuideFont, text, basePos, fontSize, scale, color);
}

inline int MeasureUiText(char const* text, int size)
{
  const float scale = static_cast<float>(size) / static_cast<float>(GuideFont.baseSize);
  const Vector2 textSize = MeasureTextEx(GuideFont, text, static_cast<float>(size), scale);
  return static_cast<int>(textSize.x);
}

inline int DrawWrappedText(char const* text, int x, int y, int maxWidth, int fontSize, Color color)
{
  std::istringstream words(text);
  std::string word;
  std::string line;
  int currentY = y;
  const int lineHeight = fontSize + 4;

  while (words >> word)
  {
    const std::string candidate = line.empty() ? word : line + " " + word;
    if (!line.empty() && MeasureUiText(candidate.c_str(), fontSize) > maxWidth)
    {
      DrawUiText(line.c_str(), x, currentY, fontSize, color);
      currentY += lineHeight;
      line = word;
    }
    else
    {
      line = candidate;
    }
  }

  if (!line.empty())
  {
    DrawUiText(line.c_str(), x, currentY, fontSize, color);
    currentY += lineHeight;
  }

  return currentY - y;
}

inline int DrawTaskBullet(char const* title, char const* details, int x, int y, int maxWidth, int titleSize, int detailSize)
{
  const std::string titleText = std::string("- ") + title;
  DrawUiText(titleText.c_str(), x, y, titleSize, AccentColor);

  const int detailsY = y + titleSize + 2;
  const int detailsX = x + 34;
  const int detailsWidth = maxWidth - 34;
  const int detailsHeight = DrawWrappedText(details, detailsX, detailsY, detailsWidth, detailSize, PrimaryTextColor);

  return (detailsY + detailsHeight) - y;
}

inline void DrawAssignmentTaskBrief()
{
  const Rectangle panel{728.0f, 120.0f, 1168.0f, 924.0f};
  DrawRectangleRec(panel, {28, 44, 62, 255});
  DrawRectangleLinesEx(panel, 2.0f, {74, 116, 146, 255});

  const int left = static_cast<int>(panel.x) + 32;
  const int top = static_cast<int>(panel.y) + 10;
  const int textWidth = static_cast<int>(panel.width) - 64;
  constexpr int headingSize = 46;
  constexpr int bulletTitleSize = 27;
  constexpr int bulletDetailSize = 24;
  constexpr int noteSize = 24;
  constexpr int scopeNoteSize = 22;
  constexpr int sectionGap = 2;

  DrawUiText("Homework Tasks", left, top, headingSize, AccentColor);
  int cursorY = top + 52;
  cursorY += DrawWrappedText("Implement the Game class and build your gameplay systems.", left, cursorY, textWidth, noteSize, SecondaryTextColor) + 12;
  cursorY += DrawWrappedText("Build a creative 2D shooter using only simple geometric shapes.", left, cursorY, textWidth, noteSize, SecondaryTextColor) + 14;
  cursorY +=
      DrawWrappedText("Use only raylib and the C++ STL (std:: namespace). Do not add any other external libraries.", left, cursorY, textWidth, scopeNoteSize, SecondaryTextColor) +
      12;
  cursorY +=
      DrawWrappedText("You are not expected to finish every task/subtask. In 2-4 hours, choose and prioritize tasks based on your ideas and approach, with emphasis on clean, "
                      "well-structured code over feature count.",
          left, cursorY, textWidth, scopeNoteSize, SecondaryTextColor) +
      12;

  cursorY += DrawTaskBullet("Player", "WSAD movement, mouse aim, mouse shooting.", left, cursorY, textWidth, bulletTitleSize, bulletDetailSize) + sectionGap;
  cursorY += DrawTaskBullet("Enemies", "Move and shoot at player continuously.", left, cursorY, textWidth, bulletTitleSize, bulletDetailSize) + sectionGap;
  cursorY += DrawTaskBullet("Architecture", "Aim for clean modern C++20/23 with modular design that makes future gameplay features easy to add.", left, cursorY, textWidth,
                 bulletTitleSize, bulletDetailSize) +
             sectionGap;
  cursorY +=
      DrawTaskBullet("Performance", "Avoid anti-patterns; design for 1000+ simultaneous entities.", left, cursorY, textWidth, bulletTitleSize, bulletDetailSize) + sectionGap;
  cursorY +=
      DrawTaskBullet("Enemy AI", "Follow player, seek cover, and predict player position for smarter attacks.", left, cursorY, textWidth, bulletTitleSize, bulletDetailSize) +
      sectionGap;
  cursorY += DrawTaskBullet("Arena walls", "Spawn random obstacles that block movement and projectiles.", left, cursorY, textWidth, bulletTitleSize, bulletDetailSize) + sectionGap;
  cursorY += DrawTaskBullet("UI", "Show health, ammo, score, and health bars.", left, cursorY, textWidth, bulletTitleSize, bulletDetailSize) + sectionGap;
  cursorY += DrawTaskBullet("Weapon system", "Ammo, magazine size, projectile speed, reload, multiple weapon types, and weapon pickups.", left, cursorY, textWidth, bulletTitleSize,
                 bulletDetailSize) +
             sectionGap;
  cursorY +=
      DrawTaskBullet("Code organization", "Split logic into multiple meaningful files/classes as the project grows.", left, cursorY, textWidth, bulletTitleSize, bulletDetailSize) +
      sectionGap;
  cursorY += DrawTaskBullet("Effects", "Add hit/death effects for readable combat feedback.", left, cursorY, textWidth, bulletTitleSize, bulletDetailSize) + 24;
}

inline void DrawShapeQuickGuide()
{
  const Rectangle panel{24.0f, 120.0f, 680.0f, 470.0f};
  DrawRectangleRec(panel, {30, 50, 40, 255});
  DrawRectangleLinesEx(panel, 2.0f, {86, 132, 106, 255});

  constexpr int guideLeft = 48;
  constexpr int guideTop = 130;
  constexpr int rowGap = 135;
  constexpr int headingY = guideTop;
  constexpr int shapeX = guideLeft + 62;
  constexpr int textX = guideLeft + 180;
  constexpr int firstRowY = guideTop + 80;
  constexpr int headingSize = 46;
  constexpr int labelSize = 24;
  constexpr int codeSize = 20;

  DrawUiText("Drawing Quick Guide", guideLeft, headingY, headingSize, AccentColor);

  const int circleRowY = firstRowY;
  constexpr int textTopOffset = 34;
  constexpr int textLineStep = 24;
  DrawCircle(shapeX, circleRowY, 34.0f, {77, 171, 247, 255});
  DrawCircleLines(shapeX, circleRowY, 34.0f, {210, 230, 248, 255});
  DrawUiText("Circle (filled + outline)", textX, circleRowY - textTopOffset, labelSize, PrimaryTextColor);
  DrawUiText("DrawCircle(centerX, centerY, 34.0f, BLUE);", textX, circleRowY - textTopOffset + textLineStep, codeSize, CodeTextColor);
  DrawUiText("DrawCircleLines(centerX, centerY, 34.0f, WHITE);", textX, circleRowY - textTopOffset + (textLineStep * 2), codeSize, CodeTextColor);

  const int rectRowY = firstRowY + rowGap;
  const Rectangle rect{static_cast<float>(shapeX - 60), static_cast<float>(rectRowY - 28), 120.0f, 58.0f};
  DrawRectangleRec(rect, {119, 221, 119, 255});
  DrawRectangleLinesEx(rect, 3.0f, {210, 240, 210, 255});
  DrawUiText("Rectangle (filled + outline)", textX, rectRowY - textTopOffset, labelSize, PrimaryTextColor);
  DrawUiText("DrawRectangle(x, y, width, height, GREEN);", textX, rectRowY - textTopOffset + textLineStep, codeSize, CodeTextColor);
  DrawUiText("DrawRectangleLines(x, y, width, height, WHITE);", textX, rectRowY - textTopOffset + (textLineStep * 2), codeSize, CodeTextColor);

  const int lineRowY = firstRowY + (rowGap * 2);
  DrawLine(shapeX - 64, lineRowY - 24, shapeX + 64, lineRowY + 20, {255, 212, 120, 255});
  DrawLineEx(
      {static_cast<float>(shapeX - 64), static_cast<float>(lineRowY + 28)}, {static_cast<float>(shapeX + 64), static_cast<float>(lineRowY - 16)}, 5.0f, {255, 233, 178, 255});
  DrawUiText("Lines (thin + thick)", textX, lineRowY - textTopOffset, labelSize, PrimaryTextColor);
  DrawUiText("DrawLine(x1, y1, x2, y2, YELLOW);", textX, lineRowY - textTopOffset + textLineStep, codeSize, CodeTextColor);
  DrawUiText("DrawLineEx(start, end, thickness, BEIGE);", textX, lineRowY - textTopOffset + (textLineStep * 2), codeSize, CodeTextColor);
}

inline void DrawInputQuickGuide()
{
  const Rectangle panel{24.0f, 600.0f, 680.0f, 444.0f};
  DrawRectangleRec(panel, {62, 34, 38, 255});
  DrawRectangleLinesEx(panel, 2.0f, {156, 86, 94, 255});

  constexpr int left = 48;
  constexpr int top = 610;
  constexpr int headingSize = 46;
  constexpr int titleSize = 24;
  constexpr int lineSize = 22;
  constexpr int rowGap = 70;

  DrawUiText("Input Quick Guide", left, top, headingSize, AccentColor);

  const int row1Y = top + 50;
  DrawUiText("Keyboard Hold", left, row1Y, titleSize, PrimaryTextColor);
  DrawUiText("if (IsKeyDown(KEY_W)) { /* move up */ }", left + 34, row1Y + 28, lineSize, CodeTextColor);

  const int row2Y = row1Y + rowGap;
  DrawUiText("Keyboard Press", left, row2Y, titleSize, PrimaryTextColor);
  DrawUiText("if (IsKeyPressed(KEY_R)) { /* reload */ }", left + 34, row2Y + 28, lineSize, CodeTextColor);

  const int row3Y = row2Y + rowGap;
  DrawUiText("Mouse Position", left, row3Y, titleSize, PrimaryTextColor);
  DrawUiText("Vector2 mouse = GetMousePosition();", left + 34, row3Y + 28, lineSize, CodeTextColor);

  const int row4Y = row3Y + rowGap;
  DrawUiText("Mouse Click", left, row4Y, titleSize, PrimaryTextColor);
  DrawUiText("if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { /* shoot */ }", left + 34, row4Y + 28, lineSize, CodeTextColor);

  const int docsY = row4Y + rowGap + 6;
  DrawUiText("Raylib docs: https://www.raylib.com/cheatsheet/cheatsheet.html", left, docsY, 22, AccentColor);
  DrawUiText("API reference: https://www.raylib.com/", left, docsY + 28, 20, SecondaryTextColor);
}

inline void DrawFpsMeter(int screenWidth)
{
  constexpr int topPadding = 20;
  constexpr int rightPadding = 22;
  constexpr int fpsFontSize = 28;
  const char* fpsText = TextFormat("FPS: %d", GetFPS());
  DrawUiText(fpsText, screenWidth - rightPadding - MeasureUiText(fpsText, fpsFontSize), topPadding, fpsFontSize, GREEN);
}

inline void DrawOverlay(int screenWidth)
{
  if (!GuideFontLoaded && !GuideFontLoadAttempted)
  {
    InitializeFont();
  }

  DrawUiText("Homework Assignment", 56, 30, 58, PrimaryTextColor);

  DrawAssignmentTaskBrief();
  DrawShapeQuickGuide();
  DrawInputQuickGuide();
  DrawFpsMeter(screenWidth);
}
} // namespace HomeworkGuide
