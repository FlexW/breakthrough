#include <string>

#include <glad/glad.h>

#include "log.hpp"
#include "opengl-util.hpp"

const static std::string LOG_TAG = "OpenGl";

void gl_clear_error()
{
  while (glGetError() != GL_NO_ERROR)
    ;
}

void gl_check_error(const char *             function,
                    const char *             filename,
                    const std::uint_fast32_t line)
{
  while (GLenum error = glGetError())
  {
    const std::string error_prefix = std::string(filename) + ": " +
                                     std::to_string(line) + " in " + function +
                                     " ";
    switch (error)
    {
    case GL_INVALID_ENUM:
      Log().e(LOG_TAG) << error_prefix << "INVALID_ENUM";
      return;
    case GL_INVALID_VALUE:
      Log().e(LOG_TAG) << error_prefix << "INVALID_VALUE";
      return;
    case GL_INVALID_OPERATION:
      Log().e(LOG_TAG) << error_prefix << "INVALID_OPERATION";
      return;
    default:
      Log().e(LOG_TAG) << error_prefix << "Unknown OpenGL error: " << error;
    }
  }
}
