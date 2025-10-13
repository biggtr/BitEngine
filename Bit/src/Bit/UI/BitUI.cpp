#include "BitUI.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/UI/Widgets.h"

namespace BitUI
{
static BitUIState* bitUIState;

static b8 initialized = false;
b8 Initialize()
{
    if(initialized)
    {
        BIT_LOG_ERROR("BitUI is Already Initialized!.");
        return false;
    }

    bitUIState = new BitUIState();
    initialized = true;
    return true;

}
void Shutdown()
{
    if(!initialized)
    {
        BIT_LOG_ERROR("BitUI is not Initialized Already!.");
        return;
    }
    delete bitUIState;
    initialized = false;
}
void BeginLayout(ElementLayout layoutConfig)
{
    bitUIState->CurrentElementLayout = layoutConfig;
    bitUIState->DrawCommands.clear();
}



//////////////////////////////////////////////////////////////////
/// Widgets
//////////////////////////////////////////////////////////////////
b8 Button(const char* text, f32 width, f32 height, BMath::Vec4 color)
{
    ElementLayout& currentElementLayout = bitUIState->CurrentElementLayout;

    f32 x = currentElementLayout.CurrentX;
    f32 y = currentElementLayout.CurrentY;
    i32 mousex, mousey;
    BitEngine::InputGetMousePosition(&mousex, &mousey);
    b8 isHovered = (u32)mousex >= x && (u32)mousex <= x + width && (u32)mousey >= y && (u32)mousey <= y + height;

    b8 isPressed = BitEngine::InputIsMouseButtonPressed(BitEngine::MOUSE_BUTTON_LEFT);
    b8 isClicked = isHovered && isPressed; 

    DrawCommand drawCommand = 
    {
        .Type = DRAW_COMMAND_TYPE::RECT, 
        .Position = BMath::Vec3(x, y, 0.0f),
        .Size = BMath::Vec3(width, height, 0.0f),
        .data= {.Color = color}
    };
    bitUIState->DrawCommands.push_back(drawCommand);

    switch (currentElementLayout.LayoutDirection) 
    {
        case LAYOUT_DIRECTION::VERTICAL:
            currentElementLayout.CurrentY+=  height + currentElementLayout.ItemGap;
            currentElementLayout.MaxWidth = width > currentElementLayout.MaxWidth ? width : currentElementLayout.MaxWidth;
            break;
        case LAYOUT_DIRECTION::HORIZONTAL:
            currentElementLayout.CurrentX+=  width + currentElementLayout.ItemGap;
            currentElementLayout.MaxHeight = height > currentElementLayout.MaxHeight ? height: currentElementLayout.MaxHeight;
            break;
    }

    return isClicked;
}
std::vector<DrawCommand> EndLayout()
{
    return bitUIState->DrawCommands;
}
}
