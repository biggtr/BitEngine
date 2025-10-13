#pragma once
#include "Bit/Core/Event.h"
#include "Bit/Core/TimeStamp.h"
#include "Platform/Platform.h"
#include <cstdint>

namespace BitEngine
{
class Game;
class Platform;
class Renderer;
class Renderer2D;
class EntityManager;
class AssetManager;
class CameraManager;
class EventManager;

struct ApplicationConfig
{
    uint32_t x = 100;
    uint32_t y = 100;
    uint32_t width = 1280;
    uint32_t height = 720;
    const char* name = "BitEngine";
};

class Application
{
public:
    Application();
    ~Application();

    b8 Create(Game* gameInstance);
    void Run();
private:
    b8 OnEvent(u16 code, EventContext data);
    b8 OnKey(u16 code, EventContext data);
    static b8 OnApplicationEventWrapper(u16 code, void* sender, void* listener, EventContext data);
    static b8 ApplicationOnKeyWrapper(u16 code, void* sender, void* listener, EventContext data);
private:
    Game* m_GameInstance;
    Time m_Time;
    b8 m_IsRunning = false;
    b8 m_IsSuspended;
    PlatformState m_Platform;
    u16 m_Width;
    u16 m_Height;


    Renderer* m_Renderer3D;
    Renderer2D* m_Renderer2D;
    EntityManager* m_EntityManager;
    AssetManager* m_AssetManager;
    CameraManager* m_CameraManager;


    u64 m_LoggerSystemMemReq;
    void* m_LoggerSystem;

    u64 m_MemorySystemMemReq;
    void* m_MemorySystem;

    u64 m_InputSystemMemReq;
    void* m_InputSystem;

    u64 m_EventSystemMemReq;
    void* m_EventSystem;

    u64 m_Physics2DSystemMemReq;
    void* m_Physics2DSystem;
};

}
