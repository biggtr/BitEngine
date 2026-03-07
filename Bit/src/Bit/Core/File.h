#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include <stdio.h>
#include <string>

enum class FILE_MODE
{
    FILE_MODE_READ_BINARY,
    FILE_MODE_WRITE_BINARY,
};
struct FileHandle
{
    FILE* Handle;
    FILE_MODE Mode;
    b8 IsValid;
};

FileHandle  FileOpen (const char* path, FILE_MODE mode);
void        FileClose(FileHandle* file);
void WriteBool(b8 value);
void WriteChar(char value);
void WriteInt(i32 value);
void WriteUInt(u32 value);
void WriteFloat(f32 value);
void WriteVec2(BMath::Vec2 v);
void WriteVec3(BMath::Vec3 v);
void WriteVec4(BMath::Vec4 v);

b8 ReadBool();
char ReadChar();
int ReadInt();
u32 ReadUInt();
f32 ReadFloat();
BMath::Vec2 ReadVec2();
BMath::Vec3 ReadVec3();
BMath::Vec4 ReadVec4();
const char* ReadString();
