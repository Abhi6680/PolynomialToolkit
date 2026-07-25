cmake_minimum_required(VERSION 3.14)
project(PolynomialCalculator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(PolynomialCalc main.cpp src/Polynomial.cpp)

# Enable Testing
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows MSVC runtime compatibility
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(runTests tests/test_polynomial.cpp src/Polynomial.cpp)
target_link_libraries(runTests gtest_main)

include(GoogleTest)
gtest_discover_tests(runTests)