#include "Game.hpp"
#include "Guide.hpp"
#include "CInputState.hpp"

void Game::Init(int screenWidth, int screenHeight, int targetFps)
{
  // TODO: m_world.Init(screenWidth, screenHeight); SetExitKey(KEY_NULL);
}

void Game::Shutdown()
{
  // TODO: Clean up game resources and transient state.
}

void Game::Update(float deltaTime)
{
  const CInputState input = CInputState::Sample();
  if (HandleGlobalInput(input))
  {
    return;
  }

  switch (m_state)
  {
    case GameState::MainMenu:
      if (m_mainMenuScreen.Update(input.m_mousePos, input.m_clickPressed))
      {
        StartNewGame();
      }
      break;
    case GameState::Playing:
      UpdatePlaying(deltaTime, input);
      break;
    case GameState::Paused:
      // Frame is frozen — nothing to do.
      break;
    case GameState::GameOver:
      if (m_gameOverScreen.Update(input.m_mousePos, input.m_clickPressed))
      {
        ResetScore();
        StartNewGame();
      }
      break;
    case GameState::Win:
      if (m_winScreen.Update(input.m_mousePos, input.m_clickPressed))
      {
        StartNewGame();
      }
      break;
  }
}

void Game::Draw(int screenWidth, int screenHeight) const
{
  switch (m_state)
  {
    case GameState::MainMenu:
      m_mainMenuScreen.Draw(screenWidth, screenHeight);
      break;
    case GameState::Playing:
      m_world.Draw();
      m_hud.Draw(m_world.GetPlayer(), m_score, m_world.EnemiesLeft());
      break;
    case GameState::Paused:
      m_world.Draw();
      m_hud.Draw(m_world.GetPlayer(), m_score, m_world.EnemiesLeft());
      m_pausedScreen.Draw(screenWidth, screenHeight);
      break;
    case GameState::GameOver:
      m_world.Draw();
      m_hud.Draw(m_world.GetPlayer(), m_score, m_world.EnemiesLeft());
      m_gameOverScreen.Draw(screenWidth, screenHeight);
      break;
    case GameState::Win:
      m_world.Draw();
      m_hud.Draw(m_world.GetPlayer(), m_score, m_world.EnemiesLeft());
      m_winScreen.Draw(screenWidth, screenHeight);
      break;
  }

  HomeworkGuide::DrawOverlay(screenWidth); // Comment this line out to hide the full starter guide.
}

bool Game::ShouldQuit() const
{
  return m_quitRequested;
}

bool Game::HandleGlobalInput(const CInputState& input)
{
  if (input.m_escPressed)
  {
    if (m_state == GameState::MainMenu)
    {
      m_quitRequested = true;
    }
    else
    {
      ReturnToMenu();
    }
    return true;
  }

  if (input.m_pausePressed)
  {
    if (m_state == GameState::Playing)
    {
      SetState(GameState::Paused);
      return true;
    }
    if (m_state == GameState::Paused)
    {
      SetState(GameState::Playing);
      return true;
    }
  }

  return false;
}

void Game::UpdatePlaying(float dt, const CInputState& input)
{
  // TODO: m_world.Update(dt, input); fold m_world.ConsumeKills() into
  // m_score; check m_world.PlayerIsDead()/AllEnemiesDead() and SetState
  // accordingly.
}

void Game::StartNewGame()
{
  // TODO: m_world.Reset(); SetState(GameState::Playing). Does not touch score.
}

void Game::ReturnToMenu()
{
  // TODO: SetState(GameState::MainMenu); ResetScore().
}

void Game::ResetScore()
{
  m_score = 0;
}

void Game::SetState(GameState s)
{
  m_state = s;
}
