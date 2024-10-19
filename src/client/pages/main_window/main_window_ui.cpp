#include "main_window_ui.hpp"

Pages::MainWindowUI::MainWindowUI()
{
    using Mode = UI::DrawParameters::SizeMode;
    
    register_element(
        new UI::Elements::Button(
            [](UI::Elements::Button* button){
                return UI::DrawParameters::TextBox{
                    .content = "Tank Stars",
                    .font_size = 5 * Global::rem,
                    .font_color =  {0x45, 0x41, 0x39, 0xff},
                    .position = {
                        .value = {0.5, 0.2},
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                    }
                };
            }
        )
    );
    register_element(
        new UI::Elements::Button(
            [](UI::Elements::Button* button){
                return UI::DrawParameters::TextBox{
                    .content = "Tank Stars",
                    .font_size = 5 * Global::rem,
                    .font_color = {0x45, 0x41, 0x39, 0x60},
                    .position = {
                        .value = {0.5, 0.2},
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                    },
                    .text_origin = {
                        .value = {Global::rem / 3, Global::rem / 4},
                        .mode = {Mode::ABSOLUTE, Mode::ABSOLUTE}
                    }
                };
            }
        )
    );

    address_input_id = register_element(
        new UI::Elements::Input(UI::Elements::InputParameters{
            .size = {
                .value = {0.3, 0.05},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            },
            .padding = {
                .value = {0.1, 0},
                .mode = {Mode::SELF_H, Mode::SELF_H}
            },
            .text_origin = {
                .value = {0, 0.05},
                .mode = {Mode::SELF_H, Mode::SELF_H}
            },
            .background_color = {0xb2, 0xad, 0x99, 0xc0},
            .font_color = {0x45, 0x41, 0x39, 0xc0},
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
                .content = "eg: 127.0.0.1:3000",
                .placeholder_font_color = {0x45, 0x41, 0x39, 0x20}
            },
            .max_input_size = 24,
        })
    );

    address_submit_id = register_element(
        new UI::Elements::Button(
            [](UI::Elements::Button* button){
                return UI::DrawParameters::TextBox{
                    .content = "Connect",
                    .font_size = Global::rem,
                    .font_color = button->hstate.hovered ? Color{0xbf, 0xba, 0xa3, 0xc0} : Color{0x45, 0x41, 0x39, 0xc0},
                    .position = {
                        .value = {0.5, 0.6},
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                    },
                    .fill = button->hstate.hovered ? Color{0x49, 0x47, 0x3f, (unsigned char)(button->mouse_down ? 0xff : 0xc0)} : Color{0xb2, 0xad, 0x99, 0xc0},
                    .border = {
                        .width = 2.0f,
                        .color = button->hstate.hovered ? Color{0x49, 0x47, 0x3f, 0xc0} : Color{0xb2, 0xad, 0x99, 0xc0},
                        .offset = button->mouse_down ? 10.0f : 12.0f,
                    },
                    .padding = {
                        .value = {0.5, 0.2},
                        .mode = {Mode::SELF_H, Mode::SELF_H}
                    }
                };
            }
        )
    );

    register_element(
        new UI::Elements::Button(
            [this](UI::Elements::Button* button){
                
                return UI::DrawParameters::TextBox{
                    .content = this->error_text,
                    .font_size = Global::rem,
                    .font_color = {0xe9, 0x7d, 0x81, 0xff},
                    .position = {
                        .value = {0.5, 0.85},
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                    },
                    .fill = {0}
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

void Pages::MainWindowUI::poll_events()
{
    if ((!visible) || (!events_enabled)) return;
    UI::Elements::PageView::poll_events();
    if (dynamic_cast<UI::Elements::Input*>(elements[address_input_id].get())->keyboard_focused){
        error_text = "";
    };
}

void Pages::MainWindowUI::show_error(std::string error)
{
    error_text = error;
}
