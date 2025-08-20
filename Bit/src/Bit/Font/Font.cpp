#include "Font.h"
#include "Bit/Core/Logger.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


namespace BitEngine
{

static FontState fontState;

u16 ReadU16BE(u8* buffer, u32 offset)
{
    return (u16)((buffer[offset] << 8) | (buffer[offset + 1]));
}
u32 ReadU32BE(u8* buffer, u32 offset)
{
    return (u32)((buffer[offset] << 24) | (buffer[offset + 1] << 16) | (buffer[offset + 2] << 8) | (buffer[offset + 3]));
}
i16 ReadI16BE(u8* buffer, u32 offset)
{
    return (i16)((buffer[offset] << 8) | (buffer[offset + 1]));
}
i32 ReadI32BE(u8* buffer, u32 offset)
{
    return (i32)((buffer[offset] << 24) | (buffer[offset + 1] << 16) | (buffer[offset + 2] << 8) | (buffer[offset + 3]));
}
void Font::ParseFont(const char* fontPath)
{
    FILE* fontFile;
    fontFile = fopen(fontPath, "rb");

    fseek(fontFile, 0, SEEK_END);
    u64 fileSize = ftell(fontFile);
    rewind(fontFile);
    fontState.buffer = new unsigned char[fileSize];
    unsigned char* buffer = fontState.buffer;
    fread(fontState.buffer, 1, fileSize, fontFile);


    u16 numTables = (buffer[4] << 8) | buffer[5];
    u32 glyfOffset = 0;
    u32 glyfTableLength = 0;
    BIT_LOG_DEBUG(" number of tables : %d", numTables); 
    for(u32 i = 0; i < numTables; ++i)
    {
        u32 tagIndex = 12 + (16 * i);
        char tagName[5];
        memcpy(tagName, &buffer[tagIndex], 4);
        u32 offsetIndex = 20 + (16 * i);
        u32 offsetValue = ReadU32BE(buffer, offsetIndex); 

        u32 lengthIndex = 24 + (16 * i);
        u32 lengthValue = ReadU32BE(buffer, lengthIndex);
        BIT_LOG_DEBUG("%s at offset %d table length %d", tagName, offsetValue, lengthValue);
        if(strcmp(tagName, "glyf") == 0)
        {
            glyfOffset = offsetValue;
            glyfTableLength = lengthValue;
            BIT_LOG_INFO("GLYF offset : %d length %d", glyfOffset, glyfTableLength);
        }
    }
    GylfDescription gylfDisc;

    // if contoursNum >= 0 -> simple contour 
    gylfDisc.ContourNums = ReadI16BE(buffer, glyfOffset); 
    gylfDisc.XMin = ReadI16BE(buffer, glyfOffset + 2);
    gylfDisc.YMin = ReadI16BE(buffer, glyfOffset + 4);
    gylfDisc.XMax = ReadI16BE(buffer, glyfOffset + 6);
    gylfDisc.YMax = ReadI16BE(buffer, glyfOffset + 8);
    BIT_LOG_INFO("Contours Number : %d bounding box XMin :%d, YMin :%d, XMax :%d, YMax :%d", 
            gylfDisc.ContourNums, gylfDisc.XMin, gylfDisc.YMin, gylfDisc.XMax, gylfDisc.YMax);
        
    u32 gylfSimpleOffset = glyfOffset + sizeof(gylfDisc);
    GylfSimple gylfSimple;
    gylfSimple.ContourEndPts = new u16[gylfDisc.ContourNums];
    if(gylfDisc.ContourNums >= 0)
    {
        for(i16 i = 0; i < gylfDisc.ContourNums; ++i)
        {
            gylfSimple.ContourEndPts[i] = ReadI16BE(buffer, gylfSimpleOffset);
            gylfSimpleOffset += 2;
            BIT_LOG_INFO("ContourEndPts for %d is %d", i, gylfSimple.ContourEndPts[i]);
        }
        u32 InsLenOffset = gylfSimpleOffset; 
        gylfSimple.InstructionLength = ReadI16BE(buffer, InsLenOffset);
        BIT_LOG_DEBUG("Gylf InstructionLength to skip %d", gylfSimple.InstructionLength);
        u32 gylfSimpleFlagOffset = InsLenOffset + 2 + gylfSimple.InstructionLength; 
        u16 numPoints = gylfSimple.ContourEndPts[gylfDisc.ContourNums - 1] + 1;
        gylfSimple.Flags = new u8[numPoints];
        for(u16 i = 0; i < numPoints; ++i)
        {
            gylfSimple.Flags[i] = (buffer[gylfSimpleFlagOffset]);
            gylfSimpleFlagOffset ++;
            BIT_LOG_INFO("flags for %d is %d", i, gylfSimple.Flags[i]);
        }
    }



}

void Font::Shutdown()
{
    delete[] fontState.buffer;
}
}
