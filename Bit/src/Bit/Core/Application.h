#pragma once
#include "Bit/Assets/AssetManager.h"
#include "Bit/Core/TimeStamp.h"
#include <cstdint>
namespace BitEngine
{
class Game;
class Window;
class Renderer2D;
class EntityManager;        
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
    EntityManager* m_EntityManager;
    AssetManager* m_AssetManager;
    static Application* s_Instance;
public:
    Application(){}
    virtual ~Application();
    static bool Create(Game* gameInstance);
    static void Run();
    static bool Shutdown();
    bool Initialize(ApplicationConfig appCfg);

    inline static Application& GetApplication() { return *s_Instance; }
    inline Renderer2D& GetRenderer() { return *m_Renderer2D; }
    inline EntityManager& GetEntityManager() { return *m_EntityManager; }
    inline AssetManager& GetAssetManager() { return *m_AssetManager; }
};
}
