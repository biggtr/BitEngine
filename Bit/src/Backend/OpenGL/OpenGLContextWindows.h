#pragma once
#ifdef BPLATFORM_WINDOWS

#include "Bit/Renderer/GraphicsContext.h"
#include <windows.h> 

namespace BitEngine {

    class OpenGLContextWindows : public GraphicsContext
    {
    public:
        OpenGLContextWindows(HWND windowHandle);

        virtual b8 Initialize() override;
        virtual void SwapBuffers() override;

    private:
        HWND m_WindowHandle;
        HDC m_DeviceContext;
        HGLRC m_RenderingContext;
    };
}

#endif


