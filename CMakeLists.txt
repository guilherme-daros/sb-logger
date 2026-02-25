cmake_minimum_required(VERSION 3.28)
project(antimony-logger VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. Dependency: Types (Submodule)
add_subdirectory(extern/antimony-types)

# 2. Define the library (Header-only)
add_library(antimony-logger INTERFACE)

# 3. Configure Include Directories
target_include_directories(antimony-logger INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 4. Mandate C++23 for all consumers
target_compile_features(antimony-logger INTERFACE cxx_std_23)

# 5. Link against types (Internal dependency)
target_link_libraries(antimony-logger INTERFACE sb::types)

# 6. Create an alias (sb::logger)
add_library(sb::logger ALIAS antimony-logger)

# Examples
option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
