cmake_minimum_required(VERSION 3.16)
project(BufferApp)

add_executable(BufferApp Main.cpp)

target_include_directories(BufferApp PRIVATE ${CMAKE_SOURCE_DIR}/include)
