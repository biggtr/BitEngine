cmake_minimum_required(VERSION 3.30)
add_executable(Sandbox)
target_sources(Sandbox
    PRIVATE
   src/Sandbox.cpp
)
target_include_directories(Sandbox
   PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(Sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PLATFORM_NAME}"
    OUTPUT_NAME "Sandbox"  
)

target_link_libraries(Sandbox PRIVATE BitEngine)
