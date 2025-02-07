#include "splash_ui.hpp"

Pages::SplashUI::SplashUI()
{
    using Mode = UI::DrawParameters::SizeMode;
    auto *bg = new UI::Elements::ImageView(
        DragonLib::DImage("resources/ui_background.png"),
        [](auto *_){
            return UI::DrawParameters::Box{
                .size = {
                    .value = {1, 1},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                }
            };
        }
    );
    register_element(bg);
}