#include "lobby_window.hpp"

Pages::LobbyWindowScene::LobbyWindowScene()
{
    background = DragonLib::DImage("resources/ui_background.png");
    map_images = {
        DragonLib::DImage("resources/map1.png"),
        DragonLib::DImage("resources/map2.png")
    };
    room_members.assign(Networking::Message::Room::MAX_ROOM_SIZE, {});
}

void Pages::LobbyWindowScene::_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    BeginDrawing();{
        ClearBackground({0x1e, 0x1e, 0x1e, 0xff});
        DrawTexturePro(
            background.tex,
            {0, 0, (float)background.tex.width, (float)background.tex.height},
            {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
            {0},
            0,
            WHITE
        );
        Rectangle map_rect = {(float)GetScreenWidth() * 0.66f, (float)GetScreenHeight() * 0.3f, (float)GetScreenWidth() * 0.3f, (float)GetScreenWidth() * 0.16875f};
        DrawTexturePro(
            map_images[ui.current_map_idx].tex,
            {0, 0, (float)map_images[ui.current_map_idx].tex.width, (float)map_images[ui.current_map_idx].tex.height},
            map_rect,
            {0},
            0,
            WHITE
        );
        DrawRectangleLinesEx(map_rect, 2, BLACK);
        ui.draw();
    }
    EndDrawing();
    ui.poll_events();

    if (ui.left_map_select->clicked || ui.right_map_select->clicked){
        // Set map
        Global::ServiceProviders::room_client.send_map_id(ui.current_map_idx);
    }

    if (ui.start_game_button->clicked){
        // Start
        Global::ServiceProviders::room_client.request_start();
    }

    for (size_t i = 0; i < Networking::Message::Room::MAX_ROOM_SIZE; ++i){
        if (ui.remove_player_button[i]->clicked){
            Global::ServiceProviders::room_client.request_disconnection(i);
        }
    }

    if (ui.exit_button->clicked){
        SceneManagement::SceneManager::load_deferred(SceneManagement::SceneName::MAIN_PAGE);   
    }

}


void Pages::LobbyWindowScene::_loading_update() {
    BeginDrawing();{
        ClearBackground({0xcb, 0xc6, 0xb2, 0xe0});
        DragonLib::Utils::Drawing::draw_text({
            .content = "Loading...",
            .font_size = Global::rem * 2,
            .font_color = {0, 0, 0, 0x60}
        });
    }
    EndDrawing();
}


void Pages::LobbyWindowScene::_load()
{
    Global::ServiceProviders::room_client_worker.await();
    persist_connection = false;
    for (size_t i = 0; i < room_members.size(); ++i) room_members[i] = "";
    
    background.load_im();
    for (auto &map: map_images) map.load_im();
    
    Global::ServiceProviders::room_client.game_start_callback = [this](){
        persist_connection = true;
        Global::ServiceProviders::room_client.reset_callbacks();
        SceneManagement::SceneManager::load_deferred(SceneManagement::GAME_PAGE);
        return;
    };
    Global::ServiceProviders::room_client.disconnect_callback = [](){
        SceneManagement::SceneManager::load_deferred(SceneManagement::SceneName::MAIN_PAGE);
        return;
    };
    Global::ServiceProviders::room_client.set_map_callback = [this](int map){
        if (Global::ServiceProviders::room_client.get_id() == 0) return;
        ui.current_map_idx = map;
        return;
    };
    Global::ServiceProviders::room_client.room_broadcast_callback = [this](std::vector<std::string> room_members_){
        room_members = room_members_;
        ui.update_room_members(room_members);
        return;
    };
    Global::ServiceProviders::room_client_worker.accomplish([](const bool &should_cancel){
        Global::ServiceProviders::room_client.run(should_cancel);
    });
    ui.make_ready({
        "Forest",
        "Catacombs"
    }, Global::ServiceProviders::room_client.get_id() == 0);
}

void Pages::LobbyWindowScene::_load_with_context()
{
    
    background.load_tex();
    for (auto &map: map_images) map.load_tex();
}

void Pages::LobbyWindowScene::_cleanup_with_context()
{
    background.unload_tex();
    for (auto &map: map_images) map.unload_tex();
}

void Pages::LobbyWindowScene::_cleanup()
{
    if(!persist_connection)
    {
        Global::ServiceProviders::room_client_worker.cancel();
    }
    background.unload_im();
    for (auto &map: map_images) map.unload_im();
}