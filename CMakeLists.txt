cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

PROJECT(earth-mantle)

# depends on vtk
find_package(VTK REQUIRED)

add_subdirectory(src)
