#pragma once

#include <concepts>
#include <iostream>
#include <mutex>
#include <ostream>

#include "../Meta.hpp"

namespace sb::logger::config {

template <typename T>
concept IsOutput = requires(T t) {
  { t.mutex() } -> std::same_as<std::mutex&>;
  { t.stream() } -> std::same_as<std::ostream&>;
};

class Console {
 public:
  using Default = Console;

  auto stream() -> std::ostream& {
    static auto& pStream = std::cout;
    return pStream;
  }

  auto mutex() -> std::mutex& { return mtx_; }

 private:
  inline static std::mutex mtx_;
};

class Terminal {
 public:
  auto stream() -> std::ostream& {
    static auto& pStream = std::cout;
    return pStream;
  }

  auto mutex() -> std::mutex& { return mtx_; }

 private:
  inline static std::mutex mtx_;
};

constexpr auto is_output = []<typename T>() { return IsOutput<T>; };

template <typename... Ts>
using Output = meta::TypeFinder_t<Console, is_output, Ts...>;

}  // namespace sb::logger::config
