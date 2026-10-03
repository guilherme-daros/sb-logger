#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "logger/Logger.hpp"
#include "logger/config/Async.hpp"
#include "logger/config/Defer.hpp"
#include "logger/config/Output.hpp"

TEST(DeferTest, BinaryPacketGeneration) {
  constexpr const char* log_filename = "test_defer_output.bin";

  using DeferLogger = sb::logger::Logger<"TestDeferDomain",
                                         sb::logger::config::File<"test_defer_output.bin">,
                                         sb::logger::config::Defer>;
  DeferLogger::logging_level = sb::logger::Level::Info;

  DeferLogger::Info() << "Binary deferred string " << 999;

  ASSERT_TRUE(std::filesystem::exists(log_filename));
  EXPECT_GE(std::filesystem::file_size(log_filename), sizeof(sb::logger::config::PacketHeader));

  std::filesystem::remove(log_filename);
}

TEST(DeferTest, AsyncDeferComposition) {
  constexpr const char* log_filename = "test_async_defer_output.bin";

  using AsyncDeferLogger = sb::logger::Logger<"AsyncDeferDomain",
                                              sb::logger::config::File<"test_async_defer_output.bin">,
                                              sb::logger::config::Async,
                                              sb::logger::config::Defer>;
  AsyncDeferLogger::logging_level = sb::logger::Level::Info;

  AsyncDeferLogger::Info() << "Async + Defer composition string " << 777;
  AsyncDeferLogger::flush();

  ASSERT_TRUE(std::filesystem::exists(log_filename));
  EXPECT_GE(std::filesystem::file_size(log_filename), sizeof(sb::logger::config::PacketHeader));

  std::filesystem::remove(log_filename);
}
