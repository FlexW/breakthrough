#include <stdexcept>

#include "renderer.hpp"

void Renderer::draw(const VertexArray &vertex_array) const
{
  vertex_array.bind();
  GL_CALL(glDrawArrays(GL_TRIANGLES, 0, vertex_array.get_count()));
}

void Renderer::blend_func(GLenum sfactor, GLenum dfactor)
{
  GL_CALL(glBlendFunc(sfactor, dfactor));
}

void Renderer::clear(GLbitfield flags) { GL_CALL(glClear(flags)); }

void Renderer::clear_color(float red, float green, float blue, float alpha)
{
  GL_CALL(glClearColor(red, green, blue, alpha));
}

void Renderer::set_viewport(const int         x,
                            const int         y,
                            const std::size_t width,
                            const std::size_t height) const
{
  GL_CALL(glViewport(x, y, width, height));
}

void Renderer::blit_framebuffer(int        srcx0,
                                int        srcy0,
                                int        srcx1,
                                int        srcy1,
                                int        dstx0,
                                int        dsty0,
                                int        dstx1,
                                int        dsty1,
                                GLbitfield mask,
                                GLenum     filter)
{
  GL_CALL(glBlitFramebuffer(srcx0,
                            srcy0,
                            srcx1,
                            srcy1,
                            dstx0,
                            dsty0,
                            dstx1,
                            dsty1,
                            mask,
                            filter));
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
  GL_CALL(glEnable(GL_BLEND));
  GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}
