#include "logger/Logger.hpp"
#include "logger/Timing.hpp"

namespace {
using timestamp_logger = sb::logger::Logger<"TimeStamp", sb::logger::timing::Timestamp>;
using uptime_logger = sb::logger::Logger<"Uptime", sb::logger::timing::Uptime>;
}  // namespace

int main(int argc, char* argv[]) {
  timestamp_logger::Info() << "This has a timestamp" << std::endl;
  uptime_logger::Info() << "This has an uptime " << std::endl;
  return 0;
}
