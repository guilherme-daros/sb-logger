#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>

#include "logger/Logger.hpp"
#include "logger/config/Output.hpp"

TEST(OutputTest, ConceptCheck) {
  static_assert(sb::logger::config::IsOutput<sb::logger::config::Console>);
  static_assert(sb::logger::config::IsOutput<sb::logger::config::File<"test.log">>);
  SUCCEED();
}

TEST(OutputTest, FileOutputSink) {
  constexpr const char* log_filename = "test_output_sink.log";
  
  if (std::filesystem::exists(log_filename)) {
    std::filesystem::remove(log_filename);
  }

  {
    using FileLogger = sb::logger::Logger<"TestFile", sb::logger::config::File<"test_output_sink.log">>;
    FileLogger::logging_level = sb::logger::Level::Info;
    FileLogger::Info() << "Hello file output test!";
  }

  ASSERT_TRUE(std::filesystem::exists(log_filename));

  std::ifstream infile(log_filename);
  std::string line;
  std::getline(infile, line);
  infile.close();

  EXPECT_TRUE(line.find("TestFile") != std::string::npos);
  EXPECT_TRUE(line.find("Hello file output test!") != std::string::npos);

  std::filesystem::remove(log_filename);
}
