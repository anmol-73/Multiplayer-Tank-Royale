#include "main_window.hpp"

void Pages::MainWindowScene::_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    BeginDrawing();{
        ClearBackground({0x1e, 0x1e, 0x1e, 0xff});
        ui.draw();
    }
    EndDrawing();

    ui.poll_events();

    if(ui.address_submit_requested() && !connect_worker.is_running()){        
        // ui.disable_submit_button();
        connect_worker.accomplish([this](const bool& cancel_requested){
            auto [succesful, error] = Global::ServiceProviders::room_client.connect(ui.address_input_value(), cancel_requested);
            
            if (succesful){
                // Connection succesful
                // Move over to the room page
                // SceneManagement::SceneManager::load_scene()
                
            } else{
                ui.show_error(error);
            }
            // ui.enable_submit_button();
        });
        
    }
}

void Pages::MainWindowScene::_loading_update() {
    // TODO:
    // Populate this properly.... (until the room_client is ready we can't do anything)
    return _update();
}

void Pages::MainWindowScene::_load()
{
    Global::ServiceProviders::room_client_worker.await(); // Wait for the room client to stop running (if it was running before)
}

void Pages::MainWindowScene::_cleanup()
{
    connect_worker.cancel();
    connect_worker.await();
}
