#pragma once

#include "Bit/Core/Defines.h"
#include <concepts>
namespace BitEngine
{

struct FontState
{
    u8* buffer;
};
struct GylfDescription
{
    i16 ContourNums;
    i16 XMin;
    i16 YMin;
    i16 XMax;
    i16 YMax;
};
struct GylfSimple
{
    u16* ContourEndPts;
    u16 InstructionLength;
    u8* Flags;
    i16* XCoordinates;
    i16* YCoordinates;
};
class Font
{
public: 
    static b8 Initialize();
    static void ParseFont(const char* fontPath);
    static void Shutdown();
};
}
