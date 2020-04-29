#pragma once

#include <memory>

// clang-format off
// Gl needs to be included before glfw
#include "opengl-util.hpp"
#include <GLFW/glfw3.h>
// clang-format on

#include "vertex-array.hpp"

class Renderer
{
public:
  void draw(const VertexArray &vertex_array) const;

  void blend_func(GLenum sfactor, GLenum dfactor);

  void clear(GLbitfield flags);

  void clear_color(float red, float green, float blue, float alpha);

  void set_viewport(const int         x,
                    const int         y,
                    const std::size_t width,
                    const std::size_t height) const;

  void init(GLADloadproc loadProc);

  void blit_framebuffer(int        srcx0,
                        int        srcy0,
                        int        srcx1,
                        int        srcy1,
                        int        dstx0,
                        int        dsty0,
                        int        dstx1,
                        int        dsty1,
                        GLbitfield mask,
                        GLenum     filter);

  void teardown();

private:
  void init_glad(GLADloadproc loadProc);

  void setup();
};
