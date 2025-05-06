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
    src/Bit/Renderer/RendererAPI.cpp
    src/Bit/Renderer/GraphicsContext.cpp
    src/Bit/Renderer/Texture.cpp
    src/Bit/Renderer/Shader.cpp
    src/Bit/Renderer/VertexArray.cpp
    src/Bit/Renderer/Buffers.cpp
    src/Bit/Scene/Entity.cpp
    src/Bit/Scene/Scene.cpp

    src/Platform/OpenGL/OpenGLBuffers.cpp
    src/Platform/OpenGL/OpenGLRendererAPI.cpp
    src/Platform/OpenGL/OpenGLVertexArray.cpp
    src/Platform/OpenGL/OpenGLContext.cpp
    src/Platform/OpenGL/OpenGLShader.cpp
    src/Platform/OpenGL/OpenGLTexture.cpp
    src/Platform/Windows/CrsPlatWindow.cpp
    
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
target_compile_options(BitEngine PRIVATE -Wall -Wextra -Wpedantic) # Enables extra warning
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
