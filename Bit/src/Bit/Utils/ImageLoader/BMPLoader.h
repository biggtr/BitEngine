#pragma once

#include "ImageLoader.h"

#pragma pack(push, 1)
struct BMPFileHeader
{
    uint16_t Signature; //"BM Header" // if we didnt disable padding compiler will add 2 empty bytes and this will mess our file structure  

    uint32_t FileSize;

    uint32_t Reserved;

    uint32_t Offset;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BMPInfoHeader
{
    uint32_t HeaderSize;
    int32_t Width;
    int32_t Height;
    uint16_t Planes;
    uint16_t BitsPerPixel;
    uint32_t CompressionMethod;
    uint32_t ImageSize;
    int32_t XPixelsPerMeter;
    int32_t YPixelsPerMeter;
    uint32_t ColorsUsed;    
    uint32_t ColorsImportant;
    
};
#pragma pack(pop)

class BMPLoader : public IImageLoader
{
public:
    BMPLoader(){}
    ~BMPLoader(){};

    std::vector<uint8_t> Load(const char* path, int& width, int& height, int& channels);



private:
    BMPFileHeader m_BMPFileHeader;
    BMPInfoHeader m_BMPInfoHeader;
};
