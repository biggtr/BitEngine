cmake_minimum_required(VERSION 3.30)
project(BitEngine)



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_PCH_ENABLED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(WIN32)
    set(PLATFORM_NAME "Win")
elseif(UNIX)
    set(PLATFORM_NAME "Linux")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -DBIT_DEBUG -Wall -Wextra -fsanitize=address -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS_RELEASE " -O3 -DNDEBUG -Wall")
endif()

add_subdirectory(Bit)
add_subdirectory(Games)
