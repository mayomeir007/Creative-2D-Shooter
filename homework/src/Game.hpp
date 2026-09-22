#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "CWorld.hpp"
#include "CHud.hpp"
#include "COverlayScreen.hpp"

/*
  Homework starter notes:
  - This class owns the main gameplay lifecycle.
  - Candidates are expected to implement the TODO methods in Game.cpp.
*/

class CInputState;

class Game
{
public:
  void Init(int screenWidth, int screenHeight, int targetFps);
  void Shutdown();
  void Update(float deltaTime);
  void Draw(int screenWidth, int screenHeight) const;
  bool ShouldQuit() const;

private:
  bool HandleGlobalInput(const CInputState& input);
  void UpdatePlaying(float dt, const CInputState& input);
  void StartNewGame();
  void ReturnToMenu();
  void ResetScore();
  void SetState(GameState s);

  GameState m_state = GameState::MainMenu;
  int m_score = 0;
  bool m_quitRequested = false;
  CWorld m_world;
  CHud m_hud;
  COverlayScreen m_mainMenuScreen{"", DARKGRAY, true};
  COverlayScreen m_pausedScreen{"PAUSED", RAYWHITE, false};
  COverlayScreen m_gameOverScreen{"GAME OVER", RED, true};
  COverlayScreen m_winScreen{"YOU WON", GREEN, true};
};
