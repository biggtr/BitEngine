#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Renderer/Texture.h"
#include <ft2build.h>
#include <freetype/freetype.h>

b8 FontLoad(const char* fontPath);

void DrawText(BitEngine::Renderer2D* renderer, const std::string& text, f32 x, f32 y, f32 scale, f32 rotation, const BMath::Vec3& color);

