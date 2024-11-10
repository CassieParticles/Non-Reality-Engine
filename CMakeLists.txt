# CMakeList.txt : CMake project for Non Reality Engine, include source and define
# project specific logic here.
#

list(APPEND ENGINE "engine/ResourceManager/ShaderManager.cpp")
list(APPEND ENGINE "engine/ResourceManager/MeshLoader.cpp")

list(APPEND ENGINE "engine/ObjectStructure/GameObject.cpp")
list(APPEND ENGINE "engine/ObjectStructure/DefaultComponents/TransformComponent.cpp")

list(APPEND ENGINE "engine/Rendering/Renderer.cpp")
list(APPEND ENGINE "engine/Rendering/Components/MeshComponent.cpp")
list(APPEND ENGINE "engine/Rendering/Components/CameraComponent.cpp")

list(APPEND ENGINE "engine/SceneManagement/Timer.cpp")
list(APPEND ENGINE "engine/SceneManagement/Scene.cpp")

list(APPEND ENGINE "engine/BaseGameLoop.cpp")

cmake_minimum_required (VERSION 3.8)


add_library(NonRealityEngine ${ENGINE})

target_include_directories(NonRealityEngine PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

add_subdirectory(GraphicsBaseFramework)
target_link_libraries(NonRealityEngine PUBLIC GraphicsFramework)
