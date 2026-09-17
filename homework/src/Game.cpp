#include "Game.hpp"
#include "Guide.hpp"

void Game::Init(int screenWidth, int screenHeight, int targetFps)
{
  // TODO: Initialize game state, entities, and systems.
}

void Game::Shutdown()
{
  // TODO: Clean up game resources and transient state.
}

void Game::Update(float deltaTime)
{
  // TODO: Implement per-frame game state updates.
}

void Game::Draw(int screenWidth, int screenHeight) const
{
  // TODO: Implement all gameplay rendering here.
  HomeworkGuide::DrawOverlay(screenWidth); // Comment this line out to hide the full starter guide.
}
