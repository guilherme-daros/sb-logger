#include <iostream>
#include <thread>
#include <chrono>

#include "logger/Logger.hpp"
#include "logger/config/Async.hpp"

using AsyncLogger = sb::logger::Logger<"AsyncApp", sb::logger::config::Async>;

int main() {
  AsyncLogger::logging_level = sb::logger::Level::Debug;

  std::cout << "Starting async logging test..." << std::endl;

  for (int i = 0; i < 1000000; ++i) {
    AsyncLogger::Info() << "Async log message #" << i;
  }

  AsyncLogger::flush();
  std::cout << "Async logs flushed successfully!" << std::endl;

  return 0;
}
