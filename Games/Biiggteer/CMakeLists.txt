cmake_minimum_required(VERSION 3.30)
add_executable(Biiggteer)
target_sources(Biiggteer
    PRIVATE
   src/BiiggteerEntry.cpp
   src/Biiggteer.cpp
)
target_include_directories(Biiggteer
   PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(Biiggteer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PLATFORM_NAME}"
    OUTPUT_NAME "Biiggteer"  
)

target_link_libraries(Biiggteer PRIVATE BitEngine)

