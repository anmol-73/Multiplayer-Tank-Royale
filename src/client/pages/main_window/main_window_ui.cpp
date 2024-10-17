#include "main_window_ui.hpp"

Pages::MainWindowUI::MainWindowUI()
{
    using Mode = UI::DrawParameters::SizeMode;
    register_element(
        new UI::Elements::Input(UI::Elements::InputParameters{
            .size = {
                .value = {0.3, 32},
                .mode = {Mode::SCREEN_W, Mode::ABSOLUTE}
            },
            .padding = {
                .value = {0.1, 0},
                .mode = {Mode::SELF_H, Mode::SELF_H}
            },
            .text_origin = {
                .value = {0, 0.05},
                .mode = {Mode::SELF_H, Mode::SELF_H}
            },
            .background_color = WHITE,
            .font_color = BLACK,
            .default_border_params = {
                .width = 2,
                .color = WHITE,
            },
            .focused_border_params = {
                .width = 2,
                .color = GREEN,
            },
            .hover_border_params = {
                .width = 2,
                .color = YELLOW,
            },
            .placeholder = {
                .content = "127.0.0.1",
                .placeholder_font_color = GRAY
            },
            .max_input_size = 16,
        })
    );
}