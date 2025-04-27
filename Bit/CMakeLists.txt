cmake_minimum_required(VERSION 3.30)
find_package(OpenGL REQUIRED)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland support")

add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/ImageLoader)


add_library(BitEngine STATIC)
target_sources(BitEngine
    PRIVATE

    src/Bit/Core/Application.cpp
    src/Bit/Core/Logger.cpp
    src/Bit/Core/EntryPoint.cpp
    src/Bit/Core/Window.cpp
    src/Bit/Renderer/GraphicsContext.cpp
)

target_include_directories(BitEngine
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    vendor/glad/include        
    vendor/GLFW/include
    vendor/ImageLoader/include
)
target_precompile_headers(BitEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Bitpch.h

)
target_link_libraries(BitEngine glfw glad ImageLoader ${CMAKE_THREAD_LIBS_INIT} OpenGL::GL)
