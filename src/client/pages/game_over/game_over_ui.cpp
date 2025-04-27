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
        new UI::Elements::Text(
            [](auto _) -> std::pair<UI::DrawParameters::Box, UI::DrawParameters::Text> {
                return {
                    { // Box (for alignment)
                        .position = {
                            .value = {0.50f, 0.14f},
                            .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                        }
                    },
                    { // Text properties
                        .content = "Final standings",
                        .font_size = Global::rem * 3,
                        .font_color = {0x45, 0x41, 0x39, 0xc0},
                        .position = {
                            .value = {0.5f, 0.5f},  // Centered within the box
                            .mode = {Mode::SELF_W, Mode::SELF_H}
                        },
                        .origin = {
                            .value = {0.5, 0.5},  // Align vertically
                            .mode = {Mode::SELF_W, Mode::SELF_H}
                        }
                    }
                };
            }
        )
    );
    { // Player list
        for (size_t i = 0; i < 12; ++i){ // The names of every player in the lobby
            bg->register_element(
                new UI::Elements::Text(
                    [i, this](UI::Elements::Text* text){
                        return std::pair<UI::DrawParameters::Box, UI::DrawParameters::Text>{
                            {
                                .position = {
                                    .value = {
                                        i < 12/2 ? 0.22f : 0.52f, 0.25f + (i % (12/2)) * 0.1f
                                    },
                                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                                },
                                .size = {
                                    .value = {0.26f, 0.06f},
                                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                                },
                                .origin = {0},
                                .fill = i < scores.size() ? Color{0xb2, 0xad, 0x99, 0xc0} : Color{0x49, 0x47, 0x3f, 0x00}
                            },
                            {
                                .content = i < scores.size() ? scores[i].first + " - " + std::to_string(scores[i].second) : "",
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

    bg->register_element(
        go_to_lobby = Components::create_span_button(
            "Return",
            {
                .value = {0.5, 0.8},
                .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
            }
        )
    );
}