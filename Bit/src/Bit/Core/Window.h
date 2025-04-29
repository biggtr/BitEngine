#pragma once

#include "Bitpch.h"
#include "Bit/Renderer/GraphicsContext.h"
namespace BitEngine
{

class GraphicsContext;
class Window
{

private:
    GLFWwindow* m_Window;

    GraphicsContext* m_Context;

public:
    Window();
    ~Window();

    void Create(unsigned int windowWidth, unsigned int windowHeight, char* windowName);
    bool ShouldClose();
    GLFWwindow* GetGLFWWindow();

    void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    void ProcessInput();
    void OnUpdate();
            
};

}
