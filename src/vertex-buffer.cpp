#include "vertex-buffer.hpp"
#include "log.hpp"
#include "renderer.hpp"

static const std::string LOG_TAG = "VertexBuffer";

VertexBuffer::VertexBuffer(const std::vector<Vertex2D> &data)
    : count(data.size()), data(data)
{
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER,
               data.size() * sizeof(Vertex2D),
               &data[0],
               GL_STATIC_DRAW);
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
  glDeleteBuffers(1, &id);
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

unsigned VertexBuffer::get_count() const { return count; }