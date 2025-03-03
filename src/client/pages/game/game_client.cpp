#include "game_client.hpp"
#include <iostream>

void ServiceConsumers::GameClient::identify(const Communication::Game::PlayerIdentification &pi)
{
    using namespace Communication::Game;
    send_message(Client::IDENTIFY, &pi, sizeof(pi));
}

void ServiceConsumers::GameClient::send_frame(Game::Frame *frame)
{
    using namespace Communication::Game;
    send_message(Client::FRAME, frame, sizeof(Game::Frame));
}

void ServiceConsumers::GameClient::send_selection(const Game::TypeSelection &selection)
{
    using namespace Communication::Game;
    send_message(Client::TYPES_SELECTION, &selection, sizeof(Game::TypeSelection));
    respawn_ok = false;
}

bool ServiceConsumers::GameClient::is_game_over()
{
    return game_over;
}
bool ServiceConsumers::GameClient::allow_respawn()
{
    return respawn_ok;
}

std::string ServiceConsumers::GameClient::on_connection_established()
{
    _is_connected = true;
    std::cout << "Game disconnected!" << std::endl;
    return std::string();
}

void ServiceConsumers::GameClient::handle_disconnection()
{
    _is_connected = false;
    ServiceConsumer::handle_disconnection();
}

void ServiceConsumers::GameClient::handle_message(Communication::Command type, const void *message, size_t size)
{
    using namespace Communication::Game;
    
    switch(static_cast<Server>(type)){
        case Server::GAME_STATE_BROADCAST: {
            game_update_callback(Game::deserialize_game_state(message, size), size);
            break;
        }

        case Server::RESPAWN_OK: {
            respawn_ok = true;
            break;
        }

        case Server::GAME_OVER: {
            game_over = true;
            break;
        }
    }
}
