#include "BitUI.h"
#include "Bit/Containers/darray.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/UI/Widgets.h"
#include <cstring>


static u64 HashName(const char* name);
struct UIContext
{
    u32 HotID;
    u32 ActiveID;
    u32 FocusedID;
    
    u64* ParentsStack;
    DrawCommand* DrawCommands;
};

static UIContext* bitUIContext;

b8 UIInitialize(u64* memoryRequirement, void* state)
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
    //Draw Commands
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
    u64 parentID;
    DArrayPop(bitUIContext->ParentsStack, &parentID);
    DrawCommand windowCmd;
    //calculate the size of the window automatically based on the content it contains 
    for(u32 i = 0; i < DArrayLength(bitUIContext->DrawCommands); ++i)
    {
        DrawCommand cmd = bitUIContext->DrawCommands[i];
        if(cmd.Element.ParentID == parentID)
        {
            windowCmd.Bounds.w += cmd.Bounds.w;
            windowCmd.Bounds.h += cmd.Bounds.h;
        }
    }
    DArrayPush(bitUIContext->DrawCommands, windowCmd);
}

b8 UIButton(const char *label, Rect bounds, const BMath::Vec4& hoverColor, const BMath::Vec4& pressedColor)
{
    u64 parentsStackLen = DArrayLength(bitUIContext->ParentsStack);
    u64 parentID = bitUIContext->ParentsStack[parentsStackLen - 1];
    u64 id = HashName(label) + parentID;
    i32 mouseX;
    i32 mouseY;
    BitEngine::InputGetMousePosition(&mouseX, &mouseY);
    b8 mouseOver = ((f32)mouseX >= bounds.x && (f32)mouseX <= bounds.x + bounds.w && mouseY >= bounds.y && mouseY <= bounds.y + bounds.h);
    if(mouseOver)
    {
        bitUIContext->HotID = id;
        if(BitEngine::InputIsMouseButtonPressed(BitEngine::MOUSE_BUTTON_LEFT))
        {
            bitUIContext->ActiveID = id;
        }
    }
    b8 clicked = false;
    if(bitUIContext->ActiveID == id && BitEngine::InputIsMouseButtonReleased(BitEngine::MOUSE_BUTTON_LEFT))
    {
        clicked = true;
        bitUIContext->ActiveID = 0;
    }
    DrawCommand cmd = {};
    cmd.Bounds = bounds;
    cmd.Label = label;
    cmd.Element.ParentID = parentID;
    cmd.Element.ID = id;
    cmd.Type = DRAW_COMMAND_TYPE::RECT;
    
    if(bitUIContext->ActiveID == id)
    {
        cmd.Color = pressedColor;
    }
    else if(bitUIContext->HotID == id)
    {
        cmd.Color = hoverColor;
    }
    else 
    {
        cmd.Color = {1.0f, 1.0f, 0.0f, 1.0f};
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

