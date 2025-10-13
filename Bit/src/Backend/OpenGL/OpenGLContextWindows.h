#pragma once
#ifdef BPLATFORM_WINDOWS
#include "Platform/Platform.h"
#include "Bit/Renderer/GraphicsContext.h"
namespace BitEngine
{

class OpenGLContextWindows : public GraphicsContext
{
public:
    OpenGLContextWindows();
    ~OpenGLContextWindows();    
    virtual b8 Initialize(void* display, void* window, i32 screen) override;
    virtual void SwapBuffers() override;

};
}
#endif
