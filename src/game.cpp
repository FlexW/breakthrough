#include "game.hpp"
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

void Game::update(float delta_time) { ball->move(delta_time, window_width); }

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
