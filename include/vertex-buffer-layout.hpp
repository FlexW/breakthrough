#pragma once

#include <vector>

class VertexBufferLayoutElement
{
public:
  VertexBufferLayoutElement(std::size_t size, int type, unsigned int normalized)
      : size(size), type(type), normalized(normalized)
  {
  }

  std::size_t  getSize() const { return size; }
  int          getType() const { return type; }
  unsigned int getNormalized() const { return normalized; }

  static int getSizeOfType(int type);

private:
  std::size_t  size       = 0;
  unsigned int type       = 0;
  unsigned int normalized = 0;
};

class VertexBufferLayout
{
public:
  const std::vector<VertexBufferLayoutElement> getElements() const
  {
    return elements;
  }

  std::size_t getStride() const { return stride; }

  void pushFloat(std::size_t size);

  void pushUInt(std::size_t size);

  void setStride(const std::size_t stride) { this->stride = stride; }

private:
  std::size_t                            stride = 0;
  std::vector<VertexBufferLayoutElement> elements;
};
