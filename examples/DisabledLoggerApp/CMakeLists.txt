add_executable(DisabledLoggerApp Main.cpp)

target_link_libraries(DisabledLoggerApp PRIVATE sb::logger)
