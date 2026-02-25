# Antimony Logger

A modern, domain-based logging system for C++23. Designed for performance and flexibility with compile-time configuration.

## Features
- **Domain-based:** `sb::logger::Logger<"MyDomain">` allows for easy filtering and identification.
- **C++23 Native:** Uses `std::source_location` for zero-overhead context (file, line, function).
- **Configurable:** 
  - Supports output redirection (Terminal, File, etc.).
  - Built-in buffering support for high-throughput logging.
  - Ability to disable loggers at compile-time via template specializations.
- **Header-only:** Easy to integrate into any project.

## Dependencies
- `sb-types` (for `StringLiteral`).

## Usage (CMake)
```cmake
add_subdirectory(extern/antimony-logger)
target_link_libraries(YourTarget PRIVATE sb::logger)
```
