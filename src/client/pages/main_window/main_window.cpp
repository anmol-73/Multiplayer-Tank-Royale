#include "main_window.hpp"

void Pages::MainWindowScene::_update()
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

    if(ui.address_submit_requested() && !connect_worker.is_running()){        
        ui.show_info("Trying to connect...", false);
        connect_worker.accomplish([this](const bool& cancel_requested){
            auto [succesful, error] = Global::ServiceProviders::room_client.connect(ui.address_input_value(), cancel_requested);
            
            if (succesful){
                SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::LOBBY_PAGE);
            } else{
                ui.show_info(error, true);
            }
        });
    }
}

void Pages::MainWindowScene::_loading_update() {
    // TODO:
    // Populate this properly.... (until the room_client is ready we can't do anything)
    return _update();
}

void Pages::MainWindowScene::_load_with_context()
{
    background_texture = LoadTextureFromImage(background_image);
}

void Pages::MainWindowScene::_cleanup_with_context()
{
    UnloadTexture(background_texture);
}

void Pages::MainWindowScene::_load()
{
    Global::ServiceProviders::room_client_worker.await(); // Wait for the room client to stop running (if it was running before)
    background_image = LoadImage("resources/ui_background.png");
}

void Pages::MainWindowScene::_cleanup()
{
    UnloadImage(background_image);
    connect_worker.cancel();
    connect_worker.await();
}
