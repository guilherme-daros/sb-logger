#pragma once

#include <ostream>
#include <type_traits>

#include "logger/Meta.hpp"
#include "logger/config/Disable.hpp"
#include "types/StringLiteral.hpp"

namespace sb::logger {

constexpr auto is_disabled = []<typename T>() { return std::is_same_v<T, config::Disable>; };

template <types::StringLiteral domain, typename... Configs>
  requires(meta::any_of<is_disabled, Configs...>)
class Logger<domain, Configs...> {
  class Log {
   public:
    template <typename T>
    Log& operator<<(const T&) {
      return *this;
    }

    void operator<<(std::ostream& (*)(std::ostream&)) {}
  };

 public:
  static inline constexpr auto logging_level = -1;

  class Debug : public Log {};
  class Info : public Log {};
  class Warning : public Log {};
  class Error : public Log {};
};

}  // namespace sb::logger