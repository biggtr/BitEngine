#pragma once
#include <glad/glad.h>
#include <iostream>

#ifdef _DEBUG
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
#else 
#define GLCall(x) x;
#endif
