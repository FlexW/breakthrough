#pragma once

#include <memory>

#include "framebuffer.hpp"
#include "renderbuffer.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex-array.hpp"

/**
 * @brief PostProcessor hosts all PostProcessing effects for the
 * Breakout Game.
 *
 * PostProcessor hosts all PostProcessing effects for the Breakout
 * Game. It renders the game on a textured quad after which one can
 * enable specific effects by enabling either the Confuse, Chaos or
 * Shake boolean. It is required to call begin_render() before
 * rendering the game and end_render() after rendering the game for the
 * class to work.
 */
class PostProcessor
{
public:
  PostProcessor(const std::shared_ptr<Renderer> renderer,
                const std::shared_ptr<Shader>   shader,
                unsigned                        width,
                unsigned                        height);
  /**
   * @brief Prepares the postprocessor's framebuffer operations before
   * rendering the game.
   */
  void begin_render();

  /**
   * @brief Should be called after rendering the game, so it stores
   * all the rendered data into a texture object.
   */
  void end_render();

  /**
   * @brief Renders the PostProcessor texture quad as a
   * screen-encompassing large sprite.
   *
   * @param time Time that passed since game start
   */
  void render(float time);

  void set_shake(bool shake) { this->shake = shake; }

  void set_confuse(bool value) { confuse = value; }

  void set_chaos(bool value) { chaos = value; }

private:
  std::shared_ptr<Renderer> renderer;

  std::unique_ptr<Framebuffer> ms_framebuffer;

  std::unique_ptr<Framebuffer> framebuffer;

  VertexArray vertex_array;

  bool confuse = false, chaos = false, shake = false;

  std::shared_ptr<Shader>    post_processing_shader = nullptr;
  std::shared_ptr<Texture2D> texture                = nullptr;
  unsigned                   width = 0, height = 0;

  void init_render_data();
};
