#pragma once

#include <glad/glad.h>

class Texture2D
{
public:
  unsigned getId() { return id; }

  Texture2D();

  ~Texture2D();

  void generate(unsigned width, unsigned height, unsigned char *data);

  void bind() const;

  void unbind() const;

private:
  unsigned id = 0;

  // Texture size
  unsigned width = 0, height = 0;

  unsigned internal_format = 0; // format of texture object
  unsigned image_format    = 0; // format of loaded image

  unsigned wrap_s     = 0; // wrapping mode on S axis
  unsigned wrap_t     = 0; // wrapping mode on T axis
  unsigned filter_min = 0; // filtering mode if texture pixels < screen pixels
  unsigned filter_max = 0; // filtering mode if texture pixels > screen pixels
};
