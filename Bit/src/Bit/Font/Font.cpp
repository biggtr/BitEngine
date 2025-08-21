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
b8 IsBitOn(u8 byte, u8 index)
{
    return ((byte >> index) & 1) == 1;
}
GylfDescription ReadGylfDescription(u8* buffer, u32& offset)
{
    GylfDescription gylfDisc;
    // if contoursNum >= 0 -> simple contour 
    gylfDisc.ContourNums = ReadI16BE(buffer, offset); 
    gylfDisc.XMin = ReadI16BE(buffer, offset + 2);
    gylfDisc.YMin = ReadI16BE(buffer, offset + 4);
    gylfDisc.XMax = ReadI16BE(buffer, offset + 6);
    gylfDisc.YMax = ReadI16BE(buffer, offset + 8);
    BIT_LOG_INFO("Contours Number : %d bounding box XMin :%d, YMin :%d, XMax :%d, YMax :%d", 
            gylfDisc.ContourNums, gylfDisc.XMin, gylfDisc.YMin, gylfDisc.XMax, gylfDisc.YMax);
    offset += 10;
    return gylfDisc;
}
GylfSimple ReadSimpleGylf(u8* buffer,i16 contourNumbers, u32& gylfSimpleOffset)
{
    GylfSimple gylfSimple;
    gylfSimple.ContourEndPts = new u16[contourNumbers];
    if(contourNumbers >= 0)
    {
        for(i16 i = 0; i < contourNumbers; ++i)
        {
            gylfSimple.ContourEndPts[i] = ReadU16BE(buffer, gylfSimpleOffset);
            gylfSimpleOffset += 2;
            BIT_LOG_INFO("ContourEndPts for %d is %d", i, gylfSimple.ContourEndPts[i]);
        }
        u32 InsLenOffset = gylfSimpleOffset; 
        gylfSimple.InstructionLength = ReadU16BE(buffer, InsLenOffset);
        BIT_LOG_DEBUG("Gylf InstructionLength to skip %d", gylfSimple.InstructionLength);
        u32 gylfSimpleFlagOffset = InsLenOffset + 2 + gylfSimple.InstructionLength; 
        u16 numPoints = gylfSimple.ContourEndPts[contourNumbers - 1] + 1;
        gylfSimple.Flags = new u8[numPoints];
        BIT_LOG_DEBUG("Numb Points, %d", numPoints)
        gylfSimple.XCoordinates = new i16[numPoints];
        gylfSimple.YCoordinates = new i16[numPoints];
        u32 i = 0;
        while(i < numPoints)
        {
            u8 flag = buffer[gylfSimpleFlagOffset++];
            gylfSimple.Flags[i] = flag;
            if(IsBitOn(flag, 3))
            {
                u8 repeatedCount = buffer[gylfSimpleFlagOffset++];
                for(u8 j = 0; j < repeatedCount; ++j)
                {
                    gylfSimple.Flags[++i] = flag;
                    BIT_LOG_DEBUG("Flag %d is not repeated", i);
                }
            }
            BIT_LOG_DEBUG("Flag[%d] = 0x%02X", i, flag);
            ++i;
        }
       // BIT_LOG_DEBUG("About to read coordinate data at offset %d, buffer values: 0x%02X 0x%02X", 
       //        gylfSimpleFlagOffset, buffer[gylfSimpleFlagOffset], buffer[gylfSimpleFlagOffset+1]); 
       i16 currentX = 0;
       for(u32 i = 0; i < numPoints; ++i)
       {
           u8 flag = gylfSimple.Flags[i];
           if(IsBitOn(flag, 1))
           {
               u8 delta = buffer[gylfSimpleFlagOffset++];
               if(IsBitOn(flag, 4))
                   currentX += delta;
               else
                   currentX -= delta;
           }
           else
           {
               if(IsBitOn(flag, 4))
               {
               }
               else
               {
                   currentX += ReadI16BE(buffer, gylfSimpleFlagOffset);
                   gylfSimpleFlagOffset += 2;
               }
           }
           gylfSimple.XCoordinates[i] = currentX;
           BIT_LOG_DEBUG("point x.%d xcoord : %d", i, currentX);

       }
        i32 currentY = 0;
        for(u32 i = 0; i < numPoints; ++i)
        {
            u8 flag = gylfSimple.Flags[i];
            if(IsBitOn(flag, 2))
            {
                u8 delta = buffer[gylfSimpleFlagOffset++];
                if(IsBitOn(flag, 5))  
                    currentY += delta;
                else
                    currentY -= delta;
            }
            else
            {
                if(IsBitOn(flag, 5))
                {
                }
                else
                {
                    currentY += ReadI16BE(buffer, gylfSimpleFlagOffset);
                    gylfSimpleFlagOffset += 2;
                }
            }
            gylfSimple.YCoordinates[i] = currentY;
            BIT_LOG_DEBUG("point y.%d ycoord : %d", i, currentY);
        }
    }
    return gylfSimple;
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
    GylfDescription gylfDisc = ReadGylfDescription(buffer, glyfOffset);
        
    
    GylfSimple gylfSimple = ReadSimpleGylf(buffer,gylfDisc.ContourNums, glyfOffset);

    delete[] gylfSimple.XCoordinates;
    delete[] gylfSimple.YCoordinates;
    delete[] gylfSimple.Flags;
    delete[] gylfSimple.ContourEndPts;
}
   


void Font::Shutdown()
{
    delete[] fontState.buffer;
}


}

