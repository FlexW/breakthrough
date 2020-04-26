#pragma once

#include <memory>

#include "renderer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex-array.hpp"

class SpriteRenderer
{
public:
  SpriteRenderer(const std::shared_ptr<Renderer> renderer,
                 const std::shared_ptr<Shader>   shader);

  void draw_sprite(const std::shared_ptr<Texture2D> texture,
                   const glm::vec2 &                position,
                   const glm::vec2 &                size   = glm::vec2(10.0f),
                   const float                      rotate = 0.0f,
                   const glm::vec3 &                color  = glm::vec3(1.0f));

private:
  const std::shared_ptr<Renderer> renderer;
  const std::shared_ptr<Shader>   shader;
  VertexArray                     vertex_array;

  void init_render_data();
};
