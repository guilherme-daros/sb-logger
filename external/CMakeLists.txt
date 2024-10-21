cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(sb-types)

if(FRAMEWORK STREQUAL "gtest")
  add_subdirectory(googletest)
endif()
