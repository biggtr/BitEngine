cmake_minimum_required(VERSION 3.10)
add_library(BitEngine STATIC)
target_sources(BitEngine
    PRIVATE
        src/Bit/Application.cpp
        src/Bit/Log.cpp
        src/Bit/EntryPoint.cpp

)
target_include_directories(BitEngine
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
