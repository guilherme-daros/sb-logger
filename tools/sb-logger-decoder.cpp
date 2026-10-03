#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "logger/config/Defer.hpp"

namespace {

struct Metadata {
  std::string domain;
  std::string level;
  std::string file;
  uint32_t line{0};
};

auto parse_elf(const std::string& elf_path) -> std::map<uintptr_t, Metadata> {
  std::map<uintptr_t, Metadata> table;

  int fd = open(elf_path.c_str(), O_RDONLY);
  if (fd < 0) {
    std::cerr << "[sb-logger-decoder] Error: cannot open ELF file: " << elf_path << std::endl;
    return table;
  }

  struct stat st {};
  if (fstat(fd, &st) < 0) {
    close(fd);
    return table;
  }

  void* map_addr = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (map_addr == MAP_FAILED) {
    close(fd);
    return table;
  }

  auto* ehdr = reinterpret_cast<const Elf64_Ehdr*>(map_addr);
  if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0 || ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
    std::cerr << "[sb-logger-decoder] Error: Invalid 64-bit ELF binary." << std::endl;
    munmap(map_addr, st.st_size);
    close(fd);
    return table;
  }

  auto* shdr = reinterpret_cast<const Elf64_Shdr*>(static_cast<const char*>(map_addr) + ehdr->e_shoff);
  const char* shstrtab = static_cast<const char*>(map_addr) + shdr[ehdr->e_shstrndx].sh_offset;

  const Elf64_Shdr* target_sec = nullptr;
  for (size_t i = 0; i < ehdr->e_shnum; ++i) {
    const char* sec_name = shstrtab + shdr[i].sh_name;
    if (strcmp(sec_name, ".sb_defmt") == 0) {
      target_sec = &shdr[i];
      break;
    }
  }

  if (!target_sec) {
    std::cerr << "[sb-logger-decoder] Warning: .sb_defmt section not found in " << elf_path << std::endl;
    munmap(map_addr, st.st_size);
    close(fd);
    return table;
  }

  const auto* entries = reinterpret_cast<const sb::logger::config::MetadataEntry*>(
      static_cast<const char*>(map_addr) + target_sec->sh_offset);
  const size_t count = target_sec->sh_size / sizeof(sb::logger::config::MetadataEntry);

  auto resolve_str = [&](const char* ptr) -> std::string {
    if (!ptr) return "";
    uintptr_t vaddr = reinterpret_cast<uintptr_t>(ptr);
    for (size_t i = 0; i < ehdr->e_shnum; ++i) {
      if (shdr[i].sh_addr != 0 && vaddr >= shdr[i].sh_addr && vaddr < shdr[i].sh_addr + shdr[i].sh_size) {
        uintptr_t offset = shdr[i].sh_offset + (vaddr - shdr[i].sh_addr);
        if (offset < static_cast<size_t>(st.st_size)) {
          return std::string(static_cast<const char*>(map_addr) + offset);
        }
      }
    }
    return "";
  };

  for (size_t i = 0; i < count; ++i) {
    Metadata meta;
    meta.domain = std::string(entries[i].domain);
    meta.level = std::string(entries[i].level);
    meta.file = std::string(entries[i].file);
    meta.line = entries[i].line;

    table[i] = meta;
  }

  munmap(map_addr, st.st_size);
  close(fd);
  return table;
}

}  // namespace

int main(int argc, char* argv[]) {
  std::string elf_path;
  std::string log_path;

  for (int i = 1; i < argc; ++i) {
    std::string_view arg(argv[i]);
    if (arg == "--elf" && i + 1 < argc) {
      elf_path = argv[++i];
    } else if (arg == "--log" && i + 1 < argc) {
      log_path = argv[++i];
    }
  }

  if (elf_path.empty()) {
    std::cerr << "Usage: sb-logger-decoder --elf <binary_elf> [--log <binary_log_file>]" << std::endl;
    return 1;
  }

  auto symbol_table = parse_elf(elf_path);

  std::istream* input = &std::cin;
  std::ifstream log_file;
  if (!log_path.empty()) {
    log_file.open(log_path, std::ios::binary);
    if (!log_file.is_open()) {
      std::cerr << "[sb-logger-decoder] Error opening log file: " << log_path << std::endl;
      return 1;
    }
    input = &log_file;
  }

  std::cout << "--- [sb-logger-decoder] Streaming Decoded Logs ---" << std::endl;

  while (input->good() && !input->eof()) {
    sb::logger::config::PacketHeader header{};
    input->read(reinterpret_cast<char*>(&header), sizeof(header));
    if (input->gcount() < static_cast<std::streamsize>(sizeof(header))) {
      break;
    }

    std::vector<char> payload(header.payload_len);
    if (header.payload_len > 0) {
      input->read(payload.data(), header.payload_len);
    }

    uintptr_t log_id = header.log_id;
    auto it = symbol_table.find(log_id);

    if (it != symbol_table.end()) {
      const auto& meta = it->second;
      std::cout << "[" << meta.domain << "] [" << meta.level << "] "
                << meta.file << ":" << meta.line << " -> Payload ("
                << header.payload_len << " bytes)" << std::endl;
    } else {
      std::cout << "[UNKNOWN_ID: 0x" << std::hex << log_id << std::dec << "] Payload ("
                << header.payload_len << " bytes)" << std::endl;
    }
  }

  return 0;
}
