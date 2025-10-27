cmake_minimum_required(VERSION 3.30)
add_executable(Dystopia)
target_sources(Dystopia
    PRIVATE
   src/DystopiaEntry.cpp
   src/Dystopia.cpp
)
target_include_directories(Dystopia
   PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(Dystopia PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PLATFORM_NAME}"
    OUTPUT_NAME "Dystopia"  
)

target_link_libraries(Dystopia PRIVATE BitEngine)
