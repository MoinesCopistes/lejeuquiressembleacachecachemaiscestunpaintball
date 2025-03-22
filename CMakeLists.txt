cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(paintball)

# Add raylib
option(BUILD_EXAMPLES "Build examples" OFF)
add_subdirectory(vendor/raylib)

# Create the executable
file(GLOB_RECURSE SOURCES src/**/*.c src/*.c)
file(GLOB_RECURSE CHANSOURCES vendor/chan/*.c)
add_executable(paintball ${SOURCES} ${CHANSOURCES})

# Include raylib headers
target_include_directories(paintball PRIVATE vendor/raylib/include)
target_include_directories(paintball PRIVATE vendor/chan/)
target_include_directories(paintball PRIVATE src)

# Link raylib
target_link_libraries(paintball PRIVATE raylib)
