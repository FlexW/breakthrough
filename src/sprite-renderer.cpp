#include "sprite-renderer.hpp"

SpriteRenderer::SpriteRenderer(const std::shared_ptr<Renderer> renderer,
                               const std::shared_ptr<Shader>   shader)
    : renderer(renderer), shader(shader)
{
  init_render_data();
}

void SpriteRenderer::draw_sprite(const std::shared_ptr<Texture2D> texture,
                                 const glm::vec2 &                position,
                                 const glm::vec2 &                size,
                                 const float                      rotate,
                                 const glm::vec3 &                color)
{
  shader->bind();

  glm::mat4 model = glm::mat4(1.0f);
  model           = glm::translate(model, glm::vec3(position, 0.0f));

  // move origin of rotation to center of quad
  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));

  model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));

  model =
      glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  model = glm::scale(model, glm::vec3(size, 1.0f));

  shader->setMat4("model", model);
  shader->setVec3("sprite_color", color);

  texture->bind();

  renderer->draw(vertex_array);
}

void SpriteRenderer::init_render_data()
{
  VertexBuffer vertex_buffer({
      {glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
      {glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
      {glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
      {glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
      {glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
      {glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
  });

  VertexBufferLayout layout;
  layout.pushFloat(2);
  layout.pushFloat(2);

  vertex_array.add_buffer(std::move(vertex_buffer), layout);
}
