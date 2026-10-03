cmake_minimum_required(VERSION 3.28)
project(sb-logger VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. Dependencies (Submodules)
if(NOT TARGET sb::types)
    add_subdirectory(extern/sb-types)
endif()
if(NOT TARGET sb::meta)
    add_subdirectory(extern/sb-meta)
endif()
if(NOT TARGET sb::container)
    add_subdirectory(extern/sb-container)
endif()

# 2. Define the library (Header-only)
add_library(sb-logger INTERFACE)

# 3. Configure Include Directories
target_include_directories(sb-logger INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 4. Mandate C++23 for all consumers
target_compile_features(sb-logger INTERFACE cxx_std_23)

# 5. Link against dependencies
target_link_libraries(sb-logger INTERFACE sb::types sb::meta sb::container)

# 6. Create an alias (sb::logger)
add_library(sb::logger ALIAS sb-logger)

# Examples
option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tools
option(BUILD_TOOLS "Build tools" ON)
if(BUILD_TOOLS)
    add_subdirectory(tools)
endif()

# Tests
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
