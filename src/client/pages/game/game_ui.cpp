#include "game_ui.hpp"

Pages::GameUI::GameUI()
{
    using Mode = UI::DrawParameters::SizeMode;
    
    tank_images.assign(Game::Data::tank_resources.size(), {});
    for (size_t i = 0; i < tank_images.size(); ++i){
        tank_images[i].path = Game::Data::tank_resources[i][0];
    }
    turret_images.assign(Game::Data::gun_resources.size(), {});
    for (size_t i = 0; i < turret_images.size(); ++i){
        turret_images[i].path = Game::Data::gun_resources[i][0];
    }
    
    { // Tank select
        register_element(
            tank_preview = new UI::Elements::ImageView(
                tank_images[0], [this](UI::Elements::ImageView *) -> UI::DrawParameters::Box{
                    return {
                        .position = { 
                            .value = {0.1f, 0.1f},
                            .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                        },
                        .size = { 
                            .value = {0.2f, 0.2},
                            .mode = {Mode::SCREEN_W, Mode::SCREEN_W} 
                        },
                        .origin = {0} 
                    };
                }
            )
        );
        register_element(
            new UI::Elements::Span([this](UI::Elements::Span* _){
                return UI::DrawParameters::TextBox{
                    .content = Game::Data::tank_resources[tank_idx][1],
                    .font_size = 1.5f * Global::rem,
                    .font_color =  {0x45, 0x41, 0x39, 0xff},
                    .position = {
                        .value = {0.2, 0.5},
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                    }
                };
            })
        );
    
        register_element(
            left_tank_select = Components::create_text_button(
                "<", {
                    .value = {0.1, 0.61},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
                },
                {
                    .value = {0.04, 0.06},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
                },
                {0}
            )
        );
        register_element(
            right_tank_select = Components::create_text_button(
                ">", {
                    .value = {0.26, 0.61},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
                },
                {
                    .value = {0.04, 0.06},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
                },
                {0}
    
            )
        );
    }

    { // Turret select
        register_element(
            turret_preview = new UI::Elements::ImageView(
                turret_images[0], [this](UI::Elements::ImageView *) -> UI::DrawParameters::Box{
                    return {
                        .position = { 
                            .value = {0.4f, 0.1f},
                            .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                        },
                        .size = { 
                            .value = {0.2f, 0.2},
                            .mode = {Mode::SCREEN_W, Mode::SCREEN_W} 
                        },
                        .origin = {0} 
                    };
                }
            )
        );
        register_element(
            new UI::Elements::Span([this](UI::Elements::Span* _){
                return UI::DrawParameters::TextBox{
                    .content = Game::Data::gun_resources[turret_idx][1],
                    .font_size = 1.5f * Global::rem,
                    .font_color =  {0x45, 0x41, 0x39, 0xff},
                    .position = {
                        .value = {0.5, 0.5},
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                    }
                };
            })
        );
    
        register_element(
            left_turret_select = Components::create_text_button(
                "<", {
                    .value = {0.4, 0.61},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
                },
                {
                    .value = {0.04, 0.06},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
                },
                {0}
            )
        );
        register_element(
            right_turret_select = Components::create_text_button(
                ">", {
                    .value = {0.56, 0.61},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
                },
                {
                    .value = {0.04, 0.06},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H},
                },
                {0}
    
            )
        );
    }

    { // Action buttons
        register_element(
            respawn_button = Components::create_span_button(
                "Spawn!",
                {
                    .value = {0.35, 0.8},
                    .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                }
            )
        );
    }
}

void Pages::GameUI::allow_respawn(bool ok)
{
    respawn_button->interactable = ok;
}

bool Pages::GameUI::should_respawn()
{
    return respawn_button->clicked;
}

void Pages::GameUI::load_async()
{
    UI::Elements::PageView::load_async();
    for (size_t i = 0; i < tank_images.size(); ++i){
        tank_images[i].load_im();
    }
    for (size_t i = 0; i < turret_images.size(); ++i){
        turret_images[i].load_im();
    }
}

void Pages::GameUI::load_sync()
{
    UI::Elements::PageView::load_sync();
    for (size_t i = 0; i < tank_images.size(); ++i){
        tank_images[i].load_tex();
    }
    for (size_t i = 0; i < turret_images.size(); ++i){
        turret_images[i].load_tex();
    }
}

void Pages::GameUI::cleanup_async()
{
    UI::Elements::PageView::cleanup_async();
    for (size_t i = 0; i < tank_images.size(); ++i){
        tank_images[i].unload_im();
    }
    for (size_t i = 0; i < turret_images.size(); ++i){
        turret_images[i].unload_im();
    }
}

void Pages::GameUI::cleanup_sync()
{
    UI::Elements::PageView::cleanup_sync();
    for (size_t i = 0; i < tank_images.size(); ++i){
        tank_images[i].unload_tex();
    }
    for (size_t i = 0; i < turret_images.size(); ++i){
        turret_images[i].unload_tex();
    }
}

void Pages::GameUI::poll_events()
{
    if ((!visible) || (!events_enabled)) return;
    UI::Elements::PageView::poll_events();

    if (right_tank_select->clicked - left_tank_select->clicked != 0){
        tank_idx = (static_cast<int>(tank_images.size()) + tank_idx + right_tank_select->clicked - left_tank_select->clicked) % tank_images.size();
        tank_preview->image = tank_images[tank_idx];
    }
    if (right_turret_select->clicked - left_turret_select->clicked != 0){
        turret_idx = (static_cast<int>(turret_images.size()) + turret_idx + right_turret_select->clicked - left_turret_select->clicked) % turret_images.size();
        turret_preview->image = turret_images[turret_idx];
    }
}
