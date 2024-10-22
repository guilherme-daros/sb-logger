#pragma once

#include <iostream>
#include <mutex>
#include <ostream>

#include "Meta.hpp"

namespace sb::logger::output {

class Console;

class Base {
 public:
  using Default = Console;

  virtual auto mutex() -> std::mutex& = 0;
  virtual auto stream() -> std::ostream& = 0;
};

template <typename T>
struct is_output : std::is_base_of<output::Base, T> {};

template <typename... Ts>
using Output = meta::TypeFinder_t<output::Base, output::is_output, Ts...>;

class Console : public Base {
 public:
  auto stream() -> std::ostream& override {
    static auto& pStream = std::cout;
    return pStream;
  }

  auto mutex() -> std::mutex& override { return mtx_; }

 private:
  inline static std::mutex mtx_;
};

}  // namespace sb::logger::output
