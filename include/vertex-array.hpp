#pragma once

#include <memory>

#include "vertex-buffer-layout.hpp"
#include "vertex-buffer.hpp"

class VertexArray
{
public:
  VertexArray();

  VertexArray(VertexArray &&vertex_array);

  ~VertexArray();

  void add_buffer_by_ref(VertexBuffer &            vertex_buffer,
                         const VertexBufferLayout &vertex_buffer_layout);

  void add_buffer(VertexBuffer              vertex_buffer,
                  const VertexBufferLayout &vertex_buffer_layout);
  void bind() const;

  void unbind() const;

  unsigned get_count() const;

private:
  unsigned int              id = 0;
  std::vector<VertexBuffer> vertex_buffers;
  unsigned                  count = 0;
};
