#include "input.hpp"

DragonLib::UI::Elements::Input::Input(InputParameters parameters)
{
    params = parameters;
    if (params.font == nullptr){
        params.font = params.sdf ? &Global::Rendering::Fonts::main_sdf : &Global::Rendering::Fonts::main;
    }
}

void DragonLib::UI::Elements::Input::draw()
{
    bounds = Utils::Drawing::draw_box({
        .position = params.position,
        .size = params.size,
        .origin = params.origin,
        .padding = params.padding,
        .fill = params.background_color,
        .border = keyboard_focused? params.focused_border_params : hstate.exclusively_hovered() ? params.hover_border_params : params.default_border_params
    });
    Vector2 content_size = Utils::Calculation::resolve_measurement(params.size);
    Vector2 padding = Utils::Calculation::resolve_measurement(params.padding, content_size);
    Vector2 text_origin = Utils::Calculation::resolve_measurement(params.text_origin, content_size);
    Vector2 text_position = {
        bounds.x + padding.x + text_origin.x,
        bounds.y + padding.y + text_origin.y
    };
    bool show_placeholder = value.empty() && (!keyboard_focused);
    Utils::Drawing::place_text(
        (show_placeholder ? params.placeholder.content : value).c_str(),
        text_position,
        show_placeholder ? params.placeholder.placeholder_font_color : params.font_color,
        content_size.y * 0.8,
        params.font,
        params.sdf
    );
}

void DragonLib::UI::Elements::Input::poll_events()
{
    hstate.hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    hstate.requested_pointer = hstate.hovered;

    bool clicked_outside = (!hstate.exclusively_hovered()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool clicked_inside = (hstate.exclusively_hovered()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    
    if (clicked_outside){
        keyboard_focused = false;
    } else if (clicked_inside){
        keyboard_focused = true;
    }

    if (keyboard_focused){
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();
        
        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            if (value.size() < params.max_input_size && ((char(key) != '\n') || params.allow_newline)){
                value += (char)key;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
        {
            if (!value.empty()) value.pop_back();
        }
    }
}

void DragonLib::UI::Elements::Input::clear_state()
{
    value.clear();
    keyboard_focused = false;
}
