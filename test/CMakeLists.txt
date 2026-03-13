cmake_minimum_required(VERSION 3.14)
project(xjb-test CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# ---------- GoogleTest via FetchContent ----------
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ---------- dragonbox (from bench/) ----------
add_library(dragonbox STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../bench/dragonbox/dragonbox_to_chars.cpp)
target_include_directories(dragonbox PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../bench/dragonbox)
target_compile_features(dragonbox PRIVATE cxx_std_17)

# ---------- xjb-test  (unit tests) ----------
add_executable(xjb-test xjb-test.cc)
target_include_directories(xjb-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(xjb-test dragonbox GTest::gtest_main)
add_test(NAME xjb-test COMMAND xjb-test)

# ---------- float-check (exhaustive all-float verification) ----------
add_executable(float-check float-check.cc)
target_include_directories(float-check PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(float-check dragonbox)
find_package(Threads REQUIRED)
target_link_libraries(float-check Threads::Threads)
add_test(NAME float-check COMMAND float-check)
