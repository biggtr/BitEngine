#include "GLFW/glfw3.h"
#include "Renderer/GraphicsContext.h"
namespace BitEngine
{


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
    void SwapBuffers();
    GLFWwindow* GetGLFWWindow();

    void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    void ProcessInput();
            
};

}
