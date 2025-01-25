cmake_minimum_required(VERSION 3.30)
add_executable(Sandbox)
target_sources(Sandbox
    PRIVATE
    src/SandboxApplication.cpp
)
target_include_directories(Sandbox
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(Sandbox PRIVATE BitEngine)
