#pragma once
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <string>

enum class IMAGE_FORMAT
{
    BMP, PNG, JPG
};

class IImageLoader
{
public:
    IImageLoader() {}
    virtual ~IImageLoader(){}
    const char* m_FilePath;
    virtual std::vector<uint8_t> Load(const char* path, int& width, int& height, int& channels) = 0;
    static IImageLoader* Create(const char* filePath);
};

class ImageLoader
{

    
public:
    static std::vector<uint8_t> Load(const char* path, int& width, int& height, int& channels)
    {
        IImageLoader* loader = IImageLoader::Create(path);

        if(loader)
        {
            loader->Load(path, width, height, channels);
        }

        delete loader;
        return {};
    }

};
