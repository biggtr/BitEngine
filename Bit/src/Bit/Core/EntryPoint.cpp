#include "Bit/Core/Application.h"
#include "GLFW/glfw3.h"
#include "./Window.h"
extern BitEngine::Application* BitEngine::CreateApplication();
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{

    BitEngine::Window  window{};
    window.Create(SCR_WIDTH, SCR_HEIGHT,(char*)"Hello");
    
    // render loop
    // -----------
    while (!window.ShouldClose())
    {
        window.ProcessInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window.SwapBuffers();
        
        glfwPollEvents();
    }
    
    return 0;
}

