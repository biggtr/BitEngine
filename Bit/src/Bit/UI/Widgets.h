#pragma once 
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"

struct Rect { f32 x, y, w, h; };
b8 UIButton(u32 id, const char *text, Rect bounds, const BMath::Vec4& hoverColor, const BMath::Vec4& pressedColor);

