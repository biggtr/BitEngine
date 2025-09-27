#pragma once
#include "Bit/Core/Defines.h"
namespace BitEngine
{
class GraphicsContext 
{
public:
    struct WindowRequirements 
    {
        void* visualInfo;
        unsigned long colormap;
        unsigned long valueMask;
        int depth;
        void* visual;
    };
public:

    virtual ~GraphicsContext() = default;
    virtual WindowRequirements GetWindowRequirements(void* display, int screen) = 0;
    virtual b8 Initialize(void* nativeDisplay, void* nativeWindow, i32 screen) = 0;
    virtual void SwapBuffers() = 0;

    static GraphicsContext* Create();
};
}
