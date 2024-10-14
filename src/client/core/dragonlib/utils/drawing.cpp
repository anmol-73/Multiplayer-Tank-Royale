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
        Calculation::resolve_item_units(params.position, params.position_mode, size),
        Calculation::resolve_item_units(params.origin, params.origin_mode, size)
    );
    
    SetTextLineSpacing(params.line_space * params.font_size);
    if (params.sdf){
        BeginShaderMode(Global::Rendering::Shaders::sdf_font_shader);{
            DrawTextEx(*params.font, params.content.c_str(), position, params.font_size, params.spacing, params.font_color);
        }
        EndShaderMode();
    } else{
        DrawTextEx(*params.font, params.content.c_str(), position, params.font_size, params.spacing, params.font_color);
    }

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
    Vector2 size = Calculation::resolve_item_units(params.size, params.size_mode);
    Vector2 position = Vector2Subtract(
        Calculation::resolve_item_units(params.position, params.position_mode, size),
        Calculation::resolve_item_units(params.origin, params.origin_mode, size)
    );
    if (params.fill.a != 0){
        DrawRectangleV(position, size, params.fill);
    }
    if (params.border.width != 0 && params.border.color.a != 0){
        DrawRectangleLinesEx({
            .x = position.x,
            .y = position.y,
            .width = size.x,
            .height = size.y,
        }, params.border.width, params.border.color);
    }

    return {
        position.x, position.y,
        size.x, size.y
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
    Vector2 position = Vector2Subtract(
        Calculation::resolve_item_units(params.position, params.position_mode, size),
        Calculation::resolve_item_units(params.rect_origin, params.rect_origin_mode, size)
    );
    
    if (params.fill.a != 0){
        DrawRectangleV(position, size, params.fill);
    }
    if (params.border.width != 0 && params.border.color.a != 0){
        DrawRectangleLinesEx({
            .x = position.x,
            .y = position.y,
            .width = size.x,
            .height = size.y,
        }, params.border.width, params.border.color);
    }

    Vector2 text_position = Vector2Add(position, Calculation::resolve_item_units(params.text_origin, params.text_origin_mode, size));
    SetTextLineSpacing(params.line_space * params.font_size);
    if (params.sdf){
        BeginShaderMode(Global::Rendering::Shaders::sdf_font_shader);{
            DrawTextEx(*params.font, params.content.c_str(), text_position, params.font_size, params.spacing, params.font_color);
        }
        EndShaderMode();
    } else{
        DrawTextEx(*params.font, params.content.c_str(), text_position, params.font_size, params.spacing, params.font_color);
    }

    return Rectangle{
        .x = position.x,
        .y = position.y,
        .width = size.x,
        .height = size.y
    };
}
