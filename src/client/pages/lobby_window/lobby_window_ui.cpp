#include "lobby_window_ui.hpp"

Pages::LobbyWindowUI::LobbyWindowUI()
{
    using Mode = UI::DrawParameters::SizeMode;
    name_ids = std::vector<size_t>(Networking::Message::Room::MAX_ROOM_SIZE, 0ll);
    room_members = std::vector<std::pair<size_t, std::string>>(Networking::Message::Room::MAX_ROOM_SIZE, {0, ""});

    register_element(
        new UI::Elements::Span(
            [](UI::Elements::Span* span){
                return UI::DrawParameters::TextBox{
                    .content = "Lobby: New Room",
                    .font_size = 2 * Global::rem,
                    .font_color = {0x45, 0x41, 0x39, 0xff},
                    
                    .position = {
                        .value = {0.01, 0.05},
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_W}
                    },

                    .rect_origin = { // Specifies the origin of the rect
                        .value = {0, 0},
                        .mode = {Mode::ABSOLUTE, Mode::ABSOLUTE}
                    }
                };
            }
        )
    );
    register_element(
        new UI::Elements::Span(
            [](UI::Elements::Span* span){
                return UI::DrawParameters::TextBox{
                    .content = "Server IP: 127.0.0.1:3000",
                    .font_size = 1.3f * Global::rem,
                    .font_color = {0x45, 0x41, 0x39, 0x80},
                    
                    .position = {
                        .value = {0.01, 0.05},
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_W}
                    },

                    .rect_origin = { // Specifies the origin of the rect
                        .value = {0, -2 * Global::rem},
                        .mode = {Mode::ABSOLUTE, Mode::ABSOLUTE}
                    }
                };
            }
        )
    );

    for (size_t i = 0; i < Networking::Message::Room::MAX_ROOM_SIZE; ++i){ // The names of every player in the lobby
        
        name_ids[i] = register_element(
            new UI::Elements::Text(
                [i, this](UI::Elements::Text* text){
                    return std::pair<UI::DrawParameters::Box, UI::DrawParameters::Text>{
                        {
                            .position = {
                                .value = {
                                    i < Networking::Message::Room::MAX_ROOM_SIZE/2 ? 0.05f : 0.35f, 0.3f + (i % (Networking::Message::Room::MAX_ROOM_SIZE/2)) * 0.1f
                                },
                                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                            },
                            .size = {
                                .value = {0.23f, 0.06f},
                                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                            },
                            .origin = {0},
                            .fill = !this->room_members[i].second.empty() ? Color{0xb2, 0xad, 0x99, 0xc0} : Color{0x49, 0x47, 0x3f, 0x60}
                        },
                        {
                            .content = this->room_members[i].second,
                            .font_size = Global::rem,
                            .font_color = {0x45, 0x41, 0x39, 0xc0},
                            .position = {
                                .value={0.05, 0.5},
                                .mode = {Mode::SELF_W, Mode::SELF_H}
                            },
                            .origin = {
                                .value = {0, 0.5},
                                .mode = {Mode::SELF_W, Mode::SELF_H}
                            }
                        }
                    };
                }
            )
        );
    }
}

void Pages::LobbyWindowUI::make_ready(std::vector<std::pair<size_t, std::string>> &_room_members)
{
    room_members = std::vector<std::pair<size_t, std::string>>(Networking::Message::Room::MAX_ROOM_SIZE, {0, ""});
    for (size_t i = 0; i < _room_members.size(); ++i){
        room_members[i] = _room_members[i];
    }
}