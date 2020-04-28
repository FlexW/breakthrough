#include <algorithm>

#include "asseration.hpp"
#include "game.hpp"
#include "resource-manager.hpp"

static const std::string LOG_TAG = "Game";

Game::Game(unsigned width, unsigned height)
    : Window("Breaktrough", width, height),
      game_state(GameState::GAME_ACTIVE)
{
  init();
}

Game::~Game()
{
  // Delete audio sources before audio buffers
  audio_source_solid.reset();
  audio_source_breakout.reset();
  audio_source_bleep.reset();
  audio_source_powerup.reset();
  audio_source_nonsolid.reset();

  ResourceManager::clear();
}

void Game::init()
{
  load_shaders();
  configure_shaders();
  init_sprite_renderer();
  load_textures();
  init_particle_generator();
  load_levels();
  configure_game_objects();
  init_post_processor();
  load_audio();
  configure_audio();
  init_text_renderer();

  audio_source_breakout->play();
}

void Game::draw()
{
  process_input(delta_time);
  update(delta_time);
  render();
}

void Game::process_input(float delta_time)
{
  switch (game_state)
  {
  case GameState::GAME_ACTIVE:
  {
    float velocity = PLAYER_VELOCITY * delta_time;

    // Move playerboard and ball
    if (get_key(GLFW_KEY_A))
    {
      const auto player_position_x = player->get_position().x;
      if (player_position_x >= 0.0f)
      {
        const auto player_position_y = player->get_position().y;
        player->set_position(
            glm::vec2(player_position_x - velocity, player_position_y));
        if (ball->is_stuck())
        {
          const auto ball_pos = ball->get_position();
          ball->set_position(glm::vec2(ball_pos.x - velocity, ball_pos.y));
        }
      }
    }

    if (get_key(GLFW_KEY_D))
    {
      const auto player_position_x = player->get_position().x;
      if (player_position_x <= window_width - player->get_size().x)
      {
        const auto player_position_y = player->get_position().y;
        player->set_position(
            glm::vec2(player_position_x + velocity, player_position_y));
        if (ball->is_stuck())
        {
          const auto ball_pos = ball->get_position();
          ball->set_position(glm::vec2(ball_pos.x + velocity, ball_pos.y));
        }
      }
    }

    if (get_key(GLFW_KEY_W))
    {
      level = (level + 1) % game_levels.size();
    }

    if (get_key(GLFW_KEY_S))
    {
      if (level > 0)
        --level;
      else
        level = game_levels.size() - 1;
    }

    // Release ball
    if (get_key(GLFW_KEY_SPACE))
    {
      ball->set_stuck(false);
    }
  }
  default:;
  }
}

void Game::update(float delta_time)
{
  update_audio();
  ball->move(delta_time, window_width);
  update_collisions();
  particle_generator->update(delta_time,
                             *ball,
                             2,
                             glm::vec2(ball->get_radius() / 2.0f));

  update_power_ups(delta_time);

  // Reduce shake time
  if (shake_time > 0.0f)
  {
    shake_time -= delta_time;
    if (shake_time <= 0.0f)
    {
      post_processor->set_shake(false);
    }
  }

  // Did ball reach bottom edge?
  if (ball->get_position().y >= window_height)
  {
    --lives_count;

    if (lives_count == 0)
    {
      reset_level();
      reset_player();
    }
  }
}

void Game::render()
{
  switch (game_state)
  {
  case GameState::GAME_ACTIVE:
  {
    post_processor->begin_render();
    {
      // Draw background
      sprite_renderer->draw_sprite(ResourceManager::get_texture("background"),
                                   glm::vec2(0.0f, 0.0f),
                                   glm::vec2(window_width, window_height),
                                   0.0f);

      // Draw level
      game_levels[level].draw(sprite_renderer);

      // Draw player
      player->draw(sprite_renderer);

      // Draw power ups
      for (auto &power_up : power_ups)
        if (!power_up.is_destroyed())
          power_up.draw(sprite_renderer);

      particle_generator->draw();

      // Draw ball
      ball->draw(sprite_renderer);
    }
    post_processor->end_render();
    post_processor->render(static_cast<float>(get_time()));

    text_renderer->render_text("Lives: " + std::to_string(lives_count),
                               5.0f,
                               5.0f,
                               1.0f);
  }
  default:;
  }
}

  bool Game::run()
  {
    show();
    return true;
  }

  void Game::load_textures()
  {
    ResourceManager::load_texture("textures/background.jpg",
                                  false,
                                  "background");
    ResourceManager::load_texture("textures/awesomeface.png", true, "face");
    ResourceManager::load_texture("textures/block.png", false, "block");
    ResourceManager::load_texture("textures/block_solid.png",
                                  false,
                                  "block_solid");
    ResourceManager::load_texture("textures/paddle.png", true, "paddle");
    ResourceManager::load_texture("textures/particle.png", true, "particle");
    texture_speed = ResourceManager::load_texture("textures/powerup_speed.png",
                                                  true,
                                                  "powerup_speed");
    texture_confuse =
        ResourceManager::load_texture("textures/powerup_confuse.png",
                                      true,
                                      "powerup_confuse");
    texture_increase =
        ResourceManager::load_texture("textures/powerup_increase.png",
                                      true,
                                      "powerup_increase");

    texture_passthrough =
        ResourceManager::load_texture("textures/powerup_passthrough.png",
                                      true,
                                      "powerup_passthrough");

    texture_chaos = ResourceManager::load_texture("textures/powerup_chaos.png",
                                                  true,
                                                  "powerup_chaos");

    texture_sticky = ResourceManager::load_texture("textures/powerup_chaos.png",
                                                   true,
                                                   "powerup_sticky");
  }

  void Game::load_shaders()
  {
    ResourceManager::load_shader("shaders/sprite/sprite.vert",
                                 "shaders/sprite/sprite.frag",
                                 "sprite");

    ResourceManager::load_shader("shaders/particle/particle.vert",
                                 "shaders/particle/particle.frag",
                                 "particle");

    ResourceManager::load_shader("shaders/post-processing/post-processing.vert",
                                 "shaders/post-processing/post-processing.frag",
                                 "post-processing");

    ResourceManager::load_shader("shaders/text/text.vert",
                                 "shaders/text/text.frag",
                                 "text");
  }

  void Game::configure_shaders()
  {
    const auto projection_matrix = glm::ortho(0.0f,
                                              static_cast<float>(window_width),
                                              static_cast<float>(window_height),
                                              0.0f,
                                              -1.0f,
                                              1.0f);

    auto shader = ResourceManager::get_shader("sprite");
    shader->bind();
    shader->set_uniform("image", 0);
    shader->set_uniform("projection_matrix", projection_matrix);
    shader->unbind();

    shader = ResourceManager::get_shader("particle");
    shader->bind();
    shader->set_uniform("sprite", 0);
    shader->set_uniform("projection_matrix", projection_matrix);
    shader->unbind();
  }

  void Game::init_sprite_renderer()
  {
    sprite_renderer =
        std::make_shared<SpriteRenderer>(renderer,
                                         ResourceManager::get_shader("sprite"));
  }

  void Game::load_levels()
  {
    GameLevel level1("levels/one.lvl", window_width, window_height / 2);
    GameLevel level2("levels/two.lvl", window_width, window_height / 2);
    GameLevel level3("levels/three.lvl", window_width, window_height / 2);
    GameLevel level4("levels/four.lvl", window_width, window_height / 2);

    game_levels.push_back(std::move(level1));
    game_levels.push_back(std::move(level2));
    game_levels.push_back(std::move(level3));
    game_levels.push_back(std::move(level4));

    level = 0;
  }

  void Game::configure_game_objects()
  {
    const auto player_pos =
        glm::vec2(window_width / 2.0f - PLAYER_SIZE.x / 2.0f,
                  window_height - PLAYER_SIZE.y);

    player =
        std::make_unique<GameObject>(player_pos,
                                     PLAYER_SIZE,
                                     ResourceManager::get_texture("paddle"));

    const auto ball_pos =
        player_pos +
        glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);

    ball = std::make_unique<BallObject>(ball_pos,
                                        BALL_RADIUS,
                                        INITIAL_BALL_VELOCITY,
                                        ResourceManager::get_texture("face"));
  }

  void Game::update_collisions()
  {
    for (auto &box : this->game_levels[level].get_bricks())
    {
      if (!box.is_destroyed())
      {
        const auto collision = check_collision(*ball, box);
        if (collision.is_collision)
        {
          // destroy block if not solid
          if (!box.is_solid())
          {
            box.set_destroyed(true);
            spawn_power_ups(box);
            audio_source_nonsolid->play();
          }
          else
          {
            shake_time = 0.05f;
            post_processor->set_shake(true);
            audio_source_solid->play();
          }

          // collision resolution
          const auto dir         = collision.direction;
          const auto diff_vector = collision.difference_center_closest_point;
          // horizontal collision, don't do collision resolution on
          // non-solid bricks if pass-through is activated
          if (!(ball->is_pass_through() && !box.is_solid()))
          {
            if (dir == Direction::LEFT || dir == Direction::RIGHT)
            {
              const auto ball_velocity = ball->get_velocity();
              // reverse horizontal velocity
              ball->set_velocity(glm::vec2(-ball_velocity.x, ball_velocity.y));
              // relocate
              const auto penetration =
                  ball->get_radius() - std::abs(diff_vector.x);
              const auto ball_position = ball->get_position();
              if (dir == Direction::LEFT)
              {
                // move ball to right
                ball->set_position(
                    glm::vec2(ball_position.x + penetration, ball_position.y));
              }
              else
              {
                // move ball to left;
                ball->set_position(
                    glm::vec2(ball_position.x - penetration, ball_position.y));
              }
            }
            else // vertical collision
            {
              const auto ball_velocity = ball->get_velocity();
              ball->set_velocity(glm::vec2(ball_velocity.x, -ball_velocity.y));
              // relocate
              const auto penetration =
                  ball->get_radius() - std::abs(diff_vector.y);
              const auto ball_position = ball->get_position();
              if (dir == Direction::UP)
              {
                // move ball back up
                ball->set_position(
                    glm::vec2(ball_position.x, ball_position.y - penetration));
              }

              else
              {
                // move ball back down
                ball->set_position(
                    glm::vec2(ball_position.x, ball_position.y + penetration));
              }
            }
          }
        }
      }
    }

    // also check collisions on PowerUps and if so, activate them
    for (auto &power_up : power_ups)
    {
      if (!power_up.is_destroyed())
      {
        // first check if powerup passed bottom edge, if so: keep as
        // inactive and destroy
        if (power_up.get_position().y >= window_height)
          power_up.set_destroyed(true);

        if (check_collision(*player, power_up))
        {
          // collided with player, now activate powerup
          activate_power_up(power_up);
          power_up.set_destroyed(true);
          power_up.set_activated(true);
          audio_source_powerup->play();
        }
      }
    }

    // check collisions for player pad (unless stuck)
    const auto result = check_collision(*ball, *player);
    if (!ball->is_stuck() && result.is_collision)
    {
      // check where it hit the board, and change velocity based on where it hit
      // the board
      const auto center_board =
          player->get_position().x + player->get_size().x / 2.0f;
      const auto distance =
          (ball->get_position().x + ball->get_radius()) - center_board;
      const auto percentage = distance / (player->get_size().x / 2.0f);

      // then move accordingly
      const auto strength     = 2.0f;
      const auto old_velocity = ball->get_velocity();
      ball->set_velocity(
          glm::vec2(INITIAL_BALL_VELOCITY.x * percentage * strength,
                    old_velocity.y));

      // Keep speed consistent over both axes (multiply by length of
      // old velocity, so total strength is not changed)
      ball->set_velocity(glm::normalize(ball->get_velocity()) *
                         glm::length(old_velocity));

      // fix sticky paddle
      ball->set_velocity(glm::vec2(ball->get_velocity().x,
                                   -1.0f * glm::abs(ball->get_velocity().y)));

      audio_source_bleep->play();
    }

    for (auto &power_up : power_ups)
    {
      if (!power_up.is_destroyed())
      {
        if (power_up.get_position().y >= window_height)
          power_up.set_destroyed(true);
        if (check_collision(*player, power_up))
        {
          // collided with player, now activate powerup
          activate_power_up(power_up);
          power_up.set_destroyed(true);
          power_up.set_activated(true);
        }
      }
    }
  }

  bool Game::check_collision(const GameObject &one, const GameObject &two)
  {
    // Collision x-axis?
    bool collisionX =
        one.get_position().x + one.get_size().x >= two.get_position().x &&
        two.get_position().x + two.get_size().x >= one.get_position().x;
    // Collision y-axis?
    bool collisionY =
        one.get_position().y + one.get_size().y >= two.get_position().y &&
        two.get_position().y + two.get_size().y >= one.get_position().y;
    // Collision only if on both axes
    return collisionX && collisionY;
  }

  Collision Game::check_collision(const BallObject &one, const GameObject &two)
  {
    // get center point circle first
    glm::vec2 center(one.get_position() + one.get_radius());
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.get_size().x / 2.0f,
                                two.get_size().y / 2.0f);
    glm::vec2 aabb_center(two.get_position().x + aabb_half_extents.x,
                          two.get_position().y + aabb_half_extents.y);
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped =
        glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to
    // circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if
    // length <= radius
    difference = closest - center;

    if (glm::length(difference) < one.get_radius())
      return Collision(true, calc_vector_direction(difference), difference);
    else
      return Collision(false, Direction::UP, glm::vec2(0.0f, 0.0f));
  }

  void Game::reset_level()
  {
    ASSERT(level < game_levels.size());
    this->game_levels[level].reset();
    lives_count = lives_max;
  }

  void Game::reset_player()
  {
    player->set_size(PLAYER_SIZE);
    player->set_position(glm::vec2(window_width / 2.0f - PLAYER_SIZE.x / 2.0f,
                                   window_height - PLAYER_SIZE.y));
    player->set_color(glm::vec4(1.0f));

    ball->reset(player->get_position() +
                    glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS,
                              -(BALL_RADIUS * 2.0f)),
                INITIAL_BALL_VELOCITY);
    ball->set_sticky(false);
    ball->set_pass_through(false);

    post_processor->set_chaos(false);
    post_processor->set_shake(false);
    post_processor->set_confuse(false);
  }

  Direction Game::calc_vector_direction(const glm::vec2 target)
  {
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // up
        glm::vec2(1.0f, 0.0f),  // right
        glm::vec2(0.0f, -1.0f), // down
        glm::vec2(-1.0f, 0.0f)  // left
    };
    float        max        = 0.0f;
    unsigned int best_match = -1;

    for (unsigned int i = 0; i < 4; ++i)
    {
      float dot_product = glm::dot(glm::normalize(target), compass[i]);
      if (dot_product > max)
      {
        max        = dot_product;
        best_match = i;
      }
    }
    return static_cast<Direction>(best_match);
  }

  void Game::init_particle_generator()
  {
    // ASSERT(renderer);

    particle_generator = std::make_unique<ParticleGenerator>(
        ResourceManager::get_shader("particle"),
        ResourceManager::get_texture("particle"),
        500,
        renderer);
  }

  void Game::init_post_processor()
  {
    // ASSERT(renderer);

    post_processor = std::make_unique<PostProcessor>(
        renderer,
        ResourceManager::get_shader("post-processing"),
        window_width,
        window_height);
  }

  void Game::spawn_power_ups(GameObject & block)
  {
    if (power_up_should_spawn(75)) // 1 in 75 chance
    {
      Log().d(LOG_TAG) << "Spawn speed powerup";
      power_ups.push_back(PowerUp(PowerUp::Type::SPEED,
                                  glm::vec3(0.5f, 0.5f, 1.0f),
                                  0.0f,
                                  block.get_position(),
                                  texture_speed));
    }
    if (power_up_should_spawn(75))
    {
      Log().d(LOG_TAG) << "Spawn sticky powerup";
      power_ups.push_back(PowerUp(PowerUp::Type::STICKY,
                                  glm::vec3(1.0f, 0.5f, 1.0f),
                                  20.0f,
                                  block.get_position(),
                                  texture_sticky));
    }
    if (power_up_should_spawn(75))
    {
      Log().d(LOG_TAG) << "Spawn pass through powerup";
      power_ups.push_back(PowerUp(PowerUp::Type::PASS_THROUGH,
                                  glm::vec3(0.5f, 1.0f, 0.5f),
                                  10.0f,
                                  block.get_position(),
                                  texture_passthrough));
    }
    if (power_up_should_spawn(75))
    {
      Log().d(LOG_TAG) << "Spawn increase powerup";
      power_ups.push_back(PowerUp(PowerUp::Type::PAD_SIZE_INCREASE,
                                  glm::vec3(1.0f, 0.6f, 0.4),
                                  0.0f,
                                  block.get_position(),
                                  texture_increase));
    }
    if (power_up_should_spawn(15)) // negative powerups should spawn more often
    {
      Log().d(LOG_TAG) << "Spawn confuse powerup";
      power_ups.push_back(PowerUp(PowerUp::Type::CONFUSE,
                                  glm::vec3(1.0f, 0.3f, 0.3f),
                                  15.0f,
                                  block.get_position(),
                                  texture_confuse));
    }
    if (power_up_should_spawn(15))
    {
      Log().d(LOG_TAG) << "Spawn chaos powerup";
      power_ups.push_back(PowerUp(PowerUp::Type::CHAOS,
                                  glm::vec3(0.9f, 0.25f, 0.25f),
                                  15.0f,
                                  block.get_position(),
                                  texture_chaos));
    }
  }

  void Game::update_power_ups(float delta_time)
  {
    for (auto &power_up : power_ups)
    {
      power_up.set_position(glm::vec2(
          power_up.get_position().x + power_up.get_velocity().x * delta_time,
          power_up.get_position().y + power_up.get_velocity().y * delta_time));

      if (power_up.is_activated())
      {
        power_up.set_duration(power_up.get_duration() - delta_time);

        if (power_up.get_duration() <= 0.0f)
        {
          // remove powerup from list (will later be removed)
          power_up.set_activated(false);
          // deactivate effects
          if (power_up.get_type() == PowerUp::Type::STICKY)
          {
            if (!is_other_power_up_active(power_ups, PowerUp::Type::STICKY))
            { // only reset if no other PowerUp of type sticky is active
              ball->set_sticky(false);
              player->set_color(glm::vec3(1.0f));
            }
          }
          else if (power_up.get_type() == PowerUp::Type::PASS_THROUGH)
          {
            if (!is_other_power_up_active(power_ups,
                                          PowerUp::Type::PASS_THROUGH))
            { // only reset if no other PowerUp of type pass-through is active
              ball->set_pass_through(false);
              ball->set_color(glm::vec3(1.0f));
            }
          }
          else if (power_up.get_type() == PowerUp::Type::CONFUSE)
          {
            if (!is_other_power_up_active(power_ups, PowerUp::Type::CONFUSE))
            { // only reset if no other PowerUp of type confuse is active
              post_processor->set_confuse(false);
            }
          }
          else if (power_up.get_type() == PowerUp::Type::CHAOS)
          {
            if (!is_other_power_up_active(power_ups, PowerUp::Type::CHAOS))
            { // only reset if no other PowerUp of type chaos is active
              post_processor->set_chaos(false);
            }
          }
        }
      }
    }
    power_ups.erase(std::remove_if(power_ups.begin(),
                                   power_ups.end(),
                                   [](const PowerUp &power_up) {
                                     return power_up.is_destroyed() &&
                                            !power_up.is_activated();
                                   }),
                    power_ups.end());
  }

  bool Game::power_up_should_spawn(unsigned chance)
  {
    unsigned int random = rand() % chance;
    return random == 0;
  }

  void Game::activate_power_up(PowerUp & power_up)
  {
    switch (power_up.get_type())
    {
    case PowerUp::Type::CHAOS:
      post_processor->set_chaos(true);
      break;
    case PowerUp::Type::CONFUSE:
      post_processor->set_confuse(true);
      break;
    case PowerUp::Type::PAD_SIZE_INCREASE:
      player->set_size(
          glm::vec2(player->get_size().x + 50, player->get_size().y));
      break;
    case PowerUp::Type::PASS_THROUGH:
      ball->set_pass_through(true);
      ball->set_color(glm::vec3(1.0f, 0.5f, 0.5f));
      break;
    case PowerUp::Type::SPEED:
      ball->set_velocity(glm::vec2(ball->get_velocity().x * 1.2,
                                   ball->get_velocity().y * 1.2));
      break;
    case PowerUp::Type::STICKY:
      ball->set_sticky(true);
      ball->set_color(glm::vec3(1.0f, 0.5f, 1.0f));
      break;
    default:
      FAIL;
    }
  }

  bool Game::is_other_power_up_active(const std::vector<PowerUp> &power_ups,
                                      const PowerUp::Type         type) const
  {
    for (const auto &power_up : power_ups)
    {
      if (power_up.is_activated())
        if (power_up.get_type() == type)
          return true;
    }
    return false;
  }

  void Game::load_audio()
  {
    ResourceManager::load_audio("audio/solid.wav", "solid");
    ResourceManager::load_audio("audio/breakout.wav", "breakout");
    ResourceManager::load_audio("audio/bleep.wav", "bleep");
    ResourceManager::load_audio("audio/powerup.wav", "powerup");
    ResourceManager::load_audio("audio/nonsolid.wav", "nonsolid");
  }

  void Game::configure_audio()
  {
    audio_source_solid =
        std::make_unique<AudioSource>(*ResourceManager::get_audio("solid"));
    audio_source_breakout =
        std::make_unique<AudioSource>(*ResourceManager::get_audio("breakout"));
    audio_source_bleep =
        std::make_unique<AudioSource>(*ResourceManager::get_audio("bleep"));
    audio_source_powerup =
        std::make_unique<AudioSource>(*ResourceManager::get_audio("powerup"));
    audio_source_nonsolid =
        std::make_unique<AudioSource>(*ResourceManager::get_audio("nonsolid"));
  }

  void Game::update_audio()
  {
    // Play background music forever
    auto state = audio_source_breakout->get_state();
    if (state == AudioSource::State::STOPPED)
    {
      audio_source_breakout->play();
    }
  }

  void Game::init_text_renderer()
  {

    text_renderer =
        std::make_unique<TextRenderer>(renderer,
                                       ResourceManager::get_shader("text"),
                                       window_width,
                                       window_height);
    text_renderer->load("fonts/ocraext.ttf", 24);
  }
