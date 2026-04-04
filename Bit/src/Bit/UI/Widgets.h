#pragma once 
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Texture.h"

struct Rect { f32 x, y, w, h; };
struct ButtonStyle 
{
    BitEngine::Texture* Texture;
    f32* UVs;
    BMath::Vec4 HoverColor = BMath::Vec4(0.0,0.0,0.0,0.0);
    BMath::Vec4 PressedColor = BMath::Vec4(0.0,0.0,0.0,0.0);};

b8 UIButton(u32 id, Rect bounds, ButtonStyle style);

