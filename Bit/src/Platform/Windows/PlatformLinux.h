#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Core/Platform.h"
#include "Bit/Renderer/GraphicsContext.h"
#include <X11/Xlib.h>

namespace BitEngine
{

class PlatformLinux : public Platform
{
private:
    PlatformWindow* m_PlatformWindow;
    u32 m_Width;
    u32 m_Height;
    const char* m_Name;
    Window m_WindowRoot;
    GraphicsContext* m_Context;

public:
    PlatformLinux(u32 width, u32 height, const char* name);
    ~PlatformLinux();
    virtual b8 Initialize() override;
    virtual void Shutdown() override;

    virtual void ProcessEvents() override;
    virtual void OnUpdate() override;

    virtual u32 GetWidth() const override;
    virtual u32 GetHeight() const override; 
    virtual void* GetWindow() const override;
};
}
