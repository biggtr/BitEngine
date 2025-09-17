cmake_minimum_required(VERSION 3.30)
add_executable(TestGame)
target_sources(TestGame
    PRIVATE
   src/GameEntry.cpp
   src/TestGame.cpp
)
target_include_directories(TestGame
   PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(TestGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PLATFORM_NAME}"
    OUTPUT_NAME "TestGame"  
)

target_link_libraries(TestGame PRIVATE BitEngine)
