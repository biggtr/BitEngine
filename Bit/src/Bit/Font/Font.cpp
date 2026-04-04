#include "Font.h"
#include "Bit/Containers/darray.h"
#include "Bit/Core/Logger.h"
#include <cstdlib>
#include <cstring>
#include <stdio.h>



#define READ_BE16(mem) ((((u8*)(mem))[0] << 8) | (((u8*)(mem))[1]))
#define READ_BE32(mem) ((((u8*)(mem))[0] << 24) | (((u8*)(mem))[1] << 16) | (((u8*)(mem))[2] << 8) | (((u8*)(mem))[3]))
#define P_MOVE(mem, a) ((mem) += (a))

#define ReadByte(mem) (((u8*)(mem))[0]); (P_MOVE((mem), 1))
#define ReadU16(mem) (READ_BE16((mem))); (P_MOVE((mem), 2))
#define ReadU32(mem) (READ_BE32((mem))); (P_MOVE((mem), 4))


struct TableDirectory
{
    char Tag[4];
    u32 CheckSum; 
    u32 Offset; 
    u32 Length;   
};
struct OffsetSubTable
{
    u32 ScalarType; 
    u16 NumTables;
    u16 SearchRange;
    u16 EntrySelector;
    u16 RangeShift;
};
struct FontDirectory 
{
    OffsetSubTable OffSubTable;
    TableDirectory* TableDirectories;
};

char* FontReadFile(const char* fontPath)
{
    FILE* fontFile = fopen(fontPath, "rb");
    if(fontFile)
    {
        fseek(fontFile, 0, SEEK_END);
        u32 fontFileSize = ftell(fontFile);
        fseek(fontFile, 0, SEEK_SET);
        char* fontFileContent = (char*)malloc(fontFileSize + 1);
        fontFileContent[fontFileSize] = '\0';
        i32 readAmout = fread(fontFileContent, sizeof(char), fontFileSize, fontFile);
        if(readAmout)
        {
            fclose(fontFile);
            return fontFileContent;
        }
        free(fontFileContent);
        fclose(fontFile);
        return NULL;
    }
    return NULL;
}
void ReadOffsetSubTable(char** mem, OffsetSubTable* outOffsetSubTable)
{
    char* m = *mem;
    outOffsetSubTable->ScalarType = ReadU32(m);
    outOffsetSubTable->NumTables = ReadU16(m);
    outOffsetSubTable->SearchRange = ReadU16(m);
    outOffsetSubTable->EntrySelector = ReadU16(m);
    outOffsetSubTable->RangeShift = ReadU16(m);
    
    BIT_LOG_INFO("num of tables %d", outOffsetSubTable->NumTables);
    *mem = m;
}
void ReadTableDirectory(char** mem, TableDirectory* outTableDirectories, u32 tablesCount)
{
    char* m = *mem;
    for(u32 i = 0; i < tablesCount; ++i)
    {
        TableDirectory tableDir;
        tableDir.Tag[0] = (char)ReadByte(m);
        tableDir.Tag[1] = (char)ReadByte(m);
        tableDir.Tag[2] = (char)ReadByte(m);
        tableDir.Tag[3] = (char)ReadByte(m);
        tableDir.CheckSum = ReadU32(m);
        tableDir.Offset 	 = ReadU32(m);
        tableDir.Length   = ReadU32(m);
        DArrayPush(outTableDirectories, tableDir);
        BIT_LOG_INFO("Tag : %c%c%c%c location : {%d}\n", tableDir.Tag[0], tableDir.Tag[1], tableDir.Tag[2], tableDir.Tag[3], tableDir.Offset);
    }
    *mem = m;
}

struct CmapEncodingSubtable
{
    u16 PlatformID;
    u16 PlatformSpecificID;
    u32 Offset;
};
struct Cmap
{
    u16 Version;
    u16 NumberSubtables;
    CmapEncodingSubtable* EncodingSubtables;
};
void ReadCmap(char* mem, Cmap* outCmap)
{
    char* m = mem;
    outCmap->Version = ReadU16(m);
    outCmap->NumberSubtables = ReadU16(m);
    BIT_LOG_INFO("Number of subtables : %d", outCmap->NumberSubtables);
    printf("#)\tpId\tpsID\toffset\ttype\n");
    for(u32 i = 0; i < outCmap->NumberSubtables; ++i)
    {
        CmapEncodingSubtable subtable;
        subtable.PlatformID = ReadU16(m);
        subtable.PlatformSpecificID = ReadU16(m);
        subtable.Offset = ReadU32(m);
        DArrayPush(outCmap->EncodingSubtables, subtable);
        printf("%d)\t%d\t%d\t%d\t", i+1, subtable.PlatformID, subtable.PlatformSpecificID, subtable.Offset);
        switch (subtable.PlatformID) 
        {
            case 0:  printf("unicode"); break;
            case 1:  printf("Mac"); break;
            case 2:  printf("Not Supported"); break;
            case 3:  printf("Mircoshit"); break;
        }
        printf("\n");
    }
}
struct Format4
{
    u16 Format;
    u16 Length;
    u16 Language;
    u16 SegCountX2;
    u16 SearchRange;
    u16 EntrySelector;
    u16 RangeShift;
    u16 ReservedPad;
    i32 GlyphIdCount;
    u16* EndCode;
    u16* StartCode;
    u16* IdDelta;
    u16* IdRangeOffset;
    u16* GlyphIdArray;
};
void ReadFormat4(char* mem, Format4* f)
{
    char* m = mem;
    f->Format = ReadU16(m);
    f->Length = ReadU16(m);
    f->Language = ReadU16(m);
    f->SegCountX2 = ReadU16(m);
    f->SearchRange = ReadU16(m);
    f->EntrySelector = ReadU16(m);
    f->RangeShift = ReadU16(m);
    u16 segCount = f->SegCountX2 >> 1; 
    
    f->EndCode = (u16*)m;
    P_MOVE(m, segCount * 2);
    //preserved padding skip
    P_MOVE(m, 2);

    f->StartCode = (u16*)m;
    P_MOVE(m, segCount * 2);

    f->IdDelta = (u16*)m;
    P_MOVE(m, segCount * 2);

    f->IdRangeOffset = (u16*)m;
    P_MOVE(m, segCount * 2);
    i32 remaining_bytes = f->Length - (m - mem);
    f->GlyphIdCount = remaining_bytes / 2;
    f->GlyphIdArray = (u16*)malloc(sizeof(u16) * remaining_bytes);
    for(int i = 0; i < f->GlyphIdCount; ++i) 
    {
        f->GlyphIdArray[i] = ReadU16(m);
    }
    printf("Format: %d, Length: %d, Language: %d, Segment Count: %d\n", f->Format, f->Length, f->Language, f->SegCountX2/2);
    printf("Search Params: (searchRange: %d, entrySelector: %d, rangeShift: %d)\n",
                    f->SearchRange, f->EntrySelector, f->RangeShift);
    printf("Segment Ranges:\tstartCode\tendCode\tidDelta\tidRangeOffset\n");
    for(int i = 0; i < f->SegCountX2/2; ++i) 
    {
        u16 start = READ_BE16(((u8*)f->StartCode) + i * 2);
        u16 end   = READ_BE16(((u8*)f->EndCode) + i * 2);
        u16 delta = READ_BE16(((u8*)f->IdDelta) + i * 2);
        u16 range = READ_BE16(((u8*)f->IdRangeOffset) + i * 2);

        printf("%d--------------:\t%9d\t%7d\t%7d\t%12d\n",
            i, start, end, delta, range);
    }
}
void FontInitialize(const char* fontPath)
{
    char* fontFileContent = FontReadFile(fontPath);
    char* mem = fontFileContent;
    FontDirectory fontDirectory = {};
    fontDirectory.TableDirectories = (TableDirectory*)DArrayCreate(TableDirectory);
    ReadOffsetSubTable(&mem, &fontDirectory.OffSubTable);
    ReadTableDirectory(&mem, fontDirectory.TableDirectories, fontDirectory.OffSubTable.NumTables);
    
    for(u32 i = 0; i < fontDirectory.OffSubTable.NumTables; ++i)
    {
        TableDirectory table = fontDirectory.TableDirectories[i];
        if(strstr(table.Tag, "cmap"))
        {
            Cmap cmap;
            cmap.EncodingSubtables = (CmapEncodingSubtable*)DArrayCreate(CmapEncodingSubtable);
            char* tablePtr = fontFileContent + table.Offset;
            ReadCmap(tablePtr, &cmap);
            char* format4Ptr = tablePtr + cmap.EncodingSubtables[0].Offset;
            Format4 format4;
            ReadFormat4(format4Ptr, &format4);
        }
    }

}
