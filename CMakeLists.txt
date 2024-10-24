# CMakeList.txt : CMake project for Non Reality Engine, include source and define
# project specific logic here.
#
list(APPEND GAME "engine/main.cpp")

list(APPEND ENGINE "engine/ResourceManager/ShaderManager.cpp")
list(APPEND ENGINE "engine/ResourceManager/MeshLoader.cpp")


cmake_minimum_required (VERSION 3.8)
project("app")

add_library(NonRealityEngine ${ENGINE})

target_include_directories(NonRealityEngine PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

add_subdirectory(GraphicsBaseFramework)
target_link_libraries(NonRealityEngine PUBLIC GraphicsFramework)


#TEMP: for testing till application layer is created

add_executable(app ${GAME})

target_link_libraries(app PUBLIC NonRealityEngine)