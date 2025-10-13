#include "Clock.h"
#include "Platform/Platform.h"
#include <ctime>

namespace BitEngine
{


void ClockStart(Clock* clock)
{
    clock->Start = PlatformGetAbsoluteTime();
    clock->Elapsed = 0.0f;
}
void ClockUpdate(Clock* clock)
{
    if(clock->Start != 0)
    {
        clock->Elapsed = PlatformGetAbsoluteTime() - clock->Start;
    }
}
void ClockStop(Clock* clock)
{
    clock->Start = 0;
}
}
