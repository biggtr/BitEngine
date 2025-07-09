#pragma once

#include "Bit/Core/TimeStamp.h"
#include "Bit/Renderer/Renderer2D.h"
#include <cstdint>
namespace BitEngine
{
class Game;
class Window;
class Renderer2D;
        
struct ApplicationConfig
{
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    const char* name;
};

class Application
{

public:
    bool m_IsRunning;
    bool m_IsSuspended;
    Time m_Time;
    uint32_t m_Width;
    uint32_t m_Height;
    bool m_Initialized = false;
    Game* m_GameInstance; // stack owns the game so dont delete it 
    
private:
    BitEngine::Window* m_Window;
    Renderer2D* m_Renderer2D;
    static Application s_Instance;
public:
    Application(){}
    virtual ~Application();
    static bool Create(Game* gameInstance);
    static void Run();
    static bool Shutdown();
    bool Initialize(ApplicationConfig appCfg);

    inline BitEngine::Renderer2D& GetRenderer() { return *s_Instance.m_Renderer2D; }
};
}
