#pragma once

#include <memory>

// clang-format off
// Gl needs to be included before glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

// #include "index-buffer.hpp"
// #include "mesh.hpp"
// #include "shader.hpp"
// #include "vertex-array.hpp"

class Renderer
{
public:
  void draw( // const Mesh &mesh, const std::shared_ptr<Shader> shader
  ) const;

  void clear(GLbitfield flags);

  void clearColor(float red, float green, float blue, float alpha);

  void setViewport(const int         x,
                   const int         y,
                   const std::size_t width,
                   const std::size_t height) const;
  void init(GLADloadproc loadProc);

  void teardown();

private:
  void initGlad(GLADloadproc loadProc);

  void setup();
};
