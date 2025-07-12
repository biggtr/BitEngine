#include "BMPLoader.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <cmath>

std::vector<uint8_t> BMPLoader::Load(const char* path, int& width, int& height, int& channels)
{

    m_FilePath = path;

    std::ifstream file(path, std::ios::binary);

    if(!file.is_open())
    {
        throw std::runtime_error("Couldn't open the bmp file..!\n");
    }
    file.read(reinterpret_cast<char*>(&m_BMPFileHeader), sizeof(m_BMPFileHeader));

    if(m_BMPFileHeader.Signature != 0x4D42) //4d42 is little endian format
    {
        throw std::runtime_error("Not BMP File..!");
    }

    file.read(reinterpret_cast<char*>(&m_BMPInfoHeader), sizeof(m_BMPInfoHeader));

    if(m_BMPInfoHeader.HeaderSize != 40 &&
        m_BMPInfoHeader.HeaderSize != 108 &&
        m_BMPInfoHeader.HeaderSize != 124
    )
    {
        throw std::runtime_error("Unsupported BMP Format..!");
    }
    
    // bits/pixel = 24, Width = 5  unPaddedRowSize = 120 bits = 3.75 byte we could round it to 4 
    // or use mod to get the reminder and subtract it from 4 to get the padding size to add to the uppadded row size
    int32_t bytesPerPixel = m_BMPInfoHeader.BitsPerPixel / 8;
    int32_t unPaddedRowSize = (bytesPerPixel * m_BMPInfoHeader.Width);
    int32_t rowPadding = (4 - (unPaddedRowSize % 4)) % 4;
    int32_t paddedRowSize = unPaddedRowSize + rowPadding;
    uint32_t absoluteHeight = std::abs(m_BMPInfoHeader.Height);

    width = m_BMPInfoHeader.Width;
    height = absoluteHeight;
    channels = bytesPerPixel;

    std::vector<uint8_t> paddedRowBuffer(paddedRowSize);
    std::vector<uint8_t> pixelBuffer(unPaddedRowSize * absoluteHeight);

    file.seekg(m_BMPFileHeader.Offset, std::ios::beg); // change the pointer position to read from the offset(the beginning of pixel data)
    for(size_t row = 0; row < absoluteHeight; row++)
    {
        file.read(reinterpret_cast<char*>(paddedRowBuffer.data()), paddedRowSize);
        
        size_t targetRow = m_BMPInfoHeader.Height < 0 ? row : absoluteHeight - row - 1; // if height < 0 = top down ,else bot down and should be flipped 

        std::vector<uint8_t> unPaddedRow(paddedRowBuffer.begin(), paddedRowBuffer.begin() + unPaddedRowSize);
        size_t targetOffset = targetRow * unPaddedRowSize;
        std::memcpy(&pixelBuffer[targetOffset], paddedRowBuffer.data(), unPaddedRowSize);


    }

    // Arranging the BMP pixel data from BGR To RGB
    for(size_t i = 0; i < pixelBuffer.size(); i+= bytesPerPixel)
    {

        // Swapping Blue Pixel with Red Pixel cuz bmp format is BGR and we want it RGB
        std::swap(pixelBuffer[i], pixelBuffer[i+2]);

        uint8_t alphaPixel = bytesPerPixel == 4 ? pixelBuffer[i+3] : 0xFF;
        float normalizedAlpha = static_cast<float>(alphaPixel) / 255.0f;
        // Premultiplying alpha channel with all the pixels to handle tranparency 
        // will use alpha directly in the game engine not here so we're not gonna Premultiply it 
        // pixelBuffer[i] = static_cast<uint8_t>(std::round(pixelBuffer[i] * normalizedAlpha));
        // pixelBuffer[i+1] = static_cast<uint8_t>(std::round(pixelBuffer[i+1] * normalizedAlpha));
        // pixelBuffer[i+2] = static_cast<uint8_t>(std::round(pixelBuffer[i+2] * normalizedAlpha));
    }
    return pixelBuffer;
}

