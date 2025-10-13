#pragma once
#include "Bit/Core/Defines.h"
namespace BitEngine
{
class GraphicsContext 
{
public:

    virtual ~GraphicsContext() = default;
    virtual b8 Initialize() = 0;
    virtual void SwapBuffers() = 0;
};
}
