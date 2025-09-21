find_package(OpenGL REQUIRED)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland support")

add_subdirectory(vendor/glad)


add_library(BitEngine STATIC)
target_sources(BitEngine
    PRIVATE

    src/Bit/Core/Application.cpp
    src/Bit/Core/Logger.cpp
    src/Bit/Core/EntryPoint.cpp
    src/Bit/Core/Platform.cpp
    src/Bit/Core/Event.cpp
    src/Bit/Core/Input.cpp

    src/Bit/Renderer/RendererAPI.cpp
    src/Bit/Renderer/RenderCommand.cpp
    src/Bit/Renderer/Renderer.cpp
    src/Bit/Renderer/Renderer2D.cpp
    src/Bit/Renderer/GraphicsContext.cpp
    src/Bit/Renderer/Texture.cpp
    src/Bit/Renderer/Shader.cpp
    src/Bit/Renderer/VertexArray.cpp
    src/Bit/Renderer/Buffers.cpp
    src/Bit/Renderer/Material.cpp
    src/Bit/Renderer/Geometry.cpp

    src/Bit/Renderer/Camera.cpp
    src/Bit/Renderer/CameraManager.cpp

    src/Bit/Scene/EntityManager.cpp
    src/Bit/Scene/Scene.cpp
    src/Bit/Systems/System.cpp


    src/Bit/Resources/AssetManager.cpp
    src/Bit/Resources/ShaderManager.cpp
    src/Bit/Resources/MaterialManager.cpp
    src/Bit/Resources/GeometryManager.cpp
    

    src/Bit/Font/Font.cpp

    src/Bit/Math/Vector.cpp
    src/Bit/Math/Matrix.cpp

    src/Bit/UI/BitUI.cpp

    src/Bit/Physics/BPhysics.cpp
    src/Bit/Physics/BCollision.cpp


    vendor/stb_image/stb_image.cpp
    

    src/Platform/OpenGL/OpenGLBuffers.cpp
    src/Platform/OpenGL/OpenGLRendererAPI.cpp
    src/Platform/OpenGL/OpenGLVertexArray.cpp
    src/Platform/OpenGL/OpenGLContext.cpp
    src/Platform/OpenGL/OpenGLShader.cpp
    src/Platform/OpenGL/OpenGLTexture.cpp
    src/Platform/Windows/PlatformLinux.cpp
    
)

target_include_directories(BitEngine
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    vendor/glad/include        
    vendor/stb_image
)
target_compile_definitions(BitEngine
    PRIVATE
    $<$<CONFIG:Debug>:BIT_DEBUG=1>
)
target_link_libraries(BitEngine glad X11 ${CMAKE_THREAD_LIBS_INIT} OpenGL::GL)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(BitEngine 
        PRIVATE -Wall -Wextra -Wpedantic 
        -Wno-unused-function -Wno-unused-parameter -Wno-pedantic 
    ) 
endif()
set_target_properties(BitEngine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin-int/$<CONFIG>/${PLATFORM_NAME}"
    OUTPUT_NAME "BitEngine"  
)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets 
     DESTINATION "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/${PLATFORM_NAME}"
)
