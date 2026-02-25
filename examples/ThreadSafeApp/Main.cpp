#include <thread>

#include "logger/Logger.hpp"

namespace {
using Main = sb::logger::Logger<"Main">;
using Test = sb::logger::Logger<"Test">;
constexpr auto num = 1000;
}  // namespace

auto runnable() {
  for (auto i = 0; i < num; i++) {
    Test::Info() << " Logging: " << i;
  }
};

int main(int argc, char* argv[]) {
  std::array<std::thread, num> th;

  for (auto i = 0; i < num; i++) {
    th[i] = std::thread(runnable);
  }

  for (auto i = 0; i < num; i++) {
    th[i].join();
  }

  return 0;
}
