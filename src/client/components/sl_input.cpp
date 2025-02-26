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
        .background_color = {0xb2, 0xad, 0x99, 0xc0},
        .font_color = {0x45, 0x41, 0x39, 0xf0},
        .default_border_params = {
            .width = 0,
            .color = {0x49, 0x47, 0x3f, 0xc0},
        },
        .focused_border_params = {
            .width = 2.0f,
            .color = {0x49, 0x47, 0x3f, 0xf0},
            .offset = 6.0f
        },
        .hover_border_params = {
            .width = 2.0f,
            .color = {0x49, 0x47, 0x3f, 0x60},
            .offset = 12.0f
        },
        .placeholder = {
            .content = placeholder_text,
            .placeholder_font_color = {0x45, 0x41, 0x39, 0x80}
        },
        .max_input_size = max_input_size
    });
}