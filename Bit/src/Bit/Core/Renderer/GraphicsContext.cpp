#include <glad/glad.h>
#include "GraphicsContext.h"
#include <iostream>

namespace BitEngine
{

GraphicsContext::GraphicsContext(GLFWwindow* window)
{
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }  
}


}
