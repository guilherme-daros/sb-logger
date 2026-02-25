#pragma once

#include <chrono>
#include <concepts>
#include <format>
#include <string_view>

#include "../Meta.hpp"

namespace sb::logger::config {

template <typename T>
concept IsTiming = requires(T t) {
  { t.get() } -> std::same_as<std::string_view>;
  { t.width() } -> std::same_as<uint8_t>;
};

class Uptime {
 public:
  auto get() const -> std::string_view {
    static const auto start = std::chrono::steady_clock::now();
    const auto now = std::chrono::steady_clock::now();

    const auto uptime_s = std::chrono::duration_cast<std::chrono::seconds>(now - start);
    const auto uptime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start) % 1000;

    // Use thread_local for the buffer to avoid allocations on every call.
    static thread_local std::string uptime_str;
    uptime_str = std::format("{:>{}}.{:0>3}", uptime_s.count(), seconds_width, uptime_ms.count());
    return uptime_str;
  }
  constexpr auto width() const -> uint8_t { return seconds_width + 1 + milliseconds_width; }

 private:
  static constexpr uint8_t seconds_width = 5;
  static constexpr uint8_t milliseconds_width = 3;
};

class Timestamp {
 public:
  using Default = Timestamp;
  auto get() const -> std::string_view {
    const auto now = std::chrono::system_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()) % 100;
    const auto s = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    static thread_local std::string timestamp_str;

    timestamp_str = std::format("{:%H:%M}:{}:{:0>3}", now, ms.count(), s.count());
    return timestamp_str;
  }
  constexpr auto width() const -> uint8_t { return timestamp_width; }

 private:
  // "HH:MM:SS.ms" -> 8 + 1 + 3 = 12
  static constexpr uint8_t timestamp_width = 12;
};

constexpr auto is_timing = []<typename T>() { return IsTiming<T>; };

template <typename... Ts>
using Timing = meta::TypeFinder_t<Timestamp, is_timing, Ts...>;

}  // namespace sb::logger::config
