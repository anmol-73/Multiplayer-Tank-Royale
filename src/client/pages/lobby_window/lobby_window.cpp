#include "lobby_window.hpp"

void Pages::LobbyWindowScene::_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    BeginDrawing();{
        ClearBackground({0x1e, 0x1e, 0x1e, 0xff});
        DrawTexturePro(
            background_texture,
            {0, 0, (float)background_texture.width, (float)background_texture.height},
            {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
            {0},
            0,
            WHITE
        );
        Rectangle map_rect = {(float)GetScreenWidth() * 0.66f, (float)GetScreenHeight() * 0.3f, (float)GetScreenWidth() * 0.3f, (float)GetScreenWidth() * 0.16875f};
        DrawTexturePro(
            map_texture[ui.current_map_idx],
            {0, 0, (float)map_texture[ui.current_map_idx].width, (float)map_texture[ui.current_map_idx].height},
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


void Pages::LobbyWindowScene::_loading_update()
{
    // I think we should wait for all the player info?
    BeginDrawing();{
        ClearBackground({0x1e, 0x1e, 0x1e, 0xff});
    }EndDrawing();
}


void Pages::LobbyWindowScene::_load()
{
    room_members = {
        {0, "Tank Destroyer"},
        {1, "Player 1"},
        {2, "Denviser"},
        {3, "Xx__VoidGamer__xX"},
        {4, "RyuKaminari"},
    };
    background_image = LoadImage("resources/ui_background.png");
    map_image = {
        LoadImage("resources/map1.png"),
        LoadImage("resources/map2.png")
    };
    std::vector<std::string> map_names = {
        "Forest",
        "Catacombs"
    };
    ui.make_ready(room_members, map_names);
}

void Pages::LobbyWindowScene::_load_with_context()
{
    background_texture = LoadTextureFromImage(background_image);
    map_texture.clear();
    map_texture.assign(map_image.size(), {});
    for (size_t i = 0; i < map_image.size(); ++i){
        map_texture[i] = LoadTextureFromImage(map_image[i]);
    }
}

void Pages::LobbyWindowScene::_cleanup_with_context()
{
    for (size_t i = 0; i < map_image.size(); ++i){
        UnloadTexture(map_texture[i]);
    }
    UnloadTexture(background_texture);
}

void Pages::LobbyWindowScene::_cleanup()
{
    UnloadImage(background_image);
    for (size_t i = 0; i < map_image.size(); ++i){
        UnloadImage(map_image[i]);
    }
    connect_worker.cancel();
    connect_worker.await();
}