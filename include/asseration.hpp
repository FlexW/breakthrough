#pragma once

#include <iostream>

void _assert_failed(const char *expr, const char *file, unsigned line)
{
  std::cout << "Asseration failed in " << file << " on line " << line << ":"
            << expr << std::endl;
}

#define FAIL_WITH_MESSAGE(message)                                             \
  do                                                                           \
  {                                                                            \
    std::cerr << "Asseration failed in " << __FILE__ << " on line "            \
              << __LINE__ << ":" << (message) << std::endl;                    \
    exit(1);                                                                   \
  } while (0)

#define FAIL                                                                   \
  {                                                                            \
    std::cerr << "Asseration failed in " << __FILE__ << " on line "            \
              << __LINE__ << std::endl;                                        \
    exit(1);                                                                   \
  }                                                                            \
  while (0)

#define ASSERT(expr)                                                           \
  (static_cast<bool>(expr) ? void(0)                                           \
                           : _assert_failed(#expr, __FILE__, __LINE__))
