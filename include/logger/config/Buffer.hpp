#pragma once

#include <string>
#include <vector>

namespace sb::logger::config {
template <size_t Size>
struct BufferConfig {
  static constexpr size_t MaxSize = Size;
};

template <typename... Configs>
class Buffer {
 private:
  template <typename T>
  static consteval size_t get_size() {
    if constexpr (requires { T::MaxSize; }) {
      return T::MaxSize;
    } else {
      return 0;
    }
  }

  template <typename... Us>
  static consteval size_t get_total_size() {
    return (get_size<Us>() + ... + 0);
  }

 public:
  static constexpr size_t max_size = get_total_size<Configs...>();

  void append(const std::string& msg) {
    if constexpr (max_size > 0) {
      buffer_.append(msg);
    }
  }

  bool should_flush() const {
    if constexpr (max_size > 0) {
      return buffer_.size() >= max_size;
    }
    return false;
  }

  const std::string& get_buffer() const { return buffer_; }
  void clear() { buffer_.clear(); }

 private:
  std::string buffer_;
};

}  // namespace sb::logger::config
