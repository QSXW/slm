cmake_minimum_required(VERSION 3.10)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(slm LANGUAGES C CXX)

add_compile_options(${COMPILER_OPTIONS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(sl)
add_subdirectory(src)
add_subdirectory(x86)
add_subdirectory(benchmarks)
