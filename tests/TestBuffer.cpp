#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>

#include "logger/Logger.hpp"
#include "logger/config/Buffer.hpp"
#include "logger/config/Output.hpp"

TEST(BufferTest, SizeCalculation) {
  using Buf50 = sb::logger::config::Buffer<sb::logger::config::BufferConfig<50>>;
  EXPECT_EQ(Buf50::max_size, 50u);

  using Buf0 = sb::logger::config::Buffer<>;
  EXPECT_EQ(Buf0::max_size, 0u);
}

TEST(BufferTest, AutoFlushing) {
  constexpr const char* log_filename = "test_buffer_autoflush.log";
  if (std::filesystem::exists(log_filename)) {
    std::filesystem::remove(log_filename);
  }

  {
    using BufferedLogger = sb::logger::Logger<"AutoFlushDomain",
                                               sb::logger::config::File<"test_buffer_autoflush.log">,
                                               sb::logger::config::BufferConfig<40>>;
    BufferedLogger::logging_level = sb::logger::Level::Info;

    // A single formatted log entry will exceed 40 characters
    BufferedLogger::Info() << "Auto flush message content";
  }

  // File should have received content automatically via auto-flush
  ASSERT_TRUE(std::filesystem::exists(log_filename));

  std::ifstream infile(log_filename);
  std::string line;
  std::getline(infile, line);
  infile.close();

  EXPECT_TRUE(line.find("AutoFlushDomain") != std::string::npos);
  EXPECT_TRUE(line.find("Auto flush message content") != std::string::npos);

  std::filesystem::remove(log_filename);
}

TEST(BufferTest, ManualFlushing) {
  constexpr const char* log_filename = "test_buffer_manualflush.log";
  if (std::filesystem::exists(log_filename)) {
    std::filesystem::remove(log_filename);
  }

  using BufferedLogger = sb::logger::Logger<"ManualFlushDomain",
                                             sb::logger::config::File<"test_buffer_manualflush.log">,
                                             sb::logger::config::BufferConfig<4000>>;
  BufferedLogger::logging_level = sb::logger::Level::Info;

  BufferedLogger::Info() << "Buffered message prior to flush";

  // Check file size before manual flush (should be 0 or non-existent)
  if (std::filesystem::exists(log_filename)) {
    EXPECT_EQ(std::filesystem::file_size(log_filename), 0u);
  }

  BufferedLogger::flush();

  ASSERT_TRUE(std::filesystem::exists(log_filename));
  EXPECT_GT(std::filesystem::file_size(log_filename), 0u);

  std::ifstream infile(log_filename);
  std::string line;
  std::getline(infile, line);
  infile.close();

  EXPECT_TRUE(line.find("Buffered message prior to flush") != std::string::npos);

  std::filesystem::remove(log_filename);
}
