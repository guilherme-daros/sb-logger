#include "logger/Logger.hpp"

namespace {
using Main = sb::logger::Logger<"Main">;
}

int main(int argc, char* argv[]) {
  Main::logging_level = sb::logger::Level::Debug;

  Main::Debug() << "Debug Log";
  Main::Info() << "Info Log";
  Main::Warning() << "Warning Log";
  Main::Error() << "Error Log";

  return 0;
}
