#pragma once
#ifdef BPLATFORM_LINUX
#include "Bit/Core/Defines.h"
#include "Platform.h"
#include "Bit/Renderer/GraphicsContext.h"
#include <string>
#include <X11/Xlib.h>

namespace BitEngine
{

class PlatformLinux : public Platform
{
private:
    std::string m_Name;
    GraphicsContext* m_Context;
    Display* m_Display;
    i32 m_Screen;
    u32 m_Window;
    Window m_WindowRoot;
    u32 m_ColorMap;
    u32 m_Width;
    u32 m_Height;
public:
    PlatformLinux(u32 width, u32 height, const std::string& name);
    ~PlatformLinux();
    virtual b8 Initialize() override;
    virtual void Shutdown() override;

    virtual void ProcessEvents() override;
    virtual void OnUpdate() override;

    virtual u32 GetWidth() const override;
    virtual u32 GetHeight() const override; 
    virtual GraphicsContext* GetGraphicsContext() const override { return m_Context; };
};
}
#endif
