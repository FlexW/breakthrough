#pragma once

#include <iostream>
#include <queue>
#include <sstream>
#include <thread>

extern std::queue<std::string> log_queue;

class Log
{

public:
  Log();

  ~Log();

  std::ostringstream &i(const std::string &tag);

  std::ostringstream &w(const std::string &tag);

  std::ostringstream &e(const std::string &tag);

  std::ostringstream &d(const std::string &tag);

private:
  std::ostringstream log_buffer;

  Log(Log &)  = delete;
  Log(Log &&) = delete;
};

/**
 * Needs to be called at start of app. Otherwise no logging will
 * occur.
 */
void start_log_system();

/**
 * Stops the log system. Waits until the logger has logged every
 * message.
 */
void stop_log_system();
