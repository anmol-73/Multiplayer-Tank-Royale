#include "drawing.hpp"


Rectangle DragonLib::Utils::Drawing::draw_text(UI::DrawParameters::Text params)
{
    if (params.font == nullptr){
        params.font = params.sdf ? &Global::Rendering::Fonts::main_sdf : &Global::Rendering::Fonts::main;
    }
    return params.wrap_width == 0 ? __draw_text_no_wrap(params) : __draw_text_with_wrap(params);
}

Rectangle DragonLib::Utils::Drawing::__draw_text_no_wrap(UI::DrawParameters::Text &params)
{
    assert(params.font != nullptr);
    Vector2 size = measure_text(params.font, params.content, params.font_size, params.spacing, params.line_space);
    Vector2 position = Vector2Subtract(
        Calculation::resolve_measurement(params.position, size),
        Calculation::resolve_measurement(params.origin, size)
    );
    
    place_text(params.content.c_str(), position, params.font_color, params.font_size, params.font, params.sdf, params.line_space);

    return Rectangle{
        .x = position.x,
        .y = position.y,
        .width = size.x,
        .height = size.y
    };
}

Rectangle DragonLib::Utils::Drawing::__draw_text_with_wrap(UI::DrawParameters::Text &params)
{
    assert(params.font != nullptr);
    // TODO: Text with wrap
    // https://www.raylib.com/examples/text/loader.html?name=text_rectangle_bounds
    return Rectangle();
}

Rectangle DragonLib::Utils::Drawing::draw_box(UI::DrawParameters::Box params)
{
    return __draw_box_no_radius(params);
}

Rectangle DragonLib::Utils::Drawing::__draw_box_no_radius(UI::DrawParameters::Box &params)
{
    Vector2 size = Calculation::resolve_measurement(params.size);
    Vector2 padding = Calculation::resolve_measurement(params.padding, size);

    Vector2 rect_size = Vector2Add(size, Vector2Scale(padding, 2));
    Vector2 position = Vector2Subtract(
        Vector2Subtract(
            Calculation::resolve_measurement(params.position, size),
            Calculation::resolve_measurement(params.origin, size)
        ), padding
    );
    
    if (params.fill.a != 0){
        DrawRectangleV(position, rect_size, params.fill);
    }
    if (params.border.width != 0 && params.border.color.a != 0){
        DrawRectangleLinesEx({
            .x = position.x,
            .y = position.y,
            .width = rect_size.x,
            .height = rect_size.y,
        }, params.border.width, params.border.color);
    }

    return {
        position.x, position.y,
        rect_size.x, rect_size.y
    };
}

Rectangle DragonLib::Utils::Drawing::__draw_box_with_radius(UI::DrawParameters::Box &params)
{
    // TODO: Do rect with border radius
    // https://www.shadertoy.com/view/WtdSDs
    // Look into Triangle Fan
    return Rectangle();
}

Rectangle DragonLib::Utils::Drawing::draw_textbox(UI::DrawParameters::TextBox params)
{
    if (params.font == nullptr){
        params.font = params.sdf ? &Global::Rendering::Fonts::main_sdf : &Global::Rendering::Fonts::main;
    }
    Vector2 size = measure_text(params.font, params.content, params.font_size, params.spacing, params.line_space);
    Vector2 padding = Calculation::resolve_measurement(params.padding, size);
    Vector2 rect_size = Vector2Add(size, Vector2Scale(padding, 2));
    
    Vector2 position = Vector2Subtract(
        Calculation::resolve_measurement(params.position, rect_size),
        Calculation::resolve_measurement(params.rect_origin, rect_size)
    );
    
    if (params.fill.a != 0){
        DrawRectangleV(position, rect_size, params.fill);
    }
    if (params.border.width != 0 && params.border.color.a != 0){
        
        DrawRectangleLinesEx({
            .x = position.x - params.border.width - params.border.offset / 2,
            .y = position.y - params.border.width - params.border.offset / 2,
            .width = rect_size.x + 2*params.border.width + params.border.offset,
            .height = rect_size.y + 2*params.border.width + params.border.offset,
        }, params.border.width, params.border.color);
    }

    Vector2 text_position = Vector2Add(
        padding,
        Vector2Add(
            position, Calculation::resolve_measurement(params.text_origin, size)
        )
    );    
    
    place_text(params.content.c_str(), text_position, params.font_color, params.font_size, params.font, params.sdf, params.line_space);
    return Rectangle{
        .x = position.x,
        .y = position.y,
        .width = rect_size.x,
        .height = rect_size.y
    };
}

void DragonLib::Utils::Drawing::place_text(const char *content, Vector2 position, Color font_color, float font_size, Font *font, bool sdf, float line_space, float spacing)
{
    if (font == nullptr){
        font = sdf ? &Global::Rendering::Fonts::main_sdf : &Global::Rendering::Fonts::main;
    }
    SetTextLineSpacing(line_space * font_size);
    if (sdf){
        BeginShaderMode(Global::Rendering::Shaders::sdf_font_shader);{
            DrawTextEx(*font, content, position, font_size, spacing, font_color);
        }
        EndShaderMode();
    } else{
        DrawTextEx(*font, content, position, font_size, spacing, font_color);
    }
}