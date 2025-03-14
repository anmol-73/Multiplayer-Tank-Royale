#include "room.hpp"
#include <cstring>

void Pages::RoomScene::_update()
{
    if (client->get_join_status() == Communication::RequestStatus::ONGOING) return _loading_update();

    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    ui.set_map(client->get_current_settings().map);
    ui.visible_players = client->get_joined_players();

    BeginDrawing();{
        ClearBackground({0x1e, 0x1e, 0x1e, 0xff});
        ui.draw();
    }
    EndDrawing();
    ui.poll_events();

    if (client->get_join_status() == Communication::RequestStatus::DENIED){
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::LOBBY, "Room rejected join!", 1);
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::LOBBY);
        return;
    }
    
    if (client->get_game_port().has_value()){
        Communication::Address addr; {
            addr.name = address.name;
            addr.port = client->get_game_port().value();
        }
        auto sz = client->get_joined_players().size();
        Communication::Game::PlayerIdentification pi; {
            pi.id = client->get_client_id();
            std::string name = client->get_government_name();

            strncpy(pi.name, name.c_str(), sizeof(Communication::Game::PlayerIdentification::name));
        }

        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::GAME, &addr, 0);
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::GAME, &sz, 1);
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::GAME, client->get_joined_players().data(), 2);
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::GAME, &client->get_current_settings(), 3);
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::GAME, &pi, 4);
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::GAME);
        return;
    }
    
    if (ui.should_leave()){
        // SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::LOBBY, "", 0);
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::LOBBY);
        return;
    }
    
    if (ui.get_map_idx_delta()){
        Communication::Room::RoomSettings new_settings(client->get_current_settings());
        new_settings.map = (static_cast<int>(new_settings.map) + ui.get_map_idx_delta() + Maps::map_count) % Maps::map_count;
        ui.set_map(new_settings.map);
        client->request_set_settings(new_settings);
    }

    auto nss = client->get_name_set_status();
    if (nss == Communication::RequestStatus::DENIED){
        return;
    }
    
    if (nss == Communication::RequestStatus::IDLE){
        auto name = ui.set_name_request();
        if (name.size() > 0){
            client->request_new_name(name);
        }
    }

    if (ui.should_start_game()){
        client->request_game_start();
    }
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
