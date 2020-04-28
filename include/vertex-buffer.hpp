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
  enum class Usage
  {
    STATIC,
    DYNAMIC
  };

  VertexBuffer(const std::size_t size);

  VertexBuffer(const std::vector<Vertex2D> &data, Usage usage = Usage::STATIC);

  VertexBuffer(VertexBuffer &&vertex_buffer);

  VertexBuffer(const VertexBuffer &vertex_buffer);

  void operator=(const VertexBuffer &vertex_buffer);

  void operator=(VertexBuffer &&vertex_buffer);

  ~VertexBuffer();

  void bind() const;

  void unbind() const;

  const std::vector<Vertex2D> &get_vertices() const { return data; }

  unsigned get_count() const;

  unsigned get_id() const { return id; }

  void set_sub_data(std::size_t                  offset,
                    std::size_t                  size,
                    const std::vector<Vertex2D> &data);

private:
  unsigned int          id    = 0;
  unsigned int          count = 0;
  std::vector<Vertex2D> data;
};
