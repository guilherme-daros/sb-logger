cmake_minimum_required(VERSION 3.16)
project(BufferApp)

add_executable(BufferApp Main.cpp)
target_link_libraries(BufferApp PRIVATE sb::logger)
