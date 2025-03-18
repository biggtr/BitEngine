#include <glad/glad.h>
#include "GraphicsContext.h"
#include <iostream>

namespace BitEngine
{

void GraphicsContext::Create(GLFWwindow* window)
{
    m_Window = window;
    glfwMakeContextCurrent(m_Window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }  
}


}
