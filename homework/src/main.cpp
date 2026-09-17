#include "raylib.h"
#include "Game.hpp"

constexpr int ScreenWidth = 1920;
constexpr int ScreenHeight = 1080;
constexpr int TargetFps = 60;
constexpr Color BackgroundColor{20, 24, 32, 255};

int main()
{
  InitWindow(ScreenWidth, ScreenHeight, "Homework Assignment");
  SetTargetFPS(TargetFps);

  Game game;
  game.Init(ScreenWidth, ScreenHeight, TargetFps);

  while (!WindowShouldClose())
  {
    game.Update(GetFrameTime());
    BeginDrawing();
    ClearBackground(BackgroundColor);
    game.Draw(ScreenWidth, ScreenHeight);
    EndDrawing();
  }

  game.Shutdown();
  CloseWindow();
  return 0;
}
