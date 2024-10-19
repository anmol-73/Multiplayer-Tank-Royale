#include "main_window_ui.hpp"

Pages::MainWindowUI::MainWindowUI()
{
    using Mode = UI::DrawParameters::SizeMode;
    
    address_input_id = register_element(
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
                .content = "eg: 127.0.0.1:3000",
                .placeholder_font_color = GRAY
            },
            .max_input_size = 16,
        })
    );

    address_submit_id = register_element(
        new UI::Elements::Button(
            [](UI::Elements::Button* buttons){
                return UI::DrawParameters::TextBox{
                    .content = "Connect!",
                    .font_size = 16,
                    .position = {
                        .value = {0.5, 0.6},
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                    },
                    .fill = buttons->hstate.hovered ? GRAY : WHITE,
                    .border = {
                        .width = 2,
                        .color = BLACK,
                        .offset = buttons->hstate.hovered ? 5.0f : 0.0f
                    },
                    .padding = {
                        .value = {0.1, 0},
                        .mode = {Mode::SELF_H, Mode::SELF_H}
                    }
                };
            }
        )
    );
}

const std::string &Pages::MainWindowUI::address_input_value()
{
    return dynamic_cast<UI::Elements::Input*>(elements[address_input_id].get())->value;
}

bool Pages::MainWindowUI::address_submit_requested()
{
    return dynamic_cast<UI::Elements::Button*>(elements[address_submit_id].get())->clicked;
}

void Pages::MainWindowUI::show_error(std::string error)
{
    std::cout << error << std::endl;
}
