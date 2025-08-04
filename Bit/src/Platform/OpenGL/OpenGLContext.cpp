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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

}
