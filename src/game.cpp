#include "game.hpp"
#include "asseration.hpp"
#include "resource-manager.hpp"

Game::Game(unsigned width, unsigned height)
    : Window("Breaktrough", width, height),
      game_state(GameState::GAME_ACTIVE)
{
  init();
}

Game::~Game() { ResourceManager::clear(); }

void Game::init()
{

  load_shaders();
  configure_shaders();
  init_sprite_renderer();
  load_textures();
  load_levels();
  configure_game_objects();
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
  ball->move(delta_time, window_width);
  update_collisions();

  // Did ball reach bottom edge?
  if (ball->get_position().y >= window_height)
  {
    reset_level();
    reset_player();
  }
}

void Game::render()
{
  switch (game_state)
  {
  case GameState::GAME_ACTIVE:
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

    // Draw ball
    ball->draw(sprite_renderer);
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
  ResourceManager::load_texture("textures/background.jpg", false, "background");
  ResourceManager::load_texture("textures/awesomeface.png", true, "face");
  ResourceManager::load_texture("textures/block.png", false, "block");
  ResourceManager::load_texture("textures/block_solid.png",
                                false,
                                "block_solid");
  ResourceManager::load_texture("textures/paddle.png", true, "paddle");
}

void Game::load_shaders()
{
  ResourceManager::load_shader("shaders/sprite/sprite.vert",
                               "shaders/sprite/sprite.frag",
                               "sprite");
}

void Game::configure_shaders()
{
  const auto projection_matrix = glm::ortho(0.0f,
                                            static_cast<float>(window_width),
                                            static_cast<float>(window_height),
                                            0.0f,
                                            -1.0f,
                                            1.0f);

  const auto shader = ResourceManager::get_shader("sprite");
  shader->bind();
  shader->setInt("image", 0);
  shader->setMat4("projection_matrix", projection_matrix);
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
  const auto player_pos = glm::vec2(window_width / 2.0f - PLAYER_SIZE.x / 2.0f,
                                    window_height - PLAYER_SIZE.y);

  player = std::make_unique<GameObject>(player_pos,
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
          box.set_destroyed(true);
        // collision resolution
        const auto dir         = collision.direction;
        const auto diff_vector = collision.difference_center_closest_point;
        // horizontal collision
        if (dir == Direction::LEFT || dir == Direction::RIGHT)
        {
          const auto ball_velocity = ball->get_velocity();
          // reverse horizontal velocity
          ball->set_velocity(glm::vec2(-ball_velocity.x, ball_velocity.y));
          // relocate
          const auto penetration = ball->get_radius() - std::abs(diff_vector.x);
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
          const auto penetration = ball->get_radius() - std::abs(diff_vector.y);
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
  glm::vec2 aabb_half_extents(two.get_size().x / 2.0f, two.get_size().y / 2.0f);
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
}

void Game::reset_player()
{
  player->set_size(PLAYER_SIZE);
  player->set_position(glm::vec2(window_width / 2.0f - PLAYER_SIZE.x / 2.0f,
                                 window_height - PLAYER_SIZE.y));

  ball->reset(
      player->get_position() +
          glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)),
      INITIAL_BALL_VELOCITY);
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
