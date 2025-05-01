#pragma once

#include "Bitpch.h"
namespace BitEngine
{

class Window
{


public:
    virtual ~Window() = default;


    virtual void OnUpdate() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0; 

    virtual void* GetWindow() const = 0;

    static Window* Create(unsigned int windowWidth, unsigned int windowHeight, char* windowName);
};

}
