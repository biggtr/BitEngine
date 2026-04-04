#include "BitUI.h"
#include "Bit/Containers/darray.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/UI/Widgets.h"
#include <cstdlib>
#include <cstring>


static u64 HashName(const char* name);
struct UIContext
{
    u64 HotID;
    u64 ActiveID;
    u64 FocusedID;
    
    u64* ParentsStack;
    DrawCommand* DrawCommands;
    BitEngine::Renderer2D* Renderer2D;
};

static UIContext* bitUIContext;

b8 UIInitialize(u64* memoryRequirement, void* state, BitEngine::Renderer2D* renderer)
{
    *memoryRequirement = sizeof(UIContext);
    if(state == nullptr)
    {
        return true;
    }

    bitUIContext = (UIContext*)state;
    memset(bitUIContext, 0, sizeof(UIContext));
    if(!bitUIContext->DrawCommands)
    {
        bitUIContext->DrawCommands = (DrawCommand*)DArrayCreate(DrawCommand);
    }
    if(!bitUIContext->ParentsStack)
    {
        bitUIContext->ParentsStack = (u64*)DArrayCreate(u64);
    }
    bitUIContext->Renderer2D = renderer;
    return true;
}

void UIShutdown(void* state)
{
    DArrayDestroy(bitUIContext->DrawCommands);
    DArrayDestroy(bitUIContext->ParentsStack);
    if(state)
    {
        bitUIContext = 0;
    }
    BIT_LOG_INFO("UI System Is shutting down..!");
}
void UIBeginFrame()
{
    bitUIContext->HotID = 0;
    DArrayClear(bitUIContext->DrawCommands);
    DArrayClear(bitUIContext->ParentsStack);
}
void UIEndFrame()
{
    u32 drawCommandsLength = DArrayLength(bitUIContext->DrawCommands);
    for(u32 i = 0;  i < drawCommandsLength; ++i)
    {
        DrawCommand cmd = bitUIContext->DrawCommands[i];
        BMath::Vec3 position = BMath::Vec3(cmd.Bounds.x, cmd.Bounds.y, 0.0f);
        BMath::Vec3 size = BMath::Vec3(cmd.Bounds.w , cmd.Bounds.h * -1.0f, 0.0f);
        if(cmd.Texture != nullptr)
        {
            bitUIContext->Renderer2D->DrawQuad(position, size, 0.0f, cmd.Texture, cmd.UVs);
        }
        else
            bitUIContext->Renderer2D->DrawQuad(position, size, 0.0f, cmd.Color);
    }
}
void UIBeginLayout(LAYOUT_TYPE type, Rect bounds);
void UIEndLayout();

void UIBegin(const char* label, Rect bounds)
{
    u64 id = HashName(label);
    DArrayPush(bitUIContext->ParentsStack, id);
}
void UIEnd()
{
}

b8 UIButton(u32 id, Rect bounds, ButtonStyle style)
{
    i32 mouseX;
    i32 mouseY;
    BitEngine::InputGetMousePosition(&mouseX, &mouseY);
    b8 mouseOver = ((f32)mouseX >= bounds.x - bounds.w * 0.5f && (f32)mouseX <= bounds.x + bounds.w * 0.5f && mouseY >= bounds.y - bounds.h * 0.5f && mouseY <= bounds.y + bounds.h * 0.5);
    if(mouseOver)
    {
        bitUIContext->HotID = id;
        if(BitEngine::InputIsMouseButtonPressed(BitEngine::MOUSE_BUTTON_LEFT))
        {
            bitUIContext->ActiveID = id;
        }
    }
    // BIT_LOG_DEBUG("mouseX : %d mouseY : %d bounds.x : %.2f, bounds.y: %.2f width: %.2f height :%.2f", mouseX, mouseY, bounds.x, bounds.y, bounds.w, bounds.h);
    b8 clicked = false;
    if(bitUIContext->ActiveID == id && BitEngine::InputIsMouseButtonReleased(BitEngine::MOUSE_BUTTON_LEFT))
    {
        clicked = true;
        bitUIContext->ActiveID = 0;
    }
    DrawCommand cmd = {};
    cmd.Bounds = bounds;
    cmd.Element.ID = id;
    cmd.Type = DRAW_COMMAND_TYPE::RECT;
    
    if(style.Texture != nullptr)
    {
        cmd.Texture = style.Texture; 
        for(u32 i = 0; i < 8; ++i)
        {
            cmd.UVs[i] = style.UVs[i];
        }
    }
    else if(bitUIContext->ActiveID == id)
    {
        cmd.Color = style.PressedColor;
    }
    else if(bitUIContext->HotID == id)
    {
        cmd.Color = style.HoverColor;
    }
    else 
    {
        cmd.Color = {1.0f, 1.0f, 1.0f, 1.0f};
    }
    DArrayPush(bitUIContext->DrawCommands, cmd);
    return clicked;
}
u64 HashName(const char* name)
{
    u32 p = 97;

    unsigned const char* us;
    u64 hash = 0;
    for(us = (unsigned const char*)name; *us; ++us)
    {
        hash = hash * p + *us;
    }
    return hash;
}

