#pragma once

#include <glad/glad.h>

class Texture2D
{
public:
  unsigned getId() { return id; }

  Texture2D(const unsigned       width,
            const unsigned       height,
            const unsigned char *data,
            const unsigned       internal_format = GL_RGB,
            const unsigned       image_format    = GL_RGB,
            unsigned             wrap_s          = GL_REPEAT,
            unsigned             wrap_t          = GL_REPEAT);

  ~Texture2D();

  void bind(unsigned slot = 0) const;

  void unbind() const;

  unsigned get_id() { return id; }

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

  void generate(const unsigned char *data);
};
