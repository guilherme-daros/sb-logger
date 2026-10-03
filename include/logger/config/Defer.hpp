#pragma once

#include <cstdint>
#include <cstring>
#include <ostream>
#include <string_view>
#include <type_traits>

#include "logger/Level.hpp"
#include "types/StringLiteral.hpp"

#if defined(__linux__) || defined(__ELF__)
#define SB_DEFMT_SECTION __attribute__((section(".sb_defmt"), used))
#else
#define SB_DEFMT_SECTION
#endif

namespace sb::logger::config {

struct Defer {};

#pragma pack(push, 1)
struct MetadataEntry {
  char domain[32]{};
  char level[8]{};
  char file[64]{};
  uint32_t line{0};

  constexpr MetadataEntry() = default;

  constexpr MetadataEntry(std::string_view dom, std::string_view lvl, std::string_view file_path, uint32_t l)
      : line(l) {
    for (size_t i = 0; i < dom.size() && i < sizeof(domain) - 1; ++i) domain[i] = dom[i];
    for (size_t i = 0; i < lvl.size() && i < sizeof(level) - 1; ++i) level[i] = lvl[i];

    size_t sep_pos = 0;
    for (size_t i = 0; i < file_path.size(); ++i) {
      if (file_path[i] == '/' || file_path[i] == '\\') sep_pos = i + 1;
    }
    std::string_view fn = file_path.substr(sep_pos);
    for (size_t i = 0; i < fn.size() && i < sizeof(file) - 1; ++i) file[i] = fn[i];
  }
};

struct PacketHeader {
  uint32_t log_id;
  uint16_t payload_len;
};
#pragma pack(pop)

template <types::StringLiteral domain, Level level, types::StringLiteral file, uint32_t line>
struct LogMetadata {
  SB_DEFMT_SECTION static constexpr MetadataEntry entry{
      domain.data,
      to_string(level),
      file.data,
      line
  };
};

namespace detail {

template <typename T>
inline void serialize_arg(std::ostream& os, const T& val) {
  if constexpr (std::is_arithmetic_v<T>) {
    os.write(reinterpret_cast<const char*>(&val), sizeof(T));
  } else if constexpr (std::is_convertible_v<T, std::string_view>) {
    std::string_view sv(val);
    uint16_t len = static_cast<uint16_t>(sv.size());
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(sv.data(), sv.size());
  } else {
    // Fallback for types outputting via operator<< or std::to_string
    std::string s = std::to_string(val);
    uint16_t len = static_cast<uint16_t>(s.size());
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(s.data(), s.size());
  }
}

}  // namespace detail

}  // namespace sb::logger::config

extern "C" const sb::logger::config::MetadataEntry __start_sb_defmt[] __attribute__((weak));
extern "C" const sb::logger::config::MetadataEntry __stop_sb_defmt[] __attribute__((weak));
