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
struct EngineComponents
{
    BitEngine::Window* Window;
};
        
class Application
{
public:
    Application();
    virtual ~Application();
    virtual void Run();
    void InitializeEngineSystems(EngineComponents* engineComponents);

    //Temporary data will be deleted 

    VertexArray* VAO;
    VertexBuffer* VBO;
    IndexBuffer* IBO;
    Shader* shader;
    Texture* texture;

    




protected:
    virtual void OnInit();
    virtual void OnUpdate(float deltaTime);
    virtual void OnRender();


private:
    bool m_IsRunning;
    Time m_Time;
    EngineComponents* m_EngineComponents;
    RendererAPI* m_RendererAPI;

};
Application* CreateApplication();

}
