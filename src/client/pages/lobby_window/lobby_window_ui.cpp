#include "lobby_window_ui.hpp"

Pages::LobbyWindowUI::LobbyWindowUI()
{
    using Mode = UI::DrawParameters::SizeMode;
    room_members = std::vector<std::string>(Networking::Message::Room::MAX_ROOM_SIZE, "");

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
                    .font_size = 1.0f * Global::rem,
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
        register_element(
            new UI::Elements::Text(
                [i, this](UI::Elements::Text* text){
                    return std::pair<UI::DrawParameters::Box, UI::DrawParameters::Text>{
                        {
                            .position = {
                                .value = {
                                    i < Networking::Message::Room::MAX_ROOM_SIZE/2 ? 0.05f : 0.35f, 0.25f + (i % (Networking::Message::Room::MAX_ROOM_SIZE/2)) * 0.1f
                                },
                                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                            },
                            .size = {
                                .value = {0.23f, 0.06f},
                                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                            },
                            .origin = {0},
                            .fill = !this->room_members[i].empty() ? Color{0xb2, 0xad, 0x99, 0xc0} : Color{0x49, 0x47, 0x3f, 0x60}
                        },
                        {
                            .content = this->room_members[i],
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
        register_element(
            remove_player_button[i] = Components::create_text_button(
                "X",
                {
                    .value = {
                        i < Networking::Message::Room::MAX_ROOM_SIZE/2 ? 0.25f : 0.55f, 0.28f + (i % (Networking::Message::Room::MAX_ROOM_SIZE/2)) * 0.1f
                    },
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                },
                {
                    .value = {0.03f, 0.03f},
                    .mode = {Mode::SCREEN_H, Mode::SCREEN_H}
                },
                {
                    .value = {0, 0.5f},
                    .mode = {Mode::SELF_H, Mode::SELF_H}
                }
            )
        );
    }

    register_element(
        new UI::Elements::Span([this](UI::Elements::Span* _){
            return UI::DrawParameters::TextBox{
                .content = this->map_names[current_map_idx],
                .font_size = 0.8f * Global::rem,
                .font_color =  {0x45, 0x41, 0x39, 0xff},
                .position = {
                    .value = {0.81, 0.62},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                }
            };
        })
    );

    register_element(
        left_map_select = Components::create_text_button(
            "<", {
                .value = {0.65, 0.66},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
            },
            {
                .value = {0.14, 0.06},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
            },
            {0}

        )
    );
    register_element(
        right_map_select = Components::create_text_button(
            ">", {
                .value = {0.83, 0.66},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
            },
            {
                .value = {0.14, 0.06},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
            },
            {0}

        )
    );

    register_element(
        start_game_button = Components::create_span_button(
            "Start!",
            UI::DrawParameters::Measurement{
                .value = {0.94, 0.85},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
            }
        )
    );
}

void Pages::LobbyWindowUI::make_ready(std::vector<std::string> map_names_, bool is_leader_)
{
    is_leader = is_leader_;
    map_names = map_names_;
    left_map_select->interactable = is_leader;
    right_map_select->interactable = is_leader;

    for (size_t i = 0; i < Networking::Message::Room::MAX_ROOM_SIZE; ++i){
        remove_player_button[i]->interactable = is_leader && !room_members[i].empty();
    }
}

void Pages::LobbyWindowUI::poll_events()
{
    if ((!visible) || (!events_enabled)) return;
    UI::Elements::PageView::poll_events();

    if (left_map_select->clicked){
        current_map_idx -= 1;
        if (current_map_idx < 0) current_map_idx += map_names.size();
    } else if (right_map_select->clicked){
        current_map_idx += 1;
        if (current_map_idx >= (int)map_names.size()) current_map_idx -= map_names.size();
    }
}

void Pages::LobbyWindowUI::update_room_members(std::vector<std::string> &room_members_)
{
    room_members = room_members_;
    for (size_t i = 0; i < Networking::Message::Room::MAX_ROOM_SIZE; ++i){
        remove_player_button[i]->interactable = is_leader && !room_members[i].empty();
    }
}
