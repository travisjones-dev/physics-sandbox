cmake_minimum_required(VERSION 3.24)
project(physics_sandbox LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# raylib only draws things and reads input. All physics lives in src/physics/.
include(FetchContent)
FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# The physics engine is its own library with no raylib dependency, so tests
# can run without opening a window.
add_library(physics STATIC src/physics/world.cpp)
target_include_directories(physics PUBLIC src)

add_executable(sandbox src/main.cpp)
target_link_libraries(sandbox PRIVATE physics raylib)

enable_testing()
add_executable(physics_tests tests/physics_tests.cpp)
target_link_libraries(physics_tests PRIVATE physics)
add_test(NAME physics_tests COMMAND physics_tests)

if(MSVC)
    target_compile_options(physics PRIVATE /W4)
    target_compile_options(physics_tests PRIVATE /W4)
else()
    target_compile_options(physics PRIVATE -Wall -Wextra)
    target_compile_options(physics_tests PRIVATE -Wall -Wextra)
endif()
