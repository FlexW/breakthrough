#pragma once

enum class GameState
{
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN
};

class Game
{
public:
  GameState state;

  bool keys[1024];

  bool keys_processed[1024];

  unsigned width, height;

  Game(unsigned width, unsigned height);

  ~Game();

  void init();

  void process_input(float delta_time);

  void update(float delta_time);

  void render();
};
