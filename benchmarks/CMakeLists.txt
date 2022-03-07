cmake_minimum_required(VERSION 3.10)

project(benchmark LANGUAGES C CXX)

add_subdirectory(3rdparty/DirectXMath)
add_subdirectory(3rdparty/glm)

add_executable(${PROJECT_NAME}
    benchmarks.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE -m64)

target_link_libraries(${PROJECT_NAME}
    x86
    DirectXMath
    glm
    slm)
