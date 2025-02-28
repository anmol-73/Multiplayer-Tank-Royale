#include "lobby.hpp"


void Pages::LobbyScene::_update()
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

}

void Pages::LobbyScene::_loading_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    BeginDrawing();{
        ClearBackground({0xcb, 0xc6, 0xb2, 0xe0});
        DragonLib::Utils::Drawing::draw_text({
            .content = "Trying to connect to the server...   :)",
            .font_size = Global::rem * 2,
            .font_color = {0, 0, 0, 0x60}
        });
    }
    EndDrawing();
}

void Pages::LobbyScene::_prepare(const void *_address)
{
    if (_address == nullptr) return;
    address = *static_cast<const Communication::Address*>(_address);
}

void Pages::LobbyScene::_load()
{
    ui.load_async();

    if (address.is_invalid()){ // This is clean af. I love you past self :)
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::SPLASH, address.name.c_str());
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::SPLASH);
        return;
    }

    client = new ServiceConsumers::LobbyClient();
    std::string error = client->connect(address);

    if (error.size() > 0){
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::SPLASH, error.c_str());
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::SPLASH);
        return;
    }

    client_worker.accomplish([this](auto _){
        client->start();
    });
}

void Pages::LobbyScene::_cleanup()
{
    ui.cleanup_async();
    
    
    if (client != nullptr){
        client->stop();
        client_worker.await();
        delete client;
        client = nullptr;
    }
}

void Pages::LobbyScene::_load_with_context()
{
    ui.load_sync();
}

void Pages::LobbyScene::_cleanup_with_context()
{
    ui.cleanup_sync();
}
