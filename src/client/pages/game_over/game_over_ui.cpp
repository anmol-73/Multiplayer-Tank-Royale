#include "game_over_ui.hpp"

Pages::GameOverUI::GameOverUI()
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
        go_to_lobby = Components::create_span_button(
            "Connect",
            {
                .value = {0.5, 0.7},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            }
        )
    );
}