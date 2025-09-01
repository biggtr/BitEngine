#pragma once
#include "Bit/Math/Vector.h"
namespace BitUI
{

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
        struct { BMath::Vec4 TintColor; u32 TintedTextureID;};
    };

};

struct BitUIState
{
    std::vector<DrawCommand> DrawCommands;
};

b8 Initialize();
void Shutdown();
void BeginLayout();

std::vector<DrawCommand> EndLayout();

}
