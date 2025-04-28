#include "Window.h"
#include "GLFW/glfw3.h"
namespace BitEngine
{

Window::Window()
{
}

void Window::Create(unsigned int windowWidth, unsigned int windowHeight, char* windowName)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    m_Window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
    if (m_Window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    m_Context = new GraphicsContext(m_Window);
    m_Context->Init();
    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);

    });

}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    delete m_Context;
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(m_Window);
}


GLFWwindow* Window::GetGLFWWindow()
{
    return m_Window;
} 

void Window::ProcessInput()
{
    if(glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(m_Window, true);
}
void Window::OnUpdate()
{
    glfwPollEvents();
    m_Context->SwapBuffers();
}



}



