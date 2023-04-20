cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

PROJECT(earth-mantle)

# depends on vtk
find_package(VTK REQUIRED)

# find sources
file(GLOB SRCFILES *.cpp)
file(GLOB HPPFILES *.h)

# create project
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRCFILES} ${HPPFILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})
vtk_module_autoinit(TARGETS ${PROJECT_NAME} MODULES ${VTK_LIBRARIES})