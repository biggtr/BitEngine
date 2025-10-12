#include "Platform.h"
#include "Platform/Windows/PlatformLinux.h"
#include "Platform/Windows/PlatformWindows.h"

namespace BitEngine
{

Platform* Platform::Create(u32 width, u32 height, const std::string& name)
{

#ifdef BPLATFORM_WINDOW 
        return PlatformWindows(width, height, name);
#elif BPLATFORM_LINUX
        return new PlatformLinux(width, height, name);
#endif
}


}



