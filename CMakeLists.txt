cmake_minimum_required(VERSION 3.16)
project(xjb CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

set(XJB_STANDARD cxx_std_20)

add_library(xjb src/ftoa.cpp)
target_compile_features(xjb PRIVATE ${XJB_STANDARD})

set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")