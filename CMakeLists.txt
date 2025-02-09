
cmake_minimum_required (VERSION 3.8)


project ("Non-Reality-Engine")

list(APPEND "SourceFiles" "main.cpp")

# Add source to this project's executable.
add_executable (Non-Reality-Engine ${SourceFiles})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Non-Reality-Engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
