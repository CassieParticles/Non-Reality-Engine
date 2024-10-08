# CMakeList.txt : CMake project for Non Reality Engine, include source and define
# project specific logic here.
#
list(APPEND ENGINE "engine/main.cpp")



cmake_minimum_required (VERSION 3.8)

project("NonRealityEngine")


add_executable(NonRealityEngine ${ENGINE})

add_subdirectory(GraphicsBaseFramework)
target_link_libraries(NonRealityEngine PUBLIC GraphicsFramework)