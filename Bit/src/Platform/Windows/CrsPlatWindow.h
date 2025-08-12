#pragma once
#include "Bit/Core/Platform.h"
#include "Bit/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>
namespace BitEngine
{

class CrsPlatWindow : public Platform
{


public:
    CrsPlatWindow(u32 width, u32 height, const char* name);
    virtual ~CrsPlatWindow() override;

    virtual b8 Initialize() override;
    virtual void Shutdown() override;
    virtual void ProcessEvents() override;
    virtual void OnUpdate() override;
    virtual unsigned int GetWidth() const override { return m_Width; }
    virtual unsigned int GetHeight() const override { return m_Height; }
    virtual void* GetWindow() const override { return (GLFWwindow*)m_Window; }
    void ProcessInput();

private:
    unsigned int m_Width, m_Height;
    GLFWwindow* m_Window;
    GraphicsContext* m_Context;
};
}
