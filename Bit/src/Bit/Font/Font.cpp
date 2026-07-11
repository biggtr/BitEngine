#include <glad/glad.h>
#include "Font.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Renderer/Texture.h"
#include "freetype/freetype.h"
#include <map>


struct FontState
{
    FT_Library FontLibrary;

    std::map<char, BitEngine::FontCharacter> Characters;
};

// const FontState* statePtr = 0;
FontState fontState = {};
b8 FontLoad(const char *fontPath)
{
    if(FT_Init_FreeType(&fontState.FontLibrary))     // zero return means success in freetype lib 
    {
        BIT_LOG_ERROR("Failed to init freetype library");
        return false;
    }
    FT_Face face;
    if(FT_New_Face(fontState.FontLibrary, fontPath, 0, &face))
    {
        BIT_LOG_ERROR("Failed to load the font");
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for(u32 i = 0; i < 128; ++i)
    {
        if(FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            BIT_LOG_ERROR("Failed to load Char");
            continue;
        }
        BitEngine::Texture* texture = BitEngine::Texture::Create(face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED);
        texture->SetData(face->glyph->bitmap.buffer, face->glyph->bitmap.width * face->glyph->bitmap.rows);
        BitEngine::FontCharacter character = {
            .Texture = texture, 
            .Size    = BMath::Vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            .Bearing = BMath::Vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            .Advance = face->glyph->advance.x
        };
        fontState.Characters.insert({i, character});
    }
    FT_Done_Face(face);
    FT_Done_FreeType(fontState.FontLibrary);
    return true;
}


void DrawText(BitEngine::Renderer2D* renderer, const std::string& text, f32 x, f32 y, f32 scale, f32 rotation, const BMath::Vec3& color)
{
    for (u32 i = 0; i < text.size(); ++i)
    {
        char c = text[i];
        BitEngine::FontCharacter& ch = fontState.Characters[c];
        renderer->DrawText(&ch, x, y, scale, rotation, color);
        x += (ch.Advance >> 6) * scale;
    }
}
