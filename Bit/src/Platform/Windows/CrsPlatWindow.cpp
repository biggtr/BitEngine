#include "CrsPlatWindow.h"
#include "Bit/Core/Event.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include <GLFW/glfw3.h>
namespace BitEngine
{

CrsPlatWindow::CrsPlatWindow(unsigned int windowWidth, unsigned int windowHeight, const char* windowName)
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
    m_Width = windowWidth;
    m_Height = windowHeight;
    m_Window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
    if (m_Window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    m_Context = GraphicsContext::Create(m_Window);
    m_Context->Init();
    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height){
        BIT_LOG_DEBUG("Window width : %d window height : %d", width, height);
        glViewport(0, 0, width, height);

    });
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



