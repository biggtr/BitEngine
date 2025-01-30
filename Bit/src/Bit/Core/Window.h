#include "GLFW/glfw3.h"
namespace BitEngine
{

class Window
{

private:
    GLFWwindow* m_window;

public:
    Window(unsigned int windowWidth, unsigned int windowHeight, char* windowName);
    ~Window();

    bool ShouldClose();
    void SwapBuffers();
    GLFWwindow* GetGLFWWindow();
};

}
