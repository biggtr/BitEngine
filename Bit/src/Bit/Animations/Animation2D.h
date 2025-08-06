#pragma once 
#include "Bit/Scene/Compontents.h"
namespace BitEngine
{
class Animation2D
{
private:
     
    
public:
    Animation2D(){}
    ~Animation2D(){}

    void GetCurrentUV(CAnimation2D& animation, uint32_t sheetWidth, uint32_t sheetHeight);
};
}
