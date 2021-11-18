cmake_minimum_required(VERSION 3.10)

project(slm LANGUAGES C CXX ASM_NASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_FILES
    nlm.h
    slm.h
    vector.h
    matrix.h)

source_group("\\" FILES ${SRC_FILES})
source_group("\\x86\\" FILES ${ASM_FILES})

add_library(${PROJECT_NAME} INTERFACE
    ${SRC_FILES})

target_compile_options(${PROJECT_NAME} INTERFACE $<$<CXX_COMPILER_ID:GNU>:-march=skylake-avx512 -mavx>)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
    INTERFACE slapi)
