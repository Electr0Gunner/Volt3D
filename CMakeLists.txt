cmake_minimum_required(VERSION 3.10.0)
project(SrcSmith VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

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

file(GLOB_RECURSE SrcSmith_SRC
    "SrcSmith/*.cpp"
    "SrcSmith/*.c"
)
file(GLOB_RECURSE SrcSmith_HEADER
    "SrcSmith/*.hpp"
    "SrcSmith/*.h"
)

add_library(SrcSmith STATIC ${SrcSmith_SRC} ${SrcSmith_HEADER} ${Dependency_SRC} ${Dependency_HEADER})
target_include_directories(SrcSmith PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SrcSmith ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

target_compile_definitions(SrcSmith PUBLIC
    -DSRCSMITH_VERSION_MAJOR=0
    -DSRCSMITH_VERSION_MINOR=1
    -DSRCSMITH_VERSION_PATCH=0
    -DSRCSMITH_VERSION_STAGE=alpha
)

target_link_libraries(SrcSmith PUBLIC
  SDL3::SDL3-static
  glm
)

set(SRCSMITH_BUILD_EXAMPLES ON CACHE BOOL "Build the examples provided by the engine" FORCE)
set(SRCSMITH_BUILD_ANVIL ON CACHE BOOL "Build the level editor" FORCE)

if (SRCSMITH_BUILD_EXAMPLES)
add_subdirectory(examples)
endif()
if (SRCSMITH_BUILD_ANVIL)
add_subdirectory(Anvil)
endif()