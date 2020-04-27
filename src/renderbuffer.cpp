#include "renderbuffer.hpp"

#include "renderer.hpp"

Renderbuffer::Renderbuffer(unsigned width, unsigned height)
{
  glGenRenderbuffers(1, &id);

  bind();

  glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
}

Renderbuffer::~Renderbuffer() { glDeleteRenderbuffers(1, &id); }

void Renderbuffer::bind() { glBindRenderbuffer(GL_RENDERBUFFER, id); }

void Renderbuffer::unbind() { glBindRenderbuffer(GL_RENDERBUFFER, 0); }
