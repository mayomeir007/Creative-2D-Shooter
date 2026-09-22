#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "CWorld.hpp"
#include "CHud.hpp"
#include "COverlayScreen.hpp"
#include "Difficulty.hpp"

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
  Difficulty DetermineDifficulty() const;

  GameState m_state = GameState::MainMenu;
  int m_score = 0;
  int m_gamesStarted = 0;
  bool m_quitRequested = false;
  CWorld m_world;
  CHud m_hud;
  COverlayScreen m_mainMenuScreen{"CREATIVE 2D SHOOTER", DARKGRAY, true, false};
  COverlayScreen m_pausedScreen{"PAUSED", RAYWHITE, false, false};
  COverlayScreen m_gameOverScreen{"GAME OVER", RED, true, true};
  COverlayScreen m_winScreen{"YOU WON", GREEN, true, true};
};
