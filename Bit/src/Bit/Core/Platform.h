#pragma once
#include "Bit/Core/Defines.h"
#include "Bitpch.h"
#include <GL/glx.h>
namespace BitEngine
{

enum PlatformType
{
    PLATFORM_LINUX,
    PLATFORM_WINDOWS,
    PLATFORM_NONE
};

struct LinuxWindowData
{
    void* Display;
    i32 Screen;
    u32 Window;
    u32 ColorMap;
    void* VisualInfo;
    GLXContext GLContext;
    GLXFBConfig FBConfig;
};
struct WindowsWindowData
{
    void* hwnd;
    void* hdc;
    
};
struct PlatformWindow
{
    PlatformType Platform;
    u32 Width;
    u32 Height;
    char Title[256];
    b8 ShouldClose;
    union
    {
        LinuxWindowData Linux;
        WindowsWindowData Windows;
    };
};
class Platform
{
public:
    static PlatformType s_Platform;

public:
    virtual ~Platform() = default;

    virtual b8 Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void ProcessEvents() = 0;
    virtual void OnUpdate() = 0;

    virtual u32 GetWidth() const = 0;
    virtual u32 GetHeight() const = 0; 
    virtual void* GetWindow() const = 0;

    static Platform* Create(u32 width, u32 height, const char* name);

    static PlatformType GetPlatform() { return s_Platform; }
    static void SetPlatform(PlatformType platform) { s_Platform = platform; }
    

};

}

