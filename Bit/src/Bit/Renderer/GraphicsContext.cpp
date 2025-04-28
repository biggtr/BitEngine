#include <glad/glad.h>
#include "../Core/Logger.h"
#include "GraphicsContext.h"
#include "GLFW/glfw3.h"
#include <iostream>

namespace BitEngine
{

    
    GraphicsContext::GraphicsContext(GLFWwindow* window)
    {
        m_Window = window;
    }

    void GraphicsContext::Init()
    {
        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return;
        }  
    }

    void GraphicsContext::SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }

}
