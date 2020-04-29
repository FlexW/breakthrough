#include "vertex-buffer.hpp"
#include "log.hpp"
#include "renderer.hpp"

static const std::string LOG_TAG = "VertexBuffer";

VertexBuffer::VertexBuffer(const std::size_t size) : count(size), data()
{
  GL_CALL(glGenBuffers(1, &id));
  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));

  GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

VertexBuffer::VertexBuffer(const std::vector<Vertex2D> &data, Usage)
    : count(data.size()),
      data(data)
{
  GL_CALL(glGenBuffers(1, &id));
  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));

  // TODO: Take usage into account
  GL_CALL(glBufferData(GL_ARRAY_BUFFER,
                       data.size() * sizeof(Vertex2D),
                       &data[0],
                       GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(VertexBuffer &&vertex_buffer)
{
  data  = vertex_buffer.data;
  count = vertex_buffer.count;
  id    = vertex_buffer.id;

  vertex_buffer.id = 0;
}

VertexBuffer::VertexBuffer(const VertexBuffer &vertex_buffer)
{
  data  = vertex_buffer.data;
  count = vertex_buffer.count;
  id    = vertex_buffer.id;
}

void VertexBuffer::operator=(const VertexBuffer &vertex_buffer)
{
  data  = vertex_buffer.data;
  count = vertex_buffer.count;
  id    = vertex_buffer.id;
}

void VertexBuffer::operator=(VertexBuffer &&vertex_buffer)
{
  data  = vertex_buffer.data;
  count = vertex_buffer.count;
  id    = vertex_buffer.id;

  vertex_buffer.id = 0;
}

VertexBuffer::~VertexBuffer()
{
  Log().d(LOG_TAG) << "Delete vertex buffer with id: " << id;
  GL_CALL(glDeleteBuffers(1, &id));
}

void VertexBuffer::bind() const { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id)); }

void VertexBuffer::unbind() const { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

unsigned VertexBuffer::get_count() const { return count; }

void VertexBuffer::set_sub_data(std::size_t                  offset,
                                std::size_t                  size,
                                const std::vector<Vertex2D> &data)
{
  bind();
  GL_CALL(glBufferSubData(GL_ARRAY_BUFFER,
                          static_cast<GLintptr>(offset),
                          static_cast<GLsizeiptr>(size),
                          static_cast<const GLvoid *>(&data[0])));

  unbind();
}
