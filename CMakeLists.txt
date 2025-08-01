cmake_minimum_required(VERSION 3.10.0)
project(Volt3D VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Dependency Setup

file(GLOB_RECURSE Dependency_SRC
    "dependencies/*.cpp"
    "dependencies/*.c"
)
file(GLOB_RECURSE Dependency_HEADER
    "dependencies/*.hpp"
    "dependencies/*.h"
)

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG main
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG master
)


FetchContent_MakeAvailable(SDL3 glm)

#Library Setup


file(GLOB_RECURSE Volt3D_SRC
    "Volt3D/*.cpp"
)
file(GLOB_RECURSE Volt3D_HEADER
    "Volt3D/*.hpp"
    "Volt3D/*.h"
)

add_library(Volt3D STATIC ${Volt3D_SRC} ${Volt3D_HEADER} ${Dependency_SRC} ${Dependency_HEADER})
target_include_directories(Volt3D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Volt3D ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

target_compile_definitions(Volt3D PUBLIC
    -DVOLT3D_VERSION_MAJOR=0
    -DVOLT3D_VERSION_MINOR=1
    -DVOLT3D_VERSION_PATCH=0
    -DVOLT3D_VERSION_STAGE=alpha
)

target_link_libraries(Volt3D PUBLIC
  SDL3::SDL3-static
  glm
)