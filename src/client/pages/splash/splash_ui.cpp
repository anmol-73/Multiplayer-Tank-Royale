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
            "Hello", "Placeholder", {
                .value = {0.5f, 0.5f},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            }, {
                .value = {0.3f, 0.05f},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            },
            23 // Max input size
        )
    );
}

std::optional<Communication::Address> Pages::SplashUI::lobby_address()
{
    return std::nullopt;
}
