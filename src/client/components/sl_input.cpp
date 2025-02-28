#include "sl_input.hpp"

DragonLib::UI::Elements::Input *Pages::Components::create_sl_input(
    const std::string& label, 
    const std::string& placeholder_text, 
    DragonLib::UI::DrawParameters::Measurement position, 
    DragonLib::UI::DrawParameters::Measurement size, 
    size_t max_input_size
){
    using namespace DragonLib::UI::Elements;
    using Mode = DragonLib::UI::DrawParameters::SizeMode;
    using namespace ColorScheme;

    return new Input(InputParameters{
        .label = label,
        .position = position,
        .size = size,
        .padding = {
            .value = {0.1, 0},
            .mode = {Mode::SELF_H, Mode::SELF_H}
        },
        .text_origin = {
            .value = {0, 0.05},
            .mode = {Mode::SELF_H, Mode::SELF_H}
        },
        .background_color = plain_fill,
        .font_color = plain_color,
        .default_border_params = {
            .width = 0,
            .color = plain_border_h,
        },
        .focused_border_params = {
            .width = 2.0f,
            .color = plain_border,
            .offset = 6.0f
        },
        .hover_border_params = {
            .width = 2.0f,
            .color = plain_border_l,
            .offset = 12.0f
        },
        .placeholder = {
            .content = placeholder_text,
            .placeholder_font_color = plain_color_l
        },
        .max_input_size = max_input_size
    });
}