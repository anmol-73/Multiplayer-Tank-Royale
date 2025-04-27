#include "lobby.hpp"


void Pages::LobbyScene::_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    if (!client->is_connected()){
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::SPLASH, "ERROR! Lost connection to lobby", 1);
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::SPLASH);
    }

    ui.visible_rooms = client->get_rooms();
    
    BeginDrawing();{
        ClearBackground({0x1e, 0x1e, 0x1e, 0xff});
        ui.draw();
    }
    EndDrawing();
    ui.poll_events();

    if (client->get_new_room_status() == Communication::RequestStatus::DENIED){
        ui.error_message = "Server rejected room creation!";
    }


    std::optional<Communication::Lobby::RoomDetail> room_to_join = std::nullopt; {
        if (client->get_new_room_status() == Communication::RequestStatus::ACCEPTED){
            room_to_join = client->get_new_room_detail();
            assert(room_to_join.has_value());
        } else{
            room_to_join = ui.join_room_request();
        }
    }
    if (room_to_join.has_value()){
        Communication::Address room_addr; {
            room_addr.name = address.name;
            room_addr.port = room_to_join.value().port;
        }

        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::ROOM, &room_addr, 0);
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::ROOM, &room_to_join.value(), 1);
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::ROOM);
        return;
    }

    if (client->get_new_room_status() == Communication::RequestStatus::IDLE){
        std::string new_room_name = ui.new_room_request();
        if (new_room_name.size() > 0){
            client->request_new_room(new_room_name);
            ui.error_message = "";
        }
    }
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
            .font_size = Global::rem * 1.5f,
            .font_color = {0, 0, 0, 0x60}
        });
    }
    EndDrawing();
}

void Pages::LobbyScene::_prepare(const void *msg, size_t command)
{
    switch(command){
        case 0: {
            assert(msg != nullptr);
            address = *static_cast<const Communication::Address*>(msg);
            break;
        }
        
        case 1: {
            assert(msg != nullptr);
            ui.error_message = static_cast<const char*>(msg);
            break;
        }

        default: assert(false);
    }
}

void Pages::LobbyScene::_load()
{
    ui.load_async();
    
    if (address.is_invalid()){ // This is clean af. I love you past self :)
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::SPLASH, address.name.c_str(), 0);
        SceneManagement::SceneManager::load_deferred(SceneManagement::SceneName::SPLASH);
        return;
    }
    
    client = new ServiceConsumers::LobbyClient();
    std::string error = client->connect(address);
    
    if (error.size() > 0){
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::SPLASH, error.c_str(), 0);
        SceneManagement::SceneManager::load_deferred(SceneManagement::SceneName::SPLASH);
        return;
    }

    ui.address_string = address.name + ":" + std::to_string(address.port);
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
    ui.error_message = "";
    ui.cleanup_sync();
}
