project(Anvil VERSION 0.1.0 LANGUAGES C CXX)

#Library Setup

file(GLOB_RECURSE Anvil_SRC
    "*.cpp"
)
file(GLOB_RECURSE Anvil_HEADER
    "*.hpp"
    "*.h"
)

add_executable(Anvil ${Anvil_SRC} ${Anvil_HEADER})
target_include_directories(Anvil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Anvil)

target_compile_definitions(Anvil PUBLIC
    -DANVIL_VERSION_MAJOR=0
    -DANVIL_VERSION_MINOR=1
    -DANVIL_VERSION_PATCH=0
    -DANVIL_VERSION_STAGE=prototype
)

target_link_libraries(Anvil PUBLIC
  SrcSmith
)

set_target_properties(Anvil
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    RUNTIME_OUTPUT_NAME Anvil
)