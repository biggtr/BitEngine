#pragma once

#include "Bit/Core/TimeStamp.h"
namespace BitEngine
{
class Window;
class Shader;
class Texture;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class RendererAPI;
class Renderer2D;

struct EngineComponents
{
    BitEngine::Window* Window;
    BitEngine::Renderer2D* Renderer2D;

};
        
class Application
{
public:
    Application();
    virtual ~Application();
    virtual void Run();
    void InitializeEngineSystems(const EngineComponents& engineComponents);

protected:
    virtual void OnInit();
    virtual void OnUpdate(float deltaTime);
    virtual void OnRender();


private:
    bool m_IsRunning;
    Time m_Time;
    EngineComponents m_EngineComponents;

};
Application* CreateApplication();

}
