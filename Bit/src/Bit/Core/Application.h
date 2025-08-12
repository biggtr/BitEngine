#pragma once
#include "Bit/Core/TimeStamp.h"
#include "Bit/Renderer/Camera.h"
#include <cstdint>
namespace BitEngine
{
class Game;
class Platform;
class Renderer2D;
class EntityManager;        
class AssetManager;
class CameraManager;
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
    CameraManager* m_CameraManager;
    EventManager* m_EventManager;
    Input* m_Input;
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
    inline CameraManager& GetCameraManager() { return *m_CameraManager; }

private:
    static b8 ApplicationOnEvent(u16 code, void* sender, void* listener, EventContext data);
    static b8 ApplicationOnKey(u16 code, void* sender, void* listener, EventContext data);
};
}
