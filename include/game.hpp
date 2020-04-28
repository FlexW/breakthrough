#pragma once

#include "audio-master.hpp"
#include "audio-source.hpp"
#include "ball-object.hpp"
#include "game-level.hpp"
#include "particle-generator.hpp"
#include "post-processor.hpp"
#include "power-up.hpp"
#include "sprite-renderer.hpp"
#include "window.hpp"

enum class GameState
{
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN
};

enum class Direction
{
  UP    = 0,
  RIGHT = 1,
  DOWN  = 2,
  LEFT  = 3
};

class Collision
{
public:
  Collision(bool            is_collision,
            const Direction direction,
            const glm::vec2 difference_center_closest_point)
      : is_collision(is_collision),
        direction(direction),
        difference_center_closest_point(difference_center_closest_point){};

  const bool      is_collision;
  const Direction direction;
  const glm::vec2 difference_center_closest_point;
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

  void update_collisions();

  bool check_collision(const GameObject &one, const GameObject &two);

  Collision check_collision(const BallObject &one, const GameObject &two);

  void reset_level();

  void reset_player();

  void spawn_power_ups(GameObject &block);

  void update_power_ups(float delta_time);

  bool power_up_should_spawn(unsigned chance);

private:
  std::shared_ptr<SpriteRenderer> sprite_renderer;

  std::vector<GameLevel> game_levels;

  unsigned level = 0;

  float shake_time = 0.0f;

  std::vector<PowerUp> power_ups;

  std::unique_ptr<GameObject> player;

  std::unique_ptr<BallObject> ball;

  std::unique_ptr<ParticleGenerator> particle_generator;

  std::unique_ptr<PostProcessor> post_processor;

  AudioMaster audio_master;

  std::unique_ptr<AudioSource> audio_source_solid;
  std::unique_ptr<AudioSource> audio_source_breakout;
  std::unique_ptr<AudioSource> audio_source_bleep;
  std::unique_ptr<AudioSource> audio_source_powerup;
  std::unique_ptr<AudioSource> audio_source_nonsolid;

  std::shared_ptr<Texture2D> texture_speed       = nullptr;
  std::shared_ptr<Texture2D> texture_sticky      = nullptr;
  std::shared_ptr<Texture2D> texture_passthrough = nullptr;
  std::shared_ptr<Texture2D> texture_increase    = nullptr;
  std::shared_ptr<Texture2D> texture_confuse     = nullptr;
  std::shared_ptr<Texture2D> texture_chaos       = nullptr;

  void load_textures();

  void load_shaders();

  void load_audio();

  void configure_audio();

  void configure_shaders();

  void init_sprite_renderer();

  void load_levels();

  void configure_game_objects();

  Direction calc_vector_direction(const glm::vec2 target);

  void init_particle_generator();

  void init_post_processor();

  void activate_power_up(PowerUp &power_up);

  bool is_other_power_up_active(const std::vector<PowerUp> &power_ups,
                                const PowerUp::Type         type) const;

  void update_audio();
};
