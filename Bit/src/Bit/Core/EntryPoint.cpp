#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Bit/Core/Application.h"
#include "./Window.h"
#include "Renderer/GraphicsContext.h"
extern BitEngine::Application* BitEngine::CreateApplication();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{


    BitEngine::Window window(SCR_WIDTH, SCR_HEIGHT,(char*)"Hello");

    
    BitEngine::GraphicsContext context(window.GetGLFWWindow());
    // render loop
    // -----------
    while (!window.ShouldClose())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        window.SwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT);

    }

    auto app = BitEngine::CreateApplication();
    app->Run();
    delete app;
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
