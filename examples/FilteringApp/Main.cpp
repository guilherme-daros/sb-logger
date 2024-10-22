#include "logger/Logger.hpp"

namespace {
using Main = sb::logger::Logger<"Main">;
}

int main(int argc, char* argv[]) {
  Main::logging_level = sb::logger::Level::Debug;

  Main::Debug() << "Debug Log" << std::endl;
  Main::Info() << "Info Log" << std::endl;
  Main::Warning() << "Warning Log" << std::endl;
  Main::Error() << "Error Log" << std::endl;

  return 0;
}
