#include "framebuffer.hpp"
#include "asseration.hpp"
#include "renderer.hpp"

Framebuffer::Framebuffer(const std::shared_ptr<Texture2D> texture)
    : texture(texture)
{
  glGenFramebuffers(1, &id);

  bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D,
                         texture->get_id(),
                         0);

  check_for_errors();
  unbind();
}

Framebuffer::Framebuffer(std::unique_ptr<Renderbuffer> renderbuffer)
    : renderbuffer(std::move(renderbuffer))
{
  glGenFramebuffers(1, &id);

  bind();
  this->renderbuffer->bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER,
                            this->renderbuffer->get_id());

  check_for_errors();
  unbind();
}

Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &id); }

void Framebuffer::bind(Target target)
{
  glBindFramebuffer(target_to_gl_target(target), id);
}

void Framebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Framebuffer::check_for_errors()
{
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    throw std::runtime_error("Failed to initialize Framebuffer");
  }
}

GLenum Framebuffer::target_to_gl_target(Target target)
{
  switch (target)
  {
  case Target::FRAMEBUFFER:
    return GL_FRAMEBUFFER;
  case Target::DRAW_FRAMEBUFFER:
    return GL_DRAW_FRAMEBUFFER;
  case Target::READ_FRAMEBUFFER:
    return GL_READ_FRAMEBUFFER;
  default:
    FAIL;
  }
}
