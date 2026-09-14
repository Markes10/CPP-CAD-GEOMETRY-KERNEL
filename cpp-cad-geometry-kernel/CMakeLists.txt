cmake_minimum_required(VERSION 3.20)
project(CadGeometryKernel CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cad_kernel
    src/main.cpp
    src/cad_kernel.cpp
)

target_include_directories(cad_kernel PRIVATE src)

enable_testing()
add_test(NAME CadKernelTest COMMAND cad_kernel)
