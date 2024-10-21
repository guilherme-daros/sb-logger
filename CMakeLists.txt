cmake_minimum_required(VERSION 3.10)
set(LIB_PROJECT_NAME logger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  project(antimony-${LIB_PROJECT_NAME}) 
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.bin)
  
endif()

add_subdirectory(external)

if(NOT ${LIB_PROJECT_NAME})
  add_library(${LIB_PROJECT_NAME} INTERFACE)
  target_include_directories(${LIB_PROJECT_NAME} INTERFACE include)
  add_library(sb::${LIB_PROJECT_NAME} ALIAS ${LIB_PROJECT_NAME})
endif()

if(BUILD_EXAMPLES STREQUAL "yes")
  add_subdirectory(examples)
endif()

if(FRAMEWORK STREQUAL "gtest")
    add_subdirectory(test/)
endif()
