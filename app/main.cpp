#include <iostream>

#include "game.hpp"
#include "log.hpp"

int main()
{
  start_log_system();

  auto res = Game(1280, 720).run();

  stop_log_system();

  return res;
}
