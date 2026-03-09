cmake_minimum_required(VERSION 3.30)
add_executable(RayMarching)
target_sources(RayMarching
    PRIVATE
   src/RayMarchingEntry.cpp
   src/RayMarching.cpp
)
target_include_directories(RayMarching
   PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(RayMarching PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PLATFORM_NAME}"
    OUTPUT_NAME "RayMarching"  
)

target_link_libraries(RayMarching PRIVATE BitEngine)

