#include "splash_ui.hpp"

Pages::SplashUI::SplashUI()
{
    using Mode = UI::DrawParameters::SizeMode;
    
    auto *bg = new UI::Elements::ImageView(
        DragonLib::DImage("resources/ui_background.png"),
        [](const auto *_){
            return UI::DrawParameters::Box{
                .size = {
                    .value = {1.0f, 1.0f},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                }
            };
        }
    );
    register_element(bg);

    bg->register_element(
        address_input = Components::create_sl_input(
            "Server Address:", "eg: 127.0.0.1:3000", {
                .value = {0.5f, 0.5f},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            }, {
                .value = {0.3f, 0.05f},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            },
            23 // Max input size
        )
    );

    address_input->value = "127.0.0.1:3000";

    bg->register_element(
        connect_button = Components::create_span_button(
            "Connect",
            {
                .value = {0.5, 0.7},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            }
        )
    );

    bg->register_element(
        new UI::Elements::Span(
            [this](UI::Elements::Span* _){
                
                return UI::DrawParameters::TextBox{
                    .content = this->error_message,
                    .font_size = Global::rem,
                    .font_color = Components::ColorScheme::light_red,
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