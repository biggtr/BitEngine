#include "OpenGLContext.h"
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
namespace BitEngine
{


    
OpenGLContext::OpenGLContext(GLFWwindow* window)
    :m_Window(window)
{
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }  
}

void OpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

}
