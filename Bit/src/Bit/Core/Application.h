#pragma once
#include "Bit/Core/TimeStamp.h"
#include <cstdint>
namespace BitEngine
{
class Game;
class Platform;
class Renderer2D;
class EntityManager;        
class AssetManager;
class Camera2DManager;
class Camera3DManager;
class EventManager;
class Input;
class EventContext;
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
    BitEngine::Platform* m_Window;
    Renderer2D* m_Renderer2D;
    EntityManager* m_EntityManager;
    AssetManager* m_AssetManager;
    Camera2DManager* m_Camera2DManager;
    Camera3DManager* m_Camera3DManager;
    EventManager* m_EventManager;
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
    inline Camera2DManager& GetCameraManager() { return *m_Camera2DManager; }
    inline Camera3DManager& GetCamera3DManager() { return *m_Camera3DManager; }
private:
    static b8 ApplicationOnEvent(u16 code, void* sender, void* listener, EventContext data);
    static b8 ApplicationOnKey(u16 code, void* sender, void* listener, EventContext data);
};
}
