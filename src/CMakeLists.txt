cmake_minimum_required(VERSION 3.18)

PROJECT(earth-mantle)

# find sources
file(GLOB SRCFILES *.cpp)
file(GLOB HPPFILES *.h)

# create project
add_executable(${PROJECT_NAME} MACOSX_BUNDLE main.cpp ${SRCFILES} ${HPPFILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

vtk_module_autoinit(TARGETS ${PROJECT_NAME} MODULES ${VTK_LIBRARIES})
