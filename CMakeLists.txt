
cmake_minimum_required (VERSION 3.8)


project ("Non-Reality-Engine")

list(APPEND SourceFiles "Engine/ECS/ECSDefinitions.h")

# Add source to this project's executable.
add_executable (Non-Reality-Engine ${SourceFiles} "main.cpp")	#Main will be removed once engine is set up with game project in mind

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	target_compile_definitions(Non-Reality-Engine PRIVATE "DEBUG")
else()
	target_compile_definitions(Non-Reality-Engine PRIVATE "RELEASE")
endif()

# Add the tracy profiler
option(TRACY_ENABLE "" ON)
option (TRACY_ON_DEMAND "" ON)
add_subdirectory(tracy)
target_link_libraries(Non-Reality-Engine PUBLIC Tracy::TracyClient)

# Add the math library glm
add_subdirectory(glm)
target_link_libraries(Non-Reality-Engine PUBLIC glm)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Non-Reality-Engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
