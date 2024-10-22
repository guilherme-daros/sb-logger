#include <thread>

#include "logger/Logger.hpp"

namespace {
using Main = sb::logger::Logger<"Main">;
using Test = sb::logger::Logger<"Test">;
}  // namespace

auto runnable1() {
  for (auto i = 0; i < 10; i++) {
    Main::Info() << " Logging: " << i << std::endl;
  }
};

auto runnable2() {
  for (auto i = 0; i < 10; i++) {
    Test::Info() << " Logging: " << i << std::endl;
  }
};

int main(int argc, char* argv[]) {
  std::array<std::thread, 10> th1;
  std::array<std::thread, 10> th2;

  for (auto i = 0; i < 10; i++) {
    th1[i] = std::thread(runnable1);
    th2[i] = std::thread(runnable2);
  }

  for (auto i = 0; i < 10; i++) {
    th1[i].join();
    th2[i].join();
  }

  return 0;
}
