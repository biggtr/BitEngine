cmake_minimum_required(VERSION 3.30)
add_executable(PoolGame)
target_sources(PoolGame
    PRIVATE
   src/PoolGameEntry.cpp
   src/PoolGame.cpp
)
target_include_directories(PoolGame
   PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(PoolGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PLATFORM_NAME}"
    OUTPUT_NAME "PoolGame"  
)

target_link_libraries(PoolGame PRIVATE BitEngine)
