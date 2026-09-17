#pragma once
#include "raylib.h"

/*
  Homework starter notes:
  - This class owns the main gameplay lifecycle.
  - Candidates are expected to implement the TODO methods in Game.cpp.
*/

class Game
{
public:
  void Init(int screenWidth, int screenHeight, int targetFps);
  void Shutdown();
  void Update(float deltaTime);
  void Draw(int screenWidth, int screenHeight) const;
};
