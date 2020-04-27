#pragma once

#include <memory>

#include "renderbuffer.hpp"
#include "texture.hpp"

class Framebuffer
{
public:
  enum class Target
  {
    READ_FRAMEBUFFER,
    DRAW_FRAMEBUFFER,
    FRAMEBUFFER
  };

  Framebuffer(const std::shared_ptr<Texture2D> texture);

  Framebuffer(std::unique_ptr<Renderbuffer> renderbuffer);

  ~Framebuffer();

  void bind(Target target = Target::FRAMEBUFFER);

  void unbind();

private:
  unsigned                            id = 0;
  const std::shared_ptr<Texture2D>    texture;
  const std::unique_ptr<Renderbuffer> renderbuffer;

  void check_for_errors();

  GLenum target_to_gl_target(Target target);
};
