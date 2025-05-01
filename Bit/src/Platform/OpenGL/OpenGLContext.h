#pragma once
#include "Bit/Renderer/GraphicsContext.h"
struct GLFWwindow;
namespace BitEngine
{

class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(GLFWwindow* window);
    virtual void Init() override;
    virtual void SwapBuffers() override;
private:
    GLFWwindow* m_Window;
};
}
