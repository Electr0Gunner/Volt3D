cmake_minimum_required(VERSION 3.10.0)
project(SrcSmithExample VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SrcSmithExample_SRC
    "src/*.cpp"
)
file(GLOB_RECURSE SrcSmithExample_HEADER
    "src/*.hpp"
    "src/*.h"
)

add_executable(SrcSmithExample ${SrcSmithExample_SRC} ${SrcSmithExample_HEADER} ${Dependency_SRC} ${Dependency_HEADER})
target_include_directories(SrcSmithExample PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(SrcSmithExample PUBLIC
  SrcSmith
)

set_target_properties(SrcSmithExample
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    RUNTIME_OUTPUT_NAME SrcSmithExample
)