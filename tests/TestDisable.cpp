#include <gtest/gtest.h>
#include "logger/Logger.hpp"
#include "logger/config/Disable.hpp"

TEST(DisableTest, DisabledLoggerCompilation) {
  using DisabledLogger = sb::logger::Logger<"DisabledDomain", sb::logger::config::Disable>;

  EXPECT_EQ(DisabledLogger::logging_level, -1);

  // All stream operations should compile seamlessly and be no-ops
  DisabledLogger::Debug() << "Debug message " << 42;
  DisabledLogger::Info() << "Info message " << 3.14;
  DisabledLogger::Warning() << "Warning message " << "test";
  DisabledLogger::Error() << "Error message";

  SUCCEED();
}
