cmake_minimum_required(VERSION 3.14)
project(xjb-test CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Release")

enable_testing()

# ---------- GoogleTest via FetchContent ----------
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2)
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ---------- dragonbox (from bench/) ----------
add_library(
  dragonbox STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../bench/dragonbox/dragonbox_to_chars.cpp)
target_include_directories(
  dragonbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../bench/dragonbox)
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

# ---------- SIMD variants ----------
include(CheckCXXCompilerFlag)

option(XJB_TEST_SSSE3 "Build and register SSSE3 test variants \
(only enable when the test machine's CPU supports SSSE3)" ON)
option(XJB_TEST_SSE4_1 "Build and register SSE4.1 test variants \
(only enable when the test machine's CPU supports SSE4.1)" ON)
option(XJB_TEST_AVX512 "Build and register AVX-512IFMA+VBMI test variants \
(only enable when the test machine's CPU supports AVX-512IFMA and AVX-512VBMI)"
       OFF)

if(XJB_TEST_SSSE3)
  check_cxx_compiler_flag(-mssse3 XJB_HAS_SSSE3)
  if(XJB_HAS_SSSE3)
    add_executable(xjb-ssse3-test xjb-test.cc)
    target_include_directories(xjb-ssse3-test
                               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_compile_options(xjb-ssse3-test PRIVATE -mssse3)
    target_link_libraries(xjb-ssse3-test dragonbox GTest::gtest_main)
    add_test(NAME xjb-ssse3-test COMMAND xjb-ssse3-test)

    add_executable(float-check-ssse3 float-check.cc)
    target_include_directories(float-check-ssse3
                               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_compile_options(float-check-ssse3 PRIVATE -mssse3)
    target_link_libraries(float-check-ssse3 dragonbox Threads::Threads)
    add_test(NAME float-check-ssse3 COMMAND float-check-ssse3)

    add_executable(xjb-ssse3-mmove-test xjb-test.cc)
    target_include_directories(xjb-ssse3-mmove-test
                               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_compile_options(xjb-ssse3-mmove-test PRIVATE -mssse3)
    target_compile_definitions(xjb-ssse3-mmove-test PRIVATE XJB_NO_MEMMOVE=0)
    target_link_libraries(xjb-ssse3-mmove-test dragonbox GTest::gtest_main)
    add_test(NAME xjb-ssse3-mmove-test COMMAND xjb-ssse3-mmove-test)

    add_executable(float-check-ssse3-mmove float-check.cc)
    target_include_directories(float-check-ssse3-mmove
                               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_compile_options(float-check-ssse3-mmove PRIVATE -mssse3)
    target_compile_definitions(float-check-ssse3-mmove PRIVATE XJB_NO_MEMMOVE=0)
    target_link_libraries(float-check-ssse3-mmove dragonbox Threads::Threads)
    add_test(NAME float-check-ssse3-mmove COMMAND float-check-ssse3-mmove)
  endif()
endif()

if(XJB_TEST_SSE4_1)
  check_cxx_compiler_flag(-msse4.1 XJB_HAS_SSE4_1)
  if(XJB_HAS_SSE4_1)
    add_executable(xjb-sse4-test xjb-test.cc)
    target_include_directories(xjb-sse4-test
                               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_compile_options(xjb-sse4-test PRIVATE -msse4.1)
    target_link_libraries(xjb-sse4-test dragonbox GTest::gtest_main)
    add_test(NAME xjb-sse4-test COMMAND xjb-sse4-test)

    add_executable(float-check-sse4 float-check.cc)
    target_include_directories(float-check-sse4
                               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_compile_options(float-check-sse4 PRIVATE -msse4.1)
    target_link_libraries(float-check-sse4 dragonbox Threads::Threads)
    add_test(NAME float-check-sse4 COMMAND float-check-sse4)

    add_executable(xjb-sse4-mmove-test xjb-test.cc)
    target_include_directories(xjb-sse4-mmove-test
                               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_compile_options(xjb-sse4-mmove-test PRIVATE -msse4.1)
    target_compile_definitions(xjb-sse4-mmove-test PRIVATE XJB_NO_MEMMOVE=0)
    target_link_libraries(xjb-sse4-mmove-test dragonbox GTest::gtest_main)
    add_test(NAME xjb-sse4-mmove-test COMMAND xjb-sse4-mmove-test)

    add_executable(float-check-sse4-mmove float-check.cc)
    target_include_directories(float-check-sse4-mmove
                               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_compile_options(float-check-sse4-mmove PRIVATE -msse4.1)
    target_compile_definitions(float-check-sse4-mmove PRIVATE XJB_NO_MEMMOVE=0)
    target_link_libraries(float-check-sse4-mmove dragonbox Threads::Threads)
    add_test(NAME float-check-sse4-mmove COMMAND float-check-sse4-mmove)
  endif()
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
  add_executable(xjb-neon-mmove-test xjb-test.cc)
  target_include_directories(xjb-neon-mmove-test
                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
  target_compile_definitions(xjb-neon-mmove-test PRIVATE XJB_NO_MEMMOVE=0)
  target_link_libraries(xjb-neon-mmove-test dragonbox GTest::gtest_main)
  add_test(NAME xjb-neon-mmove-test COMMAND xjb-neon-mmove-test)

  add_executable(float-check-neon-mmove float-check.cc)
  target_include_directories(float-check-neon-mmove
                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
  target_compile_definitions(float-check-neon-mmove PRIVATE XJB_NO_MEMMOVE=0)
  target_link_libraries(float-check-neon-mmove dragonbox Threads::Threads)
  add_test(NAME float-check-neon-mmove COMMAND float-check-neon-mmove)
endif()

if(XJB_TEST_AVX512)
  check_cxx_compiler_flag(-mavx512ifma XJB_HAS_AVX512IFMA)
  check_cxx_compiler_flag(-mavx512vbmi XJB_HAS_AVX512VBMI)
  if(XJB_HAS_AVX512IFMA AND XJB_HAS_AVX512VBMI)
    add_executable(xjb-avx512-test xjb-test.cc)
    target_include_directories(xjb-avx512-test
                               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_compile_options(xjb-avx512-test PRIVATE -mavx512ifma -mavx512vbmi)
    target_link_libraries(xjb-avx512-test dragonbox GTest::gtest_main)
    add_test(NAME xjb-avx512-test COMMAND xjb-avx512-test)

    add_executable(float-check-avx512 float-check.cc)
    target_include_directories(float-check-avx512
                               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_compile_options(float-check-avx512 PRIVATE -mavx512ifma -mavx512vbmi)
    target_link_libraries(float-check-avx512 dragonbox Threads::Threads)
    add_test(NAME float-check-avx512 COMMAND float-check-avx512)
  endif()
endif()
