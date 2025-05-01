#include "Window.h"
#include <GLFW/glfw3.h>
#include "Bit/Renderer/RendererAPI.h"
#include "Platform/Windows/CrsPlatWindow.h"
namespace BitEngine
{

Window* Window::Create(unsigned int windowWidth, unsigned int windowHeight, char* windowName)
{

    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new CrsPlatWindow(windowWidth, windowHeight, windowName);
      break;
    }
    return nullptr;
}


}



