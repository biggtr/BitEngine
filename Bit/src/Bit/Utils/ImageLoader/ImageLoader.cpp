#include "ImageLoader.h"
#include "BMPLoader.h"
#include <cstring>


IImageLoader* IImageLoader::Create(const char* filePath)
{
    if(strstr(filePath, ".bmp"))
    {
        return new BMPLoader();
    }
    return NULL;
}
