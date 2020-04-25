#include "game.hpp"

Game::Game(unsigned width, unsigned height)
    : Window("Breaktrough", width, height)
{
}

Game::~Game() {}

void Game::init() {}

void Game::draw() {}

bool Game::run()
{
  show();
  return true;
}
