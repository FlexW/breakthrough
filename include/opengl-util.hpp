#pragma once

#include <cstdint>

#include <glad/glad.h>

#define GL_CALL(x)                                                             \
  gl_clear_error();                                                            \
  x;                                                                           \
  gl_check_error(#x, __FILE__, __LINE__)

void gl_clear_error();

void gl_check_error(const char *             function,
                    const char *             file,
                    const std::uint_fast32_t line);
