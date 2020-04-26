#include <stdexcept>

#include "renderer.hpp"

void Renderer::draw(const VertexArray &vertex_array) const
{
  vertex_array.bind();
  glDrawArrays(GL_TRIANGLES, 0, vertex_array.get_count());
}

void Renderer::clear(GLbitfield flags) { glClear(flags); }

void Renderer::clear_color(float red, float green, float blue, float alpha)
{
  glClearColor(red, green, blue, alpha);
}

void Renderer::set_viewport(const int         x,
                            const int         y,
                            const std::size_t width,
                            const std::size_t height) const
{
  glViewport(x, y, width, height);
}

void Renderer::init(GLADloadproc loadProc)
{
  init_glad(loadProc);
  setup();
}

void Renderer::teardown() {}

void Renderer::init_glad(GLADloadproc loadProc)
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
