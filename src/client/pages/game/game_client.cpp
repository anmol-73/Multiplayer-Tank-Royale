#include "game_client.hpp"

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

void ServiceConsumers::GameClient::handle_message(Communication::Command type, const void *message, size_t size)
{
    using namespace Communication::Game;
    
    switch(static_cast<Server>(type)){
        case Server::GAME_STATE_BROADCAST: {
            game_update_callback(Game::deserialize_game_state(message, size), size);
            break;
        }

        case Server::RESPAWN_OK: {
            std::cout << "CAN RESPAWN!" << std::endl;
            break;
        }

        case Server::GAME_OVER: {
            std::cout << "GMAE OVER!" << std::endl;
            break;
        }
    }
}
