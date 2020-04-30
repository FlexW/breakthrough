#include "log.hpp"

#include <list>

std::queue<std::string> log_queue;

static std::unique_ptr<std::thread> log_thread;

static bool kill_log_thread = false;

static std::stringstream log_batch;

static void read_log_queue()
{
  while (!kill_log_thread)
  {
    if (log_queue.empty())
    {
      std::this_thread::sleep_for(std::chrono::seconds(2));
      continue;
    }

    // Take everything from queue and output it in one batch
    while (!log_queue.empty())
    {
      log_batch << log_queue.front() << "\n";
      log_queue.pop();
    }

    std::fprintf(stderr, "%s\n", log_batch.str().c_str());
    std::fflush(stderr);
    log_batch.clear();
  }
}

void start_log_system()
{
  kill_log_thread = false;
  log_thread      = std::make_unique<std::thread>(read_log_queue);
}

void stop_log_system()
{
  // Log thread should log all messages
  while (!log_queue.empty())
    ;

  // Kill log thread
  kill_log_thread = true;
  log_thread->join();
}

Log::Log() {}

Log::~Log() { log_queue.push(log_buffer.str()); }

std::ostringstream &Log::i(const std::string &tag)
{
  log_buffer << "[\033[1;34mINFO\033[0m] (" << tag << ") ";
  return log_buffer;
}

std::ostringstream &Log::w(const std::string &tag)
{
  log_buffer << "[\033[1;33mWARNING\033[0m] (" << tag << ") ";
  return log_buffer;
}

std::ostringstream &Log::e(const std::string &tag)
{
  log_buffer << "[\033[1;31mERROR\033[0m] (" << tag << ") ";
  return log_buffer;
}

std::ostringstream &Log::d(const std::string &tag)
{
  log_buffer << "[DEBUG] (" << tag << ") ";
  return log_buffer;
}
