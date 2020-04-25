#include <stdexcept>

#include "renderer.hpp"

void Renderer::draw( // const Mesh &mesh, const std::shared_ptr<Shader> shader
) const
{
}

void Renderer::clear(GLbitfield flags) { glClear(flags); }

void Renderer::clearColor(float red, float green, float blue, float alpha)
{
  glClearColor(red, green, blue, alpha);
}

void Renderer::setViewport(const int         x,
                           const int         y,
                           const std::size_t width,
                           const std::size_t height) const
{
  glViewport(x, y, width, height);
}

void Renderer::init(GLADloadproc loadProc)
{
  initGlad(loadProc);
  setup();
}

void Renderer::teardown() {}

void Renderer::initGlad(GLADloadproc loadProc)
{
  if (!gladLoadGLLoader(loadProc))
  {
    throw std::runtime_error("Glad could not load GL!");
  }
}

void Renderer::setup()
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
