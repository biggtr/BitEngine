#include "CrsPlatWindow.h"
#include "Bit/Core/Event.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include <GLFW/glfw3.h>
namespace BitEngine
{

CrsPlatWindow::CrsPlatWindow(u32 width, u32 height, const char* name)
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
    //
    m_Width = width;
    m_Height = height;
    m_Window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (m_Window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    m_Context = GraphicsContext::Create(m_Window);
    m_Context->Initialize();
    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height){
        BIT_LOG_DEBUG("Window width : %d window height : %d", width, height);
        glViewport(0, 0, width, height);

    });
}

b8 CrsPlatWindow::Initialize() 
{
    return true;
}
void CrsPlatWindow::Shutdown()
{

}
void CrsPlatWindow::ProcessInput()
{
    if(glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        b8 pressed = true;
        KEYS key = KEY_ESCAPE;
        Input::ProcessKey(key, pressed);
    }
}
void CrsPlatWindow::ProcessEvents()
{

}
void CrsPlatWindow::OnUpdate()
{
    ProcessInput();
    m_Context->SwapBuffers();
    glfwPollEvents();
}
CrsPlatWindow::~CrsPlatWindow()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    delete m_Context;
}

}



