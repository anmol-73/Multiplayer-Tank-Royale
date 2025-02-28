#include "room.hpp"


void Pages::RoomScene::_update()
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

void Pages::RoomScene::_loading_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    BeginDrawing();{
        ClearBackground({0xcb, 0xc6, 0xb2, 0xe0});
        DragonLib::Utils::Drawing::draw_text({
            .content = "Connecting to the room...   :)",
            .font_size = Global::rem * 1.5f,
            .font_color = {0, 0, 0, 0x60}
        });
    }
    EndDrawing();
}

void Pages::RoomScene::_prepare(const void *msg, size_t command)
{
    switch(command){
        case 0: {
            assert(msg != nullptr);
            address = *static_cast<const Communication::Address*>(msg);
            break;
        }
        
        case 1: {
            assert(msg != nullptr);
            room_detail = *static_cast<const Communication::Lobby::RoomDetail*>(msg);
            break;
        }

        default: assert(false);
    }
}

void Pages::RoomScene::_load()
{
    ui.load_async();
    
    if (address.is_invalid()){ // This is clean af. I love you past self :)
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::LOBBY, address.name.c_str(), 1);
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::LOBBY);
        return;
    }
    
    client = new ServiceConsumers::RoomClient();
    std::string error = client->connect(address);
    
    if (error.size() > 0){
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::LOBBY, error.c_str(), 1);
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::LOBBY);
        return;
    }

    client_worker.accomplish([this](auto _){
        client->start();
    });
}

void Pages::RoomScene::_cleanup()
{
    ui.cleanup_async();    
    
    if (client != nullptr){
        client->stop();
        client_worker.await();
        delete client;
        client = nullptr;
    }
}

void Pages::RoomScene::_load_with_context()
{
    ui.load_sync();
}

void Pages::RoomScene::_cleanup_with_context()
{
    ui.cleanup_sync();
}
