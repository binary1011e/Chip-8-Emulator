cmake_minimum_required(VERSION 3.14)  # FetchContent needs 3.14+
set(CMAKE_CXX_STANDARD 11)

# Enable CTest for test discovery
include(CTest)
enable_testing()

# Use FetchContent to download Catch2
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2  # or latest stable version
)

FetchContent_MakeAvailable(Catch2)

# List your test source files (no need for headers anymore)
set(CMakeDemo_TEST_SRC
    test_cmake_demo.cpp
)

# Full path to sources
PREPEND(CMakeDemo_TEST_SRC)

# Create the test executable
add_executable(TestCMakeDemo ${CMakeDemo_TEST_SRC})

# Link against your library and Catch2
target_link_libraries(TestCMakeDemo
    PRIVATE
        CMakeDemo
        Catch2::Catch2WithMain
)

# Use Catch2's CTest integration to automatically register test cases
include(Catch)
catch_discover_tests(TestCMakeDemo)
