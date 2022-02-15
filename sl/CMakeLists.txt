cmake_minimum_required(VERSION 3.21)

project(slapi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_FILES
    sl.h
    slapi.h
    slcast.h
    slintrinsic.h)

set(PROJECT_FILES
    ${SRC_FILES})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER_OPTIONS "")
else(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "CLANG")
    set(COMPILER_OPTIONS -march=native)
endif()

source_group("\\" FILE ${PROJECT_FILES})

add_library(${PROJECT_NAME} INTERFACE ${PROJECT_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)
target_compile_options(${PROJECT_NAME} INTERFACE ${COMPILER_OPTIONS})
