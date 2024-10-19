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
        connect_worker.accomplish([this](const bool& cancel_requested){
            auto [succesful, error] = Global::ServiceProviders::room_client.connect(ui.address_input_value(), cancel_requested);
            
            if (succesful){
                // Connection succesful
                // Move over to the room page
                // SceneManagement::SceneManager::load_scene()
                
            } else{
                ui.show_error(error);
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
