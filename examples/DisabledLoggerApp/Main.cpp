#include <iostream>

#include "logger/Logger.hpp"
#include "logger/config/Disable.hpp"

namespace {
using Logger = sb::logger::Logger<"Test", sb::logger::config::Disable>;
// using Logger = sb::logger::Logger<"Test">;
}  // namespace

int main() {
  Logger::Info() << "This should not be printed";
  std::cout << "This should be printed";
  return 0;
}
