#include "Animation2D.h"
#include <cstdint>
#include <cstring>



namespace BitEngine
{
void GetCurrentUV(CAnimation2D& animation, uint32_t sheetWidth, uint32_t sheetHeight, uint32_t frameWidth, uint32_t frameHeight)
{
    const uint8_t columns = sheetWidth / frameWidth;
    const uint8_t rows = sheetHeight / frameHeight;

    const uint8_t x = animation.CurrentFrame % columns;
    const uint8_t y = animation.CurrentFrame / columns;

    float u1,v1,u2,v2;

    u1 = (float)x / columns; 
    v1 = (float)y / rows;
    u2 = u1 + (1.0f / columns);
    v2 = v1 + (1.0f / rows);
    
            
    animation.UVs[0] = u1; animation.UVs[1] = v1;
    animation.UVs[2] = u2; animation.UVs[3] = v1;
    animation.UVs[4] = u2; animation.UVs[5] = v2;
    animation.UVs[6] = u1; animation.UVs[7] = v2;
}

}
