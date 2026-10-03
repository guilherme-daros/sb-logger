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
#include "config/Async.hpp"
#include "config/Buffer.hpp"
#include "config/Defer.hpp"
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

constexpr auto is_async = []<typename T>() { return std::is_same_v<T, config::Async>; };
constexpr auto is_defer = []<typename T>() { return std::is_same_v<T, config::Defer>; };

template <types::StringLiteral domain, typename... Configs>
class Logger {
  using Output = config::Output<Configs...>;
  using Timing = config::Timing<Configs...>;
  using Buffer = config::Buffer<Configs...>;

  static constexpr bool has_async = sb::meta::any_of<is_async, Configs...>;
  static constexpr bool has_defer = sb::meta::any_of<is_defer, Configs...>;

  template <Level level_v, types::StringLiteral file_v, uint32_t line_v>
  class LogImpl {
   public:
    LogImpl(std::ostream &output, std::mutex &mtx, const uint64_t id)
        : output_(output), mtx_(mtx), id_(id), timing_(), enabled_(logging_level >= level_v) {
      if (enabled_) {
        file_number_ = utils::get_pos(file_v.data);
      }
    }

    ~LogImpl() {
      if (!enabled_) {
        return;
      }

      if constexpr (has_defer) {
        using Meta = config::LogMetadata<domain, level_v, file_v, line_v>;
        const uint32_t log_id = (__start_sb_defmt != nullptr)
                                    ? static_cast<uint32_t>(&Meta::entry - __start_sb_defmt)
                                    : 0;
        std::string payload = os.str();
        const uint16_t payload_len = static_cast<uint16_t>(payload.size());

        config::PacketHeader header{log_id, payload_len};
        std::string packet;
        packet.append(reinterpret_cast<const char*>(&header), sizeof(header));
        packet.append(payload);

        if constexpr (has_async) {
          async_queue_.push([packet](std::ostream& stream) {
            stream.write(packet.data(), packet.size());
          });
        } else {
          std::scoped_lock lock{mtx_};
          output_.write(packet.data(), packet.size());
          output_.flush();
        }
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
      context << to_color(level_v) << " " << to_string(level_v) << " " << default_light;
      context << " ";
      context << std::setw(20) << std::left << file_number_;
      context << " ";

      if constexpr (has_async) {
        std::string msg = context.str() + os.str() + "\n";
        async_queue_.push([msg](std::ostream& stream) {
          stream << msg;
        });
      } else if constexpr (Buffer::max_size > 0) {
        std::scoped_lock lock{mtx_};
        buffer_.append(context.str() + os.str() + "\n");
        if (buffer_.should_flush()) {
          output_ << buffer_.get_buffer() << std::flush;
          buffer_.clear();
        }
      } else {
        std::scoped_lock lock{mtx_};
        output_ << context.str() + os.str() << std::endl;
      }
    }

    template <typename T>
    LogImpl &operator<<(const T &token) {
      if (enabled_) {
        if constexpr (has_defer) {
          config::detail::serialize_arg(os, token);
        } else {
          os << token;
        }
      }
      return *this;
    }

   private:
    std::mutex &mtx_;
    std::ostringstream os;
    std::ostream &output_;
    std::string_view file_number_;
    const uint64_t id_;
    const Timing timing_;
    const bool enabled_;
  };

  static Output output_;
  static Buffer buffer_;
  static inline config::AsyncQueue async_queue_{output_.stream()};

 public:
  static Level logging_level;

  static void flush() {
    if constexpr (has_async) {
      async_queue_.flush();
    }
    if constexpr (Buffer::max_size > 0) {
      output_.stream() << buffer_.get_buffer() << std::flush;
      buffer_.clear();
    }
  }

  template <types::StringLiteral file_v = __FILE__, uint32_t line_v = __LINE__>
  static auto Debug(const uint64_t id = utils::get_thread_id()) {
    return LogImpl<Level::Debug, file_v, line_v>(output_.stream(), output_.mutex(), id);
  }

  template <types::StringLiteral file_v = __FILE__, uint32_t line_v = __LINE__>
  static auto Info(const uint64_t id = utils::get_thread_id()) {
    return LogImpl<Level::Info, file_v, line_v>(output_.stream(), output_.mutex(), id);
  }

  template <types::StringLiteral file_v = __FILE__, uint32_t line_v = __LINE__>
  static auto Warning(const uint64_t id = utils::get_thread_id()) {
    return LogImpl<Level::Warning, file_v, line_v>(output_.stream(), output_.mutex(), id);
  }

  template <types::StringLiteral file_v = __FILE__, uint32_t line_v = __LINE__>
  static auto Error(const uint64_t id = utils::get_thread_id()) {
    return LogImpl<Level::Error, file_v, line_v>(output_.stream(), output_.mutex(), id);
  }
};

template <types::StringLiteral domain, typename... Configs>
Level Logger<domain, Configs...>::logging_level = Level::Info;

template <types::StringLiteral domain, typename... Configs>
config::Output<Configs...> Logger<domain, Configs...>::output_;

template <types::StringLiteral domain, typename... Configs>
config::Buffer<Configs...> Logger<domain, Configs...>::buffer_;

}  // namespace sb::logger

#include "specializations/Disable.hpp"
