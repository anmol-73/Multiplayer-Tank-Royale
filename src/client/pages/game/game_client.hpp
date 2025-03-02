#ifndef H_PAGES_GAME_CLIENT
#define H_PAGES_GAME_CLIENT

#include <functional>

#include "game.hpp"
#include "communication/protocol.hpp"
#include "service_consumer.hpp"
#include "game_communication.hpp"

namespace ServiceConsumers
{
    struct GameClient: public ServiceConsumer{
        public:
        GameClient(std::function<void(const Game::GameState, size_t)> game_update_callback): game_update_callback(game_update_callback){
            //
        }

        void send_frame(Game::Frame*);

        protected:
            virtual void handle_message(Communication::Command type, const void *message, size_t size);

            std::function<void(const Game::GameState, size_t)> game_update_callback;

    };
} // namespace Pages

#endif