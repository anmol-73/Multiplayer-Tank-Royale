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
        ui.draw();
    }
    EndDrawing();
    ui.poll_events();
}


void Pages::LobbyWindowScene::_loading_update()
{
    // I think we should wait for all the player info?
    return _update();
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
    ui.make_ready(room_members);
}

void Pages::LobbyWindowScene::_load_with_context()
{
    background_texture = LoadTextureFromImage(background_image);
}

void Pages::LobbyWindowScene::_cleanup_with_context()
{
    UnloadTexture(background_texture);
}

void Pages::LobbyWindowScene::_cleanup()
{
    UnloadImage(background_image);
    connect_worker.cancel();
    connect_worker.await();
}