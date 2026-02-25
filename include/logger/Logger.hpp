#pragma once

#include <filesystem>
#include <functional>
#include <iomanip>
#include <ios>
#include <iostream>
#include <mutex>
#include <ostream>
#include <source_location>
#include <sstream>
#include <string_view>
#include <thread>

#include "Level.hpp"
#include "config/Buffer.hpp"
#include "config/Output.hpp"
#include "config/Timing.hpp"

#include "types/StringLiteral.hpp"

namespace sb::logger {

namespace utils {
inline auto get_thread_id() -> uint64_t { return std::hash<std::thread::id>{}(std::this_thread::get_id()) % 1000000; }

inline auto get_pos(const std::string_view path) -> std::string_view {
  constexpr char separator = std::filesystem::path::preferred_separator;
  if (auto pos = path.rfind(separator); pos != std::string_view::npos) {
    return path.substr(pos + 1);
  }
  return path;
}

}  // namespace utils

template <types::StringLiteral domain, typename... Configs>
class Logger {
  using Output = config::Output<Configs...>;
  using Timing = config::Timing<Configs...>;
  using Buffer = config::Buffer<Configs...>;

  class Log {
   public:
    Log(Level level, std::ostream &output, std::mutex &mtx, const uint64_t id, const std::source_location file_src)
        : level_(level), output_(output), mtx_(mtx), id_(id), timing_(), enabled_(logging_level >= level_) {
      if (enabled_) {
        file_number_ = utils::get_pos(file_src.file_name());
      }
    }

    ~Log() {
      if (!enabled_) {
        return;
      }

      constexpr auto default_light = std::string_view{"\033[0;m"};
      std::ostringstream context;

      context << "[" << std::setw(timing_.width()) << timing_.get() << "]";
      context << " ";
      context << "[" << std::setw(6) << id_ << "]";
      context << " ";
      context << std::setw(30) << std::right << domain.data;
      context << " ";
      context << to_color(level_) << " " << to_string(level_) << " " << default_light;
      context << " ";
      context << std::setw(20) << std::left << file_number_;
      context << " ";

      if constexpr (Buffer::max_size > 0) {
        std::scoped_lock lock{mtx_};
        buffer_.append(context.str() + os.str() + "\n");
      } else {
        std::scoped_lock lock{mtx_};
        output_ << context.str() + os.str() << std::endl;
      }
    }

    // This captures all data_types, but only if logging is enabled for this level.
    template <typename T>
    Log &operator<<(const T &token) {
      if (enabled_) {
        os << token;
      }
      return *this;
    }

   private:
    std::mutex &mtx_;
    std::ostringstream os;
    std::ostream &output_;
    std::string_view file_number_;
    const uint64_t id_;
    const Level level_;
    const Timing timing_;
    const bool enabled_;
  };
  static Output output_;
  static Buffer buffer_;

 public:
  static Level logging_level;

  static void flush() {
    if constexpr (Buffer::max_size > 0) {
      output_.stream() << buffer_.get_buffer() << std::flush;
      buffer_.clear();
    }
  }

  class Debug : public Log {
   public:
    Debug(const uint64_t id = utils::get_thread_id(),
          const std::source_location file_src = std::source_location::current())
        : Log(Level::Debug, output_.stream(), output_.mutex(), id, file_src) {}
  };

  class Info : public Log {
   public:
    Info(const uint64_t id = utils::get_thread_id(),
         const std::source_location file_src = std::source_location::current())
        : Log(Level::Info, output_.stream(), output_.mutex(), id, file_src) {}
  };

  class Warning : public Log {
   public:
    Warning(const uint64_t id = utils::get_thread_id(),
            const std::source_location file_src = std::source_location::current())
        : Log(Level::Warning, output_.stream(), output_.mutex(), id, file_src) {}
  };

  class Error : public Log {
   public:
    Error(const uint64_t id = utils::get_thread_id(),
          const std::source_location file_src = std::source_location::current())
        : Log(Level::Error, output_.stream(), output_.mutex(), id, file_src) {}
  };
};

template <types::StringLiteral domain, typename... Configs>
Level Logger<domain, Configs...>::logging_level = Level::Info;

template <types::StringLiteral domain, typename... Configs>
config::Output<Configs...> Logger<domain, Configs...>::output_;

template <types::StringLiteral domain, typename... Configs>
config::Buffer<Configs...> Logger<domain, Configs...>::buffer_;

}  // namespace sb::logger

#include "specializations/Disable.hpp"
