
cmake_minimum_required (VERSION 3.8)


project ("Non-Reality-Engine")



list(APPEND "SourceFiles" "main.cpp")

# Add source to this project's executable.
add_executable (Non-Reality-Engine ${SourceFiles})

# Add the tracy profiler
option(TRACY_ENABLE "" ON)
option (TRACY_ON_DEMAND "" ON)
add_subdirectory(tracy-0.11.1)
target_link_libraries(Non-Reality-Engine PUBLIC Tracy::TracyClient)

add_subdirectory(glm)
target_link_libraries(Non-Reality-Engine PUBLIC glm)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Non-Reality-Engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
