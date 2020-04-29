#include <string>

#include "log.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "vertex-array.hpp"
#include "vertex-buffer-layout.hpp"

static const std::string LOG_TAG = "VertexArray";

VertexArray::VertexArray() { glGenVertexArrays(1, &id); }

VertexArray::VertexArray(VertexArray &&vertex_array)
{
  id             = vertex_array.id;
  vertex_buffers = vertex_array.vertex_buffers;

  vertex_array.id = 0;
}

VertexArray::~VertexArray()
{
  Log().d(LOG_TAG) << "Delete vertex array with id: " << id;
  GL_CALL(glDeleteVertexArrays(1, &id));
}

void VertexArray::add_buffer_by_ref(
    VertexBuffer &            vertex_buffer,
    const VertexBufferLayout &vertex_buffer_layout)
{
  std::size_t offset = 0;

  bind();
  vertex_buffer.bind();
  const auto &elements = vertex_buffer_layout.getElements();

  for (unsigned int i = 0; i < elements.size(); ++i)
  {
    const auto &element = elements[i];
    GL_CALL(glEnableVertexAttribArray(i));

    auto index      = i;
    auto size       = element.getSize();
    auto type       = element.getType();
    auto normalized = element.getNormalized();
    auto stride     = vertex_buffer_layout.getStride();

    // std::string log =
    //     "Create vertex attribute pointer with index: " +
    //     std::to_string(index) +
    //     ", " + "size: " + std::to_string(size) +
    //     ", type: " + std::to_string(type) +
    //     ", normalized: " + std::to_string(normalized) +
    //     ", stride: " + std::to_string(stride) +
    //     ", offset: " + std::to_string(offset);

    // Log::logDebug(LOG_TAG, log);

    GL_CALL(glVertexAttribPointer(index,
                                  size,
                                  type,
                                  normalized,
                                  stride,
                                  reinterpret_cast<const void *>(offset)));

    offset += element.getSize() *
              VertexBufferLayoutElement::getSizeOfType(element.getType());
  }

  count += vertex_buffer.get_count();
}

void VertexArray::add_buffer(VertexBuffer              vertex_buffer,
                             const VertexBufferLayout &vertex_buffer_layout)
{
  std::size_t offset = 0;

  bind();
  vertex_buffer.bind();
  const auto &elements = vertex_buffer_layout.getElements();

  for (unsigned int i = 0; i < elements.size(); ++i)
  {
    const auto &element = elements[i];
    GL_CALL(glEnableVertexAttribArray(i));

    auto index      = i;
    auto size       = element.getSize();
    auto type       = element.getType();
    auto normalized = element.getNormalized();
    auto stride     = vertex_buffer_layout.getStride();

    // std::string log =
    //     "Create vertex attribute pointer with index: " +
    //     std::to_string(index) +
    //     ", " + "size: " + std::to_string(size) +
    //     ", type: " + std::to_string(type) +
    //     ", normalized: " + std::to_string(normalized) +
    //     ", stride: " + std::to_string(stride) +
    //     ", offset: " + std::to_string(offset);

    // Log::logDebug(LOG_TAG, log);

    GL_CALL(glVertexAttribPointer(index,
                                  size,
                                  type,
                                  normalized,
                                  stride,
                                  reinterpret_cast<const void *>(offset)));

    offset += element.getSize() *
              VertexBufferLayoutElement::getSizeOfType(element.getType());
  }
  vertex_buffers.push_back(std::move(vertex_buffer));

  count += vertex_buffer.get_count();
}

unsigned VertexArray::get_count() const { return count; }

void VertexArray::bind() const { GL_CALL(glBindVertexArray(id)); }

void VertexArray::unbind() const { GL_CALL(glBindVertexArray(0)); }
