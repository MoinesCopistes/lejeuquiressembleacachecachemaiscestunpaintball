cmake_minimum_required(VERSION 3.31)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(paintball)


# Add raylib
option(BUILD_EXAMPLES "Build examples" OFF)
add_subdirectory(vendor/raylib)

# Create the executable
add_executable(paintball src/main.c)

# Include raylib headers
target_include_directories(paintball PRIVATE vendor/raylib/include)

# Link raylib
target_link_libraries(paintball PRIVATE raylib)
