#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

struct Vertex2D
{
  glm::vec2 position      = glm::vec2(0.0f);
  glm::vec2 textureCoords = glm::vec2(0.0f);
};

class VertexBuffer
{
public:
  VertexBuffer(const std::vector<Vertex2D> &data);

  VertexBuffer(VertexBuffer &&vertex_buffer);

  VertexBuffer(const VertexBuffer &vertex_buffer);

  void operator=(const VertexBuffer &vertex_buffer);

  ~VertexBuffer();

  void bind() const;

  void unbind() const;

  const std::vector<Vertex2D> &get_vertices() const { return data; }

  unsigned get_count() const;

private:
  unsigned int        id    = 0;
  unsigned int        count = 0;
  std::vector<Vertex2D> data;
};
