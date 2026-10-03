#include <gtest/gtest.h>
#include "logger/config/Timing.hpp"

TEST(TimingTest, ConceptCheck) {
  static_assert(sb::logger::config::IsTiming<sb::logger::config::Timestamp>);
  static_assert(sb::logger::config::IsTiming<sb::logger::config::Uptime>);
  SUCCEED();
}

TEST(TimingTest, TimestampFormat) {
  sb::logger::config::Timestamp ts;
  auto formatted = ts.get();
  EXPECT_FALSE(formatted.empty());
  EXPECT_EQ(ts.width(), 12u);
}

TEST(TimingTest, UptimeFormat) {
  sb::logger::config::Uptime uptime;
  auto formatted = uptime.get();
  EXPECT_FALSE(formatted.empty());
  EXPECT_GT(uptime.width(), 0u);
}
