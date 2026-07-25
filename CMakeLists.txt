cmake_minimum_required(VERSION 3.14)
project(PolynomialCAS CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(polynomial_lib src/Polynomial.cpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/0359723544d960b52120730427735b7dca9b30ae.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(runTests tests/test_polynomial.cpp)
target_link_libraries(runTests PRIVATE polynomial_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(runTests)