#include "game_ui.hpp"

Pages::GameUI::GameUI()
{
    using Mode = UI::DrawParameters::SizeMode;
    register_element(
        respawn_button = Components::create_span_button(
            "Create Room",
            {
                .value = {0.5, 0.7},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            }
        )
    );

}

void Pages::GameUI::allow_respawn(bool ok)
{
    respawn_button->interactable = ok;
}

bool Pages::GameUI::should_respawn()
{
    return respawn_button->clicked;
}
