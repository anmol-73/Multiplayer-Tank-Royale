#include "main_window.hpp"

Pages::MainWindowScene::MainWindowScene()
{
    background = DragonLib::DImage("resources/ui_background.png");
}

void Pages::MainWindowScene::_update()
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
        ui.draw();
    }
    EndDrawing();
    ui.poll_events();

    if(ui.address_submit_button->clicked && !connect_worker.is_running()){    
        requested_name = StringAlgorithms::stripped(ui.name_input->value);   
        if (requested_name == std::string()){
            ui.show_info("Please enter a non empty name!", true);
        } else{
            ui.show_info("Trying to connect...", false);
            connect_worker.accomplish([this](const bool& cancel_requested){
                auto [succesful, error] = Global::ServiceProviders::room_client.connect(ui.address_input->value, cancel_requested);
                
                if (succesful){
                    Global::ServiceProviders::room_client.send_name_request(requested_name);
                    SceneManagement::SceneManager::load_deferred(SceneManagement::SceneName::LOBBY_PAGE);
                } else{
                    ui.show_info(error, true);
                }
            });
        }
    }
}

void Pages::MainWindowScene::_loading_update() {
    DragonLib::Utils::Drawing::draw_text({
        .content = "Loading...",
        .font_size = Global::rem * 2
    });
}

void Pages::MainWindowScene::_load_with_context()
{
    background.load_tex();
}

void Pages::MainWindowScene::_cleanup_with_context()
{
    background.unload_tex();
}

void Pages::MainWindowScene::_load()
{
    Global::ServiceProviders::room_client_worker.await(); // Wait for the room client to stop running (if it was running before)
    background.load_im();
}

void Pages::MainWindowScene::_cleanup()
{
    background.unload_im();
    connect_worker.cancel();
    connect_worker.await();
}
