
cmake_minimum_required(VERSION 3.10)

project(x86 ASM_NASM)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(ASM_FILES
    matrix.asm
    vector.asm)

source_group("\\" FILES ${ASM_FILES})

add_library(${PROJECT_NAME} STATIC ${ASM_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE )