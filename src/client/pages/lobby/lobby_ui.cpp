#include "lobby_ui.hpp"

Pages::LobbyUI::LobbyUI()
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
        create_room_name_input = Components::create_sl_input(
            "Room Name:", "...", {
                .value = {0.5f, 0.5f},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            }, {
                .value = {0.55f, 0.05f},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            },
            30 // Max input size
        )
    );

    bg->register_element(
        create_room_button = Components::create_span_button(
            "Create Room",
            {
                .value = {0.5, 0.7},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            }
        )
    );
}

void Pages::LobbyUI::set_room_provider(std::function<const std::vector<Communication::Lobby::RoomDetail> &()> provider)
{
    room_provider = provider;
}

std::string Pages::LobbyUI::new_room_request()
{
    if (create_room_button->clicked){
        return create_room_name_input->value;
    }
    return "";
}
