include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(sb-logger-tests
    TestLevel.cpp
    TestOutput.cpp
    TestBuffer.cpp
    TestTiming.cpp
    TestDisable.cpp
    TestAsync.cpp
    TestDefer.cpp
)

target_link_libraries(sb-logger-tests PRIVATE sb::logger gtest_main)

include(GoogleTest)
gtest_discover_tests(sb-logger-tests)
