cmake_minimum_required(VERSION 3.30)
project(BitEnigne)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_PCH_ENABLED ON)

add_subdirectory(Bit)
add_subdirectory(Sandbox)
