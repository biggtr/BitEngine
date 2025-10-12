#pragma once
#ifdef BPLATFORM_WINDOWS
#include "Platform/Windows/Platform.h"
namespace BitEngine 
{
class PlatformWindows : public Platform
{
private:
    std::string m_Name;
    GraphicsContext* m_Context;
    i32 m_Screen;
    u32 m_Window;
    u32 m_ColorMap;
    u32 m_Width;
    u32 m_Height;

    PlatformWindows(u32 width, u32 height, const std::string& name);
    ~PlatformWindows();
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
