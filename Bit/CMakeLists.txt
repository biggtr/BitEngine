cmake_minimum_required(VERSION 3.30)
find_package(OpenGL REQUIRED)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland support")

add_subdirectory(vendor/GLFW)
include_directories(vendor/GLFW/include)


add_library(BitEngine STATIC)
target_sources(BitEngine
    PRIVATE
        src/Bit/Core/Application.cpp
        src/Bit/Core/Log.cpp
        src/Bit/Core/EntryPoint.cpp

)

target_include_directories(BitEngine
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(BitEngine glfw ${CMAKE_THREAD_LIBS_INIT} OpenGL::GL)
