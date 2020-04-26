#pragma once

#include "ball-object.hpp"
#include "game-level.hpp"
#include "sprite-renderer.hpp"
#include "window.hpp"

enum class GameState
{
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN
};

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);

const float PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

const float BALL_RADIUS = 12.5f;

/**
 * @brief Represents the game.
 *
 * Game holds all game-related state and functionality. Combines all
 * game-related data into a single class for easy access to each of
 * the components and manageability.
 */
class Game : public Window
{
public:
  Game(unsigned width, unsigned height);

  ~Game();

  bool run();

  void draw() override;

protected:
  GameState game_state;

  void process_input(float delta_time);

  void update(float delta_time);

  void render();

  void init();

private:
  std::shared_ptr<SpriteRenderer> sprite_renderer;

  std::vector<GameLevel> game_levels;

  unsigned level = 0;

  std::unique_ptr<GameObject> player;

  std::unique_ptr<BallObject> ball;

  void load_textures();

  void load_shaders();

  void configure_shaders();

  void init_sprite_renderer();

  void load_levels();

  void configure_game_objects();
};
