#include <iostream>
#include "logger/Logger.hpp"
#include "logger/config/Buffer.hpp"

using Logger = sb::logger::Logger<"BufferApp", sb::logger::config::BufferConfig<2048>>;

int main() {
  Logger::logging_level = sb::logger::Level::Debug;

  for (int i = 0; i < 5; ++i) {
    Logger::Info() << "This is a test message " << i;
  }

  std::cout << "--- Before flush ---" << std::endl;
  // The buffer is not public, so we can't print it directly.
  // We can see the effect of the flush in the output.

  Logger::flush();

  std::cout << "--- After flush ---" << std::endl;

  for (int i = 0; i < 5; ++i) {
    Logger::Info() << "This is another test message " << i;
  }

  Logger::flush();

  std::cout << "--- Testing Auto-Flush ---" << std::endl;
  using AutoFlushLogger = sb::logger::Logger<"AutoFlush", sb::logger::config::BufferConfig<50>>;
  AutoFlushLogger::logging_level = sb::logger::Level::Debug;

  std::cout << "Writing messages to small buffer (capacity 50)..." << std::endl;
  // Each message is > 50 chars formatted, so it should flush automatically on every message or every 1-2 messages
  AutoFlushLogger::Info() << "Auto flush message 1";
  AutoFlushLogger::Info() << "Auto flush message 2";

  return 0;
}