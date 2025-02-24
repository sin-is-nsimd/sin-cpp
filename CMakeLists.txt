# Copyright © 2024 Lénaïc Bagnères, lenaicb@singularity.fr

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


cmake_minimum_required(VERSION 3.13.4)
project(sin-cpp
  VERSION 24.03.29
  DESCRIPTION "sin-cpp library")

# OPTIONS

SET(SINCPP_LIB_TYPE "SHARED" CACHE STRING "Library type (SHARED or STATIC)")

# SIN-CMAKE

include("${CMAKE_CURRENT_SOURCE_DIR}/../sin-cmake/cmake/sin.cmake")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-pedantic -Wall -Wextra -Wdouble-promotion)
endif()

# DEPENDENCIES

find_package(benchmark REQUIRED)
find_package(GTest REQUIRED)
find_path(zlib_include PATH "zlib.h" REQUIRED)
find_library(zlib_lib NAMES z zs REQUIRED)
message(STATUS "zlib = ${zlib_include} | ${zlib_lib}")

# LIBRARY

sin_add_library(sin-cpp ${SINCPP_LIB_TYPE})
target_include_directories(sin-cpp PUBLIC ${zlib_include})
target_link_libraries(sin-cpp ${zlib_lib})

sin_add_doxygen(sincpp_doxygen INPUTS "include" "src" OUTPUT "./sincpp_doxygen")

# TESTS, EXAMPLES, BENCHMARKS

sin_add_tests(LINK_LIBRARIES sin-cpp USE_GTEST ADD_VALGRIND)
sin_add_examples(LINK_LIBRARIES sin-cpp ADD_VALGRIND)
# sin_add_benchmarks(LINK_LIBRARIES sin-cpp benchmark::benchmark)
