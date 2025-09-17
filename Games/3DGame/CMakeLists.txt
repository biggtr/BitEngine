cmake_minimum_required(VERSION 3.30)
add_executable(3DGame)
target_sources(3DGame
    PRIVATE
   src/3DGameEntry.cpp
   src/3DGame.cpp
)
target_include_directories(3DGame
   PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(3DGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PLATFORM_NAME}"
    OUTPUT_NAME "3DGame"  
)

target_link_libraries(3DGame PRIVATE BitEngine)

