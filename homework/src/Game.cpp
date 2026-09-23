#include "Game.hpp"
#include "Guide.hpp"
#include "CInputState.hpp"
#include "Config.hpp"

void Game::Init(int screenWidth, int screenHeight, int targetFps)
{
  m_world.Init(screenWidth, screenHeight);
  SetExitKey(KEY_NULL);
  HomeworkGuide::InitializeFont();
}

void Game::Shutdown()
{
  HomeworkGuide::ShutdownFont();
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
      m_mainMenuScreen.Draw(screenWidth, screenHeight, m_score);
      break;
    case GameState::Playing:
      m_world.Draw();
      m_hud.Draw(m_world.GetPlayer(), m_score, m_world.EnemiesLeft(), m_world.GetDifficulty());
      break;
    case GameState::Paused:
      m_world.Draw();
      m_hud.Draw(m_world.GetPlayer(), m_score, m_world.EnemiesLeft(), m_world.GetDifficulty());
      m_pausedScreen.Draw(screenWidth, screenHeight, m_score);
      break;
    case GameState::GameOver:
      m_world.Draw();
      m_hud.Draw(m_world.GetPlayer(), m_score, m_world.EnemiesLeft(), m_world.GetDifficulty());
      m_gameOverScreen.Draw(screenWidth, screenHeight, m_score);
      break;
    case GameState::Win:
      m_world.Draw();
      m_hud.Draw(m_world.GetPlayer(), m_score, m_world.EnemiesLeft(), m_world.GetDifficulty());
      m_winScreen.Draw(screenWidth, screenHeight, m_score);
      break;
  }
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
  m_world.Update(dt, input);
  m_score += m_world.ConsumeKills() * Config::ScorePerKill;

  if (m_world.PlayerIsDead())
  {
    m_gamesStarted = 0; // a loss restarts the Easy/Medium/Hard progression
    SetState(GameState::GameOver);
  }
  else if (m_world.AllEnemiesDead())
  {
    SetState(GameState::Win);
  }
}

void Game::StartNewGame()
{
  m_world.Reset(DetermineDifficulty());
  ++m_gamesStarted;
  SetState(GameState::Playing);
}

void Game::ReturnToMenu()
{
  SetState(GameState::MainMenu);
  ResetScore();
}

void Game::ResetScore()
{
  m_score = 0;
}

void Game::SetState(GameState s)
{
  m_state = s;
}

Difficulty Game::DetermineDifficulty() const
{
  if (m_gamesStarted < Config::EasyGameCount)
  {
    return Difficulty::Easy;
  }
  if (m_gamesStarted < Config::EasyGameCount + Config::MediumGameCount)
  {
    return Difficulty::Medium;
  }
  if (m_gamesStarted < Config::EasyGameCount + Config::MediumGameCount + Config::HardGameCount)
  {
    return Difficulty::Hard;
  }
  return Difficulty::VeryHard;
}
