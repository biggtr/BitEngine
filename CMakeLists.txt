cmake_minimum_required(VERSION 3.30)
project(BitEnigne)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF) # avoids using gnu+20 and uses c++20
set(CMAKE_PCH_ENABLED ON)

if(WIN32)
    set(PLATFORM_NAME "Win")
elseif(UNIX)
    set(PLATFORM_NAME "Linux")
endif()

add_subdirectory(Bit)
add_subdirectory(Games)
