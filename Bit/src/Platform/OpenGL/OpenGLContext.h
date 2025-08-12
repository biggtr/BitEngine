#pragma once
#include "Bit/Core/Platform.h"
#include "Bit/Renderer/GraphicsContext.h"
#include <GL/glx.h>
struct GLFWwindow;
namespace BitEngine
{

class OpenGLContext : public GraphicsContext
{
private:
    PlatformWindow* m_Window;
    GLXContext m_GLContext;
public:
    OpenGLContext(PlatformWindow* window);
    virtual b8 Initialize() override;
    virtual void SwapBuffers() override;
    virtual void Shutdown() override;
};
}
