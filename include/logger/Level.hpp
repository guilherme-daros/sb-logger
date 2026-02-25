#pragma once

#include <string_view>

namespace sb::logger {

enum class Level { None, Error, Warning, Info, Debug };

inline auto to_string(Level level) -> std::string_view {
  // clang-format off
  switch (level) {
    case Level::None:    return "None";
    case Level::Error:   return "E";
    case Level::Warning: return "W";
    case Level::Info:    return "I";
    case Level::Debug:   return "D";
    default:             return "U";
  }
  // clang-format on
}

inline auto to_color(Level level) -> std::string_view {
  // clang-format off
  switch (level) {
    case Level::None:    return "\033[0m";
    case Level::Error:   return "\033[1;41m";
    case Level::Warning: return "\033[1;43m";
    case Level::Info:    return "\033[1;42m";
    case Level::Debug:   return "\033[1;44m";
    default:             return "\033[0m";
  }
  // clang-format on
}
}  // namespace sb::logger
