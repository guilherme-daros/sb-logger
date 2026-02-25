#include "logger/Level.hpp"
#include "logger/Logger.hpp"
#include "logger/config/Output.hpp"

namespace {
using console_logger = sb::logger::Logger<"Console">;
using termina_logger = sb::logger::Logger<"Termina", sb::logger::config::Terminal>;

}  // namespace

int main() {
  console_logger::Info() << "HOLY THIS WORKS";
  termina_logger::Info() << "HOLY THIS WORKS";
  return 0;
}
