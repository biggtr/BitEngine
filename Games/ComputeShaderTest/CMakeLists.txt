cmake_minimum_required(VERSION 3.30)
add_executable(ComputeShader)
target_sources(ComputeShader
    PRIVATE
   src/ComputeShaderEntry.cpp
   src/ComputeShader.cpp
)
target_include_directories(ComputeShader
   PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(ComputeShader PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PLATFORM_NAME}"
    OUTPUT_NAME "ComputeShader"  
)

target_link_libraries(ComputeShader PRIVATE BitEngine)

