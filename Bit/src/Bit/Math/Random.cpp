#include "Random.h"

namespace BMath
{

static u32 s_RandomState = 1;
void RandomSeed(u32 seed)
{
    if(seed == 0)
        s_RandomState = 1;
    else
        s_RandomState = seed;
}

u32 RandomU32()
{
    u32 x = s_RandomState; 
    
    x = x ^ (x << 13);
    x = x ^ (x >> 17);
    x = x ^ (x << 5);
    
    s_RandomState = x; 
    return x; 
}

f32 RandomFloat()
{
    u32 rawInteger = RandomU32();
    return (f32)rawInteger / 4294967295.0f;
}

f32 RandomRange(f32 min, f32 max)
{
    f32 t = RandomFloat(); // random float from 0.0,1.0
    return min + t * (max - min);
}
i32 RandomRange(i32 min, i32 max)
{
    f32 t = RandomFloat(); // random float from 0.0,1.0
    // rangesize from 5..7 (5,6,7) 7 - 5 = 2 -> +1 = 3
    f32 rangeSize = (f32)(max - min) + 1;
    
    f32 value = (f32)min + t * rangeSize; // if t was 1 value would be bigger than max value and if theres a decimal part we have to truncate

    if(value > (f32)max)
        value = (f32)max;

    return (i32)value;
}

}
