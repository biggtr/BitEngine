#pragma once 
#include "Bit/Core/Defines.h"

namespace BMath
{

void RandomSeed(u32 seed);

u32 RandomU32();

f32 RandomFloat();

f32 RandomRange(f32 min, f32 max);
i32 RandomRange(i32 min, i32 max);
}
