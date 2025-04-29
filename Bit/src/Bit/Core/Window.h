#pragma once

#include "Bitpch.h"
#include "Bit/Renderer/GraphicsContext.h"
namespace BitEngine
{

class GraphicsContext;
class Window
{


public:
    Window();
    ~Window();

    void Create(unsigned int windowWidth, unsigned int windowHeight, char* windowName);
    bool ShouldClose();
    GLFWwindow* GetGLFWWindow();


    void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    void ProcessInput();
    void OnUpdate();

    unsigned int GetWidth() { return m_Width; }
    unsigned int GetHeight() { return m_Height; }
private:
    unsigned int m_Width, m_Height;
    GLFWwindow* m_Window;

    GraphicsContext* m_Context;
};

}
