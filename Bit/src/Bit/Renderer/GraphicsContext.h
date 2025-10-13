#pragma once
#include "Bit/Core/Defines.h"
namespace BitEngine
{
class GraphicsContext 
{
public:

    virtual ~GraphicsContext() = default;
    virtual b8 Initialize(void* display, void* window, i32 screen) = 0;
    virtual void SwapBuffers() = 0;

    static GraphicsContext* Create();
};
}
