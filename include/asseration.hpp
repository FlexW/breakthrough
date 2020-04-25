#pragma once

#include <iostream>

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
