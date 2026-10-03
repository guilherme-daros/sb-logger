#include "logger/Level.hpp"
#include "logger/Logger.hpp"
#include "logger/config/Output.hpp"

namespace {
using console_logger = sb::logger::Logger<"Console">;
using file_logger = sb::logger::Logger<"FileApp", sb::logger::config::File<"config_app.log">>;

}  // namespace

int main() {
  console_logger::Info() << "Console logging works!";
  file_logger::Info() << "File logging works!";
  return 0;
}
