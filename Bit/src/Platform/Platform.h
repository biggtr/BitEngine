#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Renderer/GraphicsContext.h"
#include <string>
namespace BitEngine
{


class Platform
{
public:
    virtual ~Platform() = default;

    virtual b8 Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void ProcessEvents() = 0;
    virtual void OnUpdate() = 0;

    virtual u32 GetWidth() const = 0;
    virtual u32 GetHeight() const = 0; 

    virtual GraphicsContext* GetGraphicsContext() const= 0;

    static Platform* Create(u32 width, u32 height, const std::string& name);

};

}

