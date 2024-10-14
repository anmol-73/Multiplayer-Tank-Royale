#include "text.hpp"
#include <iostream>

Vector2 DragonLib::Utils::Text::measure_text(Font *font, const std::string &text, float font_size, float spacing, float line_space)
{
    // Slightly modified version of rtext.h

    Vector2 textSize = { 0 };

    if (((*font).texture.id == 0) || (text.empty())) return textSize;

    int tempByteCounter = 0;        // Used to count longer text line num chars
    int byteCounter = 0;

    float textWidth = 0.0f;
    float tempTextWidth = 0.0f;     // Used to count longer text line width

    float textHeight = font_size;
    float scaleFactor = font_size/(*font).baseSize;

    int letter = 0;                 // Current character
    int index = 0;                  // Index position in sprite font
    
    for (std::size_t i = 0; i < text.size();)
    {
        byteCounter++;

        int next = 0;
        letter = GetCodepointNext(&text[i], &next);
        index = GetGlyphIndex(*font, letter);

        i += next;

        if (letter != '\n')
        {
            if ((*font).glyphs[index].advanceX != 0) textWidth += (*font).glyphs[index].advanceX;
            else textWidth += ((*font).recs[index].width + (*font).glyphs[index].offsetX);
        }
        else
        {
            if (tempTextWidth < textWidth) tempTextWidth = textWidth;
            byteCounter = 0;
            textWidth = 0;

            textHeight += line_space * font_size;
        }

        if (tempByteCounter < byteCounter) tempByteCounter = byteCounter;
    }

    if (tempTextWidth < textWidth) tempTextWidth = textWidth;

    textSize.x = tempTextWidth*scaleFactor + (tempByteCounter - 1)*spacing;
    textSize.y = textHeight;

    return textSize;
}

Font DragonLib::Utils::Text::load_sdf(const char *ttf_file, int font_size, int glyph_count)
{
    // https://www.raylib.com/examples/text/loader.html?name=text_font_sdf

    int fileSize = 0;
    unsigned char *fileData = LoadFileData(ttf_file, &fileSize);

    Font sdf_font = {0};
    sdf_font.baseSize = font_size;
    sdf_font.glyphCount = glyph_count;

    sdf_font.glyphs = LoadFontData(fileData, fileSize, sdf_font.baseSize, 0, sdf_font.glyphCount, FONT_SDF);
    
    Image atlas = GenImageFontAtlas(sdf_font.glyphs, &sdf_font.recs, sdf_font.glyphCount, sdf_font.baseSize, 0, 1);
    sdf_font.texture = LoadTextureFromImage(atlas);
    
    SetTextureFilter(sdf_font.texture, TEXTURE_FILTER_BILINEAR); // Required for SDF font

    UnloadImage(atlas);
    UnloadFileData(fileData);
    
    return sdf_font;
}

Rectangle DragonLib::Utils::Text::draw_text(UI::DrawParameters::Text params)
{
    if (params.font == nullptr){
        params.font = params.sdf ? &Global::Rendering::Fonts::main_sdf : &Global::Rendering::Fonts::main;
    }
    return params.wrap_width == 0 ? __draw_text_no_wrap(params) : __draw_text_with_wrap(params);
}

Rectangle DragonLib::Utils::Text::__draw_text_no_wrap(UI::DrawParameters::Text &params)
{
    assert(params.font != nullptr);
    Vector2 size = measure_text(params.font, params.content, params.size, params.spacing, params.line_space);
    Vector2 position = Vector2Subtract(
        Calculation::resolve_item_units(params.position, params.position_mode, size),
        Calculation::resolve_item_units(params.origin, params.origin_mode)
    );
    
    SetTextLineSpacing(params.line_space * params.size);
    if (params.sdf){
        BeginShaderMode(Global::Rendering::Shaders::sdf_font_shader);{
            DrawTextEx(*params.font, params.content.c_str(), position, params.size, params.spacing, params.color);
        }
        EndShaderMode();
    } else{
        DrawTextEx(*params.font, params.content.c_str(), position, params.size, params.spacing, params.color);
    }

    return Rectangle{
        .x = position.x,
        .y = position.y,
        .width = size.x,
        .height = size.y
    };
}

Rectangle DragonLib::Utils::Text::__draw_text_with_wrap(UI::DrawParameters::Text &params)
{
    assert(params.font != nullptr);
    // TODO: Text with wrap
    // https://www.raylib.com/examples/text/loader.html?name=text_rectangle_bounds
    return Rectangle();
}
