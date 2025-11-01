find_package(OpenGL REQUIRED)

add_subdirectory(vendor/glad)

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(ASSIMP_ASAN_RELEASE OFF CACHE BOOL "" FORCE)
set(ASSIMP_UBSAN_RELEASE OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
add_subdirectory(vendor/assimp)


add_library(BitEngine STATIC)
set(BITENGINE_COMMON_SOURCES

    src/Bit/Core/Application.cpp
    src/Bit/Core/Logger.cpp
    src/Bit/Core/EntryPoint.cpp
    src/Bit/Core/Event.cpp
    src/Bit/Core/Input.cpp
    src/Bit/Core/Clock.cpp
    src/Bit/Core/BMemory.cpp

    src/Bit/Containers/darray.cpp

    src/Bit/Renderer/RendererAPI.cpp
    src/Bit/Renderer/RenderCommand.cpp
    src/Bit/Renderer/Renderer.cpp
    src/Bit/Renderer/Renderer2D.cpp
    src/Bit/Renderer/Texture.cpp
    src/Bit/Renderer/Shader.cpp
    src/Bit/Renderer/VertexArray.cpp
    src/Bit/Renderer/Buffers.cpp
    src/Bit/Renderer/Material.cpp
    src/Bit/Renderer/Geometry.cpp
    src/Bit/Renderer/Mesh.cpp

    src/Bit/Renderer/Camera.cpp
    src/Bit/Renderer/CameraManager.cpp

    src/Bit/ECS/EntityManager.cpp
    src/Bit/Scene/Scene.cpp
    src/Bit/ECS/Systems/System.cpp


    src/Bit/Resources/AssetManager.cpp
    src/Bit/Resources/ShaderManager.cpp
    src/Bit/Resources/MaterialManager.cpp
    src/Bit/Resources/GeometryManager.cpp
    
    src/Bit/Particles/ParticleSystem.cpp

    src/Bit/Font/Font.cpp

    src/Bit/Math/BMath.cpp
    src/Bit/Math/Vector.cpp
    src/Bit/Math/Matrix.cpp
    src/Bit/Math/Transform.cpp


    src/Bit/UI/BitUI.cpp

    src/Bit/Physics/BPhysics.cpp
    src/Bit/Physics/BCollision.cpp


    vendor/stb_image/stb_image.cpp
    

    src/Backend/OpenGL/OpenGLBuffers.cpp
    src/Backend/OpenGL/OpenGLRendererAPI.cpp
    src/Backend/OpenGL/OpenGLVertexArray.cpp
    src/Backend/OpenGL/OpenGLShader.cpp
    src/Backend/OpenGL/OpenGLTexture.cpp
    
)

if(WIN32)
    target_compile_definitions(BitEngine PRIVATE BPLATFORM_WINDOWS)
    
    list(APPEND BITENGINE_COMMON_SOURCES
        src/Platform/PlatformWindows.cpp
        src/Backend/OpenGL/OpenGLContextWindows.cpp
    )

    target_link_libraries(BitEngine PRIVATE gdi32 user32 shell32)

elseif(UNIX AND NOT APPLE)
    target_compile_definitions(BitEngine PRIVATE BPLATFORM_LINUX)
    list(APPEND BITENGINE_COMMON_SOURCES
    src/Platform/PlatformLinux.cpp
    src/Backend/OpenGL/OpenGLContextLinux.cpp
    )
    find_package(X11 REQUIRED)
    target_link_libraries(BitEngine PRIVATE X11::X11 ${CMAKE_THREAD_LIBS_INIT})
endif()

target_sources(BitEngine PRIVATE ${BITENGINE_COMMON_SOURCES})

target_include_directories(BitEngine
PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/src
vendor/glad/include        
${CMAKE_BINARY_DIR}/assimp/include        
vendor/stb_image
vendor/glm
)

target_link_libraries(BitEngine PRIVATE glad assimp OpenGL::GL)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(BitEngine 
        PRIVATE 
            -Wall -Wextra -Wpedantic 
            -Wno-unused-function 
            -Wno-unused-parameter 
            -Wno-pedantic 
    ) 
elseif(MSVC)
    target_compile_options(BitEngine 
        PRIVATE 
            /W4
            /wd4100  
    )
endif()

if(WIN32)
    add_custom_command(TARGET BitEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:assimp>
            $<TARGET_FILE_DIR:BitEngine>
        COMMENT "Copying Assimp DLL to output directory..."
    )
endif()
add_custom_command(TARGET BitEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PLATFORM_NAME}/assets
    COMMENT "Copying assets..."
)
