#include "game_ui.hpp"

Pages::GameUI::GameUI()
{
    using Mode = UI::DrawParameters::SizeMode;
    
    tank_images.assign(Game::Data::tank_resources.size(), {});
    for (size_t i = 0; i < tank_images.size(); ++i){
        tank_images[i].path = Game::Data::tank_resources[i].first;
    }
    turret_images.assign(Game::Data::gun_resources.size(), {});
    for (size_t i = 0; i < turret_images.size(); ++i){
        turret_images[i].path = Game::Data::gun_resources[i].first;
    }
    
    { // Tank select
        register_element(
            tank_preview = new UI::Elements::ImageView(
                tank_images[0], [this](UI::Elements::ImageView *) -> UI::DrawParameters::Box{
                    return {
                        .position = { 
                            .value = {0.66f, 0.3f},
                            .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                        },
                        .size = { 
                            .value = {0.3f, 0.16875f},
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
                    .content = Game::Data::tank_resources[tank_idx].second,
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
            left_tank_select = Components::create_text_button(
                "<", {
                    .value = {0.65, 0.66},
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
                    .value = {0.83, 0.66},
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
                            .value = {0.66f, 0.3f},
                            .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                        },
                        .size = { 
                            .value = {0.3f, 0.16875f},
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
                    .content = Game::Data::gun_resources[turret_idx].second,
                    .font_size = 0.8f * Global::rem,
                    .font_color =  {0x45, 0x41, 0x39, 0xff},
                    .position = {
                        .value = {0.81, 0.82},
                        .mode = {Mode::SCREEN_W, Mode::SCREEN_H}
                    }
                };
            })
        );
    
        register_element(
            left_turret_select = Components::create_text_button(
                "<", {
                    .value = {0.65, 0.66},
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
                    .value = {0.83, 0.66},
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
                "Create Room",
                {
                    .value = {0.5, 0.7},
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
