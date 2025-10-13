#include "Bit/Core/Event.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/GraphicsContext.h"
#include "Backend/OpenGL/OpenGLContextWindows.h" 
#include "Platform/Platform.h"
#include <stdlib.h>
#ifdef BPLATFORM_WINDOWS
#include <windowsx.h>
#include <windows.h>

namespace BitEngine
{

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_ERASEBKGND:
            //prevent the os from clearing the screen to prevent flickering app is gonna handle clearing
            return 1;

        case WM_CLOSE:
            BIT_LOG_INFO("WM_CLOSE received, ignoring for now");
            EventFire(EVENT_CODE_APPLICATION_QUIT, 0, {});
            return 0;

        case WM_DESTROY:
            BIT_LOG_INFO("WM_DESTROY received");
            PostQuitMessage(0);
            return 0;

        case WM_SIZE:
        {
            // EventContext data = {};
            // EventManager::EventFire(EVENT_CODE_WINDOW_RESIZED, 0, data);
            break;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            //TODO: handle keyboard input in here

            break;
        }
        case WM_MOUSEMOVE:
            {
                i32 xPosition = GET_X_LPARAM(lParam);
                i32 yPosition = GET_Y_LPARAM(lParam);
                break;
            }
        case WM_MOUSEWHEEL:
            {
                i32 zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
                if(zDelta != 0)
                {
                    zDelta = (zDelta < 0) ? -1 : 1;
                }
                break;
            }
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
            {

                break;
            }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



struct InternalState
{
    HWND WindowHandle;
    HINSTANCE InstanceHandle;
};
b8 PlatformStartup(PlatformState* platformState, const char* applicationName, i32 x, i32 y, u32 width, u32 height)
{
    platformState->InternalState = malloc(sizeof(InternalState));
    InternalState* internalState = (InternalState*)platformState->InternalState;
    
    internalState->InstanceHandle = GetModuleHandle(NULL);
    const char* CLASS_NAME = "BitEngine Window Class";

    HICON icon = LoadIcon(internalState->InstanceHandle, IDI_APPLICATION);
    WNDCLASSA wc = {};
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc   = WindowProc; 
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance     = internalState->InstanceHandle;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = CLASS_NAME;

    if(!RegisterClassA(&wc))
    {
        MessageBoxA(0, "Windows RegisterClass failed", "ERROR", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }
    u32 windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 windowsExStyle = WS_EX_APPWINDOW;

    windowStyle |= WS_MAXIMIZE;
    windowStyle |= WS_MINIMIZE;
    windowStyle |= WS_THICKFRAME;


    internalState->WindowHandle = CreateWindowEx(
        windowsExStyle,                 // Window style
        CLASS_NAME,                     // Window class name
        applicationName,
        windowStyle,                    // Window style
        // Position and size
        // TODO: add x, y to the positioning
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,       // Parent window    
        NULL,       // Menu
        internalState->InstanceHandle,  // Instance handle
        NULL        // Additional application data
    );

    if (internalState->WindowHandle == NULL)
    {
        BIT_LOG_ERROR("Failed to create a window on Windows!");
        return false;
    }

    // m_Context = GraphicsContext::Create(); 
    // if (!m_Context || !m_Context->Initialize(m_WindowHandle, nullptr, 0)) // WGL init
    // {
    //     BIT_LOG_ERROR("Failed to initialize graphics context on Windows!");
    //     return false;
    // }

    ShowWindow(internalState->WindowHandle, SW_SHOW);

    return true; 
}

void PlatformShutdown(PlatformState* platformState)
{
    InternalState* internalState = (InternalState*)platformState->InternalState;
    if (internalState->WindowHandle)
    {
        DestroyWindow(internalState->WindowHandle);
    }
}

b8 PlatformPumpMessages(PlatformState* platformState)
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

void* PlatformAllocate(u64 size, b8 aligned)
{
    (void)aligned;
    void* block = malloc(size);
    return block;
}
void PlatformFree(void* block, b8 aligned)
{
    (void)aligned;
    free(block);
}
void PlatformZeroMemory(void* block, u64 size)
{
    memset(block, 0, size);
}
void PlatformSetMemory(void* dest, i32 value, u64 size)
{
    memset(dest, value, size);
}
void PlatformCopyMemory(void *dest, const void *source, u64 size)
{
    memcpy(dest, source, size);
}

f64 PlatformGetAbsoluteTime()
{
    return 0.0f;
}
void PlatformSleep(u64 ms)
{
}
}
#endif

