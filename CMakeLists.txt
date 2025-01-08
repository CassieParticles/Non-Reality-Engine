# CMakeList.txt : CMake project for Non Reality Engine, include source and define
# project specific logic here.
#

list(APPEND ENGINE "engine/ResourceManager/ShaderManager.cpp")
list(APPEND ENGINE "engine/ResourceManager/MeshLoader.cpp")
list(APPEND ENGINE "engine/ResourceManager/TextureLoader.cpp")

list(APPEND ENGINE "engine/ObjectStructure/GameObject.cpp")
list(APPEND ENGINE "engine/ObjectStructure/DefaultComponents/TransformComponent.cpp")

list(APPEND ENGINE "engine/Rendering/Renderer.cpp")
list(APPEND ENGINE "engine/Rendering/ObliqueView.cpp")
list(APPEND ENGINE "engine/Rendering/Components/MeshComponent.cpp")
list(APPEND ENGINE "engine/Rendering/Components/PlayerCameraComponent.cpp")

list(APPEND ENGINE "engine/Rendering/Components/Portal/PortalComponent.cpp")
list(APPEND ENGINE "engine/Rendering/Components/Portal/PortalCameraComponent.cpp")

list(APPEND ENGINE "engine/SceneManagement/Timer.cpp")
list(APPEND ENGINE "engine/SceneManagement/Scene.cpp")
list(APPEND ENGINE "engine/SceneManagement/GameObjectAllocator.cpp")

list(APPEND ENGINE "engine/BaseGameLoop.cpp")

cmake_minimum_required (VERSION 3.8)


add_library(NonRealityEngine STATIC ${ENGINE})

target_include_directories(NonRealityEngine PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

add_subdirectory(GraphicsBaseFramework)
target_link_libraries(NonRealityEngine PUBLIC GraphicsFramework)

add_custom_target(copyAssets ALL
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_LIST_DIR}/../Assets" "${CMAKE_BINARY_DIR}/Assets"
COMMENT "Copied assets" VERBATIM
)

add_custom_target(copyShaders ALL
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_LIST_DIR}/engine/Rendering/shaders" "${CMAKE_BINARY_DIR}/shaders"
COMMENT "Copied assets" VERBATIM
)

add_dependencies(NonRealityEngine copyShaders)