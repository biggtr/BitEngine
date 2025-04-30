#pragma once
#include "Bit/Core/Logger.h"
#include <glad/glad.h>
#include <iostream>

static void ClearGLErrors()
{
    while (glGetError() != GL_NO_ERROR);
}
static bool CheckGLError(const char* functionName, size_t line, const char* file)
{
    bool isError = false ;
    while (unsigned int newError = glGetError()) 
    {
        isError = true;
        std::cerr << " [OpenGL: Error] : " << newError << " Function Name: " << functionName 
            << " Line: " << line << " File: " << file << "\n";
    }
    return isError;
}

#define GLCall(x)\
            ClearGLErrors();\
            x;\
            CheckGLError(#x, __LINE__, __FILE__);


