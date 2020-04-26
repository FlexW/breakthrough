#include "asseration.hpp"

void assert_failed(const char *expr, const char *file, unsigned line)
{
  std::cerr << "Asseration failed in " << file << " on line " << line << ":"
            << expr << std::endl;
  exit(1);
}
