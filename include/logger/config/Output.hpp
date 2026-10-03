#pragma once

#include <concepts>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ostream>

#include "meta/Meta.hpp"
#include "types/StringLiteral.hpp"

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

template <types::StringLiteral FilePath>
class File {
 public:
  auto stream() -> std::ostream& {
    static std::ofstream ofs{FilePath.data, std::ios::out | std::ios::app};
    return ofs;
  }

  auto mutex() -> std::mutex& { return mtx_; }

 private:
  inline static std::mutex mtx_;
};

constexpr auto is_output = []<typename T>() { return IsOutput<T>; };

template <typename... Ts>
using Output = sb::meta::TypeFinder_t<Console, is_output, Ts...>;

}  // namespace sb::logger::config
