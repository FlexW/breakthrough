#pragma once

#include "window.hpp"

enum class GameState
{
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN
};

class Game : public Window
{
public:
  Game(unsigned width, unsigned height);

  ~Game();

  bool run();

  void draw() override;

protected:
  GameState state;

  bool keys[1024];

  bool keys_processed[1024];

  void process_input(float delta_time);

  void update(float delta_time);

  void render();

  void init();
};
