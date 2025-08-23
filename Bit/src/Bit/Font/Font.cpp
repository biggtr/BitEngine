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
CmapFormat12Table ReadCmapSubtable(u8* buffer, u32& offset)
{
    CmapFormat12Table subtable;
    subtable.Format = ReadU16BE(buffer, offset);
    subtable.Length = ReadU32BE(buffer, offset + 4);
    subtable.NGroups = ReadU32BE(buffer, offset + 12);

    offset+= 16;
    BIT_LOG_DEBUG("subtable.Format %d, subtable.length %d, subtable.NGroups : %d", 
            subtable.Format, subtable.Length, subtable.NGroups
            );
    return subtable;
}
CharacterGroup* ReadCharGroups(u8* buffer, u32 nGroups, u32& offset)
{
    CharacterGroup* charGroups = new CharacterGroup[nGroups];

    for(u32 i = 0; i < nGroups; ++i)
    {
        charGroups[i].StartCharCode = ReadU32BE(buffer, offset + (12 * i));
        charGroups[i].EndCharCode = ReadU32BE(buffer, (offset + 4) + (12 * i));
        charGroups[i].StartGylfCode = ReadU32BE(buffer, (offset + 8) + (12 * i));

        BIT_LOG_DEBUG("group %d StartCharCode U+%04X EndCharCode U+%04X StartGylfCode %d", 
                i,
                charGroups[i].StartCharCode,
                charGroups[i].EndCharCode,
                charGroups[i].StartGylfCode
                );
    }
    return charGroups;
}
void StrToUnicodes(u32* unicode, const char* str)
{

    u32 i = 0;
    while(i < strlen(str))
    {
        if(!IsBitOn(str[i], 8))
        {
            unicode[i] = (u32)str[i];
            ++i;
        }
        else if(IsBitOn(str[i], 8) && IsBitOn(str[i], 7) && !IsBitOn(str[i], 6))
        {
            u32 firstByte = str[i] << 3;
            u32 secondByte = str[i + 1] << 2;
            unicode[i] = (firstByte << 6) | secondByte;
            i += 2;
        }
    }
}
u32 GylfCharSearch(const CharacterGroup& charGroup, u32& unicode)
{
    if(unicode < charGroup.StartCharCode || unicode > charGroup.EndCharCode)
        return 0;

    return charGroup.StartGylfCode + (unicode - charGroup.StartCharCode);
}
u32* GetGylfFromString(CharacterGroup* charGroups,u32 nGroups, const char* str)
{
    u32 strLen = strlen(str);
    
    u32* unicodes = new u32[strLen];
    u32* gylfIndices = new u32[strLen];
    StrToUnicodes(unicodes, str);

    memset(gylfIndices, 0, sizeof(u32) * strLen);

    for(u32 j = 0; j < strLen; ++j)
    {
        for(u32 i = 0; i < nGroups; ++i)
        {
            u32 gylfIndex = GylfCharSearch(charGroups[i], unicodes[j]);
            if(gylfIndex != 0)
            {
                gylfIndices[j] = gylfIndex;
                BIT_LOG_DEBUG("Glyf Index For %c is %d unicode : U+04%x", str[j], gylfIndex, unicodes[j])
                break;
            }
        }
    }
    delete[] unicodes;
    return gylfIndices;
}
void Font::ParseFont(const char* fontPath)
{

    FILE* fontFile;
    fontFile = fopen(fontPath, "rb");

    fseek(fontFile, 0, SEEK_END);
    u64 fileSize = ftell(fontFile);
    rewind(fontFile);
    fontState.Buffer = new unsigned char[fileSize];
    unsigned char* buffer = fontState.Buffer;
    fread(fontState.Buffer, 1, fileSize, fontFile);

    u16 numTables = (buffer[4] << 8) | buffer[5];

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

        if(strcmp(tagName, "head") == 0)
        {
            fontState.HeadTableOffset = offsetValue;
            fontState.HeadTableLength = lengthValue;
            BIT_LOG_INFO("head offset : %d length %d", fontState.HeadTableOffset, fontState.HeadTableLength);
        }

        if(strcmp(tagName, "cmap") == 0)
        {
            fontState.CmapTableOffset= offsetValue;
            fontState.CmapTableLength = lengthValue;
            BIT_LOG_INFO("cmap offset : %d length %d", fontState.CmapTableOffset, fontState.CmapTableLength);
        }
        if(strcmp(tagName, "loca") == 0)
        {
            fontState.LocaTableOffset = offsetValue;
            fontState.LocaTableLength = lengthValue;
            BIT_LOG_INFO("loca offset : %d length %d", fontState.LocaTableOffset, fontState.LocaTableLength);
        }
        if(strcmp(tagName, "glyf") == 0)
        {
            fontState.GylfTableOffset = offsetValue;
            fontState.GylfTableLength = lengthValue;
            BIT_LOG_INFO("GLYF offset : %d length %d", fontState.GylfTableOffset, fontState.GylfTableLength);
        }
    }

    u16 unitsPerEm = ReadU16BE(buffer, fontState.HeadTableOffset + 18);
    BIT_LOG_DEBUG("UnitsPerEm : %d", unitsPerEm);

    u16 numOfSubtables = ReadU16BE(buffer, fontState.CmapTableOffset + 2);
    BIT_LOG_DEBUG("num of subtables: %d", numOfSubtables);

    u32 subtableOffset = 0;
    for(u32 i = 0; i < numOfSubtables; ++i)
    {

        u32 platformIDOffset     = (fontState.CmapTableOffset + 4) + (8 * i);
        u32 platformSpecIDOffset = (fontState.CmapTableOffset + 6) + (8 * i);
        u32 subtable = (fontState.CmapTableOffset + 8) + (8 * i);

        u16 platforID = ReadU16BE(buffer, platformIDOffset);
        u16 platforSpecID = ReadU16BE(buffer, platformSpecIDOffset);
        if(platforID == 3 && platforSpecID == 10)
        {
            subtableOffset = fontState.CmapTableOffset + ReadU32BE(buffer, subtable);
            break;
        }
    }
    BIT_LOG_DEBUG("platform : windows with encoding 10 , subtable Offset is %d ", subtableOffset);

    CmapFormat12Table cmapFormat12Table = ReadCmapSubtable(buffer, subtableOffset); // subtableoffset is pointing to first group now 


    CharacterGroup* charGroups = ReadCharGroups(buffer, cmapFormat12Table.NGroups, subtableOffset);
    
    GylfDescription gylfDisc = ReadGylfDescription(buffer, fontState.GylfTableOffset);
    
    GylfSimple gylfSimple = ReadSimpleGylf(buffer,gylfDisc.ContourNums, fontState.GylfTableOffset);

    const char* hellostr = "Hello";
    
    u32* gylfIndices = GetGylfFromString(charGroups,cmapFormat12Table.NGroups, hellostr);



    delete[] gylfSimple.XCoordinates;
    delete[] gylfSimple.YCoordinates;
    delete[] gylfSimple.Flags;
    delete[] gylfSimple.ContourEndPts;
    delete[] charGroups;
    delete[] gylfIndices;
}
   


void Font::Shutdown()
{
    delete[] fontState.Buffer;
}


}

