#include <gtest/gtest.h>
#include "logger/Level.hpp"
#include "logger/Logger.hpp"

TEST(LevelTest, ToString) {
  EXPECT_EQ(sb::logger::to_string(sb::logger::Level::None), "None");
  EXPECT_EQ(sb::logger::to_string(sb::logger::Level::Error), "E");
  EXPECT_EQ(sb::logger::to_string(sb::logger::Level::Warning), "W");
  EXPECT_EQ(sb::logger::to_string(sb::logger::Level::Info), "I");
  EXPECT_EQ(sb::logger::to_string(sb::logger::Level::Debug), "D");
}

TEST(LevelTest, ToColor) {
  EXPECT_EQ(sb::logger::to_color(sb::logger::Level::None), "\033[0m");
  EXPECT_EQ(sb::logger::to_color(sb::logger::Level::Error), "\033[1;41m");
  EXPECT_EQ(sb::logger::to_color(sb::logger::Level::Warning), "\033[1;43m");
  EXPECT_EQ(sb::logger::to_color(sb::logger::Level::Info), "\033[1;42m");
  EXPECT_EQ(sb::logger::to_color(sb::logger::Level::Debug), "\033[1;44m");
}

TEST(LevelTest, ThresholdFiltering) {
  using TestLogger = sb::logger::Logger<"FilterTest">;
  
  TestLogger::logging_level = sb::logger::Level::Warning;
  EXPECT_EQ(TestLogger::logging_level, sb::logger::Level::Warning);

  TestLogger::logging_level = sb::logger::Level::Debug;
  EXPECT_EQ(TestLogger::logging_level, sb::logger::Level::Debug);
}
