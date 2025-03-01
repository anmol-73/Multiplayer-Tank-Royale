#include "room_ui.hpp"

Pages::RoomUI::RoomUI()
{
    const int max_room_size = 12;
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
        new UI::Elements::Span(
            [this](UI::Elements::Span* span){
                return UI::DrawParameters::TextBox{
                    .content = "Room: " + room_name,
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

    // bg->register_element(
    //     new UI::Elements::Span(
    //         [this](UI::Elements::Span* span){
    //             return UI::DrawParameters::TextBox{
    //                 .content = "Server IP: ",
    //                 .font_size = 1.0f * Global::rem,
    //                 .font_color = {0x45, 0x41, 0x39, 0x80},
                    
    //                 .position = {
    //                     .value = {0.01, 0.05},
    //                     .mode = {Mode::SCREEN_W, Mode::SCREEN_W}
    //                 },

    //                 .rect_origin = { // Specifies the origin of the rect
    //                     .value = {0, -2 * Global::rem},
    //                     .mode = {Mode::ABSOLUTE, Mode::ABSOLUTE}
    //                 }
    //             };
    //         }
    //     )
    // );

    for (size_t i = 0; i < max_room_size; ++i){ // The names of every player in the lobby
        bg->register_element(
            new UI::Elements::Text(
                [i, this](UI::Elements::Text* text){
                    return std::pair<UI::DrawParameters::Box, UI::DrawParameters::Text>{
                        {
                            .position = {
                                .value = {
                                    i < max_room_size/2 ? 0.05f : 0.35f, 0.25f + (i % (max_room_size/2)) * 0.1f
                                },
                                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                            },
                            .size = {
                                .value = {0.23f, 0.06f},
                                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                            },
                            .origin = {0},
                            .fill = i < visible_players.size() ? Color{0xb2, 0xad, 0x99, 0xc0} : Color{0x49, 0x47, 0x3f, 0x60}
                        },
                        {
                            .content = i < visible_players.size() ? visible_players[i].name : "",
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

    bg->register_element(
        new UI::Elements::Span([this](UI::Elements::Span* _){
            return UI::DrawParameters::TextBox{
                .content = std::to_string(map_idx),
                .font_size = 0.8f * Global::rem,
                .font_color =  {0x45, 0x41, 0x39, 0xff},
                .position = {
                    .value = {0.81, 0.62},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                }
            };
        })
    );

    bg->register_element(
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
    bg->register_element(
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

    bg->register_element(
        start_game_button = Components::create_span_button(
            "Start!",
            UI::DrawParameters::Measurement{
                .value = {0.94, 0.85},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
            }
        )
    );
    bg->register_element(
        exit_button = Components::create_span_button(
            " Exit ",
            UI::DrawParameters::Measurement{
                .value = {0.94, 0.12},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
            }
        )
    );

    { // Room creation
        bg->register_element(
            name_input = Components::create_sl_input(
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
            set_name_button = Components::create_span_button(
                "Create Room",
                {
                    .value = {0.5, 0.7},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                }
            )
        );
    }
}

bool Pages::RoomUI::should_start_game()
{
    return start_game_button->clicked;
}

bool Pages::RoomUI::should_leave()
{
    return exit_button->clicked;
}

std::string Pages::RoomUI::set_name_request()
{
    if (set_name_button->clicked){
        return Utils::StringAlgorithms::stripped(name_input->value);
    }
    return "";
}

int Pages::RoomUI::get_map_idx_delta()
{
    return right_map_select->clicked - left_map_select->clicked;
}
