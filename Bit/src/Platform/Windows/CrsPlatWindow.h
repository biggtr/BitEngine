
#include "Bit/Core/Window.h"
#include "Bit/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>
namespace BitEngine
{

class CrsPlatWindow : public Window
{


public:
    CrsPlatWindow(unsigned int windowWidth, unsigned int windowHeight, const char* windowName);
    virtual ~CrsPlatWindow() override;

    virtual void OnUpdate() override;
    virtual unsigned int GetWidth() const override { return m_Width; }
    virtual unsigned int GetHeight() const override { return m_Height; }
    virtual void* GetWindow() const override { return (GLFWwindow*)m_Window; }

private:
    unsigned int m_Width, m_Height;
    GLFWwindow* m_Window;
    GraphicsContext* m_Context;
};
}
