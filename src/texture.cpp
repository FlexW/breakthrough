#include "texture.hpp"

Texture2D::Texture2D(const unsigned       width,
                     const unsigned       height,
                     const unsigned char *data,
                     unsigned             internal_format,
                     unsigned             image_format)
    : width(width), height(height), internal_format(internal_format),
      image_format(image_format), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT),
      filter_min(GL_LINEAR), filter_max(GL_LINEAR)
{
  glGenTextures(1, &this->id);
  generate(data);
}

Texture2D::~Texture2D() { glDeleteTextures(1, &id); }

void Texture2D::generate(const unsigned char *data)
{
  // Create texture
  bind();

  glTexImage2D(GL_TEXTURE_2D,
               0,
               this->internal_format,
               width,
               height,
               0,
               this->image_format,
               GL_UNSIGNED_BYTE,
               data);

  // Set texture wrap and filter modes
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

  // Unbind texture
  unbind();
}

void Texture2D::bind() const { glBindTexture(GL_TEXTURE_2D, this->id); }

void Texture2D::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
