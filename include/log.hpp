#pragma once

#include <iostream>
#include <sstream>

class Log
{

public:
  ~Log()
  {
    fprintf(stderr, "%s\n", log_buffer.str().c_str());
    fflush(stderr);
  }

  std::ostringstream &i(const std::string &tag)
  {
    log_buffer << "[\033[1;34mINFO\033[0m] (" << tag << ") ";
    return log_buffer;
  }

  std::ostringstream &w(const std::string &tag)
  {
    log_buffer << "[\033[1;33mWARNING\033[0m] (" << tag << ") ";
    return log_buffer;
  }

  std::ostringstream &e(const std::string &tag)
  {
    log_buffer << "[\033[1;31mERROR\033[0m] (" << tag << ") ";
    return log_buffer;
  }

  std::ostringstream &d(const std::string &tag)
  {
    log_buffer << "[DEBUG] (" << tag << ") ";
    return log_buffer;
  }

private:
  std::ostringstream log_buffer;
};
