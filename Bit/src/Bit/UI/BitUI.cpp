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
void BeginLayout()
{
    bitUIState->DrawCommands.clear();
}

//////////////////////////////////////////////////////////////////
/// Widgets
//////////////////////////////////////////////////////////////////
b8 Button(const char* text, u32 x, u32 y, u32 width, u32 height, BMath::Vec4 color)
{
    i32 mousex, mousey;
    BitEngine::Input::GetMousePosition(&mousex, &mousey);
    b8 isHovered = (u32)mousex >= x && (u32)mousex <= x + width && (u32)mousey >= y && (u32)mousey <= y + height;

    b8 isPressed = BitEngine::Input::IsMouseButtonPressed(BitEngine::Input::MOUSE_BUTTON_LEFT);
    b8 isClicked = isHovered && isPressed; 

    DrawCommand drawCommand = 
    {
        .Type = DRAW_COMMAND_TYPE::RECT, 
        .Position = BMath::Vec3((f32)x, (f32)y, 0.0f),
        .Size = BMath::Vec3((f32)width, (f32)height, 0.0f),
        .Color = color
    };

    bitUIState->DrawCommands.push_back(drawCommand);

    return isClicked;
}
std::vector<DrawCommand> EndLayout()
{
    return bitUIState->DrawCommands;
}
}
