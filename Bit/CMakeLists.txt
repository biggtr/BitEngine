cmake_minimum_required(VERSION 3.30)
find_package(OpenGL REQUIRED)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland support")

add_subdirectory(Vendor/GLFW)
add_subdirectory(Vendor/glad)
include_directories(Vendor/GLFW/include)


add_library(BitEngine STATIC)
target_sources(BitEngine
    PRIVATE

    Src/Bit/Core/Application.cpp
    Src/Bit/Core/Logger.cpp
    Src/Bit/Core/EntryPoint.cpp
    Src/Bit/Core/Window.cpp
    Src/Bit/Renderer/GraphicsContext.cpp


)

target_include_directories(BitEngine
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Src
    Vendor/glad/include        
    Vendor/GLFW/include
)
target_precompile_headers(BitEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/Bitpch.h

)
target_link_libraries(BitEngine glfw glad ${CMAKE_THREAD_LIBS_INIT} OpenGL::GL)
