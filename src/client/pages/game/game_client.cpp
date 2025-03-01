#include "game_client.hpp"

void ServiceConsumers::GameClient::send_frame(Game::Frame* frame)
{
    send_message(Communication::Game::Client::FRAME, frame, sizeof(Game::Frame));
}

void ServiceConsumers::GameClient::handle_message(Communication::Command type, const void *message, size_t size)
{
    switch(type){
        case Communication::Game::Server::GAME_STATE_BROADCAST:
            Game::GameState* server_gs;
            server_gs = Game::deserialize_game_state(message, size);
            game_update_callback(static_cast<const Game::GameState*>(server_gs), size);
            break;
    }

}
