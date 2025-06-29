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
        GameClient(std::function<void(const Game::GameState, size_t)> game_update_callback): ServiceConsumer(30), game_update_callback(game_update_callback){
            //
        }

        void identify(const Communication::Game::PlayerIdentification& pi);
        void send_frame(Game::Frame*);
        void send_selection(const Game::TypeSelection& selection);

        bool is_game_over();
        bool allow_respawn();
        bool is_connected();

        protected:
            std::string on_connection_established();
            void handle_disconnection();
            virtual void handle_message(Communication::Command type, const void *message, size_t size);

            bool game_over = false;
            bool respawn_ok = false;
            std::function<void(const Game::GameState, size_t)> game_update_callback;

        private:
            bool _is_connected = false;

    };
} // namespace Pages

#endif