#pragma once
#include "Bit/Math/Vector.h"
#include <vector>
namespace BitUI
{

enum class LAYOUT_DIRECTION
{
    VERTICAL,
    HORIZONTAL
};
enum class DRAW_COMMAND_TYPE
{
    RECT,
    CIRCLE,
    LINE,
    TEXTURE,
};
struct DrawCommand
{
    DRAW_COMMAND_TYPE Type;
    BMath::Vec3 Position;
    BMath::Vec3 Size;
    union 
    {
        BMath::Vec4 Color;
        u32 TextureID;
        struct { BMath::Vec4 TintColor; u32 TintedTextureID;} ColoredTexture;
    } data;

};

struct ElementLayout
{
    LAYOUT_DIRECTION LayoutDirection;
    // Current Cursor Position To Place the items Relative to the layout
    f32 CurrentX, CurrentY; 
    f32 ItemGap;

    f32 MaxWidth = 0.0f;
    f32 MaxHeight = 0.0f;

};

struct BitUIState
{
    ElementLayout CurrentElementLayout;
    std::vector<DrawCommand> DrawCommands;
};

b8 Initialize();
void Shutdown();
void BeginLayout(ElementLayout layoutConfig = {});

////////////////////////////////////////////////////
/// Util Functions
////////////////////////////////////////////////////

inline ElementLayout VerticalLayout(f32 x, f32 y, f32 itemGap)
{
    return {.LayoutDirection = LAYOUT_DIRECTION::VERTICAL, .CurrentX = x, .CurrentY = y, .ItemGap = itemGap, .MaxWidth = 0.0f, .MaxHeight = 0.0f};
}
inline ElementLayout HorizontalLayout(f32 x, f32 y, f32 itemGap)
{
    return {.LayoutDirection = LAYOUT_DIRECTION::HORIZONTAL, .CurrentX = x, .CurrentY = y, .ItemGap = itemGap, .MaxWidth = 0.0f, .MaxHeight = 0.0f};
}

std::vector<DrawCommand> EndLayout();

}
