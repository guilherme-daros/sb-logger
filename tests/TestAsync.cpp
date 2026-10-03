#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>

#include "logger/Logger.hpp"
#include "logger/config/Async.hpp"
#include "logger/config/Output.hpp"

TEST(AsyncTest, OffloadedFormattingAndFlush) {
  constexpr const char* log_filename = "test_async_output.log";

  using AsyncLogger = sb::logger::Logger<"TestAsyncDomain",
                                          sb::logger::config::File<"test_async_output.log">,
                                          sb::logger::config::Async>;
  AsyncLogger::logging_level = sb::logger::Level::Info;

  AsyncLogger::Info() << "Async offloaded test message " << 123;
  AsyncLogger::flush();

  ASSERT_TRUE(std::filesystem::exists(log_filename));

  std::ifstream infile(log_filename);
  std::string line;
  bool found = false;
  while (std::getline(infile, line)) {
    if (line.find("TestAsyncDomain") != std::string::npos &&
        line.find("Async offloaded test message 123") != std::string::npos) {
      found = true;
      break;
    }
  }
  infile.close();

  EXPECT_TRUE(found);
  std::filesystem::remove(log_filename);
}
