#include <iostream>

#include "logger/Logger.hpp"
#include "logger/config/Defer.hpp"
#include "logger/config/Output.hpp"

using DeferLogger = sb::logger::Logger<"DeferApp", sb::logger::config::Defer, sb::logger::config::Console>;

int main() {
  DeferLogger::logging_level = sb::logger::Level::Debug;

  DeferLogger::Info() << "Deferred log entry string " << 100;
  DeferLogger::Warning() << "Deferred warning message " << 3.14159;

  return 0;
}
