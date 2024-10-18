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

    if(ui.address_submit_requested()){
        assert(!worker.is_running());
        
        // ui.disable_submit_button();
        worker.accomplish([this](const bool& cancel_requested){
            auto [succesful, error] = Global::ServiceProviders::room_client.connect(ui.address_input_value(), cancel_requested);
            if (succesful){
                // Connection succesful
            } else{
                // Connection failed
            }
        });
        // ui.enable_submit_button();
    }
}

void Pages::MainWindowScene::_loading_update() {
    // TODO:
    // Populate this properly
    return _update();
}

void Pages::MainWindowScene::_cleanup()
{
    worker.cancel();
    worker.await();
    // if (!persist_connection) room_client.disconnect();
}
