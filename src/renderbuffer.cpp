#include "renderbuffer.hpp"

#include "renderer.hpp"

Renderbuffer::Renderbuffer(unsigned width, unsigned height)
{
  GL_CALL(glGenRenderbuffers(1, &id));

  bind();

  GL_CALL(glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                                           4,
                                           GL_RGB,
                                           width,
                                           height));
}

Renderbuffer::~Renderbuffer() { GL_CALL(glDeleteRenderbuffers(1, &id)); }

void Renderbuffer::bind() { GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, id)); }

void Renderbuffer::unbind() { GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0)); }
