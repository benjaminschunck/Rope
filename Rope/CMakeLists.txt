# CMakeList.txt : CMake project for Rope, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Rope "Rope.cpp" "Rope.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Rope PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(Rope PRIVATE SDL2::SDL2main SDL2::SDL2-static)