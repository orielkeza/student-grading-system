cmake_minimum_required(VERSION 3.14)
project(MyGTestProject)

# 1. Enable CTest
include(CTest)
enable_testing()

# 2. Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com
)
# For Windows: prevent GTest from overriding compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 3. Create your test executable
add_executable(my_tests test_source.cpp)

# 4. Link GTest libraries
# Use 'gtest_main' so you don't have to write your own main() function
target_link_libraries(my_tests PRIVATE gtest_main)

# 5. Discover tests automatically
include(GoogleTest)
gtest_discover_tests(my_tests)
