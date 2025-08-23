#pragma once

#include "Bit/Core/Defines.h"
#include <concepts>
namespace BitEngine
{

struct FontState
{
    u8* Buffer;

    u32 HeadTableOffset;
    u32 HeadTableLength;

    u32 GylfTableOffset;
    u32 GylfTableLength;

    u32 CmapTableOffset;
    u32 CmapTableLength;

    u32 LocaTableOffset;
    u32 LocaTableLength;
};
struct CmapFormat12Table
{
    u16 Format;
    u32 Length;
    u32 NGroups;
};
struct CharacterGroup
{
    u32 StartCharCode;
    u32 EndCharCode;
    u32 StartGylfCode;
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
