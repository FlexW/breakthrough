#include "texture.hpp"
#include "log.hpp"
#include "opengl-util.hpp"

static const std::string LOG_TAG = "Texture2D";

Texture2D::Texture2D(const unsigned       width,
                     const unsigned       height,
                     const unsigned char *data,
                     unsigned             internal_format,
                     unsigned             image_format,
                     unsigned             wrap_s,
                     unsigned             wrap_t)
    : width(width),
      height(height),
      internal_format(internal_format),
      image_format(image_format),
      wrap_s(wrap_s),
      wrap_t(wrap_t),
      filter_min(GL_LINEAR),
      filter_max(GL_LINEAR)
{
  GL_CALL(glGenTextures(1, &this->id));
  generate(data);
}

Texture2D::~Texture2D()
{
  Log().d(LOG_TAG) << "Delete 2d texture with id: " << id;
  GL_CALL(glDeleteTextures(1, &id));
}

void Texture2D::generate(const unsigned char *data)
{
  // Create texture
  bind();

  GL_CALL(glTexImage2D(GL_TEXTURE_2D,
                       0,
                       this->internal_format,
                       width,
                       height,
                       0,
                       this->image_format,
                       GL_UNSIGNED_BYTE,
                       data));

  // Set texture wrap and filter modes
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min));
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max));

  // Unbind texture
  unbind();
}

void Texture2D::bind(unsigned slot) const
{
  GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
  GL_CALL(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture2D::unbind() const { GL_CALL(glBindTexture(GL_TEXTURE_2D, 0)); }
