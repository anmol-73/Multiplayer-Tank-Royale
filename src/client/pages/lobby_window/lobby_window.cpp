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
}


void Pages::LobbyWindowScene::_loading_update() {
    DragonLib::Utils::Drawing::draw_text({
        .content = "Loading...",
        .font_size = Global::rem * 2
    });
}


void Pages::LobbyWindowScene::_load()
{
    for (size_t i = 0; i < room_members.size(); ++i) room_members[i] = "";
    
    background.load_im();
    for (auto &map: map_images) map.load_im();
    
    Global::ServiceProviders::room_client_worker.accomplish([](const bool &should_cancel){
        Global::ServiceProviders::room_client.run(should_cancel);
    });
    ui.update_room_members(room_members);
    ui.make_ready({
        "Forest",
        "Catacombs"
    });
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
    background.unload_im();
    for (auto &map: map_images) map.unload_im();
}