#include "main_window_ui.hpp"

Pages::MainWindowUI::MainWindowUI()
{
    // TODO: Show reason of disconnection when player gets kicked, or player manually disconnects or when server dies
    using Mode = UI::DrawParameters::SizeMode;
    
    register_element(
        new UI::Elements::Span(
            [](UI::Elements::Span* _){
                return UI::DrawParameters::TextBox{
                    .content = "Tank Stars",
                    .font_size = 5 * Global::rem,
                    .font_color = {0x45, 0x41, 0x39, 0x60},
                    .position = {
                        .value = {0.5, 0.2},
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                    },
                    .text_origin = {
                        .value = {-Global::rem / 3, -Global::rem / 4},
                        .mode = {Mode::ABSOLUTE, Mode::ABSOLUTE}
                    }
                };
            }
        )
    );
    register_element(
        new UI::Elements::Span(
            [](UI::Elements::Span* _){
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
        name_input = new UI::Elements::Input(UI::Elements::InputParameters{
            .label = "Screen name:",
            .position = {
                .value = {0.5, 0.5},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            },
            .size = {
                .value = {0.3, 0.05},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            },
            .padding = {
                .value = {0.1, 0},
                .mode = {Mode::SELF_H, Mode::SELF_H}
            },
            .text_origin = {
                .value = {0, 0.07},
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
                .content = "Enter a name...",
                .placeholder_font_color = {0x45, 0x41, 0x39, 0x80}
            },
            .max_input_size = 23,
        })
    );

    register_element(
        address_input = new UI::Elements::Input(UI::Elements::InputParameters{
            .label = "Server Address:",
            .position = {
                .value = {0.5, 0.6},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            },
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
                .content = "eg: 127.0.0.1:3000",
                .placeholder_font_color = {0x45, 0x41, 0x39, 0x80}
            },
            .max_input_size = 23,
        })
    );

    register_element(
        address_submit_button = Components::create_span_button(
            "Connect",
            {
                .value = {0.5, 0.7},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            }
        )
    );

    register_element(
        new UI::Elements::Span(
            [this](UI::Elements::Span* _){
                
                return UI::DrawParameters::TextBox{
                    .content = this->information.second,
                    .font_size = Global::rem,
                    .font_color = this->information.first ? Color{0xd9, 0x5d, 0x61, 0xff} : Color{0x49, 0x47, 0x3f,  0xe0},
                    .position = {
                        .value = {0.5, 0.85},
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                    },
                    .fill = {0}
                };
            }
        )
    );
    address_input->value = "127.0.0.1:3000";
    name_input->value = "Ryu";
}

void Pages::MainWindowUI::show_info(std::string text, bool is_error)
{
    information = {is_error, text};
}
