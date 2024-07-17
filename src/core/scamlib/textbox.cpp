#include "textbox.hpp"

template <bool sdf>
Rectangle Scamlib::Draw::textbox(TextDrawParameters text_params, BoxDrawParameters box_params, Vector2 padding)
{
    if (text_params.font == nullptr){
        text_params.font = default_font<sdf>();
    }
    Vector2 text_size = measure_text(text_params.font, text_params.content, text_params.size, text_params.spacing, text_params.line_space);

    box_params.size = {
        text_size.x + padding.x,
        text_size.y + padding.y
    };
    Rectangle container_size = Scamlib::Draw::box(box_params);

    Scamlib::Draw::text<sdf>(text_params);

    return container_size;
}