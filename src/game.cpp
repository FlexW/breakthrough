#include "game.hpp"
#include "resource-manager.hpp"

Game::Game(unsigned width, unsigned height)
    : Window("Breaktrough", width, height)
{
  init();
}

Game::~Game() { ResourceManager::clear(); }

void Game::init()
{

  ResourceManager::load_shader("shaders/sprite/sprite.vert",
                               "shaders/sprite/sprite.frag",
                               "sprite");

  const auto projection_matrix = glm::ortho(0.0f,
                                            static_cast<float>(windowWidth),
                                            static_cast<float>(windowHeight),
                                            0.0f,
                                            -1.0f,
                                            1.0f);

  const auto shader = ResourceManager::get_shader("sprite");
  shader->bind();
  shader->setInt("image", 0);
  shader->setMat4("projection_matrix", projection_matrix);

  sprite_renderer = std::make_unique<SpriteRenderer>(renderer, shader);
  ResourceManager::load_texture("textures/awesomeface.png", true, "face");
}

void Game::draw()
{
  sprite_renderer->draw_sprite(ResourceManager::get_texture("face"),
                               glm::vec2(200.0f, 200.0f),
                               glm::vec2(300.0f, 400.0f),
                               45.0f,
                               glm::vec3(0.0f, 1.0f, 0.0f));
}

bool Game::run()
{
  show();
  return true;
}
