#pragma once 
#include "Bit/Core/Defines.h"


namespace BitEngine
{


struct Clock
{
    f64 Start;
    f64 Elapsed;
};


void ClockStart(Clock* clock);
void ClockUpdate(Clock* clock);
void ClockStop(Clock* clock);
}
