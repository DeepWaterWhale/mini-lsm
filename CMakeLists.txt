cmake_minimum_required(VERSION 3.20)

project(mini_lsm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(MINI_LSM_BUILD_TESTS "Build the test executable" ON)
option(MINI_LSM_BUILD_BENCHMARKS "Build the benchmark executable" ON)
option(MINI_LSM_ENABLE_ASAN_UBSAN "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(MINI_LSM_ENABLE_TSAN "Enable ThreadSanitizer" OFF)
option(MINI_LSM_ENABLE_COVERAGE "Enable GCC coverage instrumentation" OFF)

if(MINI_LSM_ENABLE_ASAN_UBSAN AND MINI_LSM_ENABLE_TSAN)
  message(FATAL_ERROR "ASan/UBSan and TSan cannot be enabled together")
endif()

add_library(mini_lsm_project_options INTERFACE)

if(MSVC)
  target_compile_options(mini_lsm_project_options INTERFACE /W4 /permissive-)
else()
  target_compile_options(mini_lsm_project_options INTERFACE -Wall -Wextra -Wpedantic)
endif()

if(MINI_LSM_ENABLE_ASAN_UBSAN OR MINI_LSM_ENABLE_TSAN)
  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    message(FATAL_ERROR "Sanitizers require Clang or GCC")
  endif()

  if(MINI_LSM_ENABLE_ASAN_UBSAN)
    target_compile_options(mini_lsm_project_options INTERFACE -fsanitize=address,undefined)
    target_link_options(mini_lsm_project_options INTERFACE -fsanitize=address,undefined)
  else()
    target_compile_options(mini_lsm_project_options INTERFACE -fsanitize=thread)
    target_link_options(mini_lsm_project_options INTERFACE -fsanitize=thread)
  endif()
endif()

if(MINI_LSM_ENABLE_COVERAGE)
  if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "Coverage instrumentation requires GCC")
  endif()

  target_compile_options(
    mini_lsm_project_options
    INTERFACE
      --coverage
      -fprofile-update=atomic
  )
  target_link_options(mini_lsm_project_options INTERFACE --coverage)
endif()

add_library(mini_lsm_core STATIC sources/mini_lsm_app.cpp)
target_include_directories(mini_lsm_core PUBLIC sources)
target_link_libraries(mini_lsm_core PRIVATE mini_lsm_project_options)

add_executable(mini_lsm sources/main.cpp)
target_link_libraries(mini_lsm PRIVATE mini_lsm_core mini_lsm_project_options)

if(MINI_LSM_BUILD_TESTS OR MINI_LSM_BUILD_BENCHMARKS)
  include(FetchContent)
endif()

if(MINI_LSM_BUILD_TESTS)
  enable_testing()

  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(googletest)

  add_executable(mini_lsm_test tests/mini_lsm_app_test.cpp)
  target_link_libraries(
    mini_lsm_test
    PRIVATE
      mini_lsm_core
      mini_lsm_project_options
      GTest::gtest_main
  )

  include(GoogleTest)
  gtest_discover_tests(mini_lsm_test DISCOVERY_MODE PRE_TEST)
endif()

if(MINI_LSM_BUILD_BENCHMARKS)
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.1
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(googlebenchmark)

  add_executable(mini_lsm_bench bench/mini_lsm_bench.cpp)
  target_link_libraries(
    mini_lsm_bench
    PRIVATE
      mini_lsm_core
      mini_lsm_project_options
      benchmark::benchmark
  )
endif()

file(
  GLOB_RECURSE MINI_LSM_FORMAT_FILES
  CONFIGURE_DEPENDS
  sources/*.c
  sources/*.cc
  sources/*.cpp
  sources/*.cxx
  sources/*.h
  sources/*.hh
  sources/*.hpp
  sources/*.hxx
  tests/*.c
  tests/*.cc
  tests/*.cpp
  tests/*.cxx
  tests/*.h
  tests/*.hh
  tests/*.hpp
  tests/*.hxx
  bench/*.c
  bench/*.cc
  bench/*.cpp
  bench/*.cxx
  bench/*.h
  bench/*.hh
  bench/*.hpp
  bench/*.hxx
)

find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)
if(CLANG_FORMAT_EXECUTABLE)
  add_custom_target(
    format
    COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${MINI_LSM_FORMAT_FILES}
    COMMAND_EXPAND_LISTS
    VERBATIM
  )
  add_custom_target(
    format-check
    COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run --Werror ${MINI_LSM_FORMAT_FILES}
    COMMAND_EXPAND_LISTS
    VERBATIM
  )
else()
  message(STATUS "clang-format not found; format targets will not be available")
endif()
