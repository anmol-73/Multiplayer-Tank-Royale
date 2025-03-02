#include "lobby_ui.hpp"
const float y_position = 0.20f;

Pages::LobbyUI::LobbyUI()
{
    using Mode = UI::DrawParameters::SizeMode;
    
    const size_t max_room_count = 14;
    room_selects.assign(max_room_count, nullptr);
    
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

    { // Active rooms
        for (size_t i = 0; i < max_room_count; ++i){
            bg->register_element(
                room_selects[i] = new UI::Elements::Text(
                    [this, i, max_room_count](auto _) -> std::pair<UI::DrawParameters::Box, UI::DrawParameters::Text>{
                        return {
                            {
                                .position = {
                                    .value = {
                                        i < max_room_count/2 ? 0.05f : 0.45f, 0.25f + (i % (max_room_count/2)) * 0.1f
                                    },
                                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                                },
                                .size = {
                                    .value = {0.34f, 0.06f},
                                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                                },
                                .origin = {0},
                                .fill = visible_rooms.size() > i ? Color{0xb2, 0xad, 0x99, 0xc0} : Color{0x49, 0x47, 0x3f, 0x60}
                            },
                            {
                                .content = visible_rooms.size() > i ? visible_rooms[i].name : "",
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
    auto *ip_name_text = new UI::Elements::Text(
        [](auto _) -> std::pair<UI::DrawParameters::Box, UI::DrawParameters::Text> {
            return {
                { // Box (for alignment)
                    .position = {
                        .value = {0.10f, 0.14f},  // Position of "Room Name"
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                    }
                },
                { // Text properties
                    .content = "Server Address : 127.0.0.1",
                    .font_size = Global::rem,
                    .font_color = {0x45, 0x41, 0x39, 0xc0},
                    .position = {
                        .value = {0.0f, 0.5f},  // Centered within the box
                        .mode = {Mode::SELF_W, Mode::SELF_H}
                    },
                    .origin = {
                        .value = {0, 0.5},  // Align vertically
                        .mode = {Mode::SELF_W, Mode::SELF_H}
                    }
                }
            };
        }
    );
    bg->register_element(ip_name_text);

    auto *room_name_text = new UI::Elements::Text(
        [](auto _) -> std::pair<UI::DrawParameters::Box, UI::DrawParameters::Text> {
            return {
                { // Box (for alignment)
                    .position = {
                        .value = {0.10f, y_position},  // Position of "Room Name"
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                    }
                },
                { // Text properties
                    .content = "Room Name:",
                    .font_size = Global::rem,
                    .font_color = {0x45, 0x41, 0x39, 0xc0},
                    .position = {
                        .value = {0.0f, 0.5f},  // Centered within the box
                        .mode = {Mode::SELF_W, Mode::SELF_H}
                    },
                    .origin = {
                        .value = {0, 0.5},  // Align vertically
                        .mode = {Mode::SELF_W, Mode::SELF_H}
                    }
                }
            };
        }
    );
    bg->register_element(room_name_text);


    { // Room creation
        bg->register_element(
            create_room_name_input = Components::create_sl_input(
                "", "...", {
                    .value = {0.43f, 0.20f},
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
                    .value = {0.78f, 0.20f},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                }
            )
        );
    }

    create_room_name_input->value = "Default Room";
}

std::optional<Communication::Lobby::RoomDetail> Pages::LobbyUI::join_room_request()
{
    for (size_t i = 0; i < visible_rooms.size(); ++i){
        if (room_selects[i]->clicked){
            return visible_rooms[i];
        }
    }
    return std::nullopt;
}

std::string Pages::LobbyUI::new_room_request()
{
    if (create_room_button->clicked){
        return Utils::StringAlgorithms::stripped(create_room_name_input->value);
    }
    return "";
}

void Pages::LobbyUI::poll_events()
{
    if ((!visible) || (!events_enabled)) return;
    UI::Elements::PageView::poll_events();

    for (size_t i = 0; i < room_selects.size(); ++i){
        room_selects[i]->interactable = (i < visible_rooms.size());
    }
}
