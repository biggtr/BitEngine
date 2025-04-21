#include "Application.h"
#include "Window.h"
extern BitEngine::Application* BitEngine::CreateApplication();
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{

    const float vertices[] = 
    {
        -0.5,   0.5,   0.0, 1.0, 
        -0.5,  -0.5,   0.0, 0.0, 
        0.5,  -0.5,   1.0, 0.0, 
        0.5,   0.5,   1.0, 1.0, 
    };
    const float indices[] = 
    {
        0, 1, 2,
        0, 2, 3
    };

    
    
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

