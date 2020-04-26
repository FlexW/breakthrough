#include "vertex-buffer-layout.hpp"
#include "asseration.hpp"
#include "renderer.hpp"

int VertexBufferLayoutElement::getSizeOfType(int type)
{
  switch (type)
  {
  case GL_FLOAT:
    return sizeof(GL_FLOAT);
  case GL_UNSIGNED_INT:
    return sizeof(GL_UNSIGNED_INT);
  default:
    FAIL;
  }
}

void VertexBufferLayout::pushFloat(std::size_t size)
{
  VertexBufferLayoutElement element(size, GL_FLOAT, GL_FALSE);
  elements.push_back(element);
  stride += VertexBufferLayoutElement::getSizeOfType(GL_FLOAT) * size;
}

void VertexBufferLayout::pushUInt(std::size_t size)
{
  VertexBufferLayoutElement element(size, GL_UNSIGNED_INT, GL_FALSE);
  elements.push_back(element);
  stride += VertexBufferLayoutElement::getSizeOfType(GL_UNSIGNED_INT) * size;
}
