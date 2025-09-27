#include "Platform.h"
#include "Platform/Windows/PlatformLinux.h"
#include "Platform/Windows/PlatformWindows.h"


namespace BitEngine
{

PlatformType Platform::s_Platform = PLATFORM_LINUX;
Platform* Platform::Create(u32 width, u32 height, const std::string& name)
{

    switch (Platform::GetPlatformType()) 
    {

        case PLATFORM_NONE:
             return nullptr;
        case PLATFORM_LINUX:
            return new PlatformLinux(width, height, name);
        case PLATFORM_WINDOWS:
            return nullptr;

    }
    return nullptr;
}


}



